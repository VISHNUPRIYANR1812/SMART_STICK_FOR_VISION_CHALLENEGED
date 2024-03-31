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
#include <WiFiClientSecure.h>
#include <TinyGPS++.h> 
#include <UniversalTelegramBot.h>
#define BOTtoken "6738608250:AAGVkVdpl7Kf1rYVQ7qKAuQqXBJNZNVlvvI"
#define CHAT_ID "1436137789"
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
const int buzzerPin = 5; 
const int moisturePin = 18;
const int trigPin = 13; // GPIO 12
const int echoPin = 12; // GPIO 13
bool systemInitialized = false;
#define RXD2 16
#define TXD2 17
HardwareSerial neogps(2);
TinyGPSPlus gps;
BlynkTimer timer;
#define INTERVAL 1000L
bool sosAlertSent = false; // Global variable to track SOS alert status

void sendSOSAlertToTelegram();

void setup() 
{
  pinMode(moisturePin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(115200);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  setupWiFi();
  Blynk.run();
  if (!systemInitialized)
  {
    sendInitMessageToTelegram();
    systemInitialized = true;
  }
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("neogps serial initialize");
  delay(10);
  timer.setInterval(INTERVAL, sendGps);
}

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
  String latLongString = "LAT : " + String(latitude, 6) + " : LONG : " + String(longitude, 6);
  Blynk.virtualWrite(V1, latLongString); 
  String mapLink = "http://maps.google.com/maps?q=" + String(latitude, 6) + "," + String(longitude, 6);
  Blynk.virtualWrite(V3, mapLink); 
}

BLYNK_WRITE(V0) 
{
  int value = param.asInt(); 
  if (value == HIGH && !sosAlertSent) 
  {
    digitalWrite(buzzerPin, HIGH); // Activate buzzer for SOS alert
    sendSOSAlertToTelegram();
    sosAlertSent = true; // Mark SOS alert as sent
  } 
  else 
  {
    digitalWrite(buzzerPin, LOW);
    sosAlertSent = false; // Reset SOS alert status for next cycle
  }
}

void sendSOSAlertToTelegram()
{
  String message = "Alerting person \nSoS Buzzer is Set ON";

  if (client.connect("api.telegram.org", 443))
  {
    bot.sendMessage(CHAT_ID, message, "Markdown");
    delay(10);
  }
}

void beepBuzzer(int beeps) // beep counter 
{
  for (int i = 0; i < beeps; i++) 
  {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
}

void checkDistance()
{
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; 
  if (distance < 40)  // set distance here
  { 
    beepBuzzer(2); // two beeps for 40cm distance range
    delay(2000);
  }
}

void checkMoisture()
{
  int moistureValue = digitalRead(moisturePin);
  if (moistureValue == LOW) 
  {
    beepBuzzer(3); 
    delay(1000); 
  }
}

void sendInitMessageToTelegram()
{
  String message = "System initialized and ready.";
  if (client.connect("api.telegram.org", 443))
  {
    bot.sendMessage(CHAT_ID, message, "Markdown");
    delay(10);
  }
}

void setupWiFi()
{
  Blynk.begin(auth, ssid, pass);
}

void loop() 
{
  Blynk.run();
  timer.run();
  checkDistance();
  checkMoisture();
}
