

int ptrig = 2;            // pin 3 como (ptrig) 
int pecho = 3;            // pin 2 como (pecho) 
int duracion, distancia;  // Calcular distacia

void setup()  { 
  Serial.begin (9600);
  pinMode(pecho, INPUT);   
  pinMode(ptrig,OUTPUT);   
} 

void loop()  { 

     digitalWrite(ptrig, HIGH); // Enviar un pulso de trigger por 10us
     delay(0.01);
     digitalWrite(ptrig, LOW);
     
     duracion = pulseIn(pecho, HIGH); // Lee el tiempo del Echo
     //distancia = duracion*0.017;    // calcula la distancia en centimetros
     distancia = duracion/58.8;
     delay(10); 

  Serial.print("distancia en cm: ");
  Serial.println(distancia);
 
}
