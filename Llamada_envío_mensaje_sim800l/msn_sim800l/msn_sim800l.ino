
 /* Se usa la libreiar SoftwareSerial para definir los pines de comunicacion del
    Arduino Uno (D7 y D8), en alimentación el regulador L7805CV (5V) y el diodo Zener 
    IN4728A (Pueden Utilizar otro Zener o un diodo de proposito general). Se envía 
    comandos AT para Establecr comunicación con el modulo SIM800l y se utiliza Sim Car 
    del operador TIGO (Colombia).
    
    ARDUINO UNO     SIM800L
      D7              TX 
      D8              RX
      RESET           RST
      GND             GND
  */
  
#include <SoftwareSerial.h>
  
//Pulsador para enviar el Mensaje
#define SIM800_PULSADOR 9

int contador = 0;
 
//Se declara los pines en el arduino Uno
SoftwareSerial MOD_SIM800L(7, 8);
 
void setup() {
  //Se establece la velocidad para el puerto serie
  Serial.begin(115200);
  while(!Serial);
   
  //Velocidad de trabajo entre el Arduino Uno y el Modulo SIM800L
  MOD_SIM800L.begin(115200);
  delay(1000);
 
  pinMode (SIM800_PULSADOR, INPUT);
 }
 
void loop() {

  if((digitalRead(SIM800_PULSADOR) == 1)&&(contador == 0)){
  Serial.println("Configuracion Completa!");
  Serial.println("Enviando SMS...");
   
  //Se establece el formato de SMS en ASCII
  MOD_SIM800L.write("AT+CMGF=1\r\n");
  delay(1000);
 
  //Enviar comando para un nuevos SMS al numero establecido
  MOD_SIM800L.write("AT+CMGS=\"+573114009876\"\r\n");
  delay(1000);
   
  //Enviar contenido del SMS
  MOD_SIM800L.write("Hola bienvenidos a su canal: Diseno Electronico A2P");
  delay(1000);
   
  //Enviar Ctrl+Z 
  MOD_SIM800L.write((char)26);
  delay(1000);
     
  Serial.println("Mensaje enviado!");
  }
}
