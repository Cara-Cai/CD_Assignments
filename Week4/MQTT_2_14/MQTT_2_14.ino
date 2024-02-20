#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>

WiFiClient wifi;
MqttClient mqtt(wifi);

//WiFi network info: ssid and password
const char wifi_ssid[] = "Verizon_R73FLF";
const char wifi_pass[] = "sleek4-yea-dare";

//MQTT broker info: url and port (1883 default for MQTT)
const char broker[] = "theyonetwork.cloud.shiftr.io";
const int  port = 1883;

//if needed: broker authentication credentials
const char mqtt_user[] = "theyonetwork";
const char mqtt_pass[] = "ConnDevSP24";

//the topic this device will publish messages to
const char pubTopic[] = "caaaaara";

float x, y, z;
int plusThreshold = 30, minusThreshold = -30;

void setup() {
  Serial.begin(9600);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    WiFi.begin(wifi_ssid, wifi_pass);
    delay(1000);
  }
  Serial.println("\nWiFi connected!");

  //give your device any name, to use for identification
  mqtt.setId("Cara's Arduino");

  //set mqtt credentials, if needed
  mqtt.setUsernamePassword(mqtt_user, mqtt_pass);

  if (!mqtt.connect(broker, port)) {
    //error codes
    //  -1: credentials rejected
    //  -2: can't connect to broker
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqtt.connectError());
    while (true) {}; //do nothing forever
  } else Serial.println("MQTT connected.");

  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");  
  Serial.println();
  Serial.println("Gyroscope in degrees/second");

}

const int sendInterval = 1000;
String message;  

void loop() {
  
   if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);
  }
  if(y > plusThreshold)
  {
    message = "Collision front";
    Serial.println(message);
    delay(500);
  }
  if(y < minusThreshold)
  {
    message = "Collision back";
    Serial.println(message);

    delay(500);
  }
  if(x < minusThreshold)
  {
    message = "Collision right";
    Serial.println(message);

    delay(500);
  }
    if(x > plusThreshold)
  {
    message = "Collision left";
    Serial.println(message);

    delay(500);
  }

   if (millis() % sendInterval < 10) {
    mqtt.beginMessage(pubTopic);
//    mqtt.print(millis());
    mqtt.print(message);
    mqtt.endMessage();
    delay(10);
  }
}
