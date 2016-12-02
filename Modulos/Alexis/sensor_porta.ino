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
#define ESP8266_BAUD 9600

#define ALARM_DELAY 10000L

ESP8266 wifi(&EspSerial);

SimpleTimer timer;

int sensor_porta = 6;
int buzzer_porta = 3; // Buzzer (Atuador)
int valor_digital = HIGH;
int alarmID;
int alarmIDOn;
int alarmValue;

WidgetLED led8(V11);
WidgetLED led10(V13);

void setup()
{
  // Set console baud rate
  Serial.begin(9600);
  delay(10);
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  led8.on();
  led10.off();
  timer.setInterval(1000L, watchdog);
  timer.setInterval(1500L, blinkLED);
  alarmID = timer.setTimeout(ALARM_DELAY, alarmEnable); 
  alarmIDOn = timer.setTimeout(ALARM_DELAY, alarmEnable);
  timer.disable(alarmIDOn);
  timer.disable(alarmID);

  pinMode(sensor_porta, INPUT);
  pinMode(buzzer_porta, OUTPUT);
  Blynk.begin(auth, wifi, ssid, pass, server, port);
  
}

BLYNK_READ(V7)
{
  // Sensor Magnético
  valor_digital = digitalRead(sensor_porta);
  Blynk.virtualWrite(V7, valor_digital);
}

BLYNK_WRITE(V9)
{
  // Buzzer
  alarmValue = param.asInt();
  digitalWrite(buzzer_porta, alarmValue);
}

void watchdog(){
  led8.on();
}

void blinkLED(){
  if(led10.getValue()){
    led10.off();
  }else{
    led10.on();
  }
}

void alarmEnable(){
  digitalWrite(buzzer_porta, HIGH);
}

void alarmDisable(){
  digitalWrite(buzzer_porta, LOW);
}

void runOffline(){
  // Reads gas sensor and stores its value
  valor_digital = digitalRead(sensor_porta);
  // Verify if door open.
  if(valor_digital == LOW){
    // Detected open door.
    if(!timer.isEnabled(alarmID)){
      // Set alarm to start after 10 seconds if not already.
      alarmID = timer.setTimeout(ALARM_DELAY, alarmEnable);
      timer.enable(alarmID);
    }
  }else{
    // Door closed
    if(!timer.isEnabled(alarmID)){
      // If alarm was set previously, turns it off.
      timer.disable(alarmID);
    }
    alarmDisable();
  }
}

void verifyDoor(){
  if(valor_digital == LOW){
    // Door is open
    if(!timer.isEnabled(alarmIDOn) and !alarmValue){
      alarmIDOn = timer.setTimeout(ALARM_DELAY, alarmEnable);
      timer.enable(alarmIDOn);
    }
  }else{
    alarmDisable();
  }
  if(alarmValue){
    timer.disable(alarmIDOn);
  }
}

void loop()
{
  Blynk.run();
  if(!Blynk.connected()){
    runOffline();
  }else{
    verifyDoor();
  }
  timer.run();
}
