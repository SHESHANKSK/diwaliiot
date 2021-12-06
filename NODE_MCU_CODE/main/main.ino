#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

//Set Name of your wifi and Password here
const char *wifiName = "wifiname";
const char *wifiPass = "password";
WiFiClient wifiClient;

int rgb_red = 5;
int rgb_blue = 4;
int rgb_green = 0;

const char *host = " Enter the Side Address where you will deploy followed by / api";
//Example
//const char *host = "http://diwaliiot.herokuapp.com/api";

void setup()
{

  Serial.begin(115200);
  delay(10);
  Serial.println();

  Serial.print("Connecting to ");
  Serial.println(wifiName);

  WiFi.begin(wifiName, wifiPass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); //You can get IP address assigned to ESP

  pinMode(rgb_red, OUTPUT);
  pinMode(rgb_blue, OUTPUT);
  pinMode(rgb_green, OUTPUT);
}

void loop()
{
  HTTPClient http; //Declare object of class HTTPClient
  Serial.print("Request Link:");
  Serial.println(host);
  http.begin(wifiClient, host);      //Specify request destination
  int httpCode = http.GET();         //Send the request
  String payload = http.getString(); //Get the response payload from server
  Serial.print("Response Code:");    //200 is OK
  Serial.println(httpCode);          //Print HTTP return code
  Serial.print("Returned data from Server:");
  Serial.println(payload); //Print request response payload
  if (httpCode == 200)
  {
    // Allocate JsonBuffer
    // Use arduinojson.org/assistant to compute the capacity.
    const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + 60;
    DynamicJsonBuffer jsonBuffer(capacity);
    JsonObject &root = jsonBuffer.parseObject(payload);

    if (!root.success())
    {
      Serial.println(F("Parsing failed!"));
      return;
    }

    JsonObject &rgb_value = root["rgb_value"];
    int rgb_value_blue_value = rgb_value["blue_value"];
    int rgb_value_green_value = rgb_value["green_value"];
    int rgb_value_red_value = rgb_value["red_value"];

    analogWrite(rgb_red, 255 - rgb_value_red_value);
    analogWrite(rgb_green, 225 - rgb_value_green_value);
    analogWrite(rgb_blue, 225 - rgb_value_blue_value);

    // Parse JSON object
    // Decode JSON/Extract values
    Serial.println(F("Response:"));
    Serial.println(rgb_value_blue_value);
    Serial.println(rgb_value_green_value);
    Serial.println(rgb_value_red_value);
    root.printTo(Serial);
  }
  else
  {
    Serial.println("Error in response");
  }

  http.end();

  delay(500); //GET Data at every 0.5 seconds
}
