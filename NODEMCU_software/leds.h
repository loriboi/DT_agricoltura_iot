#define red D7
#define green D8

void initializeLeds(){
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
}

void greenon_led(){
  digitalWrite(green, HIGH);
  digitalWrite(red, LOW);
}

void greenoff_led(){
  digitalWrite(green, LOW);
  digitalWrite(red, LOW);
}

void greenblink_led(){
  delay(200);
  greenon_led();
  delay(200);
  greenoff_led();
}

void configurationmode_led(){
  digitalWrite(green, HIGH);
  digitalWrite(red, HIGH);
}

void statusok_led(){
  digitalWrite(green, HIGH);
  digitalWrite(red, LOW);
}

void statusbad_led(){
  digitalWrite(red, HIGH);
  digitalWrite(green, LOW);
}

void test_leds(){
  digitalWrite(green, HIGH);
  digitalWrite(red, LOW);
  delay(500);
  digitalWrite(green, LOW);
  digitalWrite(red, HIGH);
  delay(500);
  digitalWrite(green, HIGH);
  digitalWrite(red, LOW);
  delay(500);
  digitalWrite(green, LOW);
  digitalWrite(red, HIGH);
  delay(500);
  digitalWrite(red,LOW);
}