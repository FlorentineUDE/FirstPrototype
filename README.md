# FirstPrototype
This repository contains the firmware for a low-fidelity wristband prototype designed to explore tangible privacy controls in XR. The ESP32-based code handles LED feedback, single-button input, and UDP communication with a simulated XR device.

🚀 Nutzung & Testanleitung für den Wristband-Prototyp

Diese Anleitung beschreibt, wie die Firmware auf den ESP32 geladen wird, wie die UDP-Simulation gestartet wird und wie der Prototyp anschließend getestet werden kann.

📦 Voraussetzungen

- ESP32 Dev Board

- RGB-LED-Streifen (z. B. WS2812 / Neopixel)

- Push-Button

- Stromversorgung für den ESP32

- Laptop mit Python

- Zugang zum HCI-Router WLAN

- Dieses Repository (Firmware + UDP-Simulation)

1️⃣ Firmware auf den ESP32 flashen

- Repository klonen oder herunterladen.

- Ordner /firmware mit PlatformIO oder Arduino IDE öffnen.

Falls nötig:

- Boardtyp einstellen (z. B. ESP32 Dev Module)

- WLAN-SSID/Passwort im Code anpassen.

- Code auf den ESP32 flashen.

- ESP32 mit Strom verbinden (USB, Netzteil oder Powerbank).

- Nach dem Flashen wartet der ESP32 darauf, dass der Laptop eine START-Nachricht sendet.

2️⃣ Laptop mit dem HCI-Router verbinden

Laptop ins HCI-Router WLAN einloggen.

Der ESP32 muss sich im gleichen Netz befinden.

Stelle sicher, dass die Firewall UDP-Pakete zulässt.

3️⃣ UDP-Simulation starten

- Terminal öffnen.

- In den Ordner /simulator wechseln.

- Simulation starten:

   - python sendToPrototype.py


Der Laptop sendet nun eine START-Nachricht an den ESP32.

4️⃣ Verhalten des Prototyps
✨ Initialer Zustand

Nach dem Empfang der START-Nachricht:

Der LED-Streifen leuchtet rot

Der ESP32 sendet seinen Status als UDP-Nachricht zurück (BLOCK)

🔘 Zustandswechsel per Button

Der Button steuert die drei Privacy-Modi:

Aktion	LED-Farbe	Zustand	UDP-Nachricht
1× drücken	Gelb	Setup	SETUP
2× drücken	Grün	Allow	ALLOW
weiterer Druck / Zyklus	Rot	Block	BLOCK

Die LED zeigt immer den aktuellen Privacy-Status an.
Alle Änderungen werden per UDP zurück an den Laptop gesendet.


