/*  - verificar que en el menu Herramientas este habilitado Placa (arduino uno) y puerto (/dev/ttyACM0)
 - En linux-Ubuntu dar permiso para leer y escribir en salida USB
      -copiar y pegar en Terminal para dar permiso al puerto: sudo chmod 666 /dev/ttyACM0
 - actualizat datos
      -fecha
      -estacion
 - compilar codigo
 - subir a placa
 
 * 1. sensor MQ7 para CO
 * 2. reloj 
 * Es necesario correr previamente PONER a Tiempo RTC
 * 3. sensor temperatura y humewdad DHT11
 * 4. sENSOR MQ3 para Alcohol
 * 5. tiempo de muestreo
 * 6. Micro SD
 * 7. bateria
 *  
 *  Especificaciones
 *  nos basamos en el codigo dht11 e incorporamos mq7
https://www.yo utube.com/watch?v=BIf_mpnsZvY

codigo 
corre bien en arduino del sistema, pero falla en arduino online
https://www.luisllamas.es/arduino-detector-gas-mq/


tesis 
http://www.ub.edu/bioelectronano/documents/Thesis_Raul_Diaz.pdf

calcular resistencioas
https://www.inventable.eu/paginas/ResCalculatorSp/ResCalculatorSp.html

new Calibrar sensores
https://www.youtube.com/watch?v=QYSDSKn2Vf8

codigo arduino 
http://electronfun.com/project_2.php

 *  Librerias por descargar

la liberia Arduino para sensor de Temperatura
-DHT11 
No funciona este código https://programarfacil.com/blog/arduino-blog/sensor-dht11-temperatura-humedad-arduino/


accuracy of +/-5% for humidity and 
+/-2 degrees Celsius for temperature
heat index (sensacion termica), 
relationshipt (multiples regresiones) between Humidity and temperatura
https://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml
ver tabla https://es.wikipedia.org/wiki/Temperatura_de_bochorno


-adafruit unified sensor (last version from
 https://github.com/adafruit/Adafruit_Sensor/archive/1.0.2.zip
 
Originalmente usamos la libreria 
#include <Time.h>
desde internet https://www.prometec.net/time-arduino/
esto esta fuera de uso
 
el Tiempo es por medio de adquirir el 
- Reloj RTC DS1307 , 
descragar la libreria (ver ejemplo en Example_time_RTC)
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

- es necesario poner a tiempo el reloj con el programa 
"poner-a-tiempo-rtc"
https://www.geekfactory.mx/tutoriales/tutoriales-arduino/ds1307-en-tinyrtc-con-arduino/

Compilado y escrito desde https://create.arduino.cc/editor
Es necesario instalar plugin para la interfaz 
No usar FireFox, no deja subir el Sketch
Usar Chrome

En Ubuntu
para saber los puertos disponibles
ls -l /dev/tty*


si indica que hay error en el puerto haz:
1. al lado de compilar, selecciona el tipo de arduino y el puerto
2. En ubuntu es necesario que des permisos a leer y escribir en el puerto (ttyACM0)
sudo chmod 666 /dev/ttyACM0
3. del lado izquiero, ve al menu "Monitor"
4. verifica que se este reportando las lectu


Pendientes
-Guardando datos tarjeta SD
http://www.educachip.com/como-leer-y-escribir-datos-en-la-tarjeta-sd-de-arduino/
 
 */
//////////////////////////////////////
// Variables por ajustar
int estacion =2 ;  
unsigned long tiempo1 =60000 ;   // 1 minuto  Laboratorio
//unsigned long tiempo1 =30000 ; // 0.5 minuto  DRONE 
///////////////////////////////////////




// int timeseconds = 0;  //this do not work beacause it reach limit to 32767 and then negative
unsigned long timeseconds = 0;
unsigned long time_1 = 0;


#include <SPI.h>  // librery SD
#include <SD.h>    // librery SD
const int chipSelect = 4;  // Micro SD
 
 
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <TimeLib.h>    //Descargar de harocorzo Github
#include <DS1307RTC.h>
// Definimos el pin digital donde se conecta el sensor dht11
#define DHTPIN 2
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

// Mq 7
// Digital pin 9 will be called 'pin9'
int pin9 = 9;
// Analog pin 0 will be called 'sensor'
int sensor = A0;
// Set the initial sensorValue to 0
int sensorValue = 0;
 
// Mq 3
// Digital pin 8 will be called 'pin8'
int pin8 = 8;
// Analog pin 0 will be called 'sensor'
int sensormq3 = A1;
// Set the initial sensorValue to 0
int sensorValuemq3 = 0;
int encabezado = 0;

// using millis instead delay
void print_time(unsigned long time_millis);







// The setup routine runs once when you press reset
void setup() {
// Comenzamos el sensor DHT
  dht.begin();
// Initialize the digital pin 8 as an output
  pinMode(pin9, OUTPUT);
// Initialize serial communication at 9600 bits per second
  Serial.begin(9600);

  // Starting SD
 Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  

  
}







// The loop routine runs over and over again forever
void loop() {

if(millis() > time_1){

    // save the current time 
    time_1 = millis() + tiempo1;
// print to SD
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);


if (encabezado == 0) {
//Header
Serial.println ("");  
Serial.println ("");  
Serial.print ("Proyecto Calidad del Aire con Arduino, escala de tiempo: ");
Serial.println(tiempo1); 
  Serial.print(estacion);
  Serial.println(" No. Estacion ");
//Datos por imprimir en pantalla
// Header encabezado de 1 sola vez
//////////////////////////////

  Serial.print("Fecha, hora, segundos, ");  
  Serial.print("Hum %, ");
  Serial.print("T(C),");
  Serial.print("icalor,");
  Serial.print("CO(PPM),");
  Serial.println("Al(PMM)");  


dataFile.println ("");  
dataFile.println ("");  
dataFile.print ("Proyecto Calidad del Aire con Arduino, escala de tiempo: ");
dataFile.println(tiempo1); 
dataFile.print(estacion);
  dataFile.println(" No. Estacion ");
//Datos por imprimir en pantalla
// Header encabezado de 1 sola vez
//////////////////////////////

  dataFile.print("Fecha, hora, seg,");  
  dataFile.print("Hum %, ");
  dataFile.print("T(C),");
  dataFile.print("icalor(C),");
  dataFile.print("CO(PPM),");
  dataFile.println("Al(PMM)");  

  
  encabezado = 1;
}


  
// Read the input on analog pin 0 (named 'sensor') MQ7
sensorValue = analogRead(sensor);
// Read the input on analog pin 1 (named 'sensor') MQ3
sensorValuemq3 = analogRead(sensormq3);

// Print out the value you read

 // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  // Leemos la temperatura en grados Fahreheit
  //float f = dht.readTemperature(true);
 
  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) ) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }
 
  // Calcular el índice de calor en Fahreheit
//  float hif = dht.computeHeatIndex(f, h);
  // Calcular el índice de calor en grados centígrados
  float hic = dht.computeHeatIndex(t, h, false);
 

   // Reloj: Declaramos una estructura "tm" llamada datetime para almacenar
  // La fecha y la hora actual obtenida desde el chip RTC.
  tmElements_t datetime;
 
  // Obtener la fecha y hora desde el chip RTC
  if (RTC.read(datetime)) {
 
    // Comenzamos a imprimir la informacion reloj
        Serial.print(datetime.Day);
    Serial.write(+ " ");
    Serial.print(datetime.Month);
    Serial.write(+ " ");
    Serial.print(tmYearToCalendar(datetime.Year));
    Serial.print(",");
    
//    print2digits(datetime.Hour);
    Serial.print(datetime.Hour);
    Serial.write(+ " ");
//    print2digits(datetime.Minute);
    Serial.print(datetime.Minute);
    Serial.write(+ " ");
//    print2digits(datetime.Second);
    Serial.print(datetime.Second);
    Serial.print(",");
// time in minutes
    timeseconds = datetime.Hour*60*60 + datetime.Minute*60 + datetime.Second;    
    Serial.print(timeseconds);
    Serial.print(",");
    Serial.write(+ " ");
    


  }
  else {
    if (RTC.chipPresent()) {
      // El reloj esta detenido, es necesario ponerlo a tiempo
      Serial.println(F("EL RELOJ DS1307 ESTA DETENIDO"));
      Serial.println(F("CORRE EL PROGRAMA: PONER A TIEMPO RTC, descargalo de harocorzo Github "));
    }
    else {
      // No se puede comunicar con el RTC en el bus I2C, revisar las conexiones.
      Serial.println(F("NO SE DETECTO EL RELOJ DS1307"));
      Serial.println(F("REVISA TUS CONEXIONES E INTENTA DE NUEVO"));
    }
    // Esperamos indefinidamente
    for (;;);
  }


//Datos por imprimir en pantalla
// Header encabezado que se repite en ciclo
//////////////////////////////

  Serial.print(h);
  Serial.print(",");
      Serial.write(+ " ");
  Serial.print(t);
  Serial.print(",");
      Serial.write(+ " ");
  Serial.print(hic);
  Serial.print(",");
   Serial.write(+ " ");
  Serial.print(sensorValue, DEC);
      Serial.write(+ " ");
      Serial.print(",");
   Serial.write(+ " ");
  Serial.println(sensorValuemq3, DEC);
//      Serial.write(+ " ");
// If sensorValue MQ7 is greater than 500
  if (sensorValue > 500) {
// Activate digital output pin 9 - the LED will light up
    digitalWrite(pin9, HIGH);
  }
  else {
// Deactivate digital output pin 9 - the LED will not light up
    digitalWrite(pin9, LOW);
  }


  // if the file is available, write to it:
  if (dataFile) {

 // Reloj: Declaramos una estructura "tm" llamada datetime para almacenar
  // La fecha y la hora actual obtenida desde el chip RTC.
  tmElements_t datetime;
 
  // Obtener la fecha y hora desde el chip RTC
  if (RTC.read(datetime)) {
 
    // Comenzamos a imprimir la informacion reloj
    dataFile.print(datetime.Day);
    dataFile.write(+ " ");
    dataFile.print(datetime.Month);
    dataFile.write(+ " ");
    dataFile.print(tmYearToCalendar(datetime.Year));
    dataFile.print(",");
    
//    print2digits(datetime.Hour);
    dataFile.print(datetime.Hour);
    dataFile.write(+ " ");
//    print2digits(datetime.Minute);
    dataFile.print(datetime.Minute);
    dataFile.write(+ " ");
//    print2digits(datetime.Second);
    dataFile.print(datetime.Second);
    dataFile.print(",");
    dataFile.write(+ " ");
    dataFile.print(timeseconds);
    dataFile.print(",");
    dataFile.write(+ " ");
  

}
  else {
    if (RTC.chipPresent()) {
      // El reloj esta detenido, es necesario ponerlo a tiempo
      Serial.println(F("EL RELOJ DS1307 ESTA DETENIDO"));
      Serial.println(F("CORRE EL PROGRAMA: PONER A TIEMPO RTC, descargalo de harocorzo Github "));
    }
    else {
      // No se puede comunicar con el RTC en el bus I2C, revisar las conexiones.
      Serial.println(F("NO SE DETECTO EL RELOJ DS1307"));
      Serial.println(F("REVISA TUS CONEXIONES E INTENTA DE NUEVO"));
    }
    // Esperamos indefinidamente
    for (;;);
  }


//Datos por imprimir en pantalla
// Header encabezado que se repite en ciclo
//////////////////////////////


  dataFile.print(h);
  dataFile.print(",");
      dataFile.write(+ " ");
  dataFile.print(t);
  dataFile.print(",");
      dataFile.write(+ " ");
  dataFile.print(hic);
  dataFile.print(",");
   dataFile.write(+ " ");
  dataFile.print(sensorValue, DEC);
      dataFile.write(+ " ");
      dataFile.print(",");
   dataFile.write(+ " ");
  dataFile.println(sensorValuemq3, DEC);

// If sensorValue MQ7 is greater than 500
  if (sensorValue > 500) {
// Activate digital output pin 9 - the LED will light up
    digitalWrite(pin9, HIGH);
  }
  else {
// Deactivate digital output pin 9 - the LED will not light up
    digitalWrite(pin9, LOW);
  }

    
    dataFile.close();
    // print to the serial port too:
   // Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
//  delay(tiempo1);

}  
}
