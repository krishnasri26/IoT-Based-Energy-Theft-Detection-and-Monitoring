#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

char ssid[] = "wifi_name";
char pass[] = "wifi_password";

// Telegram credentials
#define BOT_TOKEN ""
#define CHAT_ID ""

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

bool theftSent = false;
String data = ""; //leave as it is

void setup() {
  Serial.begin(9600);

  // Start Blynk (handles WiFi automatically)
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Required for Telegram HTTPS
  client.setInsecure();
  client.setTimeout(15000);

  Serial.println("System Started...");

  // 🔥 TEST TELEGRAM MESSAGE (check once)
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Sending test Telegram message...");
    bool sent = bot.sendMessage(CHAT_ID, "✅ ESP8266 Connected!", "");
    
    if (sent) {
      Serial.println("Test message sent");
    } else {
      Serial.println("Test message FAILED");
    }
  }
}

void loop() {
  Blynk.run();

  // Read serial data
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {
      Serial.println("Received: " + data);
      processData(data);
      data = "";
    } else {
      data += c;
    }
  }
}

void processData(String d) {
  float I1, I2, V, P1, P2;
  char status[10];

  int parsed = sscanf(d.c_str(), "%f,%f,%f,%f,%f,%s",
                      &I1, &I2, &V, &P1, &P2, status);

  if (parsed < 6) {
    Serial.println("Parse Error");
    return;
  }

  Serial.println("Parsed OK");

  // Send to Blynk
  Blynk.virtualWrite(V0, I1);
  Blynk.virtualWrite(V1, I2);
  Blynk.virtualWrite(V2, V);
  Blynk.virtualWrite(V3, P1);
  Blynk.virtualWrite(V4, P2);

  // Theft detection
  if (String(status) == "THEFT") {
    Blynk.virtualWrite(V5, "THEFT DETECTED");
    Blynk.logEvent("theft_alert", "Electricity Theft Detected!");

    if (!theftSent) {
      Serial.println("Sending Telegram alert...");

      if (WiFi.status() == WL_CONNECTED) {
        bool sent = bot.sendMessage(
          CHAT_ID,
          "⚠️ Electricity Theft Detected!\n"
          "I1: " + String(I1) + "\n" +
          "I2: " + String(I2) + "\n" +
          "Voltage: " + String(V),
          ""
        );

        if (sent) {
          Serial.println("Telegram sent successfully");
          theftSent = true;
        } else {
          Serial.println("Telegram FAILED");
        }
      } else {
        Serial.println("WiFi not connected");
      }
    }

  } else {
    theftSent = false;
    Blynk.virtualWrite(V5, "NORMAL");
  }

  // Debug connection
  if (Blynk.connected()) {
    Serial.println("Blynk Connected!");
  } else {
    Serial.println("Blynk Not Connected!");
  }
}