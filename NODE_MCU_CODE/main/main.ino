#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

//Set Name of your wifi and Password here
const char *wifiName = "wifiname";
const char *wifiPass = "password";
WiFiClient wifiClient;

int rgb_red = 12;
int rgb_blue = 4;
int rgb_green = 14;

//Set the API end point here
//Example: const char *host = "http://diwaliiot.herokuapp.com/api";
const char *host = "http://yourapiendpoint/api";

void setup()
{

  Serial.begin(115200);
  delay(10);
  WiFi.begin(wifiName, wifiPass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println("WiFi connection established");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(rgb_red, OUTPUT);
  pinMode(rgb_blue, OUTPUT);
  pinMode(rgb_green, OUTPUT);
}

void loop()
{
  HTTPClient http;
  http.begin(wifiClient, host);
  int httpCode = http.GET();
  String payload = http.getString();
  Serial.println(httpCode); //Print the Response gode from the server
  Serial.println(payload);  //Print json data that we get from the API
  if (httpCode == 200)
  {
    const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + 60;
    DynamicJsonBuffer jsonBuffer(capacity);
    JsonObject &root = jsonBuffer.parseObject(payload);

    if (!root.success())
    {
      Serial.println(F("Parsing Unsuccessful!"));
      return;
    }

    JsonObject &rgb_value = root["rgb_value"];
    int rgb_value_blue_value = rgb_value["blue_value"];
    int rgb_value_green_value = rgb_value["green_value"];
    int rgb_value_red_value = rgb_value["red_value"];

    analogWrite(rgb_red, rgb_value_red_value);
    analogWrite(rgb_green, rgb_value_green_value);
    analogWrite(rgb_blue, rgb_value_blue_value);

    Serial.println(F("Response Received:"));
    Serial.println(rgb_value_blue_value);
    Serial.println(rgb_value_green_value);
    Serial.println(rgb_value_red_value);
    root.printTo(Serial);
  }
  else
  {
    Serial.println("Error");
  }

  http.end();

  delay(500); //GET Data after every 0.5 seconds
}
