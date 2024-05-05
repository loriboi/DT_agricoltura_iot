#include "mqtt_manager.h"
#include "leds.h"
#include "sensor.h"
#include "display.h"
const char *ssidserver = "esp8266";     // SSID dell'hotspot WiFi
const char *passwordserver = "1234";  // Password dell'hotspot WiFi

float soglia_temp_max = 24.00;
float soglia_hum_max = 60.00;
float soglia_temp_min = 15.00;
float soglia_hum_min = 45.00;


char ssidwifi[32];
char passwordwifi[32];
String ssidwifi2;
String passwordwifi2;
bool configuration;
bool connected = false; 
void ICACHE_RAM_ATTR restart();
ESP8266WebServer server(80);  
bool ntpconnection = false;
float midtemp = 0;
float midhum = 0;
int counter = 0;


void handleRoot() {
 server.send(200, "text/html", 
  "<!DOCTYPE html>\
  <html lang='en'>\
  <head>\
    <meta charset='UTF-8'>\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
    <title>Form</title>\
    <style>\
      body {\
        font-family: Arial, sans-serif;\
        margin: 0;\
        padding: 20px;\
        background-color: #e0f2f1;\
        display: flex;\
        justify-content: center;\
        align-items: center;\
        height: 100vh;\
      }\
      form {\
        width: 300px;\
        background-color: #fff;\
        padding: 20px;\
        border-radius: 5px;\
        box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);\
      }\
      label {\
        display: block;\
        margin-bottom: 10px;\
      }\
      input[type='text'],\
      input[type='password'] {\
        width: calc(100% - 22px);\
        padding: 10px;\
        margin-bottom: 20px;\
        border: 1px solid #ccc;\
        border-radius: 5px;\
      }\
      input[type='submit'] {\
        width: 100%;\
        padding: 10px;\
        background-color: #007bff;\
        color: #fff;\
        border: none;\
        border-radius: 5px;\
        cursor: pointer;\
      }\
      input[type='submit']:hover {\
        background-color: #0056b3;\
      }\
    </style>\
  </head>\
  <form action='/submit' method='post'>\
      <label for='ssid'>SSID:</label>\
      <input type='text' name='ssid' id='ssid'>\
      <label for='password'>Password:</label>\
      <input type='password' name='password' id='password'>\
      <input type='submit' value='Invia'>\
    </form>\
  </body>\
  </html>");
}

void handleSubmit() {
  configuration = true;
  if (server.hasArg("ssid")) {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    Serial.print("Dati ricevuti: ");
    Serial.println(ssid);
    Serial.println(password);
    strcpy(ssidwifi, ssid.c_str());
    strcpy(passwordwifi, password.c_str());
    server.send(200, "text/html", "<!DOCTYPE html>\
    <html lang='en'>\
    <head>\
      <meta charset='UTF-8'>\
      <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
      <title>Form</title>\
      <style>\
        body {\
          font-family: Arial, sans-serif;\
          margin: 0;\
          padding: 20px;\
          background-color: #e0f2f1;\
          display: flex;\
          justify-content: center;\
          align-items: center;\
          height: 100vh;\
          text-align: center;\
        }\
        form {\
          width: 300px;\
          background-color: #fff;\
          padding: 20px;\
          border-radius: 5px;\
          box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);\
        }\    
    </style>\
    </head>\
    <form action='/submit' method='post'>\
      <h3>Dati ricevuti</h3>\
      <h3>Correttamente</h3>\
    </form>\
    </body>\
    </html>");


    Serial.println("Connessione alla rete WiFi...");
    ssidwifi2 = ssid;
    passwordwifi2 = password;
    unsigned long startTime;
    WiFi.begin(ssidwifi2, passwordwifi2);
    startTime = millis(); 
    clear_display();
    while (WiFi.status() != WL_CONNECTED) {
      greenon_led();
      connecting_display();
      // Verifica se è passato il timeout di 20 secondi
      if (millis() - startTime > 20000) {
        Serial.println("Impossibile collegarsi alla rete WiFi.");
        notconnected_display();
        statusbad_led();
        ssid = "";
        password = "";
        connected = false;
        server.close();
        WiFi.softAPdisconnect(true);
        
        configuration = false;
        delay(5000);
        clear_display();
        confOff_display();
        data_display(0.0,0.0);
        battery_display();
        badParam_display();
        return; 
      }
      
      greenblink_led();
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("Connesso alla rete WiFi!");
    Serial.print("Indirizzo IP assegnato: ");
    statusok_led();
    
    connected = true;

    server.close();
    Serial.println("Server Web chiuso");
    WiFi.softAPdisconnect(true);
    configuration = false;
    connected_display();
    delay(5000);
    clear_display();
    confOff_display();
    battery_display();
    badParam_display();
    data_display(0.0,0.0);
  } else {
    server.send(400, "text/plain", "Nessun dato ricevuto");
  }
}

void connectwifi(){
    WiFi.begin(ssidwifi2, passwordwifi2);
    unsigned long startTime = millis(); // Salva il tempo di inizio della connessione
    while (WiFi.status() != WL_CONNECTED) {
      if (millis() - startTime > 5000) {
        Serial.println("Impossibile collegarsi alla rete WiFi.");
        statusbad_led();
        connected = false;
        delay(500);
        return;
      }
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connesso alla rete WiFi!");
    Serial.print("Indirizzo IP assegnato: ");
    statusok_led();
    connected = true;
}

void sethotspot(){
  configuration = true;
  WiFi.softAP(ssidserver, passwordserver);
  Serial.println("Hotspot WiFi creato!");
  IPAddress ip = WiFi.softAPIP();
  Serial.print("Indirizzo IP dell'hotspot: ");
  Serial.println(ip);
  configurationmode_led();
  hotspot_display(ip);
  server.on("/", handleRoot);
  server.on("/submit", handleSubmit);
  server.begin();
  Serial.println("Server Web avviato");
}

bool checkSoglia(float t,float h){
  if(t>soglia_temp_max || h>soglia_hum_max || t<soglia_temp_min || h<soglia_temp_min){
    badParam_display();
    return false;
  }
  else{
    goodParam_display();
    return true;
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  initializeDisplay();
  initializeLeds();
  initializeSensor();
  test_leds();
  test_sensor();
  delay(5000);
  sethotspot();
  delay(5000);
  initialize_mqtt();
  
  pinMode(D6, INPUT_PULLUP); // Imposta il pin di interrupt come input con pull-up
  attachInterrupt(digitalPinToInterrupt(D6), restart, RISING); // Assegna la funzione di reset al pin di interrupt
}

void loop() {
  if(connected && !ntpconnection){
    ntpconnection = NTPConnect();
  }
  if(WiFi.status() != WL_CONNECTED){
    connected = false;
    statusbad_led();
  }
  if(configuration == false){
  wifiStatus_display(connected);
  }
  if(connected){
      if (!client.connected()) {
      reconnect();
    }
    client.loop();
  }
  if (connected == false && configuration == true && strlen(ssidwifi) == 0 && strlen(passwordwifi) == 0) {
    statusbad_led();
    server.handleClient();
  }
  midhum = midhum + hum();
  midtemp = midtemp + tem();
  counter = counter + 1;
  Serial.println(counter);
  if(counter == 30){
    float temp = midtemp/counter;
    float hum = midhum/counter;
    counter = 0;
    midhum = 0;
    midtemp = 0;
    Serial.println(temp);
    Serial.println(hum);
    if(configuration == false){
    data_display(temp,hum);
    bool status = checkSoglia(temp,hum);
    if(!connected){
        connectwifi();
    }
    if(client.connected()){
      send_data(temp,hum);
      if(!checkSoglia(temp, hum)){
        send_alert(temp,hum);
      }
    }
    }
  }
  delay(1000);
}

void ICACHE_RAM_ATTR restart() {
  ESP.restart();
}

