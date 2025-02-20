#include <WiFi.h>
#include <PubSubClient.h>

#include<DHT.h>
#include<Adafruit_Sensor.h>

WiFiClient wifiClient;

PubSubClient mqttClient(wifiClient);
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "PTIT_WIFI";
const char* password = "";

char*mqttServer = "broker.hivemq.com";
int mqttPort =1883;

void setupMQTT(){
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
}

void reconnect(){
  Serial.println("Connecting...");
  while (!mqttClient.connected()){
    Serial.println("Reconnecting...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if(mqttClient.connect(clientId.c_str())){
      Serial.println("Connected.");
      mqttClient.subscribe("esp32/message");
    }
  }
}

void setup() {
  Serial.begin (115200);
  WiFi.begin(ssid, password);
  while(WiFi.status()!= WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
    Serial.println("");   
    Serial.println("Connected to Wi-Fi");
    dht.begin();
    setupMQTT();
}

void loop(){
  if(!mqttClient.connected()) 
    reconnect();
  mqttClient.loop();
  long now=millis();
  long previous_time =0;

  if(now-previous_time >1000){
    previous_time = now;
    double temperature = dht.readTemperature();
    char tempString[8];
    dtostrf(temperature, 1,2, tempString);
    Serial.println("Temperature: ");
    Serial.println(tempString);
    mqttClient.publish("esp32/temp", tempString);
    double humidity = dht.readHumidity();
    char humString[8];
    dtostrf(humidity,1,2,humString);
    Serial.println("Humidity: ");
    Serial.println(humString);
    mqttClient.publish("esp32/hum", humString);
  }
}

void callback(char* topic, byte* message, unsigned int length){
  Serial.print("Callback-");
  Serial.print("Message: ");
  for(int i=0;i<length;i++){
    Serial.print((char)message[i]);
  }
}
