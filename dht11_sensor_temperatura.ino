/* 
 
 1. En el menu herramientas verificar: Placa(arduino) y puerto(comUSB) 
 2. Para este sensor DHT11 hay varios problemas, 
 porque hay 2 diagramas para conectar:
 ver comentarios en el manual de Programar Facil
 https://programarfacil.com/blog/arduino-blog/sensor-dht11-temperatura-humedad-arduino/#Programando_el_DHT11_desde_el_IDE_de_Arduino
este diagrama no concuerda con este otro diagrama 
http://mthackathon.info/resources/37-SENSOR-KIT-TUTORIAL-FOR-UNO-AND-MEGA.pdf
Nuestro sensor funcionó con el este último 

3. Usamos el código básico de 
https://programarfacil.com/blog/arduino-blog/sensor-dht11-temperatura-humedad-arduino/#Programando_el_DHT11_desde_el_IDE_de_Arduino

4. Para evitar los errores en la compilación, se debe de incluimos las sig, dos librerías (mas fácil de procesar información)
 ir a programa -> include library -> manage libraries
 DHT
 adafruit unified sensor
 Agregar la linea  #Adafruit_sensor.h   de acuerdo a 
 https://github.com/adafruit/DHT-sensor-library/issues/62
 en el directorio .../Arduino/libraries/Adafruit_Unified_Sensor/

5 Para ver los datos generados por el sensor, ir a menu
 herramientas -> Monitor Serie 
 
*/

#include <Adafruit_Sensor.h>
#include <DHT.h>
 
// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 2
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11
 
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  // Inicializamos comunicación serie
  Serial.begin(9600);
 
  // Comenzamos el sensor DHT
  dht.begin();
 
}
 
void loop() {
    // Esperamos 5 segundos entre medidas
  delay(5000);
 
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
 
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Índice de calor: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
 
}
