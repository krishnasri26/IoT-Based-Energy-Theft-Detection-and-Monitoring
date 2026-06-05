#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// GSM
//SoftwareSerial gsm(11, 10); // RX, TX

// ESP8266 (use hardware serial or SoftwareSerial if needed)

// Pins
#define ACS1 A0
#define ACS2 A1
// #define VOLT A2

#define buzzer 13

float sensitivity = 0.185;

float offset1 = 2.5;
float offset2 = 2.5;
float voltageOffset = 2.5;

// -------- Calibration ----------
void calibrate() {
  float sum1 = 0, sum2 = 0, sumV = 0;

  for (int i = 0; i < 500; i++) {
    sum1 += analogRead(ACS1);
    sum2 += analogRead(ACS2);
    // sumV += analogRead(VOLT);
    delay(2);
  }

  offset1 = (sum1 / 500.0) * 5.0 / 1023.0;
  offset2 = (sum2 / 500.0) * 5.0 / 1023.0;
  voltageOffset = (sumV / 500.0) * 5.0 / 1023.0;
}

// -------- RMS Current ----------
float readCurrent(int pin, float offset) {
  float sum = 0;
  int samples = 200;

  for (int i = 0; i < samples; i++) {
    float v = analogRead(pin) * 5.0 / 1023.0;
    float c = (v - offset) / sensitivity;
    sum += c * c;
    delayMicroseconds(200);
  }

  float rms = sqrt(sum / samples);

  if (rms < 0.05) rms = 0;
  return rms;
}

//-------- RMS Voltage ----------
float readVoltage() {
  float sum = 0;
  int samples = 200;

  for (int i = 0; i < samples; i++) {
    float v = analogRead(220) * 5.0 / 1023.0;
    float realV = (v - voltageOffset) * 100; // adjust scaling
    sum += realV * realV;
    delayMicroseconds(200);
  }

  float rms = sqrt(sum / samples);

  if (rms < 1) rms = 0;
  return rms;
}

//-------- GSM Alert ----------
// void sendSMS(String msg) {
//   gsm.println("AT+CMGF=1");
//   delay(1000);
//   gsm.println("AT+CMGS=\"+91\"");
//   delay(1000);
//   gsm.println(msg);
//   delay(100);
//   gsm.write(26);
// }

// -------- Setup ----------
void setup() {
  Serial.begin(9600);
  //gsm.begin(9600);

  pinMode(buzzer, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.print("Calibrating...");
  calibrate();
  delay(2000);
}

// -------- Loop ----------
void loop() {

  float I1 = readCurrent(ACS1, offset1);
  float I2 = readCurrent(ACS2, offset2);
  float V = 220;

  float power1 = V * I1;
  float power2 = V * I2;

  float diff = I1 - I2;

  // LCD Display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("I1:");
  lcd.print(I1);
  lcd.print(" I2:");
  lcd.print(I2);

  // lcd.setCursor(0, 1);
  // lcd.print("P:");
  // lcd.print(power2);

  // Serial Monitor
  // Serial.print("Voltage: "); Serial.println(V);
  // Serial.print("I1: "); Serial.println(I1);
  // Serial.print("I2: "); Serial.println(I2);
  // Serial.print("Power: "); Serial.println(power1);
  // Serial.print("Power: "); Serial.println(power2);

  String status;

  if (diff > 0.10) {
    status = "THEFT";
    lcd.setCursor(0, 1);
    lcd.print("THEFT DETECTED!");
    digitalWrite(buzzer, HIGH);
    // sendSMS("Electricity Theft Detected!");
  } else {
    status = "NORMAL";
    lcd.setCursor(0, 1);
    lcd.print("Normal use");
    digitalWrite(buzzer, LOW);
  }

  Serial.println(String(I1) + "," +
               String(I2) + "," +
               String(V) + "," +
               String(power1) + "," +
               String(power2) + "," +
               status);

  // 🔁 Recalibrate every 1 minute
  static unsigned long lastCal = 0;
  if (millis() - lastCal > 60000) {
    calibrate();
    lastCal = millis();
  }

  delay(1000);
}