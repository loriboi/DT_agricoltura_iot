import paho.mqtt.client as mqtt
from pymongo import MongoClient
import datetime
import json

client = MongoClient('mongodb://localhost:27017/')
db = client['prototype']  
collection_data = db['prototype_data']
collection_alerts = db['prototype_alerts']

# Callback per i messaggi sul topic dei dati
def on_data_message(client, userdata, msg):
    message = json.loads(msg.payload)
    document = {
        'reported': message['reported'],
        'timestamp_misurazione': message['timestamp'],
        'timestamp': datetime.datetime.now()
    }
    collection_data.insert_one(document);
    print("\nMessaggio salvato su MongoDB (data):", document)

# Callback per i messaggi sul topic degli avvisi
def on_alerts_message(client, userdata, msg):
    message = json.loads(msg.payload)
        document = {
        'alert': message['alert'],
        'reported': message['reported'],
        'timestamp_misurazione': message['timestamp'],
        'timestamp': datetime.datetime.now()
    }
   
    collection_alerts.insert_one(document)
    print("\nMessaggio salvato su MongoDB (alerts):", document)

mqtt_client = mqtt.Client()

mqtt_client.message_callback_add("prototype_data_topic", on_data_message)
mqtt_client.message_callback_add("prototype_alerts_topic", on_alerts_message)

mqtt_client.connect("192.168.186.175", 1883, 60)

mqtt_client.subscribe("prototype_data_topic")
mqtt_client.subscribe("prototype_alerts_topic")

mqtt_client.loop_forever()
