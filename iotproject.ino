#ifdef ESP8266
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif

#include "secrets.h"
#include <PubSubClient.h>
#include <DHT.h>
#include <WiFiClientSecure.h>

// // WiFi Credentials
// const char* ssid = "your wifi username";
// const char* password = "your wifi password";

// // HiveMQ Cloud MQTT Broker details
// const char* mqtt_server = "your mqtt url";  // Replace with your actual broker URL
// const char* mqtt_username = "mqtt username";            // Replace with your MQTT username
// const char* mqtt_password = "your mqtt password";            // Replace with your MQTT password
// const int mqtt_port = 8883;                                  // SSL/TLS Port for HiveMQ

// GPIO pins for sensors
const int ldrPin = A0;   // LDR connected to A0 (analog input pin)
const int dhtPin = D1;   // DHT11 connected to D1 (digital input pin)
const int ledPin = D6;   // LED connected to D6

// Set up DHT11 sensor
#define DHTTYPE DHT11     // DHT11 sensor type
DHT dht(dhtPin, DHTTYPE);  // Initialize DHT sensor

WiFiClientSecure espClient;  // Secure WiFi Client
PubSubClient client(espClient);  // MQTT Client

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

// Set up Wi-Fi connection
void setup_wifi() {
  delay(10);
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected, IP address: ");
  Serial.println(WiFi.localIP());
}

// Connect to the MQTT Broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// Callback function when a message is received
void callback(char* topic, byte* payload, unsigned int length) {
  String incomingMessage = "";
  for (int i = 0; i < length; i++) {
    incomingMessage += (char)payload[i];
  }
  Serial.println("Message arrived: [" + String(topic) + "] " + incomingMessage);
}

// Publish message to MQTT
void publishMessage(const char* topic, String payload, boolean retained) {
  if (client.publish(topic, payload.c_str(), retained)) {
    Serial.println("Message published: [" + String(topic) + "] " + payload);
    digitalWrite(ledPin, HIGH);  // Turn on LED to indicate successful data publish
  } else {
    digitalWrite(ledPin, LOW);   // Turn off LED if publish fails
  }
}

void setup() {
  pinMode(ldrPin, INPUT);    // LDR as input
  pinMode(ledPin, OUTPUT);   // LED as output
  Serial.begin(115200);      // Start serial communication
  setup_wifi();              // Connect to Wi-Fi
  dht.begin();               // Initialize DHT sensor

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Set up SSL/TLS connection
  espClient.setInsecure();  // Disable SSL certificate verification for easier setup
}

void loop() {
  if (!client.connected()) {
    reconnect();  // Reconnect to the MQTT broker if not connected
  }
  client.loop();  // Maintain the MQTT connection

  // Read temperature and humidity from the DHT11 sensor
  float temp = dht.readTemperature();  // Temperature in Celsius
  float humidity = dht.readHumidity(); // Humidity in percentage

  // Check if any read failed
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Read light intensity from the LDR (Photoresistor)
  int lightLevel = analogRead(ldrPin);  // Get LDR value (0-1023)

  // Serialize the data into JSON format
  String payload = "{\"temperature\": " + String(temp) + ", \"humidity\": " + String(humidity) + ", \"light\": " + String(lightLevel) + "}";

  // Publish the data to the MQTT broker
  publishMessage("home/sensors", payload, true);  // Topic: home/sensors

  // Optional: Turn on LED when data is published (for visualization)
  digitalWrite(ledPin, HIGH);  
  delay(200);  // LED on for 200ms
  digitalWrite(ledPin, LOW);  
  delay(9800);  // Wait 1.8 seconds before sending next message
}