
# IoT-Based Energy Theft Detection and Monitoring

## Overview

This project implements an IoT-Based Energy Theft Detection and Monitoring System using Arduino Uno, ESP8266, ACS712 Current Sensors, LCD Display, Blynk IoT, and Telegram Alerts.

The system continuously monitors current flow at two points in the electrical distribution line using two ACS712 current sensors. The first sensor measures the source-side current, while the second measures the consumer-side current. By comparing these values, the system detects possible electricity theft or unauthorized load connections.

Real-time electrical parameters are displayed on a 16x2 LCD and transmitted to the Blynk IoT platform for remote monitoring. If energy theft is detected, a buzzer is activated and an instant Telegram notification is sent to the authorized user.

For practical demonstration, an energy meter, two bulb holders, and switches are connected as loads to simulate normal consumption and illegal tapping conditions.

---

# Objectives

* Detect unauthorized electricity consumption.
* Monitor current and power remotely using IoT.
* Generate instant theft alerts.
* Display electrical parameters locally and remotely.
* Demonstrate smart energy management concepts.

---

# Hardware Components

| Component                        | Quantity    |
| -------------------------------- | ----------- |
| Arduino Uno                      | 1           |
| ESP8266 NodeMCU                  | 1           |
| ACS712 Current Sensor (5A/20A)   | 2           |
| 16x2 LCD Display with I2C Module | 1           |
| Energy Meter                     | 1           |
| Bulb Holder                      | 2           |
| Bulb                             | 2           |
| SPST Switch                      | 2           |
| Active Buzzer                    | 1           |
| Power Supply                     | 1           |
| Breadboard                       | 1           |
| Jumper Wires                     | As Required |

---

# System Architecture

```text
          Source Supply
                |
         ACS712 Sensor 1
                |
          Energy Meter
                |
          Distribution Line
                |
         ACS712 Sensor 2
                |
        -----------------
        |               |
     Switch 1       Switch 2
        |               |
      Bulb 1         Bulb 2
        |
      Consumer Load

ACS712 Data
      |
      v
 Arduino Uno
      |
      | Serial Communication
      |
      v
   ESP8266
      |
      +------------------+
      |                  |
      v                  v
 Blynk IoT         Telegram Bot

      ^
      |
  LCD Display
```

---

# Working Principle

### Normal Condition

1. Current Sensor 1 measures incoming current.
2. Current Sensor 2 measures outgoing current to authorized loads.
3. Both current readings remain nearly equal.
4. System displays "NORMAL" on LCD and Blynk Dashboard.

### Theft Condition

1. An unauthorized load is connected before Sensor 2.
2. Sensor 1 measures higher current than Sensor 2.
3. Current difference exceeds threshold (0.10A).
4. Arduino identifies a theft condition.
5. Buzzer activates.
6. LCD displays "THEFT DETECTED".
7. Blynk generates an event notification.
8. Telegram Bot sends an alert message.

---

# Hardware Connections

## ACS712 Sensors

| Sensor    | Arduino Pin |
| --------- | ----------- |
| ACS712 #1 | A0          |
| ACS712 #2 | A1          |

## LCD Display

| LCD Pin | Arduino Pin |
| ------- | ----------- |
| SDA     | A4          |
| SCL     | A5          |

## Buzzer

| Buzzer | Arduino Pin |
| ------ | ----------- |
| Signal | D13         |

## Arduino to ESP8266

| Arduino | ESP8266 |
| ------- | ------- |
| TX      | RX      |
| RX      | TX      |
| GND     | GND     |

---

# Software Used

* Arduino IDE
* ESP8266 Board Package
* Blynk IoT Platform
* Telegram Bot API
* LiquidCrystal_I2C Library
* SoftwareSerial Library
* UniversalTelegramBot Library
* ESP8266WiFi Library

---

# Data Sent to Blynk

## Virtual Pins

| Virtual Pin | Parameter           |
| ----------- | ------------------- |
| V0          | Source Current (I1) |
| V1          | Load Current (I2)   |
| V2          | Voltage             |
| V3          | Source Power        |
| V4          | Load Power          |
| V5          | Theft Status        |

---

# LCD Display

### Normal Condition

```text
I1:0.45 I2:0.43
Normal use
```

### Theft Condition

```text
I1:1.20 I2:0.50
THEFT DETECTED!
```

---

# Telegram Alert

```text
⚠️ Electricity Theft Detected!

I1: 1.20 A
I2: 0.50 A
Voltage: 220 V

Possible Unauthorized Connection Found.
```

---

# Project Flow

```text
Start
   |
Read Sensor 1 Current
   |
Read Sensor 2 Current
   |
Calculate Difference
   |
Difference > Threshold?
   |
  Yes -------------------- No
   |                        |
Activate Buzzer        Normal Status
LCD Alert              LCD Update
Blynk Event            Blynk Update
Telegram Message
   |
Repeat Monitoring
```

---

# Results

* Successfully measures source and load currents.
* Detects illegal electricity tapping.
* Displays real-time current values.
* Sends remote data to Blynk IoT.
* Generates Telegram alerts instantly.
* Provides audible warning using buzzer.

---

# Applications

* Smart Grid Monitoring
* Electricity Board Monitoring Systems
* Industrial Energy Management
* Residential Power Monitoring
* Utility Theft Prevention Systems
* Smart City Energy Infrastructure

---

# Future Enhancements

* Smart Energy Meter Integration
* GPS-Based Theft Localization
* Cloud Data Analytics
* Historical Consumption Reports
* AI-Based Theft Prediction
* Mobile Application Development

---

# Authors

Krishna Srivastava

Department of Electronics and Communication Engineering

---

# License

This project is intended for educational and research purposes.
