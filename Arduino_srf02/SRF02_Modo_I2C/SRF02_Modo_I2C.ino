
   /* Código para Sensor de medición ultrasónico SRF02 utilizando Arduion Uno ó Arduino Nano,
      por comunicacion I2C
      
      SRF02       Arduino
      5v Vcc    -> 5V
      SDA       -> A4
      SCL       -> A5
      Mode      -> No conectar
      0v Ground -> GND
   */

  #include <Wire.h> // Libreria que permite la comunicacion I2C
  
  #define ADDRESS  115 // Se definimos la direccion del sensor SRF02 (112)
  #define sirena 6    // Declaracion del pin para activar sumbador

  void setup(){
   Serial.begin(9600);       //Se inicia el puerto serie
   pinMode(sirena, OUTPUT);
   Wire.begin();
  }
 
  void loop(){
   byte ByteAlto, ByteBajo;
   int Distancia;

   Wire.beginTransmission(ADDRESS); // Se empieza la comunicación con el sensor SFR02
   Wire.write(byte(0x00));          // Se escribe en el regostro (0x00)
   Wire.write(byte(0x51));          // Devuelve el resultado en centimetros
   Wire.endTransmission();          // Final de la transmisión

   delay(70); // Retardo de 70ms para que el sensor ejecute los comando enviados 
  
   Wire.beginTransmission(ADDRESS); // Se empieza la comunicación con el sensor SFR02
   Wire.write(byte(0x02));          // Se ubica en el registro #2 para leer el Byte alto de la medida realizada
   Wire.endTransmission();          // Final de la transmisión
   
   Wire.requestFrom(ADDRESS, 2);    // Se solicita dos Bytes al sensor SFR02 (2 bytes)
  
   while(Wire.available() < 2);     // Se comprueba si los bytes llegan
  
   ByteAlto = Wire.read();  //Se lee el Byte alto
   ByteBajo = Wire.read();  //Se lee el Byte bajo
 
   Distancia = ((ByteAlto<<8) + ByteBajo); //Calcula la distancia de un objeto al frente del sensor

  if (Distancia < 50){
      digitalWrite(sirena, HIGH);        }
    else {
      digitalWrite(sirena, LOW);    
     } 

    Serial.print("Distancia: ");
    Serial.println(Distancia, DEC);
 
   delay(20);
  }
