#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SimpleTimer.h>

char auth[] = "7fb8c29988504f5ab263b797a869827b";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Alexis's iPhone";
char pass[] = "lex166789";
char server[] = "elderaid.me";
int port = 8442;

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial

// Your ESP8266 baud rate:
#define ESP8266_BAUD  115200

#define LIMITE_GAS  300
#define ALARM_DELAY 1000L
#define ACTUATOR_DELAY  1500L

ESP8266 wifi(&EspSerial);

SimpleTimer timer;

int sensor_gas = A0;    // Leitura do sensor de Gas
int atuador_gas = 9;     // Atuador do sensor de gas (LED)
int buzzer_gas = 7;     // Buzzer
int valor_analogico = 0;
int nivel_sensor = 40;
int alarmID;
int actuatorID;
int alarmIDOn;
int actuatorIDOn;
int actuatorValue;

WidgetLED led7(V10);
WidgetLED led9(V12);

void(* resetFunc) (void) = 0;   // Software reset of Arduino

void setup()
{
  // Set console baud rate
  Serial.begin(115200);
  delay(10);
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(2000);

  led7.on();  
  led9.off();
  timer.setInterval(1000L, watchdog);
  timer.setInterval(1500L, blinkLED);
  alarmID = timer.setTimeout(ALARM_DELAY, alarmEnable);
  actuatorID = timer.setTimeout(ACTUATOR_DELAY, actuatorEnable);
  alarmIDOn = timer.setTimeout(ALARM_DELAY, alarmEnable);
  timer.disable(alarmID);
  timer.disable(actuatorID);
  timer.disable(alarmIDOn);

  pinMode(sensor_gas, INPUT);
  pinMode(atuador_gas, OUTPUT);
  pinMode(buzzer_gas, OUTPUT);
  Blynk.begin(auth, wifi, ssid, pass, server, port);
}

BLYNK_READ(V6)
{
  // Sensor de Gás
  valor_analogico = analogRead(sensor_gas);
  Blynk.virtualWrite(V6, valor_analogico);
}

BLYNK_WRITE(V8)
{
  // LED
  actuatorValue = param.asInt();
  digitalWrite(atuador_gas, actuatorValue);
}

void watchdog(){
  led7.on();
}

void blinkLED(){
  if(led9.getValue()){
    led9.off();
  }else{
    led9.on();
  }
}

void alarmEnable(){
  digitalWrite(buzzer_gas, HIGH);
}

void alarmDisable(){
  digitalWrite(buzzer_gas, LOW);
}

void actuatorEnable(){
  digitalWrite(atuador_gas, HIGH);
}

void actuatorDisable(){
  digitalWrite(atuador_gas, LOW);
}

void runOffline(){
  // Reads gas sensor and stores its value
  valor_analogico = analogRead(sensor_gas);
  // Verify gas leakage.
  if(valor_analogico >= LIMITE_GAS){
    // Detected gas leakage.
    if(!timer.isEnabled(alarmID)){
      // Set alarm and actuator to start after 10 and 15 seconds, respectively.
      alarmID = timer.setTimeout(ALARM_DELAY, alarmEnable);
      actuatorID = timer.setTimeout(ACTUATOR_DELAY, actuatorEnable);
      timer.enable(alarmID);
      timer.enable(actuatorID);
    }
  }else{
    // Gas leakage stopped or not detected
    if(!timer.isEnabled(alarmID)){
      // If alarm and actuator was set previously, turns it off.
      timer.disable(alarmID);
      timer.disable(actuatorID);
    }
    actuatorDisable();
    alarmDisable();
  }
}

void verifyLeakage(){
  if(valor_analogico >= LIMITE_GAS){
    // Gas leakage
    if(!timer.isEnabled(alarmIDOn) and !actuatorValue){
      alarmIDOn = timer.setTimeout(ALARM_DELAY, alarmEnable);
      actuatorIDOn = timer.setTimeout(ACTUATOR_DELAY, actuatorEnable);
      timer.enable(alarmIDOn);
      timer.enable(actuatorIDOn);
    }
  }else{
    alarmDisable();
    actuatorDisable();
  }
  if(actuatorValue){
    timer.disable(alarmIDOn);
  }
}

void loop()
{
  Blynk.run();
  if(!Blynk.connected()){
    runOffline();
  }else{
    verifyLeakage();
  }
  timer.run();
}

