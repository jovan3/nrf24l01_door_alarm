#include <ArduinoMqttClient.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <ESP8266WiFi.h>

// WiFi Configuration
#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

// MQTT Configuration
#ifndef MQTT_BROKER
#define MQTT_BROKER ""
#define MQTT_PORT 1883
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#endif

const char broker[] = MQTT_BROKER;
int        port     = MQTT_PORT;
const char mqttUser[] = MQTT_USERNAME;
const char mqttPass[] = MQTT_PASSWORD;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

RF24 radio(2, 4); // CE, CSN
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  mqttClient.setUsernamePassword(mqttUser, mqttPass);
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("MQTT connected");

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  mqttClient.poll();

  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);

    /*  The received data is in the following format:
     *  "some_identifier/current_state"
     *  The first part will be used as mqtt topic.
     */
    char* parts[2];
    char* token = strtok(text, "/");
    byte index = 0;
    while (token != NULL) {
      parts[index] = token;
      index++;
      token = strtok(NULL, "/");
    }

    /*  Publish a message on the topic only if the
     *  received message is valid and has been split
     *  in two parts: topic and state.
     */
    if (index == 2) { 
      Serial.println("Sending mqtt data...");
      mqttClient.beginMessage(parts[0]);
      mqttClient.print(parts[1]);
      mqttClient.endMessage();
    }
  }
  delay(5);
}
