#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid     = "ssid";
const char *password = "password";

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);

void setup(){
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.print ( "\n-----Connected-----\n" );

  timeClient.begin();
}

void loop() {
  timeClient.update();
  int fix_Day = timeClient.getDay();
  int fix_Hours = timeClient.getHours();
  int fix_Minutes = timeClient.getMinutes();
  //int fix_Seconds = timeClient.getSeconds();
  fix_Hours = (fix_Hours + 7) % 24;

  Serial.print(daysOfTheWeek[fix_Day]);
  Serial.print(", ");
  Serial.print(fix_Hours);
  Serial.print(":");
  Serial.print(fix_Minutes);
  //Serial.print(":");
  //Serial.println(fix_Seconds);
  //Serial.println(timeClient.getFormattedTime());

  delay(60 * 1000);
}