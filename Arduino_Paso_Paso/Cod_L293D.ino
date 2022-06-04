   
   //Motor paso a paso-Arduino-Puente H L293B
      
    #include <Stepper.h>  //librería para controlar motores paso a paso
    #define STEPS 200 //Número de pasos que necesita el motor para dar una vuelta. 
 
   
   Stepper stepper(STEPS, 8, 9, 10, 11); //STEPS Número de pasos por vuelta, definido anteriormente (200)
 
   void setup(){
    stepper.setSpeed(100); // Velocidad del motor en RPM
   }
 
   void loop(){
    stepper.step(200); //Girar una vuelta entera en un sentido, aumentar numero para mas vueltas
    delay(500); 
 
    stepper.step(-200); //Girar una vuelta entera en sentido contrario, aumentar numero para mas vueltas
    delay(500); 
   }
