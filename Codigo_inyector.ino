#include "I2Cdev.h" 
#include "MPU6050.h" 
#include "Wire.h" 
MPU6050 sensor; 
volatile int NumPulsos; 
int PinSensor = 2; 
float factor_conversion = 7.5; 
int anguloActual =0; 
int Redpin = 9; 
int Greenpin = 10; 
int Bluepin = 11; 
int red_val = 0; 
int green_val = 0; 
int blue_val = 0; 
int pinRele = 7; 
bool activado = HIGH; 
unsigned long tiempoAnterior = 0; 
const unsigned long intervalo = 1000; 
int ax, ay, az; 
void ContarPulsos(){ 
NumPulsos++; 
} 
void setup() { 
pinMode(Redpin,OUTPUT); 
pinMode(Greenpin,OUTPUT); 
pinMode(Bluepin,OUTPUT); 
pinMode(pinRele,OUTPUT); 
pinMode(PinSensor, INPUT_PULLUP); 
Serial.begin(9600); 
Wire.begin(); 
sensor.initialize(); 
if (sensor.testConnection())  
Serial.println("Sensor iniciado correctamente");  
  else  
    Serial.println("Error al iniciar el sensor"); 
 
  attachInterrupt(0, ContarPulsos, RISING); 
 
} 
 
void loop() { 
  unsigned long tiempoActual = millis(); 
   
  // Verifica si ha pasado 1 segundo 
  if (tiempoActual - tiempoAnterior >= intervalo) { 
    tiempoAnterior = tiempoActual; 
    int frecuencia = NumPulsos; 
    NumPulsos = 0; // Reinicia el conteo de pulsos para la próxima medición 
    float caudal_L_m = frecuencia / factor_conversion; // Calcula el caudal 
 
    sensor.getAcceleration(&ax, &ay, &az); 
    int accel_ang_y = round(atan(ay / sqrt(pow(ax, 2) + pow(az, 2))) * (180.0 / 3.14)); 
 
    Serial.print(caudal_L_m, 2); 
    Serial.print(","); 
    Serial.println(accel_ang_y); 
  } 
   
  // put your main code here, to run repeatedly: 
  if (Serial.available()>0){ 
    String letra = Serial.readString(); 
    letra.trim(); 
    if(letra == "r"){ 
      red_val = 0; 
      green_val = 255; 
      blue_val = 255; 
    }else if(letra == "g"){ 
      red_val = 255; 
      green_val = 0; 
      blue_val = 255; 
    }else if(letra == "b"){ 
      red_val = 255; 
      green_val = 255; 
      blue_val = 0; 
    }else if(letra == "l"){ 
      red_val = 255; 
      green_val = 0; 
      blue_val = 0;  
    }else if(letra == "n"){ 
      red_val = 0; 
      green_val = 215; 
      blue_val = 255; 
    }else if(letra == "m"){ 
      red_val = 135; 
      green_val = 255; 
      blue_val = 0; 
    }else if(letra == "x"){ 
      red_val = 255; 
      green_val = 255; 
      blue_val = 255; 
      Serial.println("apagado"); 
    } 
    
      if (letra != "p") { 
        analogWrite(Redpin, red_val); 
        analogWrite(Greenpin, green_val); 
        analogWrite(Bluepin, blue_val); 
      } 
       
      delay(10); 
      while (Serial.available() > 0) Serial.read(); 
     
  } 
} 