/*
 * Include libraries
 */

#include <WiFi.h>
#include <PubSubClient.h>

/*
 * defining pins for sensor
 */
 
int sensorVal;
int sensorPin = 36;


/*
 *  Define the wireless network credentials to connect your esp32 board to the network, and initialize the WiFi client library.
 */

const char* ssid = "Qeslanet";  // Enter SSID here
const char* pass = "BenignAlgorithm";  //Enter Password here
char mqttUserName[] = "MQTTtest01";  // Use any name.
char mqttPass[] = "VSN0XKBJRX54R3BD"; // Change to your MQTT API Key from Account > MyProfile.   
char writeAPIKey[] = "PALYDHGZFOO9FVG2"; // Change to your channel write API key.
long channelID = 1383134; // Change to your channel ID.

static const char alphanum[] ="0123456789"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                              "abcdefghijklmnopqrstuvwxyz";  // For random generation of client ID.


WiFiClient client;  // Initialize the Wi-Fi client library


/*
 * Create a partially initialized PubSubClient instance, and define the ThingSpeak MQTT broker.
 */

PubSubClient mqttClient(client); // Initialize the PubSubClient library.
const char* server = "mqtt.thingspeak.com"; 


/*
 * Define other global variables to track the last connection time and to define the time interval to publish the data.
 */

unsigned long lastConnectionTime = 0; 
const unsigned long postingInterval = 20 * 1000; // Post data every 20 seconds.


/*
 * Initialize serial data transfer, connect to wireless network, and set the MQTT broker details in the setup method
 */

void setup() {
  
  Serial.begin(9600);
  int status = WL_IDLE_STATUS;  // Set temporary Wi-Fi status.

  // Attempt to connect to Wi-Fi network.
  while (status != WL_CONNECTED) 
  {
    status = WiFi.begin(ssid, pass); // Connect to WPA/WPA2 Wi-Fi network.
    delay(5000);
  }

  Serial.println("Connected to wifi");
  mqttClient.setServer(server, 1883);   // Set the MQTT broker details.
}


/*
 * Establish an MQTT connection, and publish data to the channel at regular time intervals in the loop method.
 */

void loop() {

 // Reconnect if MQTT client is not connected.
  if (!mqttClient.connected()) 
  {
    reconnect();
  }

  mqttClient.loop();   // Call the loop continuously to establish connection to the server.

  // If interval time has passed since the last connection, publish data to ThingSpeak.
  if (millis() - lastConnectionTime > postingInterval) 
  {  
     mqttPublishFeed();; // Use this function to publish to a  field
  }
}


/*
 * Define the reconnect method to connect the Arduino client with the MQTT broker.
 */
void reconnect() 
{
  char clientID[9];

  // Loop until reconnected.
  while (!mqttClient.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Generate ClientID
    for (int i = 0; i < 8; i++) {
        clientID[i] = alphanum[random(51)];
    }
    clientID[8]='\0';

    // Connect to the MQTT broker.
    if (mqttClient.connect(clientID,mqttUserName,mqttPass)) 
    {
      Serial.println("connected");
    } else 
    {
      Serial.print("failed, rc=");
      // Print reason the connection failed.
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}



/*
 * Define the mqttPublishField method to publish the sensor data to a ThingSpeak channel field.
 */




void mqttPublishFeed() {
  
  sensorVal = analogRead(sensorPin); // Read value from ldr
  
  // Create data string to send to ThingSpeak.
  String data = String("field1=") + String(sensorVal, DEC);// + "&field2=" + String(sensorVal, DEC);
  int length = data.length();
  const char *msgBuffer;
  msgBuffer=data.c_str();
  Serial.println(msgBuffer);
  
  // Create a topic string and publish data to ThingSpeak channel feed. 
  String topicString = "channels/" + String( channelID ) + "/publish/"+String(writeAPIKey);
  length = topicString.length();
  const char *topicBuffer;
  topicBuffer = topicString.c_str();
  mqttClient.publish( topicBuffer, msgBuffer );
  lastConnectionTime = millis();
}
