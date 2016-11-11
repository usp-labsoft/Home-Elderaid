// Fazer respostas locais para proteção no Arduino
// Retirado de Arduino Serial USB,Terminal, led e button do blynk docs (30/09/2016), RTC (16/10/2016), FilipeFLOP Blog (16/10/2016):
// You could use a spare Hardware Serial on boards that have it (like Mega)
#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX

#define BLYNK_PRINT DebugSerial
#include <BlynkSimpleStream.h>
#include <SimpleTimer.h>
#include <WidgetRTC.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "607cc39ee6de48708f1b831dacb40e37";
float presenca = 0;
float gas = 0;

//Para ler valor do botão de SOS
int SOSPin = 7;
//Para sensor de luz
int LuzPin = A4;
//Para sensor de presença
int PresPin = A5;
//Para sensor de gás
int GasPin = A1;
//Para lâmpada
int lampada = 12;
int lampadavirtual = 0;
int cont = 0;

//Variável auxiliar de controle
int primeiraVez;

// Attach virtual serial terminal to Virtual Pin V0
WidgetTerminal terminal(V0);

//led 1 para botão de Pânico/SOS
WidgetLED led1(V1);

//led2 para luz ambiente
WidgetLED led2(V4);

//led3 para presença
WidgetLED led3(V5);

SimpleTimer timer;
WidgetRTC rtc;

BLYNK_ATTACH_WIDGET(rtc, V31);


//Para input da lâmpada a partir do app
BLYNK_WRITE(V3)
{
  if (param.asInt()==1)
  {
    lampadavirtual = 1;
  }
  else
  {
    lampadavirtual = 0;
  }
}


void OfflineMode()
{
  //Modo Offline
  //Para acender e apagar a lâmpada automaticamente
  
  if ((digitalRead(PresPin) > 0)and(analogRead(LuzPin)<700)) //Para acender a luz automaticamente
  {
    digitalWrite(lampada, HIGH);
    delay(10000);
  }
  else
  {
    digitalWrite(lampada, LOW);
  }
}




void atualizaApp()
{
  
  //Para o RTC
  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + "/" + month() + "/" + year();
  
  //Caso de SOS
  if (digitalRead(SOSPin)) {
    led1.on();
    terminal.println("SOCORRO!");
    terminal.flush();
    Blynk.tweet("#ElderAid SOCORRO!");
    Blynk.email("ElderAid SOS","SOCORRO de ElderAid!");
    Blynk.notify("SOS de ElderAid!!!");
  } else {
    led1.off();
  }

  //Caso de Gás vazando
  if (analogRead(GasPin)>500)
  {
    terminal.println("Vazamento detectado!");
    terminal.flush();
    Blynk.tweet("#ElderAid Vazamento de Gás!");
    Blynk.email("ElderAid Gás","Vazamento de Gás!");
    Blynk.notify("Vazamento de Gás!");
  }

  //Caso de termos luz ambiente
  if (analogRead(LuzPin)>700)
  {
    led2.on();
  }
  else
  {
    led2.off();
  }

  //Para o sensor de presença
  presenca = digitalRead(PresPin)*10.0;
  if (presenca > 5.0)
  {
    led3.on();
  }
  else{
    led3.off();
  }
  Blynk.virtualWrite(V30, presenca);

  //Para acender e apagar a lâmpada automaticamente
  cont = cont + 1;
  if ((presenca > 5.0)and(analogRead(LuzPin)<700)or(lampadavirtual == 1)) //Para acender a luz rapidamente
  {
    digitalWrite(lampada, HIGH);
  }

  if (cont == 30)
  {
    if ((presenca > 5.0)and(analogRead(LuzPin)<700)or(lampadavirtual == 1)) //Se tiver alguém e não termos luz ambiente ou lâmpada não estiver setada em high pelo app
    {
      digitalWrite(lampada, HIGH);
    }
    else
    {
      digitalWrite(lampada, LOW);
    }
    cont = 0;
  }

  //Para o RTC
  if (primeiraVez == 1)
  {
    if ((hour()>6)and(hour()<12))
    {
        terminal.println("Bom dia!");
    }
    else if (hour()<18)
    {
        terminal.println("Boa tarde!");
    }
    else
    {
        terminal.println("Boa noite!");
    }
    terminal.println(currentDate);
    terminal.println(currentTime);
    terminal.flush();
  }
  else{}
  primeiraVez = 0;


  if (Blynk.connected() == false)
  {
    OfflineMode();
  }

  
}

void setup()
  {
  // Debug console
  DebugSerial.begin(9600);

  // Blynk will work through Serial
  Serial.begin(9600);
  Blynk.begin(auth, Serial);

  pinMode(lampada, OUTPUT);
   
   //while (Blynk.connect() == false) {
    // Wait until connected
   //}

   
   //Set interval timer
   timer.setInterval(1000L, atualizaApp);
   
   // This will print Blynk Software version to the Terminal Widget when
   // your hardware gets connected to Blynk Server
   terminal.println("");
   terminal.println("ElderAid v.05");
   terminal.flush();

   //led inicialmente desligado
   led1.off();
   led2.off();
   led3.off();

   
   // Begin synchronizing time
   rtc.begin();

   primeiraVez = 1;

   rtc.begin();
   }

   
void loop()
{
    Blynk.run();
    timer.run();
}
