
   /*-------------------------------------------------------------------
     Control de elementos electrónico por mensaje de texto a través
     del módulo GSM/GPRS Sim900 y el arduino Uno.
     En este proyecto se enciende y apaga un motor DC y 3 diodo led
     mediante mensajes de texto enviado al modulo sim900, lo cual es
     decodificado por la tarjeta Arduino Uno y ejecuta la acción de 
     encender-apagar estos elementos, cuando se enciende y apaga el
     motor DC, se recibe un mensaje de notificación que se ha encendido
     ó apagado dicho elemento.

    By:
       Diseño Electrónico A2P

     YouTube:   https://www.youtube.com/channel/UCCxkP4if4efvsn27aBX2wxg
     Instagram: https://www.instagram.com/dis.elect.a2p/
     Facebook:  https://www.facebook.com/DiseElectrA2P/
   ------------------------------------------------------------------------*/



   String mensaje  = "";    // Se le asigna los mensajes que se reciben
   char   caracter = "";    //Variable que guarda los caracteres que recive el SIM900

   int motor    = 5;   //Pin 5 para led el motor
   int rojo     = 4;   //Pin 4 para led rojo
   int verde    = 3;   //Pin 3 para led verde
   int amarillo = 2;   //Pin 2 para led amarillo
   

   void setup() {

     pinMode(motor,    OUTPUT);
     pinMode(rojo,     OUTPUT);
     pinMode(verde,    OUTPUT);
     pinMode(amarillo, OUTPUT);
    
     Serial.begin(19200);
       
     while(!Serial);

    //Configuracion inicial del SIM900
  
     while( (EnviarComandoAT("AT+CREG?", "+CREG: 0,1", 1000) || EnviarComandoAT("AT+CREG?", "+CREG: 0,5", 1000)) == 0 );

     EnviarComandoAT("AT+CLIP=1", "OK", 1000);          //Identificador de Llamada.   
     EnviarComandoAT("AT+CMGF=1", "OK", 1000); 
     EnviarComandoAT("AT+CNMI=2,2,0,0,0", "OK", 1000);  //Transmisión de mensaje directamente al puerto serial del dispositivo.
     EnviarComandoAT("AT+CMGD=1,4", "OK", 1000);        //Borra todos los mensajes 
     EnviarComandoAT("AT+CMGL=\"ALL\",0", "OK", 1000);  //Borra todos los mensajes 

     while(Serial.available() != 0){
       Serial.read();  
     }
     delay(1000);
    }


    void loop() {

     if (Serial.available() != 0) {                                                       
      caracter = Serial.read();                                                    
      mensaje = mensaje + caracter; // Añadimos el carácter leído al mensaje      
      //Serial.print(caracter);  
     } 

    identifica_SMS();
                                                                               
   }

    void identifica_SMS(){

      int motorON    = mensaje.indexOf("MotorON");
      int motorOF    = mensaje.indexOf("MotorOF");
      int rojoON     = mensaje.indexOf("RojoON"); 
      int rojoOF     = mensaje.indexOf("RojoOF");
      int verdeON    = mensaje.indexOf("VerdeON"); 
      int verdeOF    = mensaje.indexOf("VerdeOF");
      int amarilloON = mensaje.indexOf("AmarilloON"); 
      int amarilloOF = mensaje.indexOf("AmarilloOF");      


       if ( motorON >= 0 ) { 
        delay(1000);  
        digitalWrite(motor,HIGH);
        mensaje = "";  //Bórralo para la próxima vez         
        Serial.println("Motor encendido");
        delay(1000);      
          
//        Serial.write("AT+CMGF=1\r\n");
//        delay(1000);
// 
//        //Enviar comando para un nuevos SMS al numero establecido
//        Serial.write("AT+CMGS=\"+573135437865\"\r\n");
//        delay(1000);
//   
//        //Enviar contenido del SMS
//        Serial.write("Motor encendido");
//        delay(1000);
//   
//        //Enviar Ctrl+Z 
//        Serial.write((char)26);
//        delay(1000);                                                                             
      }

    //-------------------------------------------------------------------
       if ( motorOF >= 0 ) { 
        delay(1000);
        digitalWrite(motor,LOW);
        mensaje = "" ;  //Bórralo para la próxima vez           
        Serial.println("Motor apagado");

        delay(1000);      
          
//        Serial.write("AT+CMGF=1\r\n");
//        delay(1000);
// 
//        //Enviar comando para un nuevos SMS al numero establecido
//        Serial.write("AT+CMGS=\"+573135437865\"\r\n");
//        delay(1000);
//   
//        //Enviar contenido del SMS
//        Serial.write("Motor apagado");
//        delay(1000);
//   
//        //Enviar Ctrl+Z 
//        Serial.write((char)26);
//        delay(1000);          
      } 
   
   //-------------------------------------------------------------------
       if ( rojoON >= 0 ) { 
        delay(1000);  
        digitalWrite(rojo,HIGH);
        mensaje = "" ;  //Bórralo para la próxima vez         
        Serial.println("Luz Roja encendida");
        
      }

       if ( rojoOF >= 0 ) { 
        delay(1000);
        digitalWrite(rojo,LOW);
        mensaje = "" ;  //Bórralo para la próxima vez           
        Serial.println("Luz Roja apagada");
      }    
   //-------------------------------------------------------------------
       if ( verdeON >= 0 ) { 
        delay(1000);  
        digitalWrite(verde,HIGH);
        mensaje = "" ;  //Bórralo para la próxima vez         
        Serial.println("Luz Verde encendida");
      }

       if ( verdeOF >= 0 ) { 
        delay(1000);
        digitalWrite(verde,LOW);
        mensaje = "" ;  //Bórralo para la próxima vez           
        Serial.println("Luz Verde apagada");

      } 

   //-------------------------------------------------------------------
       if ( amarilloON >= 0 ) { 
        delay(1000);  
        digitalWrite(amarillo,HIGH);
        mensaje = "" ;  //Bórralo para la próxima vez         
        Serial.println("Luz Amarilla encendida");
      }

       if ( amarilloOF >= 0 ) { 
        delay(1000);
        digitalWrite(amarillo,LOW);
        mensaje = "" ;  //Bórralo para la próxima vez           
        Serial.println("Luz Amarilla apagada");

      }             
    }




    int8_t EnviarComandoAT(char* ComandoAT, char* Resp_Esperada, unsigned int Retardo){

     uint8_t x=0,  Responde=0;
     char Respuesta[100];
     unsigned long Anterior;

     memset(Respuesta, '\0', 100);    // Inicializa la cadena
     delay(100);
    
     while( Serial.available() > 0)
     Serial.read();    // Limpie el buffer de entrada
    
     if (ComandoAT[0] != '\0') {
      Serial.println(ComandoAT);    // Enviar comando AT
      //Serial.println(ComandoAT);     // Mostrar comando por serial
     }


     x = 0;
     Anterior = millis();

     // Este bucle espera la respuesta
     do{
       if(Serial.available() != 0){    // Si hay datos en el búfer de entrada de UART, léalo y verifica la respuesta
          Respuesta[x] = Serial.read();
          //Serial.print(response[x]);
          x++;
        if (strstr(Respuesta, Resp_Esperada) != NULL) {   // Verificar si la respuesta deseada (OK) está en la respuesta del módulo
          Responde = 1;
         }
       }
     }while((Responde == 0) && ((millis() - Anterior) < Retardo));    // Espera la respuesta con tiempo de espera
      return Responde;
    }   


    
