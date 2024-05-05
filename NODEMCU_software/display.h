#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

int lcdColumns = 20;
int lcdRows = 4;
LiquidCrystal_I2C lcd(0x27, lcdColumns,lcdRows);
char message[20]; 
int displaycount = 0;




void testdisplay(){
  
}

//funzione di inizializzazione del display
void initializeDisplay(){
 lcd.init();
 lcd.backlight();
 lcd.setCursor(0, 0);  
 lcd.init();
  
  lcd.backlight();
  
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0, 1);
  lcd.print("ACCENSIONE IN CORSO");
  lcd.setCursor(0,2);
  lcd.print("..PROTOTIPO IOT..");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
}
void clear_display(){
  lcd.clear();
}
void hotspot_display(IPAddress ip){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HOTSPOT ATTIVO");
  lcd.setCursor(0,1);
  lcd.print("Server: ");
  lcd.print(ip);
  lcd.setCursor(0,2);
  lcd.print("Collegarsi, inserire");
  lcd.setCursor(0,3);
  lcd.print("SSID e PASSWORD WIFI");
}

void connecting_display(){
  lcd.setCursor(0,0);
  lcd.print("Connessione...");
}

void connected_display(){
  lcd.setCursor(0,1);
  lcd.print("CONNESSO ALLA RETE");
}
void notconnected_display(){
  lcd.setCursor(0,1);
  lcd.print("NON CONNESSO");
  lcd.setCursor(0,2);
  lcd.print("MISURAZIONE OFFLINE");
}

void wifiStatus_display(bool connected){
  lcd.setCursor(0, 0);
  lcd.print("WiFi:");
  if(connected){
    lcd.setCursor(5,0);
    lcd.print("OK");
  }
  else {
    lcd.setCursor(5,0);
    lcd.print("NO");
  }
}

void battery_display(){
  lcd.setCursor(8,0);
  lcd.print("Batteria:90%");
}

void data_display(float t, float h){
  lcd.setCursor(0,1);
  lcd.print(t);
  lcd.print("C ");
  lcd.print(h);
  lcd.print("%");
}

void goodParam_display(){
  lcd.setCursor(0,2);
  lcd.print("                   ");
  lcd.setCursor(0,2);
  lcd.print("Valori nella norma");
}
void badParam_display(){
  lcd.setCursor(0,2);
  lcd.print("Valori non conformi");
}
void confOn_display(){
  lcd.setCursor(0,3);
  lcd.print("Configurazione: ON");
}
void confOff_display(){
  lcd.setCursor(0,3);
  lcd.print("Configurazione: OFF");
}
