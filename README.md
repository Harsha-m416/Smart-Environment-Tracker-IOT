# Smart Environment Tracker – IoT Project

This repository contains the source code and configuration files for our academic IoT project titled **Smart Environment Tracker**. This project was completed as part of a university course and is intended strictly for educational purposes.

## 👨‍🏫 Academic Project Statement

> This project is built purely for **academic demonstration** as part of our IoT coursework. All code, configuration, and integrations are implemented for learning and presentation only. No sensitive or production-level data is involved.

---

## 👥 Team Members

- Harsha  
- Akash  
- Sravya

---

## 📌 Project Summary

We developed a smart environment monitoring system using an **ESP8266 NodeMCU** microcontroller connected with:
- A **DHT11** sensor to collect temperature and humidity data
- An **LDR** (light-dependent resistor) to measure light intensity

This sensor data is transmitted in real-time via **MQTT** using **HiveMQ**, and passed through **Telegraf** to be stored in **InfluxDB**. We used **Grafana** to visualize the data and created alert rules to send real-time notifications to **ntfy.sh** when specific conditions (e.g., high temperature) are met.

---

## 🔁 System Architecture

ESP8266 + DHT11 + LDR
↓
MQTT (HiveMQ)
↓
Telegraf
↓
InfluxDB
↓
Grafana
↓
via ntfy.sh



---

## 🧠 Features

- Real-time sensor data publishing via MQTT
- InfluxDB time-series storage through Telegraf
- Grafana dashboards for visualizing live data
- Comfort zone indicator using combined temperature and humidity logic
- Alert rule set up to notify on **ntfy.sh/heatalert** when temperature exceeds 30°C
- Collected over **2900+ sensor data points**

---

## 🛠️ Tools & Technologies

- ESP8266 (NodeMCU)
- DHT11 Temperature & Humidity Sensor
- LDR Light Sensor
- HiveMQ Cloud (MQTT Broker)
- Telegraf
- InfluxDB 2.x
- Grafana OSS
- ntfy Alerts via [ntfy.sh](https://ntfy.sh/heatalert)

---

## 🗂️ Folder Structure

Smart-Environment-Tracker-IOT/
├── arduino_code/ → Arduino (.ino) source code
├── telegraf_config/ → Telegraf MQTT-to-InfluxDB config
├── influxdb_setup/ → Setup notes for bucket and token
├── grafana_dashboards/ → Dashboard screenshots or JSON exports
├── alerting/ → ntfy alert setup instructions
├── README.md → Project documentation
├── .gitignore → File ignore rules

---

## ⚙️ How to Run the Project

1. Flash the Arduino `.ino` code from `arduino_code/` to ESP8266
2. Ensure `secrets.h` contains Wi-Fi, MQTT, and HiveMQ credentials
3. Start Telegraf using 'your' config:
./telegraf --config telegraf_config/telegraf.conf
4. Run InfluxDB and Grafana (via Podman/Docker)
5. Access Grafana at `http://localhost:3000`
6. Connect InfluxDB as a data source and build dashboards
7. Subscribe to alerts using `ntfy.sh/your_alert_name`

---

## 📺 Demo Video

🎥 You can watch our full project walkthrough here:  
👉 [https://youtu.be/k0PyUI4xx6s]

---

## 📄 License

This project is intended for **academic use only**.  
All source code and resources are shared for learning purposes as part of a university course.