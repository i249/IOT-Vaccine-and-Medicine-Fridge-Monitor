#include <LiquidCrystal.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// 1. Wokwi Virtual Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// 2. Telegram Bot Credentials
const String BOT_TOKEN = "enter your bot token";
const String CHAT_ID = "enter chat id";

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// 3. LCD & Hardware Pins
LiquidCrystal lcd(22, 23, 5, 18, 19, 21);
const int sensorPin = 35; 
const int greenLed = 12;   
const int redLed = 14;     
const int buzzer = 27;    

// THE MAGIC LOCK: This tracks the current state to prevent spam
bool currentlyInDanger = false; 

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  // CONNECT TO VIRTUAL WI-FI
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  client.setInsecure(); 
  
  lcd.clear();
  lcd.print("WiFi Connected!");
  delay(1000);
  lcd.clear();
}

void loop() {
  // Read the NTC Temp Sensor
  int analogValue = analogRead(sensorPin);
  const float BETA = 3950; 
  float tempC = 1 / (log(1 / (4095. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
  
  // Update LCD Screen
  lcd.setCursor(0, 0);
  lcd.print("Temp: "); 
  lcd.print(tempC); 
  lcd.print(" C   "); 

  // --- SYSTEM LOGIC ---
  if (tempC >= 2.0 && tempC <= 8.0) {
    
    // 1. LOCAL HARDWARE (SAFE)
    digitalWrite(greenLed, HIGH);  
    digitalWrite(redLed, LOW);     
    noTone(buzzer); 
    lcd.setCursor(0, 1);
    lcd.print("STATUS: SAFE    ");
    
    // 2. IOT NOTIFICATION LOGIC (Only trigger if we WERE in danger)
    if (currentlyInDanger == true) {
      Serial.println("Fridge has returned to safe temperatures. Sending update...");
      
      String safeMessage = "✅ UPDATE: Fridge is SAFE ✅\n";
      safeMessage += "Temperature has returned to: ";
      safeMessage += String(tempC);
      safeMessage += " °C.";
      
      bot.sendMessage(CHAT_ID, safeMessage, "");
      Serial.println("Safe Telegram sent!");
      
      currentlyInDanger = false; // Reset the lock
    }
    
  } else {
    
    // 1. LOCAL HARDWARE (DANGER)
    digitalWrite(greenLed, LOW);   
    digitalWrite(redLed, HIGH);    
    tone(buzzer, 1000); 
    lcd.setCursor(0, 1);
    lcd.print("ALERT: SPOIL!   ");
    
    // 2. IOT NOTIFICATION LOGIC (Only trigger if we WERE safe)
    if (currentlyInDanger == false) {
      Serial.println("Fridge is too warm/cold. Sending Alert...");
      
      String alertMessage = "🚨 SPOIL ALERT! 🚨\n";
      alertMessage += "Fridge Temperature is Critical: ";
      alertMessage += String(tempC);
      alertMessage += " °C\nPlease check the door immediately!";
      
      bot.sendMessage(CHAT_ID, alertMessage, "");
      Serial.println("Alert Telegram sent!");
      
      currentlyInDanger = true; // Engage the lock so it doesn't spam
    }
  }
  
  delay(1000); 
}