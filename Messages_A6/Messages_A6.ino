
    unsigned long measurementTime; //Contador para envio mensaje 
    unsigned long tiempoPrevioMSN; //Contador para control envio mensaje  

    String numMovil[] = {"+57313XXXXXXX","+57313XXXXXXX","57313XXXXXXX","+57313XXXXXXX"}; //# Numeros celulares 
    #define GetSize(numMovil) (sizeof(numMovil)/sizeof(*(numMovil)))                                      
 
    void setup() {
      Serial.begin(115200);

      measurementTime = 60000; //Tiempo para enviar mensage --> 60000=1 min.

      Serial.println("Start!...");          
    }
 
    void loop(){

      unsigned long tiempoActualMSN = millis(); //Inicio contador
      if ((unsigned long)(tiempoActualMSN - tiempoPrevioMSN) >= measurementTime){  
        sendMultiplesMessage();
       tiempoPrevioMSN = tiempoActualMSN;      
      } 
      
   }

   void sendAMessage(){
       Serial.println("Enviando SMS...");
       
       //Se establece el formato de SMS en ASCII
       Serial.write("AT+CMGF=1\r\n");
       delay(1000);
      
       //Enviar comando para un nuevos SMS al numero establecido
       Serial.write("AT+CMGS=\"+57313XXXXXXX\"\r\n");
       delay(1000);
       
       //Enviar contenido del SMS
       Serial.write("Hola bienvenidos a su canal: Diseno Electronico A2P");
       delay(1000);
       
       //Enviar Ctrl+Z 
       Serial.write((char)26);
//       Serial.write(0x1A); 
       delay(5000);
         
       Serial.println("\nMensaje enviado!");    
   }


   void sendMultiplesMessage(){
    
      Serial.println("Configuración del modo SMS...");
      Serial.println("AT+CMGF=1");
      Serial.println("Enviando SMS...");
   
      int canMovil=GetSize(numMovil);                                               
      for (int i = 0; i<canMovil; i++) {      
         Serial.println("AT+CMGS=\""+numMovil[i]+"\"");      // Numero para enviar mensaje  
         delay(500);                                                                     
         Serial.write("Hola suscribete a tu canal: Diseno Electronico A2P...!");  // Mensaje a enviar 
         Serial.println();                             
         delay(1000);                                                          
         Serial.write((char)26); //ctrl+z                                               
         delay(9000);      
      }
   }
