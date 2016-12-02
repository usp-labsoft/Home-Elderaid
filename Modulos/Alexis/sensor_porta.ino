#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SimpleTimer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "dc5c0245aed7442dbd466779298b0e3a";
//char auth[] = "28bd93dbfd9f48ad8100bb187880d771";
char auth[] = "7fb8c29988504f5ab263b797a869827b";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Alexis's iPhone";
char pass[] = "lex166789";
char server[] = "elderaid.me";
int port = 8442;
// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial

// or Software Serial on Uno, Nano...
//#include <SoftwareSerial.h>
//SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

//ESP8266 wifi(&EspSerial);
ESP8266 wifi(&EspSerial);

SimpleTimer timer;

int pin_d6 = 6;
int pin_d3 = 3; // Buzzer (Atuador)
int valor_digital = LOW;

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

  pinMode(pin_d6, INPUT);
  pinMode(pin_d3, OUTPUT);
  Blynk.begin(auth, wifi, ssid, pass, server, port);
  
}

BLYNK_READ(V7)
{
  // Sensor Magnético
  Blynk.virtualWrite(V7, digitalRead(pin_d6));
}

BLYNK_WRITE(V9)
{
  // Buzzer
  int value = param.asInt();
  digitalWrite(pin_d3, value);
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

void loop()
{
  Blynk.run();
  timer.run();
}

