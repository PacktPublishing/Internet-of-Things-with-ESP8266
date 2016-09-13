// Import required libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aREST.h>

// Define motor pins
int motorOnePlus = 12;
int motorOneMinus = 13;
int motorOneEnable = 14;

int motorTwoPlus = 5;
int motorTwoMinus = 16;
int motorTwoEnable = 4;

// Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Create aREST instance
aREST rest = aREST(client);

// Unique ID to identify the device for cloud.arest.io
char* device_id = "40ep12";

// WiFi parameters
const char* ssid = "Jarex_5A";
const char* password = "connect1337";

// Functions
void callback(char* topic, byte* payload, unsigned int length);

// Declare functions to be exposed to the API
int stop(String command);
int forward(String command);
int left(String command);
int right(String command);
int backward(String command);

void setup()
{

  Serial.begin(1152000);
    
  // Set pins
  pinMode(motorOnePlus, OUTPUT);
  pinMode(motorOneMinus, OUTPUT);
  pinMode(motorOneEnable, OUTPUT);
   
  pinMode(motorTwoPlus, OUTPUT);
  pinMode(motorTwoMinus, OUTPUT);
  pinMode(motorTwoEnable, OUTPUT);

  // Set callback
  client.setCallback(callback);

  // Give name and ID to device
  rest.set_id(device_id);
  rest.set_name("robot");

  // Function to be exposed
  rest.function("forward", forward);
  rest.function("stop", stop);
  rest.function("right", right);
  rest.function("left", left);
  rest.function("backward", backward);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
   
   
}

void loop()
{
  
  // Connect to the cloud
  rest.handle(client);
   
}

// Function to control the motor
void setMotorOne(boolean forward, int motor_speed){
   digitalWrite(motorOnePlus, forward);
   digitalWrite(motorOneMinus, !forward);
   analogWrite(motorOneEnable, motor_speed);
}

// Function to control the motor
void setMotorTwo(boolean forward, int motor_speed){
   digitalWrite(motorTwoPlus, forward);
   digitalWrite(motorTwoMinus, !forward);
   analogWrite(motorTwoEnable, motor_speed);
}

// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  // Handle
  rest.handle_callback(client, topic, payload, length);

}

// Functions to control the robot
int stop(String command) {

  setMotorOne(true, 0);
  setMotorTwo(true, 0);
  
}

int forward(String command) {

  setMotorOne(true, 1000);
  setMotorTwo(true, 1000);
  
}

int backward(String command) {

  setMotorOne(false, 1000);
  setMotorTwo(false, 1000);
  
}

int left(String command) {

  setMotorOne(false, 700);
  setMotorTwo(true, 700);
  
}

int right(String command) {

  setMotorOne(true, 700);
  setMotorTwo(false, 700);
  
}
