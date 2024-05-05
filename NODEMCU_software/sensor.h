#include <DHT.h>
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


void initializeSensor(){
dht.begin();
}

float hum(){
  float humidity;  
  humidity = dht.readHumidity();
  if (isnan(humidity)) {
    Serial.println("Errore nella lettura del sensore di temperatura e umidità");
    return 0.0;
  }
  return humidity;
}

float tem(){
  float temperature;
  temperature = dht.readTemperature();
  if (isnan(temperature)) {
    Serial.println("Errore nella lettura del sensore di temperatura e umidità");
    return 0.0;
  }
  return temperature;
}

void test_sensor(){
  Serial.println(hum());
  Serial.println(tem());
  delay(1000);
}