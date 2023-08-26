#include <WiFiNINA.h>
#include <ThingSpeak.h> 
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

const int DHT_PIN = 2; // Digital pin connected to the DHT sensor
#define DHT_TYPE DHT22

char wifiSSID[] = "realme Narzo 10";  // WiFi Name
char wifiPass[] = "mayank@1818";                // WiFi Password

unsigned long channelID = 2252230;
const char *apiKey = "ODDDBDX91D5UOXPZ"; 

WiFiClient wifiClient;
DHT_Unified dhtSensor(DHT_PIN, DHT_TYPE);

void setup()
{
  Serial.begin(9600);
  connectToWiFi();
  ThingSpeak.begin(wifiClient);
  dhtSensor.begin();
}

void loop()
{
  delay(1000);

  // Read DHT22 values
  sensors_event_t sensorevent;
  dhtSensor.temperature().getEvent(&sensorevent);
  float temp = sensorevent.temperature;
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println("°C");

  float humid = sensorevent.relative_humidity;
  Serial.print("Humidity: ");
  Serial.print(humid);
  Serial.println("%");

  if (!isnan(temp) && !isnan(humid))
  {
    sendToThingSpeak(temp, humid);
  } 
  else 
  {
    Serial.println("Failed to read sensor values");
  }
}

void connectToWiFi() 
{
  while (WiFi.status()!= WL_CONNECTED) 
  {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(wifiSSID, wifiPass);
    delay(1000);
  }
  Serial.println("Connected to WiFi");
}

void sendToThingSpeak(float temp, float humid)
{
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, humid);

  int statuscode= ThingSpeak.writeFields(channelID, apiKey);

  if (statuscode == 200)
  {
    Serial.println("Data sent to ThingSpeak");
  } 
  else
  {
    Serial.println("Error while sending data to ThingSpeak");
  }
}