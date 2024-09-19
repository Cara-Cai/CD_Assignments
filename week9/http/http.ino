
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>

// #define SECRET_SSID "sandbox370"
// #define SECRET_PASS ""

#define SECRET_SSID "sandbox370"
#define SECRET_PASS ""

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

float x, y, z;
int plusThreshold = 30, minusThreshold = -30;
String message;  


char serverAddress[] = "165.227.182.85";  //server IP address
int port = 1880; //use HTTP default port 80, or use 1880 for node-red

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);                   

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    delay(1000);
  }

  Serial.println("WiFi connected.");

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

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

void loop() {
  Serial.println("making POST request");

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

  //if sending raw text:
 String contentType = "text/plain";
 String postData = message; //to send a string, or
//  String postData = String(123.456); //to send a number

  //if sending a formatted JSON object (useful for updating multiple values)
  // String contentType = "application/json"; //if sending a formatted JSON object
  // String postData = "{\"deviceID\": 1, \"value\": VALUE}"; //use single-quotes inside a JSON object literal
  // postData.replace("VALUE", String(millis())); //replace millis() with your own sensor data
  
  char endpoint[] = "/messages";
  client.post(endpoint, contentType, postData); //use client.put() to make a put request

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}

