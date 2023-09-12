
#include <Separador.h>

Separador s;

#define ledAzul 5  // Pin 5: Led Azul 
#define ledVerde 6 // Pin 6: Led Verde
#define ledRojo 7  // Pin 7: Led Rojo
#define resetA6 22 // Pin 22: Resetear módulo A6 

String bodyMsm;
String commands;
String firstNumeral;

bool answerAtConectionRed;
int errorAtConectionRed = 0;

void setup() {

  pinMode(ledAzul, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(resetA6, OUTPUT);
  digitalWrite(resetA6, LOW);

  Serial.begin(115200);
  Serial1.begin(115200);

  sendAtConectionRed();

  Serial.println("Start!...");
  clearSerial("all");
}

void loop() {
  readDataA6();
}

void readDataA6() {
  String texto;

  while (!Serial1.available()) {}
  texto = Serial1.readString();
  delay(500);
  Serial.println(texto);
  clearSerial("all");

  if (texto.length() > 0) {
    separateMsm(texto);
    if (commands.equalsIgnoreCase("AzulOn")) {
      Serial.println("Se ha encendido led Azul");
      digitalWrite(ledAzul, HIGH);
    }

    if (commands.equalsIgnoreCase("azulOff")) {
      Serial.println("Se ha apagado led Azul");
      digitalWrite(ledAzul, LOW);
    }

    if (commands.equalsIgnoreCase("verdeOn")) {
      Serial.println("Se ha encendido led Verde");
      digitalWrite(ledVerde, HIGH);
    }

    if (commands.equalsIgnoreCase("verdeOff")) {
      Serial.println("Se ha apagado led Verde");
      digitalWrite(ledVerde, LOW);
    }

    if (commands.equalsIgnoreCase("RojoOn")) {
      Serial.println("Se ha encendido led Rojo");
      digitalWrite(ledRojo, HIGH);
    }

    if (commands.equalsIgnoreCase("RojoOff")) {
      Serial.println("Se ha apagado led Rojo");
      digitalWrite(ledRojo, LOW);
    }

    clearSerial("all");
  }
}

void separateMsm(String bodyMsm) {

  firstNumeral = s.separa(bodyMsm, '#', 0);   
  commands = s.separa(bodyMsm, '#', 1); 

  Serial.print("Comando:");
  Serial.println(commands);

  delay(500);
}

void clearSerial(String s) {
  if (s.equalsIgnoreCase("all")) {
    while (Serial.available() > 0)
      Serial.read();
    delay(1000);
    while (Serial1.available() > 0)
      Serial1.read();
    delay(500);
  } else if (s.equalsIgnoreCase("0")) {
    while (Serial.available() > 0)
      Serial.read();
    delay(500);
  } else if (s.equalsIgnoreCase("1")) {
    while (Serial1.available() > 0)
      Serial1.read();
    delay(500);
  }
}


bool enviarComandoAT(char* comandoAT, char* respEsperada, unsigned int retardo) {

  uint8_t x = 0;
  bool responde = false;
  char respuesta[100];
  unsigned long anterior;

  memset(respuesta, '\0', 100);    // Borra el contenido del array
  delay(100);

  while ( Serial1.available() > 0)
    Serial1.read();                  // Limpie el buffer de entrada

  if (comandoAT[0] != '\0') {
    Serial1.println(comandoAT);  // Enviar comando AT
    Serial.println(comandoAT);   // Mostrar comando por serial
  }

  x = 0;
  anterior = millis();

  // Este bucle espera la respuesta
  do {
    if (Serial1.available() != 0) {  // Si hay datos en el búfer de entrada de UART, léalo y verifica la respuesta
      respuesta[x] = Serial1.read();
      //          Serial.print(respuesta[x]);
      x++;
      if (strstr(respuesta, respEsperada) != NULL) {   //Verificar si la respuesta deseada (OK)
        responde = true;                               //está en la respuesta del módulo
      }
    }
  } while ((!responde) && ((millis() - anterior) < retardo));   // Espera la respuesta con tiempo de espera

  return responde;
}


void sendAtConectionRed() {

  while (!answerAtConectionRed) {

    //Descomentar si no conoce respuesta a este comando: AT+CREG?
    //  answerAtConectionRed = enviarComandoAT("AT", "OK", 1000);

    //Comentar si no conoce respuesta a este comando: AT+CREG?
    answerAtConectionRed = enviarComandoAT("AT+CREG?", "+CREG: 1,1", 1000);
    errorAtConectionRed++;
    if (errorAtConectionRed > 8) {
      errorAtConectionRed = 0;
      delay(2000);
      Serial.println("Reiniciando A6...");
      digitalWrite(resetA6, LOW);
      delay(100);
      digitalWrite(resetA6, HIGH);
      delay(100);
      digitalWrite(resetA6, LOW);
      delay(15000);
    }

    if (answerAtConectionRed) {
      errorAtConectionRed = 0;
      enviarComandoAT("AT", "OK", 1000);
      enviarComandoAT("AT+CLIP=1", "OK", 1000);
      enviarComandoAT("AT+CNMI=2,2,0,0,0", "OK", 1000); /* Transmisión de mensaje directamente al puerto serial del dispositivo. */
      enviarComandoAT("AT+CMGD=1,4", "OK", 1000);
      enviarComandoAT("AT+CMGF=1", "OK", 2000);
    }
  }
  answerAtConectionRed = false;
}
