
 /* Se usa la libreiar SoftwareSerial para definir los pines de comunicacion del
    Arduino Uno (D7 y D8), Se envía comandos AT para Establecr comunicación con 
    el modulo SIM900 y se utiliza SIM Card del operador TIGO (Colombia).

     ------CONEXION------- 
    ARDUINO UNO     SIM800L
      D7              TX 
      D8              RX
      GND             GND
  */
  
#include <SoftwareSerial.h>

char   incoming_char       = 0;    //Variable que guarda los caracteres que envia el SIM900

String Num_Movil[]     = {/*#1:*/"311XXXXXXX",/*#2:*/"318XXXXXXX",/*#3:*/"313XXXXXXX"/*#4:*//*#5:*/ /*#6:*/ /*#7:*/ /*#8:*/ /*#9:*/ /*#10:*/}; //# Numeros celulares
int    Cantidad_Movil  = 3;    //Cantidad de numeros Movil 
  
//Pulsador para enviar el Mensaje
#define SIM900_PULSADOR 5

int contador = 0;
 
//Se declara los pines en el arduino Uno
SoftwareSerial MOD_SIM900(7, 8);
 
void setup() {
  //Se establece la velocidad para el puerto serie
  Serial.begin(19200);
  MOD_SIM900.begin(19200);
 
  pinMode (SIM900_PULSADOR, INPUT);

  MOD_SIM900.print("AT+CMGF=1\r");           // Configura SMS en modo de texto.
  delay(100);
  MOD_SIM900.print("AT+CNMI=2,2,0,0,0\r");   /* Transmisión de mensaje directamente
                                                al puerto serial del dispositivo. */
  delay(2000);
  MOD_SIM900.println("AT+CMGD=1,4");         // Eliminar todos los mensajes
  delay(1000);
  MOD_SIM900.println("AT+CMGL=\"ALL\",0 ");
  delay(2000);

  Serial.println("Comandos AT enviado correctamente.");
  
 }
 
void loop() {

  if (MOD_SIM900.available() > 0) {                                                      
    incoming_char = MOD_SIM900.read();                                                    
    Serial.print(incoming_char);                                                      
  } 

  if((digitalRead(SIM900_PULSADOR) == 1)&&(contador == 0)){
  Serial.println("Configurando sistema...");
  delay(1000);  
  Serial.println("Configuracion Completa!");
  delay(1000);
  Serial.println("Enviando SMS...");
  Envio_MSN_Destinos();    
 }
}


 void Envio_MSN_Destinos() {                                                           
  for (int n = 0; n < Cantidad_Movil; n++) {                                            
    delay(1000);                                                                     
    MOD_SIM900.println("AT + CMGS = \"" + Num_Movil[n] + "\"");  // Envia mensaje  
    delay(100);                                                                     
    MOD_SIM900.print("Hola bienvenidos a su canal: Diseno Electronico A2P");                                
    delay(100);                                                          
    MOD_SIM900.write((char)26); //ctrl+z                                               
    delay(9000);   
    Serial.println(); 
    Serial.print("Mensaje numero: ");   
    Serial.print(n+1);         
    Serial.print(" enviado");                                                               
  }                                                                                  
}                                                                                 
