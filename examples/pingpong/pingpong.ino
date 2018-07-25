#include <SPI.h>
#include <WebSocketClient.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
WebSocketClient client;

void onOpen(WebSocketClient &ws) {
  Serial.println(F("Connected"));

  char message[] = "Hello from Arduino!";
  ws.send(TEXT, message, strlen(message));
}

void onClose(WebSocketClient &ws, const eWebSocketCloseEvent code, const char *reason, uint16_t length) {
  Serial.print(F("Disconnected: ")); Serial.print(code);
  Serial.print(F(" ")); Serial.println(reason);
}

void onMessage(WebSocketClient &ws, const eWebSocketDataType dataType, const char *message, uint16_t length) {
  switch (dataType) {
    case TEXT:
      Serial.print(F("Received: ")); Serial.println(message);
      break;
    case BINARY:
      Serial.println(F("Received binary data"));
      break;
  }
}

void onError(const eWebSocketError code) {
  Serial.print("Error: "); Serial.println(code);
}

void setup() {
  Serial.begin(57600);
  Serial.print(F("Initializing ... "));

  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Can't open ethernet device"));
    while (true) ;
  }

  Serial.print(F("Client IP: "));
  Serial.println(Ethernet.localIP());

  // ---

  client.setOnOpenCallback(onOpen);
  client.setOnCloseCallback(onClose);
  client.setOnMessageCallback(onMessage);

  if (!client.open("192.168.46.10", 3000)) {
    Serial.println(F("Connection failed!"));
    while (true) ;
  }
}

unsigned long previousTime = 0;

void loop() {
  client.listen();

  unsigned long currentTime = millis();
  if (currentTime - previousTime > 5000) {
    previousTime = currentTime;

    client.ping();
  }
}