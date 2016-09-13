// Import required libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aREST.h>

// Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Create aREST instance
aREST rest = aREST(client);

// Unique ID to identify the device for cloud.arest.io
char* device_id = "gveie2y5";

// WiFi parameters
const char* ssid = "wifi-ssid";
const char* password = "wifi-pass";

// IFTTT settings
const char* host = "maker.ifttt.com";
const char* eventName   = "lock_opened";
const char* key = "key";

// Functions
void callback(char* topic, byte* payload, unsigned int length);

// Lock status
bool lockStatus;
bool previousLockStatus;

void setup(void)
{
  
  // Start Serial
  Serial.begin(115200);

  // Set callback
  client.setCallback(callback);
  
  // Give name and ID to device
  rest.set_id(device_id);
  rest.set_name("door_lock");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  

  // Initial read of lock status
  lockStatus = digitalRead(5);
  previousLockStatus = lockStatus;
  
}

void loop() {

  // Connect to the cloud
  rest.handle(client);

  // Check status of lock
  lockStatus = digitalRead(5);

  // Check if status of door lock changed & lock is opened
  if (lockStatus != previousLockStatus && lockStatus == 0) {

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

    // Set previous status
    previousLockStatus = lockStatus;
    
  }

}

// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  // Handle
  rest.handle_callback(client, topic, payload, length);

}
