import socket

ESP32_IP = "192.168.8.123"  # IP vom ESP32 aus Serial Monitor
ESP32_PORT = 4210

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.settimeout(5)

# Kein bind() nötig – OS wählt Port automatisch
sock.sendto(b"START", (ESP32_IP, ESP32_PORT))
print("Signal 'START' gesendet an ESP32")

while True:
    try:
        data, addr = sock.recvfrom(1024)
        print("Antwort vom ESP32:", data.decode())
    except socket.timeout:
        print("Keine Antwort, warte weiter...")
