#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX
#include <SPI.h>

#define BLYNK_PRINT DebugSerial
#include <BlynkSimpleStream.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "c7421dc8d43c4f67afb9b01f21fcc952";

// Definicoes dos pinos dos leds e buzzer
int pin_led_verm = 8;
int pin_led_verde = 9;

// Definicoes dos pinos ligados ao sensor 
int pin_d0 = 7;
int pin_a0 = A2;

int nivel_sensor = 250;

WidgetLCD lcd(V1);

void setup()
{
  // Debug console
  DebugSerial.begin(9600);

  // Blynk will work through Serial
  Blynk.begin(auth, Serial);
  // Define os pinos de leitura do sensor como entrada
  pinMode(pin_d0, INPUT);
  pinMode(pin_a0, INPUT);
  // Define pinos leds e buzzer como saida
  pinMode(pin_led_verm, OUTPUT);
  pinMode(pin_led_verde, OUTPUT);
  // Inicializa a serial
  Serial.begin(9600);

  lcd.clear();
}

void loop()
{
  Blynk.run();
  // Le os dados do pino digital D0 do sensor
  int valor_digital = digitalRead(pin_d0);
  // Le os dados do pino analogico A0 do sensor
  int valor_analogico = analogRead(pin_a0);
  // Mostra os dados no serial monitor
  //Serial.print("Pino D0 : ");
  //Serial.print(valor_digital);
  //Serial.print(" Pino A0 : ");
  Blynk.virtualWrite(V0, valor_analogico);
  //Serial.println(valor_analogico);
  // Verifica o nivel de gas/fumaca detectado
  if (valor_analogico > nivel_sensor)
  {
    // Liga o buzzer e o led vermelho, e
    // desliga o led verde
    lcd.clear();
    lcd.print(0, 0, "Vazamento");
    lcd.print(0, 1, "Detectado!");
    digitalWrite(pin_led_verm, HIGH);
    digitalWrite(pin_led_verde, LOW);

  }
  else
  {
    // Desliga o buzzer e o led vermelho, e
    // liga o led verde
    lcd.clear();
    lcd.print(0, 0, "OK");
    digitalWrite(pin_led_verm, LOW);
    digitalWrite(pin_led_verde, HIGH);
  }
  delay(1000);
}
