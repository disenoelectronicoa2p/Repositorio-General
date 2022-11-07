
int Sensor_agua = 1;
int led=13;
int Lectura=0;
int Dato=0;

void setup()
{
 pinMode(led, OUTPUT);
 Serial.begin(9600);
}

void loop()
{
 Lectura=analogRead(Sensor_agua);
 if (Lectura>300)
  {
   digitalWrite(led,HIGH);
  }
   else
  {
   digitalWrite(led,LOW);
  }
   Dato=Lectura;
   Serial.print("Lectura: ");
   Serial.println(Dato);
   delay(100);
}

