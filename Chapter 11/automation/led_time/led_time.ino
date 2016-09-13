// Import required libraries
#include "ESP8266WiFi.h"
#include <PubSubClient.h>
#include <aREST.h>

// Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Unique ID to identify the device for cloud.arest.io
char* device_id = "6g37g4";

// Create aREST instance
aREST rest = aREST(client);

// WiFi parameters
const char* ssid = "wifi-name";
const char* password = "wifi-pass";

// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

void setup(void)
{  
  // Start Serial
  Serial.begin(115200);
 
  // Set callback
  client.setCallback(callback);
    
  // Give name and ID to device
  rest.set_id(device_id);
  rest.set_name("dimmer");
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.println(WiFi.localIP());

  // LED pin to output
  pinMode(5, OUTPUT);
  
}

void loop() {
  
  // Handle REST calls
  rest.handle(client);
 
}

// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  // Handle
  rest.handle_callback(client, topic, payload, length);

}
