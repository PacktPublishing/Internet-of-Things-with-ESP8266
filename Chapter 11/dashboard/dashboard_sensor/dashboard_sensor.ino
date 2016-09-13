// Import required libraries
#include "ESP8266WiFi.h"
#include <PubSubClient.h>
#include <aREST.h>
#include "DHT.h"

// DHT11 sensor pins
#define DHTPIN 5
#define DHTTYPE DHT11

// Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Unique ID to identify the device for cloud.arest.io
char* device_id = "gveie2y5";

// Create aREST instance
aREST rest = aREST(client);

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

// WiFi parameters
const char* ssid = "wifi-name";
const char* password = "wifi-pass";

// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Variables to be exposed to the API
float temperature;
float humidity;

void setup(void)
{  
  // Start Serial
  Serial.begin(115200);
  
  // Init DHT 
  dht.begin();

  // Set callback
  client.setCallback(callback);
  
  // Init variables and expose them to REST API
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);
    
  // Give name and ID to device
  rest.set_id(device_id);
  rest.set_name("sensor");
  
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
  
}

void loop() {
    
  // Reading temperature and humidity
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  // Handle REST calls
  rest.handle(client);
 
}

// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  // Handle
  rest.handle_callback(client, topic, payload, length);

}
