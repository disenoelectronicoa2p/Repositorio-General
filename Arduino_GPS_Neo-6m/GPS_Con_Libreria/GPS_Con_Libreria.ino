


  /* Conexion pines
     Arduino        GPS         
        D3           RX
        D4           TX
  */

  #include <SoftwareSerial.h>  //incluimos SoftwareSerial
  #include <TinyGPS.h>         //incluimos TinyGPS

  TinyGPS gps;  //Declaramos el objeto GPS
  SoftwareSerial serialgps(4,3);      //Declaramos el pin 4 (Tx del GPS) y 3 (Rx del GPS)
   
  //Declaramos la variables para la obtención de datos
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long chars;
  unsigned short sentences, failed_checksum;

  void setup()
  {
   Serial.begin(115200);  //Iniciamos el puerto serie
   serialgps.begin(9600); //Iniciamos el puerto serie del gps
   
   //Imprimimos en el monitor serial:
   Serial.println("");
   Serial.println("GPS GY-GPS6MV2 Leantec");
   Serial.println(" ---Buscando senal--- ");  
   Serial.println("");
  }

  void loop()
  {
   while(serialgps.available()) 
   {
    int c = serialgps.read(); 
    if(gps.encode(c)) 
    {
     float latitude, longitude;
     gps.f_get_position(&latitude, &longitude);
     Serial.print("Latitud/Longitud: "); 
     Serial.print(latitude,5); 
     Serial.print(", "); 
     Serial.println(longitude,5);
     gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
     Serial.print("Fecha: "); Serial.print(day, DEC); Serial.print("/"); 
     Serial.print(month, DEC); Serial.print("/"); Serial.print(year);
     Serial.print(" Hora: "); Serial.print(hour-5, DEC); Serial.print(":"); 
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
  }
