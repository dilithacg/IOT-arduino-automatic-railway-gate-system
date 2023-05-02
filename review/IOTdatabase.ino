#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#define WIFI_SSID "0452275489"
#define WIFI_PASSWORD "12345678#"
#define FIREBASE_HOST "railway-9efa5-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyD7DlCSE3WhAf7ZSsAala_UjDfxsspBMdM"
#define NTP_SERVER "pool.ntp.org"

#define trigPin1 3
#define echoPin1 2
#define trigPin2 6
#define echoPin2 7

FirebaseData firebaseData;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER);

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);    
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // Initialize NTP client
  timeClient.begin();
}

void loop() {
  // Read sensor data and store in variables
  long duration1, distance1, duration2, distance2;

  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1 / 2) / 29.1;
  Serial.print("Distance from sensor 1: ");
  Serial.print(distance1);
  Serial.println(" cm");

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2 / 2) / 29.1;
  Serial.print("Distance from sensor 2: ");
  Serial.print(distance2);
  Serial.println(" cm");

  // Get current time
  timeClient.update();
  String timestamp = timeClient.getFormattedTime();

  // Push data to Firebase
  Firebase.pushInt(firebaseData, "/distance1", distance1);
  Firebase.pushString(firebaseData, "/time1", timestamp);
  Firebase.pushInt(firebaseData, "/distance2", distance2);
  Firebase.pushString(firebaseData, "/time2", timestamp);
  
  // Wait for 5 seconds before taking the next reading
  delay(5000);
}
