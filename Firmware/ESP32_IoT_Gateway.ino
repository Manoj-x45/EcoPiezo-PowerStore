#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/RTDBHelper.h"
#include <WiFiUdp.h>
#include <NTPClient.h>

// Wi-Fi Credentials
#define WIFI_SSID "EcoPiezo Powerstore"
#define WIFI_PASSWORD "Spectracore"

// Firebase Credentials
#define API_KEY "AIzaSyByWQeSDP61YHBDPYEhF5" // Replace with your actual Key if changed
#define DATABASE_URL "https://eco-piezo-powerstore-rtdb.firebaseio.com/"
#define FIREBASE_EMAIL "ewit@gmail.com"
#define FIREBASE_PASSWORD "Arjun18"

// Objects
FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800); // IST Offset

#define RXD2 16
#define TXD2 17
#define UPLOAD_INTERVAL 2000 // milliseconds
unsigned long lastUpload = 0;

void setupWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    Serial.print(".");
    delay(500);
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi connected: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nWi-Fi connection failed. Resetting ESP32...");
    ESP.restart();
  }
}

void setupFirebase() {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  
  // Assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback; 

  auth.user.email = FIREBASE_EMAIL;
  auth.user.password = FIREBASE_PASSWORD;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  if (!Firebase.ready()) {
    Serial.println("Firebase initialization failed. Resetting...");
    ESP.restart();
  } else {
    Serial.println("Firebase ready!");
  }
}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Communication with Arduino
  setupWiFi();
  
  // Initialize NTP
  timeClient.begin();
  while (!timeClient.update()) timeClient.forceUpdate();
  Serial.println("Time: " + timeClient.getFormattedTime());
  
  setupFirebase();
}

void loop() {
  timeClient.update();
  
  if (Serial2.available() && millis() - lastUpload > UPLOAD_INTERVAL) {
    lastUpload = millis();
    String line = Serial2.readStringUntil('\n');
    line.trim();
    
    int firstComma = line.indexOf(',');
    int secondComma = line.indexOf(',', firstComma + 1);
    
    if (firstComma > 0 && secondComma > 0 && Firebase.ready()) {
      float voltage = line.substring(0, firstComma).toFloat();
      String status = line.substring(firstComma + 1, secondComma);
      int timeLeft = line.substring(secondComma + 1).toInt();
      String currentTime = timeClient.getFormattedTime();
      
      // Create JSON object
      FirebaseJson json;
      json.set("voltage", voltage);
      json.set("status", status);
      json.set("timeLeft", timeLeft);
      json.set("currentTime", currentTime);
      
      // Upload JSON to Firebase
      if (Firebase.RTDB.updateNode(&fbdo, "/EcoPiezo", &json)) {
         Serial.println("Uploaded JSON: " + line + " | Time: " + currentTime);
      } else {
         Serial.println("Firebase write failed: " + fbdo.errorReason());
      }
    } else {
      Serial.println("Invalid data format or Firebase not ready");
    }
  }
  delay(10);
}