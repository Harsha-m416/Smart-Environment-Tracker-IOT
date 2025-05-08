import paho.mqtt.client as mqtt
import json
from influxdb_client import InfluxDBClient, Point, WriteOptions

# InfluxDB Setup
influxdb_url = "http://localhost:8086"  # InfluxDB URL (replace if needed)
influxdb_token = "Replace with your InfluxDB token"   # Replace with your InfluxDB token
influxdb_org = "Replace with your InfluxDB organization"     # Replace with your InfluxDB organization
influxdb_bucket = "Replace with your InfluxDB bucket" # Replace with your InfluxDB bucket

# MQTT Setup
mqtt_broker = "b6514f0afb2247619065c7fb675b663d.s1.eu.hivemq.cloud"
mqtt_port = 8883
mqtt_username = "your MQTT username"   # Replace with your MQTT username
mqtt_password = "your MQTT password"   # Replace with your MQTT password
mqtt_topic = "your MQTT topic"            # Replace with your MQTT topic

# Initialize InfluxDB Client
client = InfluxDBClient(url=influxdb_url, token=influxdb_token, org=influxdb_org)
write_api = client.write_api(write_options=WriteOptions(batch_size=1000))

# Callback when the MQTT client connects
def on_connect(client, userdata, flags, rc):
    print(f"Connected to MQTT Broker with result code {rc}")
    client.subscribe(mqtt_topic)

# Callback when a message is received from MQTT
def on_message(client, userdata, msg):
    print(f"Received message on topic {msg.topic}: {msg.payload.decode()}")

    # Parse the incoming message (assuming JSON format)
    try:
        data = json.loads(msg.payload.decode())
        temperature = data.get("temperature")
        humidity = data.get("humidity")
        light = data.get("light")

        # Prepare the data point for InfluxDB
        point = Point("sensor_data") \
            .tag("location", "room1") \
            .field("temperature", temperature) \
            .field("humidity", humidity) \
            .field("light", light)

        # Write data to InfluxDB
        write_api.write(bucket=influxdb_bucket, org=influxdb_org, record=point)
        print("Data written to InfluxDB")
    except Exception as e:
        print(f"Failed to parse or write data: {e}")

# Setup MQTT Client
client = mqtt.Client()
client.username_pw_set(mqtt_username, mqtt_password)
client.on_connect = on_connect
client.on_message = on_message

# Connect to the MQTT broker with TLS (if needed)
client.tls_set()  # Uncomment if your broker uses SSL/TLS
client.connect(mqtt_broker, mqtt_port, 60)

# Loop to keep the client connected and processing messages
client.loop_forever()
