// Import required libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aREST.h>
#include <SHT1x.h>

// Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Pins
#define dataPin  4
#define clockPin 5

// Create aREST instance
aREST rest = aREST(client);

// Create instance for the sensor
SHT1x sht1x(dataPin, clockPin);

// Unique ID to identify the device for cloud.arest.io
char* device_id = "gveie2y5";

// WiFi parameters
const char* ssid = "your-wifi";
const char* password = "your-password";

// Variables to be exposed to the API
float temperature;
float humidity;

// Functions
void callback(char* topic, byte* payload, unsigned int length);

void setup(void)
{
  
  // Start Serial
  Serial.begin(115200);

  // Set callback
  client.setCallback(callback);

  // Init variables and expose them to REST API
  rest.variable("temperature", &temperature);
  rest.variable("humidity", &humidity);
  
  // Give name and ID to device
  rest.set_id(device_id);
  rest.set_name("gardening");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
}

void loop() {

  // Read values from the sensor
  temperature = sht1x.readTemperatureC();
  humidity = sht1x.readHumidity();

  // Connect to the cloud
  rest.handle(client);

}

// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  // Handle
  rest.handle_callback(client, topic, payload, length);

}
