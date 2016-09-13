// Libraries
#include <ESP8266WiFi.h>
#include <SHT1x.h>

// WiFi parameters
const char* ssid = "wifi-name";
const char* password = "wifi-pass";

// Humidity threshold
float threshold = 30.00;

// Pins
#define dataPin  4
#define clockPin 5

// Variables to be measured
float temperature;
float humidity;

// Create instance for the sensor
SHT1x sht1x(dataPin, clockPin);

// IFTTT settings
const char* host = "maker.ifttt.com";
const char* eventName   = "alert";
const char* key = "ifttt-key";

void setup() {
  
  Serial.begin(115200);
  delay(10);


  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {

  // Read values from the sensor
  temperature = sht1x.readTemperatureC();
  humidity = sht1x.readHumidity();

  // Check humidity data
  if (humidity < threshold) {

    Serial.print("connecting to ");
    Serial.println(host);
  
    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }
  
    // We now create a URI for the request
    String url = "/trigger/";
    url += eventName;
    url += "/with/key/";
    url += key;
    
    Serial.print("Requesting URL: ");
    Serial.println(url);
    
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
    int timeout = millis() + 5000;
    while (client.available() == 0) {
      if (timeout - millis() < 0) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    
    Serial.println();
    Serial.println("closing connection");

    // Wait a long time before new alert
    delay(10 * 60 * 1000);
    
  }
 
}

