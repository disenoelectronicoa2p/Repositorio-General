
#include <DHT11.h>
#include <ArduinoJson.h>
#include "esp_task_wdt.h"

#define DTH11_PIN 2       //Pin 2: Sensor DHT11
#define RESET_SIM800L 5   //Pin 5: Resetear modulo Sim800l

DHT11 dht11(DTH11_PIN);

bool validateConfig;

unsigned long measurementTime = 0; //Contador para envio mensaje
unsigned long tiempoPrevioMSN;     //Contador para control envio mensaje

float temp,hum;   
long currentTemp,currentHum;
int err;
String data;

void setup() {

  Serial.begin(115200);
  Serial2.begin(115200);
  pinMode(RESET_SIM800L, OUTPUT);

  disableCore1WDT(); 

  measurementTime = 20000; // 20 segundos

    /**
  **Valida si el sim800l se concecto correctamente a la red movil
  **Descomentar la siguiente linea de codigo "sendAtConectionRed()" si se requiere validar esta accion.
  **/
  //sendAtConectionRed(); 

  //Cierra la sesión GPRS si esta activa.
  enviarComandoAT("AT+SAPBR=0,1", "OK", 3000);  
  
  // Configura el tipo de conexión a GPRS
  enviarComandoAT("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", 3000); 
  
  // Configura la APN para la conexión GPRS
  enviarComandoAT("AT+SAPBR=3,1,\"APN\",\"internet.comcel.com.co\"", "OK", 3000); 

  Serial.println("Start!...");

  randomSeed(analogRead(0));
}

void loop() {

  while (!validateConfig) {
    bool answer = false;
    answer = enviarComandoAT("AT+SAPBR=1,1", "OK", 2000); //Activa la conexión GPRS
    if (answer) {
      answer = enviarComandoAT("AT+HTTPINIT", "OK", 2000); //Inicia el servicio HTTP en el SIM800L
      if (answer) {
        answer = enviarComandoAT("AT+HTTPPARA=\"CID\",1", "OK", 4000); //Configura el Context Identifier (CID) para la conexión HTTP
        if (answer) {
          enviarComandoAT("AT+HTTPPARA=\"URL\",\"http://prueba-sim800l-post.atwebpages.com/test-plant-humidity-json.php\"", "OK", 3000);
          enviarComandoAT("AT+HTTPPARA=\"CONTENT\",\"application/json\"", "OK", 2000);
          validateConfig = true;
        }
      } else {
        enviarComandoAT("AT+HTTPTERM", "OK", 2000);
      }
    } else {
      enviarComandoAT("AT+SAPBR=0,1", "OK", 2000);
    }
  }

  unsigned long tiempoActualMSN = millis(); //Inicio contador
  if ((unsigned long)(tiempoActualMSN - tiempoPrevioMSN) >= measurementTime) {
    
    if((err=dht11.read(hum, temp))==0){
      currentHum = hum,2;
      currentTemp = temp,2;
    }else{
      Serial.println();
      Serial.print("Error No :");
      Serial.print(err);
      Serial.println();
    }

    // Crear el objeto JSON
    StaticJsonDocument<200> doc;
    doc["humidity"] = currentHum;
    doc["temperature"] = currentTemp;

    // Convertir el JSON a string
    String jsonString;
    serializeJson(doc, jsonString);

    //enviarComandoAT("AT+HTTPDATA=43,5000", "DOWNLOAD", 5000);
    Serial2.print("AT+HTTPDATA=");
    Serial.print("AT+HTTPDATA=");
    Serial2.print((jsonString.length()+10));
    Serial.print((jsonString.length()+10));
    Serial2.println(",5000"); // Tiempo máximo de 5 segundos para enviar los datos
    Serial.println(",5000");
    enviarComandoAT("", "", 4000);

    // Enviar el JSON
    Serial2.println(jsonString);
    Serial.println(jsonString);
    delay(2000); 

    Serial2.println("AT+HTTPACTION=1"); //Se realizará una solicitud HTTP POST
    Serial.println("AT+HTTPACTION=1");
    enviarComandoAT("", "", 8000);
   
    bool answer = false;
    answer = enviarComandoAT("AT+HTTPREAD", "Data saved successfully", 3000);
    if(answer){
      Serial.println("Datos guardados correctamente.");
      currentHum = 0; currentTemp = 0; answer = false;
    }else{
      Serial.println("No se pudo guardar los datos.");     
    }

    tiempoPrevioMSN = tiempoActualMSN;
  }

}

//================================
//CONFIG SIM800L
void sendAtConectionRed() {

  bool answerAtConectionRed = false;
  int errorAtConectionRed = 0;
  while ( !answerAtConectionRed ) {
    answerAtConectionRed = enviarComandoAT("AT+CREG?", "+CREG: 0,1", 1000) || enviarComandoAT("AT+CREG?", "+CREG: 0,5", 1000);
    errorAtConectionRed++;
    if (errorAtConectionRed > 8) {
      errorAtConectionRed = 0;
      delay(2000);
      Serial.println("Reiniciando SIM800L...");
      enviarComandoAT("AT+CFUN=1,1", "OK", 6000); 
      delay(4000);
    }

    if (answerAtConectionRed) {
      errorAtConectionRed = 0;
      enviarComandoAT("AT", "OK", 1000);
    }
  }
}

bool enviarComandoAT(char* comandoAT, char* respEsperada, unsigned int retardo) {
  uint8_t x = 0;
  bool responde = false;
  char respuesta[100];
  unsigned long anterior;

  memset(respuesta, '\0', 100);    // Borra el contenido del array
  delay(100);

  while (Serial2.available() > 0)
    Serial2.read();                  // Limpie el buffer de entrada

  if (comandoAT[0] != '\0') {
    Serial2.println(comandoAT);  // Enviar comando AT
    Serial.println(comandoAT);   // Mostrar comando por serial
  }

  x = 0;
  anterior = millis();

  // Este bucle espera la respuesta
  do {
    if (Serial2.available() != 0) {  // Si hay datos en el búfer de entrada de UART, léalo y verifica la respuesta
      respuesta[x] = Serial2.read();
      //          Serial.print(respuesta[x]);
      x++;
      if (strstr(respuesta, respEsperada) != NULL) {   //Verificar si la respuesta deseada (OK)
        responde = true;                               //está en la respuesta del módulo
      }
    }
  } while ((!responde) && ((millis() - anterior) < retardo));   // Espera la respuesta con tiempo de espera

  return responde;
}


