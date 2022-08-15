#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <stdlib.h> 

const char *ssid = "Home351_2.4G";
const char *password = "Home351Home351";

StaticJsonDocument<200> doc;
HTTPClient http;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");

  http.begin("https://mg-iot.siamimo.com/asgardhttpv2/api/v2/thing/report"); //Specify destination for HTTP request
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  { //Check WiFi connection status
    String payload;
    doc["S1"] = (float)random(0, 100) / 100.00;
    doc["S2"] = (float)random(0, 1000) / 100.00;
    doc["S3"] = (float)random(0, 10000) / 100.00;
    doc["S4"] = random(0, 100);
    doc["S5"] = random(0, 1000);
    doc["S6"] = random(0, 10000);
    doc["Latitude"] = (float)random(-9000, 9000) / 100.00000000;
    doc["Longtitude"] = (float)random(-18000, 18000) / 100.00000000;
    serializeJson(doc, payload);
    
    http.addHeader("accept", "application/json");                               //Specify content-type header
    http.addHeader("Authorization", "Bearer 9ad14a20-91cf-4264-aba8-663078cf05b7");
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0)
    {
      String response = http.getString(); //Get the response to the request

      Serial.println(httpResponseCode); //Print return code
      Serial.println(response);         //Print request answer
    }
    else
    {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    // http.end();
  }
  else
  {
    Serial.println("Error in WiFi connection");
  }

  delay(5000); //Send a request every 10 seconds
}