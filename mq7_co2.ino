/* Ver el video para conectarlo
 * https://www.youtube.com/watch?v=BIf_mpnsZvY
 * Conexión http://wiring.org.co/learning/basics/airqualitymq135.html
 * Pines https://cdn.sparkfun.com/datasheets/BreakoutBoards/Gas_Sensor_Breakout-v12.pdf
 * productores https://www.sparkfun.com/products/8891
 * 
 */

int sensorValue;

void setup()
{
  Serial.begin(9600);      // sets the serial port to 9600
}

void loop()
{
  sensorValue = analogRead(0);       // read analog input pin 0
  Serial.println(sensorValue, DEC);  // prints the value read
  delay(100);                        // wait 100ms for next reading
}

