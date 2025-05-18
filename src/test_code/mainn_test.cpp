#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <ESPAsyncTCP.h>
#include <WiFi.h>
#include <Servo.h>
#include <ArduinoJson.h>

// Constants
const char* ssid = "YourSSID";
const char* password = "YourPassword";
const int port = 80;
const int servo1Pin = 27;
const int servo2Pin = 26;
const int servo3Pin = 25;
const int servo4Pin = 33;
const int servo5Pin = 32;

// Global objects
AsyncWebServer server(port);
AsyncWebSocket webSocket("/ws");

// Servo objects
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// Handle WebSocket events
void onWebSocketEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("WebSocket client connected");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("WebSocket client disconnected");
  } else if (type == WS_EVT_DATA) {
    // Parse JSON message from client
    String message = String((char*)data);
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, message);

    // Update servo positions
    servo1.write(doc["servo1"]);
    servo2.write(doc["servo2"]);
    servo3.write(doc["servo3"]);
    servo4.write(doc["servo4"]);
    servo5.write(doc["servo5"]);

    // Send servo positions to all clients
    String response = "{\"servo1\":" + String(doc["servo1"]) +
                      ",\"servo2\":" + String(doc["servo2"]) +
                      ",\"servo3\":" + String(doc["servo3"]) +
                      ",\"servo4\":" + String(doc["servo4"]) +
                      ",\"servo5\":" + String(doc["servo5"]) + "}";
    webSocket.textAll(response);
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize servos
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);
  servo5.attach(servo5Pin);

  // Route for root page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Route for JS and CSS files
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Start WebSocket server
  webSocket.onEvent(onWebSocketEvent);
  server.addHandler(&webSocket);
