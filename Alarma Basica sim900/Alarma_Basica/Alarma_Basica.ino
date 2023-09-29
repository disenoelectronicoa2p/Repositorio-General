
  /*  Alarma Basica con el modulo Sim900, Tarjeta Arduino Uno y el 
      Sensor Ultrasonico HC-SR04. La alarma realiza una llamada 
      o envia un mensaje cuando se detecta un movimiento a traves 
      del sensor HC-SR04, ademas de activar un zumbador */



  #define pulsador 4          // Activar sensor
  #define Zumbador 5          // Zumbador o sirena
  #define indica1 10          // Encendido de la Alarma-Led Rojo
  #define indica2 11          // Conexion en la red-Led Amarillo
  #define indica3 12          // Sensor Activado-Led Verde
  
  int Encender   = 9;         // Pin Para encender SIM900   
  int ptrig      = 2;         // pin 3 como (ptrig) 
  int pecho      = 3;         // pin 2 como (pecho) 
  int duracion, distancia;    // Calcular distacia
  
  int contador1 = 2;
  int contador2 = 0;
  int estado;

  bool tiempoSistema;
  
  void setup()  { 
   Serial.begin (19200);
   pinMode(pecho,    INPUT);   
   pinMode(ptrig,    OUTPUT);  
   pinMode(Encender, OUTPUT); 
   pinMode(indica1,  OUTPUT);    
   pinMode(indica2,  OUTPUT); 
   pinMode(indica3,  OUTPUT);   
   pinMode(Zumbador, OUTPUT);

   
   Bienvenida();
   Encender_SIM900();
   Configuracion();
   
  } 

  void loop()  { 

   if (digitalRead(pulsador)== HIGH){
     estado=1;
   }

   if (estado == 1 && digitalRead(pulsador)== LOW){
     contador1++;
//     Serial.print("Numero ");
//     Serial.println(contador1);
     estado=0;
   }
     
    if(contador1 % 2==1){            // Activar sensor
      
       digitalWrite(indica3, HIGH);  // Luz indicacion-Sensor Activo  
       
       digitalWrite(ptrig, HIGH);    // Enviar un pulso de trigger por 10us
       delay(0.01);
       digitalWrite(ptrig, LOW);
     
       duracion = pulseIn(pecho, HIGH); // Lee el tiempo del Echo
       //distancia = duracion*0.017;    // calcula la distancia en centimetros
       distancia = duracion/58.8;
       delay(10); 

//       Serial.print("distancia en cm: ");
//       Serial.println(distancia);

      if (distancia<=10){
       delay(1000);
       tiempoSistema = true;
       digitalWrite(Zumbador, HIGH);
       Serial.println("Alerta! se detecto un movimiento");

       while( (EnviarComandoAT("AT+CREG?", "+CREG: 0,1", 1000) || EnviarComandoAT("AT+CREG?", "+CREG: 0,5", 1000)) == 0 );
       delay(1000);
       llamar();

       while( (EnviarComandoAT("AT+CREG?", "+CREG: 0,1", 1000) || EnviarComandoAT("AT+CREG?", "+CREG: 0,5", 1000)) == 0 );
       delay(1000);       
       mensaje();              
     }  
    }
    else{
     digitalWrite(indica3, LOW);     
    }

    tiempoSirena(); 
    
   }



 //------------------------- Funciones -------------------------//

   void Bienvenida(){
    Serial.println("******************************************");
    Serial.println("**** Bienvenidos al sistema de Alarma ****");
    Serial.println("************* Basica V1.0 ****************");
    Serial.println("******************************************");
    Serial.println("********** Diseño Electrónico A2P ********");
    Serial.println("******************************************");
   }

    void Encender_SIM900(){                            
     delay(2000);                                      
     digitalWrite(Encender,LOW);                       
     delay(1000);                                      
     digitalWrite(Encender,HIGH);                      
     delay(3000);                                      
     digitalWrite(Encender,LOW);                       
     delay(3000);    
     Serial.println("Modulo Sim900 Encendido...");
     digitalWrite(indica1, HIGH);     
     delay(3000);                                             
  }                                                    


   void Configuracion(){
    Serial.println("Configurando Alarma...");
    delay(3000);
    while( (EnviarComandoAT("AT+CREG?", "+CREG: 0,1", 1000) || EnviarComandoAT("AT+CREG?", "+CREG: 0,5", 1000)) == 0 );
    Serial.println("Alarma totalmente iniciada...");
    delay(5000);
    digitalWrite(indica2, HIGH);     
   }

   void llamar(){   //Funcion para Realizar llamada
     Serial.println("Realizando llamada...");
     Serial.println("ATD313XXXXXXX;");         
     delay(15000);
     Serial.println("ATH"); 
     delay(2000);     
   }


   void mensaje(){     //Funcion para enviar mensaje
     
     Serial.println("Enviando mensaje...");     
     Serial.print("AT+CMGF=1\r");  
     delay(1000);
     Serial.println("AT+CMGS=\"313XXXXXXX\"");  
     delay(1000);
     Serial.println("Advertencia! Se ha detectado movimiento...");  
     delay(100);
     Serial.println((char)26); 
     delay(1000);
     Serial.println("Mensaje Enviado...");
    }

    void tiempoSirena (){                                     
     if (tiempoSistema){                              
      contador2 ++;                                          
       if (contador2 >= 50){          // Duracion Zumbador                          
        digitalWrite(Zumbador, LOW);                                     
        delay(1000);                                           
        tiempoSistema = false;                                
       }                                                       
     }                                                         
     else                                                      
      contador2 = 0;                                         
   }                                                             


  int8_t EnviarComandoAT(char* ComandoAT, char* Resp_Esperada, unsigned int Retardo){

    uint8_t x=0,  Responde=0;
    char Respuesta[100];
    unsigned long Anterior;

    memset(Respuesta, '\0', 100);    // Borra el contenido del array
    delay(100);
    
    while( Serial.available() > 0)
    Serial.read();    // Limpie el buffer de entrada
    
    if (ComandoAT[0] != '\0') {
      Serial.println(ComandoAT);    // Enviar comando AT
//      Serial.println(ComandoAT);  // Mostrar comando por serial
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

 
