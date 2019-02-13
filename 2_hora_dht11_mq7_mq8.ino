


/* 
 Pasos
 - verificar que en el menu Herramientas este habilitado Placa (arduino uno) y puerto (/dev/ttyACM0)
 - En linux-Ubuntu dar permiso para leer y escribir en salida USB
      -copiar y pegar en Terminal para dar permiso al puerto: sudo chmod 666 /dev/ttyACM0
 - actualizat datos
      -fecha
      -estacion
 - compilar codigo
 - subir a placa
Guardando datos tarjeta SD
http://www.educachip.com/como-leer-y-escribir-datos-en-la-tarjeta-sd-de-arduino/
 
 combinando los sensores dht11 y Mq7
 nos basamos en el codigo dht11 e incorporamos mq7
https://www.youtube.com/watch?v=BIf_mpnsZvY

Librerias por descargar

 la liberia Arduino para sensor de Temperatura
-DHT11 
accuracy of +/-5% for humidity and 
+/-2 degrees Celsius for temperature
heat index, rerlationshipt between Humidity and heat
https://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml


-adafruit unified sensor (last version from
 https://github.com/adafruit/Adafruit_Sensor/archive/1.0.2.zip
 
Originalmente usamos la libreria 
#include <Time.h>
desde internet https://www.prometec.net/time-arduino/
 
 pero al adquirir el 
- Reloj RTC DS1307 , 
descragar la libreria (ver ejemplo en Example_time_RTC)
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
- es necesario poner a tiempo el reloj con el programa "poner-a-tiempo-rtc"
https://www.geekfactory.mx/tutoriales/tutoriales-arduino/ds1307-en-tinyrtc-con-arduino/

Compilado y escrito desde https://create.arduino.cc/editor

*/

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
 
// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 2
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11

// Definiendo el valor del sensor mq7
int sensorValue;
int sensorValue1;

// Tiempo1, son los mili segundos entre lectura
int tiempo1 = 1000*60*30;
 
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  // Inicializamos comunicación serie
  // Sets the data rate (speed) in bits per second (baud) for serial data transmission. 
  //For communicating with the computer, use one of these rates: 
  //300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, or 115200.
  Serial.begin(9600);

  

  //Inicio  Actualizar
  //////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////
 
int estacion = 4;  
  //Fin  Actualizar
  //////////////////////////////////////////////////////////////////  
    //////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////

 
  // Comenzamos el sensor DHT
  dht.begin();
Serial.print ("Proyecto Calidad del Aire con Arduino, escala de tiempo: ");
Serial.println(tiempo1); 

// Header encabezado
  Serial.print(estacion);
  Serial.println(" No. Estacion ");
  Serial.print("Fecha, hora,");  
  Serial.print("Humedad %, ");
  Serial.print("Temp(C),");
  Serial.print("Temp(F),");  
  Serial.print("indice de calor (C),");
  Serial.print("indice de calor (F),");
  Serial.print("CO (V),");
  Serial.print("alcohol,");
  Serial.println("H2 (V)"); 


}
 
void loop() {

// Sensor mq7
  sensorValue = analogRead(0);       // read analog input pin 0
  const int R0 = 10;                      // 10k ohms
  float voltaje = sensorValue * (5.0 / 1023.0); //Convertimos la lectura en un valor de voltaje
  float Rs = 1000*((5-voltaje)/voltaje);  //Calculamos Rs con un RL de 1k
  double alcohol = 0.4091*pow(Rs/5463, -1.497);

// Sensor  mq8
  sensorValue1 = analogRead(1);      // read analog input pin 1
  
  
  
    // Declaramos una estructura "tm" llamada datetime para almacenar
  // La fecha y la hora actual obtenida desde el chip RTC.
  tmElements_t datetime;
 
  // Obtener la fecha y hora desde el chip RTC
  if (RTC.read(datetime)) {
 
    // Comenzamos a imprimir la informacion
        Serial.print(datetime.Day);
    Serial.write(+ " ");
    Serial.print(datetime.Month);
    Serial.write(+ " ");
    Serial.print(tmYearToCalendar(datetime.Year));
    Serial.print(",");
    
    print2digits(datetime.Hour);
    Serial.write(+ " ");
    print2digits(datetime.Minute);
    Serial.write(+ " ");
    print2digits(datetime.Second);
    Serial.print(",");
    


  }
  else {
    if (RTC.chipPresent()) {
      // El reloj esta detenido, es necesario ponerlo a tiempo
      Serial.println(F("EL DS1307 ESTA DETENIDO"));
      Serial.println(F("CORRE EL PROGRAMA PARA PONERLO A TIEMPO"));
    }
    else {
      // No se puede comunicar con el RTC en el bus I2C, revisar las conexiones.
      Serial.println(F("NO SE DETECTO EL DS1307"));
      Serial.println(F("REVISA TUS CONEXIONES E INTENTA DE NUEVO"));
    }
    // Esperamos indefinidamente
    for (;;);
  }
  
 

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
 


  Serial.print(h);
  Serial.print(",");
  Serial.print(t);
  Serial.print(",");
  Serial.print(f);
  Serial.print(",");
  Serial.print(hic);
  Serial.print(",");
  Serial.print(hif);
  Serial.print(",");
  Serial.print(sensorValue, DEC);  // prints the value read for MQ7
  Serial.print(",");
  Serial.print(alcohol);
  Serial.print(","); 
  Serial.println(sensorValue1, DEC);  // prints the value read for MQ8

 
}

/**
   Funcion auxiliar para imprimir siempre 2 digitos
*/
void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}