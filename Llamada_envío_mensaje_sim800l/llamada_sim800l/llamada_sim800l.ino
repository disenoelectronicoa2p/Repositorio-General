

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

 #include <SoftwareSerial.h>  //Incluir Libreria SoftwareSerial

 SoftwareSerial MOD_SIM800L(7, 8); // Se declaran los pines del arduino uno D7 y D8
                                   //(RX Del Modulo SIM800l), (TX Del Modulo SIM800l)

 void setup(){
  Serial.begin(115200);
  MOD_SIM800L.begin(115200);
 }

  void loop(){
  
   if (MOD_SIM800L.available()){         // Obtiene el número de bytes (caracteres) disponibles 
     Serial.write(MOD_SIM800L.read());   // para su lectura desde el puerto serie.
   }
  
   if (Serial.available()){
    while(Serial.available()) {
     MOD_SIM800L.write(Serial.read());
    }
     MOD_SIM800L.println();
   }
}
