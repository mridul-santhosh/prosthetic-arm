#include <math.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

const int x_out = 32; /* connect x_out of module to A1 of UNO board */
const int y_out = 33; /* connect y_out of module to A2 of UNO board */
const int z_out = 35; /* connect z_out of module to A3 of UNO board */

void setup() {
  Serial.begin(115200); 
}

void loop() {
  int x_adc_value, y_adc_value, z_adc_value; 
  double x_g_value, y_g_value, z_g_value;
  double roll, pitch, yaw;
  x_adc_value = analogRead(x_out); /* Digital value of voltage on x_out pin */ 
  y_adc_value = analogRead(y_out); /* Digital value of voltage on y_out pin */ 
  z_adc_value = analogRead(z_out); /* Digital value of voltage on z_out pin */ 

  x_g_value = ( ( ( (double)(x_adc_value * 5)/4096) - 1.65 ) / 0.330 ); /* Acceleration in x-direction in g units */ 
  y_g_value = ( ( ( (double)(y_adc_value * 5)/4096) - 1.65 ) / 0.330 ); /* Acceleration in y-direction in g units */ 
  z_g_value = ( ( ( (double)(z_adc_value * 5)/4096) - 1.80 ) / 0.330 ); /* Acceleration in z-direction in g units */ 


  Serial.print("xval = ");
  Serial.print(x_g_value);
  Serial.print("\t");
  Serial.print("yval = ");
  Serial.print(y_g_value);
  Serial.print("\t");
  Serial.print("zval = ");
  Serial.print(z_g_value);
  Serial.print("\n");
//   delay(1000);
}