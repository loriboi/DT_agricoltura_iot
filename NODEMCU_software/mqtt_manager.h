#include <ArduinoJson.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#ifdef USE_SUMMER_TIME_DST
uint8_t DST = 1;
#else
uint8_t DST = 0;
#endif
int8_t TIME_ZONE = +1; //ITALY +1 UTC

unsigned long lastMs = 0;
time_t now;
time_t nowish = 1510592825;
unsigned long previousMillis = 0;
const long interval = 5000;

char* mqtt_server = "192.168.186.175";
char* topic_data = "prototype_data_topic";
char* topic_alerts = "prototype_alerts_topic";
char* topic_commands = "prototype_commands_topic";
WiFiClient espClient;
PubSubClient client(espClient);

void initialize_mqtt(){
  client.setServer(mqtt_server, 1883);
}

String gettime(){
  struct tm timeinfo;
  now = time(nullptr);
  gmtime_r(&now, &timeinfo); 
  String y = String(timeinfo.tm_year);
  String m = String(timeinfo.tm_mon + 1);
  String h = String(timeinfo.tm_hour + 2);
  String mn = String(timeinfo.tm_min);
  String s = String(timeinfo.tm_sec);
  String final =y+"-"+m+"-"+h+"-"+mn+"-"+s;
  return final;
}

bool NTPConnect(void)
{
 configTime(TIME_ZONE * 3600, DST * 3600, "pool.ntp.org", "time.nist.gov");
 now = time(nullptr);
 while (now < nowish)
 {
 delay(500);
 Serial.print(".");
 now = time(nullptr);
 }
 Serial.println("done!");
 struct tm timeinfo;
 gmtime_r(&now, &timeinfo);
 Serial.print("Current time: ");
 Serial.print(asctime(&timeinfo));
 return true;
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connessione a MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void send_data(float t, float h){
  DynamicJsonDocument jsonBuffer(JSON_OBJECT_SIZE(5) + 200);
  JsonObject root = jsonBuffer.to<JsonObject>();
    root["timestamp"] = gettime();
    root["reported"] = String(t)+ "C" + String(h) + "%";
    Serial.printf("Sending [%s]: ", topic_data);
    serializeJson(root, Serial);
    Serial.println();
    char shadow[measureJson(root) + 1];
    serializeJson(root, shadow, sizeof(shadow));
    client.publish(topic_data, shadow);
}

void send_alert(float t, float h){
  DynamicJsonDocument jsonBuffer(JSON_OBJECT_SIZE(5) + 200);
  JsonObject root = jsonBuffer.to<JsonObject>();
  root["timestamp"] = gettime();
  root["alert"] = "Valori non a norma";
  root["reported"] = String(t)+ "C" + String(h) + "%";
  serializeJson(root, Serial);
  char shadow[measureJson(root) + 1];
  serializeJson(root, shadow, sizeof(shadow));
  client.publish(topic_alerts,shadow);
}
