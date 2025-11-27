#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// -------------------- LED --------------------
#define LED_PIN 15
#define NUM_LEDS 8
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// -------------------- Button --------------------
#define BUTTON_PIN 22
bool lastButtonReading = HIGH;
bool buttonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 40;

// -------------------- Status --------------------
int colorState = 0; // 0=Rot, 1=Grün, 2=Orange

// -------------------- WLAN --------------------
const char *ssid = "HCI";
const char *password = "wearedoingresearch.";

// -------------------- UDP --------------------
WiFiUDP Udp;
const unsigned int localPort = 4210;

IPAddress laptopIP;
unsigned int laptopPort = 0;

// ================= Funktionen =================
void setColor(uint8_t r, uint8_t g, uint8_t b)
{
  for (int i = 0; i < NUM_LEDS; i++)
    strip.setPixelColor(i, strip.Color(r, g, b));
  strip.show();
}

void showColorByState()
{
  const char *msg;
  if (colorState == 0)
  {
    setColor(255, 0, 0);
    msg = "BLOCK";
  }
  else if (colorState == 1)
  {
    setColor(0, 255, 0);
    msg = "ALLOW";
  }
  else
  {
    setColor(255, 165, 0);
    msg = "SETUP";
  }

  // Nur senden, wenn Laptop-IP bekannt
  if (laptopPort != 0)
  {
    Udp.beginPacket(laptopIP, laptopPort);
    Udp.write((const uint8_t *)msg, strlen(msg));
    Udp.endPacket();
  }

  Serial.printf("STATE: %s\n", msg);
}

// ================= Setup =================
void setup()
{
  Serial.begin(115200);
  delay(300);
  Serial.println("\n=== ESP32 UDP Prototype ===");

  // Button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // LEDs
  strip.begin();
  strip.clear();
  strip.setBrightness(100);
  strip.show();

  // WLAN verbinden
  Serial.printf("Verbinde mit WLAN '%s'...\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nVerbunden!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // UDP starten
  if (Udp.begin(localPort))
    Serial.printf("UDP gestartet auf Port %d\n", localPort);
  else
    Serial.println("Fehler beim Starten von UDP!");
}

// ================= Loop =================
void loop()
{
  // UDP empfangen
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    char incoming[256];
    int len = Udp.read(incoming, 255);
    if (len > 0)
      incoming[len] = 0;

    Serial.printf("Empfangen von %s:%d -> %s\n",
                  Udp.remoteIP().toString().c_str(),
                  Udp.remotePort(),
                  incoming);

    // Laptop-IP + Port speichern
    laptopIP = Udp.remoteIP();
    laptopPort = Udp.remotePort();

    // START → Rot
    if (strcmp(incoming, "START") == 0)
    {
      colorState = 0;
      showColorByState();
    }
  }

  // Button entprellen
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonReading)
  {
    lastDebounceTime = millis();
    lastButtonReading = reading;
  }
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;
      if (buttonState == LOW)
      { // gedrückt
        colorState++;
        if (colorState > 2)
          colorState = 0;
        showColorByState();
      }
    }
  }
}
