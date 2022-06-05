
/*
  Código para Sensor de medición ultrasónico SRF02 utilizando Arduion Uno ó Arduino Nano,
  por comunicacion Serial (UART)
  SRF02         Arduino
  5V Vcc   ====> 5V
  Rx       ====> 2(TX)
  Tx        ===> 3(RX)
  Mode      ===> GND - Para utilizar como comunicacion Serial
  0V Ground ===> GND
*/

#include <SoftwareSerial.h>
SoftwareSerial SRF02(3, 2); // SDL=RX, SCL=TX

#define sirena 6  //Declaracion del pin para activar sumbador

//Función para definir la direcció y enviar comando al SRF02
void EnvioComando(unsigned char direccion, unsigned char comando) 
{
  SRF02.write(direccion); // Definimos la direccion del sensor SRF02 (Por defecto 0)
  delayMicroseconds(100); //Retardo de 100 microsegundos 
  SRF02.write(comando); //Envío del comando al SRF02
  delayMicroseconds(100); //Retardo de 100 microsegundos 
}

void setup(void)
{
  Serial.begin(9600);
  SRF02.begin(9600);
  Serial.println("SRF02 TEST!");
  pinMode(sirena, OUTPUT);
}

void loop(void)
{
  unsigned int LeerDista;
  EnvioComando(0x00, 0x51); //Se escribir sobre el registro 0x00 -- 0x51 Devuelve el resultado en centimetros
  delay(70);//Esperar que el SRF02 Mida el rango
  EnvioComando(0x00, 0x5E); //Se escribir sobre el registro 0x00 -- Leer la medida, Devuelve dos bytes 
  delay(10);//Esperarque el arduino reciba los dos bytes


  if (SRF02.available() >= 2) //Si se reciben dos bytes
  {
    LeerDista = SRF02.read() << 8; //Recibe el byts alto de 8 bits
    LeerDista |= SRF02.read(); // Recibe el byts mas bajos de 8 bits
    Serial.print(LeerDista); // Imprimimos la lectura
    Serial.println("cm");
  }

  if (LeerDista < 20) { // Distancia minima de 30 Centimetros
    digitalWrite(sirena, HIGH);   
  }
  else {
    digitalWrite(sirena, LOW);    
  }
  delay(250); 
}
