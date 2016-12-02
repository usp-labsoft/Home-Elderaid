# Módulo de Automação Residencial
Módulo com sensor de luz (LDR, com sensibilidade ajustada através de um potenciômetro), módulo sensor de presença PIR e lâmpada com relé.
Montado em placa padrão. Para a versão anterior, veja Elderaid v.05.
Além de possibilitar ao usuário que controle a lâmpada pelo aplicativo Blynk, essa versão possui um grau de automação para a luz (em determinada condição, a luz acende e fica assim por um determinado tempo, facilmente ajustável no código Arduino.
Se não houver luz ambiente (observado pelo sensor LDR) e houver alguém na sala (detectado pelo sensor de presença), a lâmpada é acesa e assim permanece por determinado tempo.
Há dois modos: manual (usuário aciona a lâmpada remotamente) e automático, com a funcionalidade descrita.
Implementado em Arduino Uno, plataforma Blynk e ESP8266 como shield Wifi.
Etapas de implementação: conexão com ESP (dificuldades sanadas pelo uso de fios mais curtos para evitar interferência entre o ESP e o Arduino;
Depuração através do servidor do Blynk; Integração com o nosso servidor e então com o aplicativo próprio (Android).

## SOS
Nesse protótipo, também foram trabalhados os alertas no caso de pedido de SOS (socorro por parte do usuário e alerta remoto por parte do parente ou próprio usuário).
Foi usada uma interrupção externa do Arduino para confiabilidade da informação.
O acionamento do alarme (Buzzer + Led) pode ser feito pelo aplicativo ou por meio de botões físicos (aqui representando algum botão que fique com a pessoa, ou em lugar crítico da casa).

**Código**
[Código Arduino]()


**Parte Física**

![Visão Geral](v05_visaogeral.jpg)

[Placa](v05_placa.jpg)

[LDR](v05_LDR.jpg)

[Placa](v05_placa.jpg)

[Lâmpada](v05_lampada.jpg)

![Esquemático Maquete Arduino v05](ElderAidv05_labview.PNG)




**App Blynk**

![Interface App Blynk](v05_blynk_interface.png)

![SOS](blynk_sos.png)

[Gás](blynk_gas.png)

[Caso de desconexão](blynk_disconect.png)

![Twitter](blynk_twitter.png)

[Email SOS](blynk_sos_email.png)

![Email Gás](blynk_gas_email.png)
