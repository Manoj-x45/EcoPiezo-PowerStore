#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// LCD pin connections: rs=13, en=12, d4=8, d5=9, d6=10, d7=11
const int rs = 13, en = 12, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int Relay = 4; // Relay control pin
SoftwareSerial RfidSerial(6, 7); // RX=6, TX=7

bool charging = false;
int chargeTimeLeft = 0;
unsigned long lastSecond = 0;

void setup() {
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, LOW); // Keep relay OFF initially
  
  Serial.begin(9600);      // Serial communication with ESP32
  RfidSerial.begin(9600);  // RFID reader
  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Footstep Power");
  lcd.setCursor(0, 1);
  lcd.print("Generation...");
  delay(2000);
}

float readPiezoVoltage() {
  int raw = analogRead(A0);       // Read analog value from piezo sensor
  return (raw * 5.0) / 1023.0;    // Convert ADC value to voltage
}

void loop() {
  float voltage = readPiezoVoltage(); 
  
  // Check if RFID data is available
  if (RfidSerial.available() > 0) {
    String tag = RfidSerial.readStringUntil('\n');
    tag.trim(); // Remove extra spaces
    
    // Check Authorized RFID tag UID (Example: 4B00E1D0552F)
    if (tag == "4B00E1D0552F") { 
      if (!charging) {
        charging = true;
        chargeTimeLeft = 60; // Set charging time to 60 seconds
        digitalWrite(Relay, HIGH); // Turn ON relay
        
        lcd.clear();
        lcd.print("Charging Start");
        delay(1000);
      }
    }
  }
  
  // Charging Timer Logic
  if (charging) {
    if (millis() - lastSecond >= 1000) { // One second timer
      chargeTimeLeft--;
      lastSecond = millis();
      
      if (chargeTimeLeft <= 0) {
        charging = false;
        digitalWrite(Relay, LOW); // Turn OFF relay
        lcd.clear();
        lcd.print("Charging Over");
        delay(1000);
      }
    }
  }
  
  // Send Data to ESP32 via Serial
  // Format: Voltage,Status,TimeLeft
  Serial.print(voltage, 2);
  Serial.print(",");
  Serial.print(charging ? "Charging" : "Idle");
  Serial.print(",");
  Serial.println(chargeTimeLeft);
  
  // Update LCD Display
  lcd.clear();
  lcd.print("V:" + String(voltage, 2)); 
  lcd.setCursor(0, 1);
  if (charging) {
    lcd.print("Chg " + String(chargeTimeLeft) + "s");
  } else {
    lcd.print("Idle");
  }
  
  delay(1000); // Update every second
}