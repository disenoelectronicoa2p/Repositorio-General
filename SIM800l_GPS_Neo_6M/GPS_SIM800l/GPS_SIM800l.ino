
  /* Conexion pines
     Arduino        GPS         
        D1 (Tx)     RX
        D0 (Rx)     TX

                   SIM800L
        D2           TX
        D3           RX
        Reset        RST
  */

  #include <SoftwareSerial.h>    // incluimos SoftwareSerial
  #include <TinyGPS.h>           // incluimos TinyGPS
  
  #define SIM800_PULSADOR 9      // Pulsador para enviar mensaje
  float latitud, longitud;       // Variable que guarda la latitud y Longitud
  
  int contador = 0;              // Variable para controlar la cantidad de mensajes enviado
 
  TinyGPS gps;                       // Declaramos el objeto GPS
  SoftwareSerial MOD_SIM800L(2,3);   // Declaramos el objeto SIM800L en los pines D2 y D3 del arduino
  
  //Declaramos la variables para la obtención de datos
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long chars;
  unsigned short sentences, failed_checksum;

  void setup()
  {
   Serial.begin(9600);        //Iniciamos el puerto serie
   MOD_SIM800L.begin(115200); //Iniciamos el puerto serie del SIM800
   while(!Serial);
   pinMode(SIM800_PULSADOR, INPUT); // Decalaramos el pin del pulsador como entrada
   
   //Imprimimos en el monitor serial:
   Serial.println("");
   Serial.println("GPS GY-GPS6MV2 Leantec");
   Serial.println(" ---Buscando senal--- ");    
   Serial.println("");
  }

  void loop()
  {

  while(Serial.available()) 
   {
    int c = Serial.read(); 
    if(gps.encode(c)) 
    {
     gps.f_get_position(&latitud, &longitud);
     Serial.print("Latitud/Longitud: "); 
     Serial.print(latitud,5); 
     Serial.print(", "); 
     Serial.println(longitud,5);
     gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
     Serial.print("Fecha: "); Serial.print(day, DEC); Serial.print("/"); 
     Serial.print(month, DEC); Serial.print("/"); Serial.print(year);
     Serial.print(" Hora: "); Serial.print(hour, DEC); Serial.print(":"); 
     Serial.print(minute, DEC); Serial.print(":"); Serial.print(second, DEC); 
     Serial.print("."); Serial.println(hundredths, DEC);
     Serial.print("Altitud (metros): "); Serial.println(gps.f_altitude()); 
     Serial.print("Rumbo (grados): "); Serial.println(gps.f_course()); 
     Serial.print("Velocidad(kmph): "); Serial.println(gps.f_speed_kmph());
     Serial.print("Satelites: "); Serial.println(gps.satellites());
     Serial.println();
     gps.stats(&chars, &sentences, &failed_checksum);
    }
   }

    if((digitalRead(SIM800_PULSADOR) == 1)&&(contador == 0)){
    Serial.println("Configuracion Completa!");
    Serial.println("Enviando SMS...");
   
    //Se establece el formato de SMS en ASCII
    MOD_SIM800L.write("AT+CMGF=1\r\n");
    delay(100);
 
    //Enviar comando para un nuevos SMS al numero establecido
    MOD_SIM800L.write("AT+CMGS=\"+573114009876\"\r\n");
    delay(100);
   
    //Enviar contenido del SMS
    MOD_SIM800L.println("Latitud/Longitud: ");
    MOD_SIM800L.print(latitud,5);
    MOD_SIM800L.print(" ");
    MOD_SIM800L.print(longitud,5);
    delay(1000);
   
    //Enviar Ctrl+Z 
    MOD_SIM800L.write((char)26);
    delay(200);
     
    Serial.println("Mensaje enviado!");
   }
  }
