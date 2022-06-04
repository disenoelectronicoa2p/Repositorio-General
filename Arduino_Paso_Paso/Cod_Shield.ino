
   /* Motor Shield de Arduino, Motor Paso a Paso Unipolar.................
     Como la Shield está hecha para conectarse directamente sobre el Arduino UNO, esta se puede 
     conectar solo con los pines que funcionan, que son: 4, 7, 8, 12, a un Arduino Nano u otra 
     placa mediante jumper o cables y conectar a su respectiva alimentación (5v y GND) y los  12V 
     para alimentar el motor.
     
       Arduino         Motor Shield
         4    ============> 4  
         7    ============> 7
         8    ============> 8
         12   ============> 12
         5v   ============> 5v
         GND  ============> GND
   */
   
   #include <AccelStepper.h>  // librería para controlar motores paso a paso
   #include <AFMotor.h>       // librería para usar la tarjeta Shield donde van conectados los motores
   AF_Stepper motor(256, 2);  // 256 son los Steps por vuelta para el motor
                              // 1 si se conecta en la Shield el motor en M1 y M2, 2 si se conecta el motor en M3 y M4

  void setup() {
   motor.setSpeed(80); // 80 rpm 
  }

  void loop() {
   motor.step(1000, FORWARD, DOUBLE); // 1000=Número de pasos que gira el motor
   delay(500);                        // FORWARD=Motor hacia Adelante, DOUBLE=Se excitan dos bobinas cada vez
        
   motor.step(1000, BACKWARD, DOUBLE); //BACKWARD=Motor hacia atrás
   delay(500);
  }
