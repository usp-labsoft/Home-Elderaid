# Home-Elderaid
##Business Driver
Monitoramento para assistência de idosos
##Cliente
Familiar de idoso que mora sozinho com boa saúde física
##Funcionalidades
Monitoramento do cotidiano por meio de sensores

Acionamento de atuadores remotamente pelo familiar (alertas de emergência)

Informação armazenada em cloud

Interação por aplicativo de celular (Android)
##Requisitos
Algoritmo de Inteligência sobre dados históricos
Grande escalabilidade
##Arquitetura
Atuadores e sensores agem por meio de interfaces com o controlador, que se comunica com o servidor. O familiar observa as informações no seu aplicativo, que se comunica com o servidor.
##Servidor
A plataforma em nuvem Digital Ocean é a responsável pela troca de informações entre o aplicativo e os módulos de sensores e atuadores. Em cima dele foi utilizado o API Blynk Server, que implementa um protocolo de comunicação onde GETs eram executados a fim de monitorar e acionar pinos de um microcontrolador Arduíno UNO.  
