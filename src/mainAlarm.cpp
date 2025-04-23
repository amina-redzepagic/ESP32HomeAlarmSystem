#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi Credentials
const char* ssid = "Your WiFi name";
const char* password = "Your WiFi password";

// Telegram Bot Token
const char* botToken = "telegram bot token";

// Your Telegram Chat ID
const char* chatID = "telegram chat ID";

// Telegram API URL
String telegramApiUrl = "https://api.telegram.org/bot" + String(botToken) + "/sendMessage";

// Pins
const int pirPin = 13;     // PIR sensor pin
const int buzzerPin = 14;  // Buzzer pin
const int buttonPin = 26;  // Push button pin (using GPIO26 from your list)

// State flags
bool motionTriggered = false;   // For detecting rising edge on the PIR sensor
bool systemArmed = false;       // Alarm system armed/disarmed state
bool lastButtonState = HIGH;    // Button state (assumes internal pull-up: default HIGH)

void sendTelegramNotification(const String& message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = telegramApiUrl + "?chat_id=" + chatID + "&text=" + message;
    http.begin(url);
    int httpResponseCode = http.GET();
    Serial.print("Telegram Response Code: ");
    Serial.println(httpResponseCode);
    http.end();
  } else {
    Serial.println("WiFi not connected.");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);1

  // Configure push button with internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.println("Warming up PIR sensor...");
  delay(60000);  // Allow the PIR sensor to stabilize
  Serial.println("PIR sensor ready.");

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
}

void loop() {
  // Read push button state
  bool currentButtonState = digitalRead(buttonPin);
  // Detect button press (transition from HIGH to LOW)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    delay(50); // Debounce delay
    systemArmed = !systemArmed;  // Toggle system state
    if (systemArmed) {
      Serial.println("System armed.");
      sendTelegramNotification("🔒 Alarm system armed.");
    } else {
      Serial.println("System disarmed.");
      sendTelegramNotification("🔓 Alarm system disarmed.");
    }
  }
  lastButtonState = currentButtonState;

  // Only check the PIR sensor if the system is armed
  if (systemArmed) {
    int sensorState = digitalRead(pirPin);
    if (sensorState == HIGH && !motionTriggered) {
      motionTriggered = true;  // Prevent multiple triggers
      Serial.println("🚨 Motion detected!");
      digitalWrite(buzzerPin, HIGH);
      sendTelegramNotification("🚨 Motion detected at the front door!");
      delay(5000);  // Keep buzzer on for 5 seconds
      digitalWrite(buzzerPin, LOW);
      delay(10000);  // Wait a bit before rearming
    }
    if (sensorState == LOW && motionTriggered) {
      motionTriggered = false;
    }
  }

  delay(100);  // Polling delay
}
