import paho.mqtt.client as mqtt
import time

flag_connected = 0

def on_connect(client, userdata, flags, reason_code, properties):
    global flag_connected
    if reason_code == 0:
        flag_connected = 1
        print("Connected to MQTT server successfully")
        client_subscriptions(client)
    else:
        print(f"Failed to connect. Reason code: {reason_code}")

def on_disconnect(client, userdata, disconnect_flags, reason_code, properties):
    global flag_connected
    flag_connected = 0
    print("Disconnected from MQTT server")
    
# --- Callback functions ---

# Removed ESP sensor1

def callback_esp32_sensor1(client, userdata, msg):
    print('ESP sensor1 data: ', str(msg.payload.decode('utf-8')))

def callback_rpi_broadcast(client, userdata, msg):
    print('RPi Broadcast message:  ', str(msg.payload.decode('utf-8')))

# Added a dedicated callback for messages meant for your PC
def callback_pc_messages(client, userdata, msg):
    print('PC received message:  ', str(msg.payload.decode('utf-8')))

def callback_stm32_sensor2(client, userdata, msg):
    print('STM32_TEMP_PRES_Node:', str(msg.payload.decode('utf-8')))
def client_subscriptions(client):
    client.subscribe("esp32/sensor1")
    client.subscribe("rpi/broadcast")
    client.subscribe("pc/messages")
    client.subscribe("stm32/sensor2")
    print("Subscribed to topics.")

# Updated the client name to reflect this is running on your PC
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, "laptop_client")

client.on_connect = on_connect
client.on_disconnect = on_disconnect

# Map the topics to the correct functions
client.message_callback_add('esp32/sensor1', callback_esp32_sensor1)
client.message_callback_add('rpi/broadcast', callback_rpi_broadcast) #This is just to test with your own publish
client.message_callback_add('pc/messages', callback_pc_messages)
client.message_callback_add('stm32/sensor2', callback_stm32_sensor2)

# IMPORTANT: If you are running this script on your laptop, 
# change '127.0.0.1' to the actual IP address of your Raspberry Pi!
client.connect('127.0.0.1', 1883)

client.loop_start()

print("......client setup complete............")

while True:
    time.sleep(4)
    if flag_connected != 1:
        print("Waiting for MQTT connection to establish...")
