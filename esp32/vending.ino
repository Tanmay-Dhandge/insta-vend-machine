#include <WiFi.h>
#include <WiFiClientSecure.h>

#define MQTT_MAX_PACKET_SIZE 1024
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ── WiFi + MQTT ─────────────────────────────
const char* ssid        = "Tanmay";
const char* password    = "tnmy9416";

const char* mqtt_server = "366c887e88bf40c496821b5e646d3a12.s1.eu.hivemq.cloud";
const char* mqtt_user   = "hivemq.webclient.1773649961783";
const char* mqtt_pass   = "FklRd4q@2&7DUc,.AZx6";

#define TOPIC "vendbot/order"

// ── GPIO Mapping ───────────────────────────
#define PIN_A1   23
#define PIN_A2   22
#define PIN_B1   21
#define PIN_B2   19

// ── Slot → Pin Mapping (FIXED) ─────────────
int pinForSlot(const char* slotRaw) {

  String slot = String(slotRaw);
  slot.trim();
  slot.toUpperCase();   // 🔥 MATCH FRONTEND

  Serial.print("Processed Slot: ");
  Serial.println(slot);

  if (slot == "A1") return PIN_A1;
  if (slot == "A2") return PIN_A2;
  if (slot == "B1") return PIN_B1;
  if (slot == "B2") return PIN_B2;

  Serial.print("[ERROR] Unknown slot: ");
  Serial.println(slot);

  return -1;
}

// ── Timing per Slot ────────────────────────
int timeForSlot(const char* slotRaw) {

  String slot = String(slotRaw);
  slot.trim();
  slot.toUpperCase();

  if (slot == "A1") return 1100;
  if (slot == "A2") return 1200;
  if (slot == "B1") return 1150;
  if (slot == "B2") return 1150;

  return 1100;
}

// ── MQTT Objects ───────────────────────────
WiFiClientSecure espClient;
PubSubClient client(espClient);

unsigned long lastReconnect = 0;

// ── MQTT Callback ─────────────────────────
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.println("\n===== NEW ORDER =====");

  char json[length + 1];
  memcpy(json, payload, length);
  json[length] = '\0';

  Serial.println(json);

  StaticJsonDocument<1024> doc;
  if (deserializeJson(doc, json)) {
    Serial.println("[ERROR] JSON parse failed");
    return;
  }

  JsonArray items = doc["items"];

  for (JsonObject item : items) {

    const char* slot = item["slot"];
    int qty          = item["qty"];

    Serial.print("\nRAW SLOT: ");
    Serial.println(slot);

    int pin = pinForSlot(slot);
    int duration = timeForSlot(slot);

    Serial.printf("Pin: %d | Time: %d ms | Qty: %d\n", pin, duration, qty);

    if (pin == -1) continue;

    // ── DISPENSE LOOP ───────────────────
    for (int i = 0; i < qty; i++) {

      Serial.printf("Dispensing %d/%d\n", i + 1, qty);

      digitalWrite(pin, HIGH);

      unsigned long start = millis();
      while (millis() - start < duration) {
        client.loop();
      }

      digitalWrite(pin, LOW);

      delay(400);
    }

    Serial.println("DONE");
  }

  Serial.println("===== ORDER COMPLETE =====\n");
}

// ── Reconnect ─────────────────────────────
bool reconnect() {

  Serial.println("Connecting MQTT...");

  if (client.connect("esp32-vend", mqtt_user, mqtt_pass)) {
    Serial.println("Connected");
    client.subscribe(TOPIC);
    return true;
  }

  Serial.println("Failed");
  return false;
}

// ── Setup ─────────────────────────────────
void setup() {

  Serial.begin(115200);

  int pins[] = { PIN_A1, PIN_A2, PIN_B1, PIN_B2 };

  for (int p : pins) {
    pinMode(p, OUTPUT);
    digitalWrite(p, LOW);
  }

  WiFi.begin(ssid, password);
  Serial.print("WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected");

  espClient.setInsecure();

  client.setServer(mqtt_server, 8883);
  client.setCallback(callback);
  client.setBufferSize(1024);

  reconnect();
}

// ── Loop ──────────────────────────────────
void loop() {

  if (!client.connected()) {
    if (millis() - lastReconnect > 5000) {
      lastReconnect = millis();
      reconnect();
    }
  }

  client.loop();
}
