/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 *
 * This example shows how to use ESP8266 Shield (with AT commands)
 * to connect your project to Blynk.
 *
 * Note: Ensure a stable serial connection to ESP8266!
 *       Firmware version 1.0.0 (AT v0.22) is needed.
 *       You can change ESP baud rate. Connect to AT console and call:
 *           AT+UART_DEF=9600,8,1,0,0
 *       In general, Soft Serial may be unstable.
 *       It is highly recommended to switch to Hard Serial.
 *
 * Change WiFi ssid,M pass, and Blynk auth token to run :)
 * Feel free to apply it to any other example. It's simple!
 *
 **************************************************************/





//1.Conexão Wifi pelo ESP 01, com Arduino Mega

      //#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
      #include <ESP8266_Lib.h>
      #include <BlynkSimpleShieldEsp8266.h>
      
      // You should get Auth Token in the Blynk App.
      // Go to the Project Settings (nut icon).
      //char auth[] = "607cc39ee6de48708f1b831dacb40e37"; //Auth do Blynk
      char auth[] = "7fb8c29988504f5ab263b797a869827b"; //Auth do server Oficial
      //char auth[] = "fa1f8e6aecba4c089278ee5128073c3e"; //Auth do server Oficial, só este módulo
      
      // Your WiFi credentials.
      // Set password to "" for open networks.
      char ssid[] = "dee848";
      char pass[] = "272137463";
      char server[] = "elderaid.me";
      int port = 8442;
      //char ssid[] = "VTH";
      //char pass[] = "charizard";
      
      // Hardware Serial on Mega, Leonardo, Micro...
      #define EspSerial Serial
      
      // Your ESP8266 baud rate:
      #define ESP8266_BAUD 115200
      
      ESP8266 wifi(&EspSerial);







//2. Programa do Módulo de Automação Residencial

      //#include <BlynkSimpleStream.h>
      #include <SimpleTimer.h>
      //#include <WidgetRTC.h>

      float presenca = 0; //Valor final
      float presencaAnt = 0; //Valor anterior
      int contPres = 0;
      float gas = 0;
      
      //Para ler valor do botão de SOS
      
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
      int lampadaconfig = 0;

      //Para buzzer e led de alerta
      int alertPin = 11;
      int SOS = 0; //SOS pelo botão físico
      int lastSOSapp = 0; //SOS pelo botão virtual
      int alerta = 0; //estado do buzzer e led
      int SOSokPin = 10; //Botão de SOS ok físico
      
      //Variável auxiliar de controle
      int primeiraVez;
      int contOff;
      
      // Attach virtual serial terminal to Virtual Pin V0
      WidgetTerminal terminal(V0);
      
      //led 1 para botão de Pânico/SOS
      WidgetLED led1(V1);
      
      //led2 para luz ambiente
      WidgetLED led2(V4);
      
      //led3 para presença
      WidgetLED led3(V5);
      
      //led4 para gás
      WidgetLED led4(V15);

      //led5 para conexão
      WidgetLED led5(V18);

      //led6 para watchdog
      WidgetLED led6(V19);

      //led7 para leitura do atuador deste módulo de luz
      WidgetLED led7(V29);
      
      SimpleTimer timer;
      //WidgetRTC rtc;
      
      //BLYNK_ATTACH_WIDGET(rtc, V31);

      //Para input da lâmpada a partir do app
      BLYNK_WRITE(V3)
      {
        if ((param.asInt()==1)and(lampadaconfig==0))
        {
          digitalWrite(lampada, HIGH);
        }
        else if ((param.asInt()==0)and(lampadaconfig==0))
        {
          digitalWrite(lampada, LOW);
        }
        else{
          digitalWrite(lampada, LOW);
        }
      }

      //Para input de SOS a partir do app
      BLYNK_WRITE(V28)
      {
        if (param.asInt()==1)
        {
          lampadaconfig = 1;
        }
        else
        {
          lampadaconfig = 0;
        }
      }


      //Para configurar o modo de acionamento do atuador
      BLYNK_WRITE(V31)
      {
        if ((lastSOSapp==0)and(param.asInt()==1))
        {
          SOS = 1;
          alerta = 1;
        }
        else if ((lastSOSapp==1)and(param.asInt()==0))
        {
          SOS = 0;
          alerta = 0;
        }
        else{
          SOS = 0;
          alerta = 0;
        }
        lastSOSapp = param.asInt();
      }
      
      
      void OfflineMode()
      {
        //Modo Offline
        //Para acender e apagar a lâmpada automaticamente
        
        if ((digitalRead(PresPin) > 0)and(analogRead(LuzPin)<700)) //Para acender a luz automaticamente
        {
          digitalWrite(lampada, HIGH);
        }
        else if (contOff == 10000){
          digitalWrite(lampada, LOW);
          contOff = 0;
        }

        contOff = contOff + 1;
        
      }
      
      
      
      
      void atualizaApp()
      {
        led6.on();  //Led de watchdog

        Blynk.run();


        if (digitalRead(lampada)){
          led7.on();
        } else{
          led7.off();
        }

        Blynk.run();
        
        //Para a conexão
        if (led5.getValue()){
          led5.off();
        }else{
          led5.on();
        }

        Blynk.run();
        //Para o RTC
        //String currentTime = String(hour()) + ":" + minute() + ":" + second();
        //String currentDate = String(day()) + "/" + month() + "/" + year();
        
        //Caso de SOS
        if (SOS == 1){
          terminal.println("SOCORRO!");
          terminal.flush();
          Blynk.tweet("#ElderAid SOCORRO!");
          Blynk.email("ElderAid SOS","SOCORRO de ElderAid!");
          Blynk.notify("SOS de ElderAid!!!");
          SOS = 0;
        }

        Blynk.run();

        if (alerta == 1){
          digitalWrite(alertPin, HIGH);
          led1.on();
        }else {
          digitalWrite(alertPin, LOW);
          led1.off();
        }

        Blynk.run();

        if (digitalRead(SOSokPin) == HIGH){
          alerta = 0;
        }

        Blynk.run();
        
      
        //Caso de Gás vazando
        if (analogRead(GasPin)>500)
        {
          led4.on();
          terminal.println("Vazamento detectado!");
          terminal.flush();
          //Blynk.tweet("#ElderAid Vazamento de Gás!");
          Blynk.email("ElderAid Gás","Vazamento de Gás!");
          Blynk.notify("Vazamento de Gás!");
        } else {
          led4.off();
        }

        Blynk.run();
      
        //Caso de termos luz ambiente
        if (analogRead(LuzPin)>600)
        {
          led2.on();
        }
        else
        {
          led2.off();
        }
        Blynk.virtualWrite(V14, analogRead(LuzPin));

        Blynk.run();
      
        //Para o sensor de presença
        contPres = contPres + 1;
        presencaAnt = presencaAnt + digitalRead(PresPin)*10.0;
        if (contPres == 3){
          presenca = presencaAnt/3;
          contPres = 0;
          presencaAnt = 0.0;
        }

        Blynk.run();
        
        if (presenca > 5.0)
        {
          led3.on();
        }
        else{
          led3.off();
        }
        Blynk.virtualWrite(V30, presenca);

        Blynk.run();
      
        //Para acender e apagar a lâmpada automaticamente
        cont = cont + 1;
        if ((presenca > 5.0)and(analogRead(LuzPin)<600)and(lampadaconfig==1)) //Para acender a luz rapidamente, modo automático
        {
          digitalWrite(lampada, HIGH);
        }

        Blynk.run();
      
        if (cont == 30)
        {
          if ((presenca > 5.0)and(analogRead(LuzPin)<600)and(lampadaconfig==1)) //Se tiver alguém e não termos luz ambiente ou lâmpada não estiver setada em high pelo app
          {
            digitalWrite(lampada, HIGH);
          } else if(lampadaconfig==1){
            digitalWrite(lampada, LOW);
          }
          cont = 0;
        }


        Blynk.run();
        //Para o RTC
//        if (primeiraVez == 1)
//        {
//          if ((hour()>6)and(hour()<12))
//          {
//              terminal.println("Bom dia!");
//          }
//          else if (hour()<18)
//          {
//              terminal.println("Boa tarde!");
//          }
//          else
//          {
//              terminal.println("Boa noite!");
//          }
//          terminal.println(currentDate);
//          terminal.println(currentTime);
//          terminal.flush();
//        }
//        else{}
        primeiraVez = 0;
      
        Blynk.run();
        if (Blynk.connected() == false)
        {
          OfflineMode();
        }
      }


void intSOS(){
  if (digitalRead(3)){
    SOS = 1;
    alerta = 1;
  }
}


void setup()
{
  // Set console baud rate
  Serial.begin(115200);
  delay(10);
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass, server, port);

  
  pinMode(lampada, OUTPUT);
  pinMode(alertPin, OUTPUT);

  pinMode(3,INPUT);
   
   if (Blynk.connect() == false) {
    // Wait until connected
    OfflineMode();
   }


   
   //Set interval timer
   timer.setInterval(1000L, atualizaApp);
   
   // This will print Blynk Software version to the Terminal Widget when
   // your hardware gets connected to Blynk Server
   terminal.println("");
   terminal.println("ElderAid v.014");
   terminal.println("Sistema de Apoio a Idosos");
   terminal.println("2016-02");
   terminal.flush();

   //led inicialmente desligado
   led1.off();
   led2.off();
   led3.off();
   led4.off();
   led5.off();
   led6.on();

   
   // Begin synchronizing time
   //rtc.begin();
  
   primeiraVez = 1;

   cont = 0;

   attachInterrupt(1,intSOS,RISING); //Configurando a interrupção do botão de SOS (deve estar conectado no pino D2 do Arduino Uno
   
}

void loop()
{
  Blynk.run();
  timer.run();
}

