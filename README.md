# 🧒📍 Child Monitoring and Safety System

This project is an IoT-based **Child Monitoring System** designed to enhance child safety by using GPS tracking, sound detection, and emergency alert features. It enables real-time location tracking and sends alerts via SMS or call when certain conditions are triggered.

## 🎯 Objective

To develop a portable and reliable embedded system that helps parents or guardians monitor their child's location and respond instantly in case of emergencies.

---

## 🔧 Technologies and Components Used

| Hardware Components            | Purpose                                      |
|-------------------------------|----------------------------------------------|
| Arduino Nano                  | Main microcontroller                         |
| GPS Neo-6M Module             | Real-time location tracking                  |
| GSM SIM800L Module            | Sends SMS and makes calls for alerts         |
| Sound Sensor Module           | Detects loud sounds indicating distress      |
| Push Buttons                  | Trigger manual emergency alerts              |
| 9V Battery / Power Bank       | Power supply for the circuit                 |

| Software & Tools              | Role                                         |
|------------------------------|----------------------------------------------|
| Arduino IDE                  | Code development and serial monitoring       |
| Embedded C                   | Programming logic                            |
| AT Commands                  | Communication with GSM module                |

---

## 🧠 Features

- 📍 **Real-time GPS Tracking**: Continuously tracks the child's location and can send it via SMS.
- 📢 **Sound Detection**: Automatically sends an alert if a sudden loud noise is detected (e.g., a scream).
- 🆘 **Manual Emergency Button**: Child can press a button to send emergency SMS or call guardians.
- 📲 **SMS and Call Alerts**: Sends formatted location via SMS and makes a call to the guardian's number.

