#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

const char* ssid        = "Tanmay";
const char* password    = "tnmy9416";
const char* mqtt_server = "366c887e88bf40c496821b5e646d3a12.s1.eu.hivemq.cloud";
const char* mqtt_user   = "hivemq.webclient.1775540663302";
const char* mqtt_pass   = "7@1wDCrTa0jP<Wn>8yN,";

#define RELAY_PIN 5
#define TOPIC     "vendbot/order"

WiFiClientSecure espClient;
PubSubClient client(espClient);

unsigned long lastReconnect = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.println("\n----- MESSAGE RECEIVED -----");

  // Print topic
  Serial.print("Topic: ");
  Serial.println(topic);

  // Convert payload to string
  String msg = "";
  Serial.print("Raw Payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);   // print raw
    msg += (char)payload[i];          // build string
  }
  Serial.println();

  // Final parsed message
  Serial.print("Parsed Message: ");
  Serial.println(msg);

  Serial.println("----------------------------");

  // Relay logic
  if (msg.indexOf("COLA") >= 0 || msg.indexOf("LAYS") >= 0) {
    Serial.println("Triggering Relay...");
    digitalWrite(RELAY_PIN, HIGH);
    delay(2000);
    digitalWrite(RELAY_PIN, LOW);
  }
}

bool reconnect() {
  if (client.connect("esp32-vend", mqtt_user, mqtt_pass)) {
    client.subscribe(TOPIC);
    Serial.println("MQTT connected & subscribed");
    return true;
  }
  Serial.print("MQTT fail, rc=");
  Serial.println(client.state());
  return false;
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  espClient.setInsecure();  // dev mode

  client.setServer(mqtt_server, 8883);
  client.setCallback(callback);
  client.setKeepAlive(60);

  reconnect();
}

void loop() {
  if (!client.connected()) {
    unsigned long now = millis();
    if (now - lastReconnect > 5000) {
      lastReconnect = now;
      reconnect();
    }
  }
  client.loop();
}
