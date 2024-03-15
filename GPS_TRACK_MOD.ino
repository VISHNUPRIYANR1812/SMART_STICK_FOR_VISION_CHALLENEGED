/*
    Author     : NIRANJAN & vishnu
    Target MCU : ESP-32 WROOM-DA-MODULE
*/
#define BLYNK_TEMPLATE_ID "TMPL3D8ap6CNe"
#define BLYNK_TEMPLATE_NAME "SMART STICK SYSTEM"
#define BLYNK_AUTH_TOKEN "cHjLW1JMyZTodOhC-jazo5U--o0PydfV"
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "TP-LINK_MR100_2.4Ghz"; // replace wifi name
char pass[] = "Vishnu@18122002"; // Replace passkey
#define BLYNK_PRINT Serial
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TinyGPS++.h> 
#define RXD2 16
#define TXD2 17
HardwareSerial neogps(2);
TinyGPSPlus gps;
BlynkTimer timer;
#define INTERVAL 1000L
void sendGps()
{
  while(neogps.available())
  {
    if (gps.encode(neogps.read()))
    {
      break;
    }
  }
  if (!gps.location.isValid())
  {
    Serial.println("Failed to read from GPS Module!");
    return;
  }
  float latitude = gps.location.lat();
  float longitude = gps.location.lng();
  float speed = gps.speed.kmph();
  Serial.print("Latitude:  ");
  Serial.println(latitude, 6);
  Serial.print("Longitude: ");
  Serial.println(longitude, 6);
  Serial.print("Speed: ");
  Serial.println(speed, 6);
  Blynk.virtualWrite(V1, String(latitude, 6));
  Blynk.virtualWrite(V2, String(longitude, 6));
  Blynk.virtualWrite(V0, String(speed));
  String mapLink = "http://maps.google.com/maps?q=" + String(latitude, 6) + "," + String(longitude, 6);
  Blynk.virtualWrite(V3, mapLink); 
}
void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("neogps serial initialize");
  delay(10);
  timer.setInterval(INTERVAL, sendGps);
}
void loop()
{
  Blynk.run();
  timer.run();
}
