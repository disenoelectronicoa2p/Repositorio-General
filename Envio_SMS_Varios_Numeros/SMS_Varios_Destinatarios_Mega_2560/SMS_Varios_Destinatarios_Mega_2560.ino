

 /* Se usan los pines de comunicacion por hardware, número 1 del arduino Mega 2560
    (TX1=D18 y RX1=D19). Se envía comandos AT para Establecr comunicación con el 
    modulo SIM900 y se utiliza SIM Card del operador TIGO (Colombia).
   
        ------CONEXION-------
    ARDUINO Mega 2560       SIM900
         D18                D7(RX) 
         D19                D8(TX)
         GND                GND
  */
  

char   incoming_char       = 0;    //Variable que guarda los caracteres que envia el SIM900

 //# Numeros celulares
 String Num_Movil[]= {/*#1:*/"311XXXXXXX",/*#2:*/"318XXXXXXX",/*#3:*/"313XXXXXXX"}; 

 //Cantidad de numeros Movil 
 int Cantidad_Movil = 3;    
  
//Pulsador para enviar el Mensaje
#define SIM900_PULSADOR 5

int contador = 0;
 
 
void setup() {
  //Se establece la velocidad para el puerto serie
  Serial.begin(19200);
  Serial1.begin(19200);
 
  pinMode (SIM900_PULSADOR, INPUT);

  Serial1.print("AT+CMGF=1\r");           // Configura SMS en modo de texto.
  delay(1000);
  Serial1.print("AT+CNMI=2,2,0,0,0\r");   /* Transmisión de mensaje directamente
                                             al puerto serial del dispositivo. */
  delay(2000);
  Serial1.println("AT+CMGD=1,4");         // Eliminar todos los mensajes
  delay(1000);
  Serial1.println("AT+CMGL=\"ALL\",0 ");
  delay(2000);

  Serial.println("Comandos AT enviado correctamente.");
  
 }
 
void loop() {

   if (Serial1.available() > 0) {                                                      
    incoming_char = Serial1.read();                                                    
//    Serial.print(incoming_char);                                                      
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
    Serial1.println("AT + CMGS = \"" + Num_Movil[n] + "\"");  // Envia mensaje  
    delay(100);                                                                     
    Serial1.print("Hola bienvenidos a su canal: Diseno Electronico A2P");                                
    delay(100);                                                          
    Serial1.write((char)26); //ctrl+z                                               
    delay(9000);   
    Serial.println(); 
    Serial.print("Mensaje numero: ");   
    Serial.print(n+1);         
    Serial.print(" enviado");                                                               
  }                                                                                  
}                                                                                 
