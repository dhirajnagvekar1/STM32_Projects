import paho.mqtt.client as mqtt
import time
import random

# CONFIGURATION
PI_IP = "10.10.33.196"  # <--- Change this to your Pi's IP
TOPIC = "pc/messages"

def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print("Connected to Raspberry Pi Broker!")
    else:
        print(f"Failed to connect. Code: {reason_code}")

# 1. Initialize Client (Version 2.0+)
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, "Laptop_Data_Source")
client.on_connect = on_connect

# 2. Connect to the Pi
client.connect(PI_IP, 1883, 60)

# 3. Start background networking
client.loop_start()

print(f"Publishing data to {TOPIC}... Press Ctrl+C to stop.")

try:
    while True:
        # Simulate some data (e.g., a random value or a status)
        data_value = random.randint(20, 30) 
        message = f"PC_Value: {data_value}"
        
        # 4. The actual Publish command
        result = client.publish(TOPIC, message, qos=1)
        
        # Check if sent
        status = result[0]
        if status == 0:
            print(f"Sent: {message}")
        else:
            print(f"Failed to send message to topic {TOPIC}")
            
        time.sleep(5) # Send every 5 seconds

except KeyboardInterrupt:
    print("Stopping publisher...")
    client.loop_stop()
    client.disconnect()