/* 
 Liberias 
     de arduino
 -Adafruit unified sensor
 -DHT
 
    de internet
 -Time   https://www.prometec.net/time-arduino/
 
 Ubuntu linux
 copiar y pegar para dar permiso al puerto
 sudo chmod 666 /dev/ttyACM0
 
 combinando los sensores dht11 y Mq7
 nos basamos en el codigo dht11 e incorporamos mq7
*/

#include <Adafruit_Sensor.h>
#include <DHT.h>

// incluir el tiempo 
#include <Time.h>
 
// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 2
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11

// Definiendo el valor del sensor mq7
int sensorValue;

// Tiempo1, son los mili segundos entre lectura
int tiempo1 = 1000*60;
 
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  // Inicializamos comunicación serie
  // Sets the data rate (speed) in bits per second (baud) for serial data transmission. 
  //For communicating with the computer, use one of these rates: 
  //300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, or 115200.
  Serial.begin(9600);
  
  // fijando hora,minutos segundos dia mes año
  setTime(13,55,00,1,10,2018);
  

 
  // Comenzamos el sensor DHT
  dht.begin();
Serial.print ("Proyecto Calidad del Aire con Arduino, escala de tiempo: ");
Serial.println(tiempo1); 
}
 
void loop() {
    // incorporamos el código mq7
  sensorValue = analogRead(0);       // read analog input pin 0
  time_t tiempo = now();

      Serial.print(day(tiempo));
      Serial.print(+ " ") ;
      Serial.print(month(tiempo));
      Serial.print(+ " ") ;
      Serial.print(year(tiempo)); 
      Serial.print( " ") ;
      Serial.print(hour(tiempo));  
      Serial.print(+ " ") ;
      Serial.print(minute(tiempo));
      Serial.print(" ") ;
      Serial.print(second(tiempo));

  // dht11

  delay(tiempo1);


  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  // Leemos la temperatura en grados Fahreheit
  float f = dht.readTemperature(true);
 
  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }
 
  // Calcular el índice de calor en Fahreheit
  float hif = dht.computeHeatIndex(f, h);
  // Calcular el índice de calor en grados centígrados
  float hic = dht.computeHeatIndex(t, h, false);
 

  Serial.print(" Humedad: ");
  Serial.print(h);
  Serial.print(" % ");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F ");
  Serial.print("indice de calor: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.print(" *F ");
  Serial.print(" Gas CO ");
  Serial.print(sensorValue, DEC);  // prints the value read for MQ7
  Serial.println(" PPM");
 
}
