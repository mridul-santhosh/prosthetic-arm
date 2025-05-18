#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <SPI.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
 
void setup()
{
Serial.begin(115200);
mlx.begin();
}
 
void loop()
{
Serial.print("Ambient = ");
Serial.print(mlx.readAmbientTempC());
Serial.print("*C\tObject = ");
Serial.print(mlx.readObjectTempC());
Serial.println("*C");
Serial.print("Ambient = ");
Serial.print(mlx.readAmbientTempF());
Serial.print("*F\tObject = ");
Serial.print(mlx.readObjectTempF());
Serial.println("*F");
 
Serial.println();
delay(1000);
}