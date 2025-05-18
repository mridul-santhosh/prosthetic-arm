#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Adafruit_MPU6050.h>
#include <ESP32Servo.h>
#include <Adafruit_MLX90614.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

Adafruit_MPU6050 mpu;

WiFiServer server(80);

Servo ringservo;  // create servo object to control a servo
Servo middleservo;  // create servo object to control a servo
Servo pinkyservo;  // create servo object to control a servo
Servo thumbservo;  // create servo object to control a servo
Servo indexservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
int Middlepin = 25;
int Ringpin = 27; 
int Pinkeypin = 33;
int Thumbpin = 32; 
int Indexpin = 26;

int vib1 = 12;
int vib2 = 13;
int vib3 = 14;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

volatile int analogValue = 0;


void grab()
{
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    indexservo.write( pos);    // tell servo to go to position in variable 'pos'
    ringservo.write(180 - pos);    // tell servo to go to position in variable 'pos'
    middleservo.write(pos);    // tell servo to go to position in variable 'pos'
    pinkyservo.write( 180 - pos);    // tell servo to go to position in variable 'pos'
    thumbservo.write(180 - pos);    // tell servo to go to position in variable 'pos'
    delay(10);             // waits 15ms for the servo to reach the position
  }
  
}
void one()
{
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    indexservo.write(180 - pos);    // tell servo to go to position in variable 'pos'
    ringservo.write(180 - pos);    // tell servo to go to position in variable 'pos'
    middleservo.write(pos);    // tell servo to go to position in variable 'pos'
    pinkyservo.write( 180 - pos);    // tell servo to go to position in variable 'pos'
    thumbservo.write(180 - pos);    // tell servo to go to position in variable 'pos'
    delay(10);             // waits 15ms for the servo to reach the position
  }
  
}
void two()
{
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    indexservo.write(180 - pos);    // tell servo to go to position in variable 'pos'
    ringservo.write(180 - pos);    // tell servo to go to position in variable 'pos'
    middleservo.write(180 -pos);    // tell servo to go to position in variable 'pos'
    pinkyservo.write( 180 - pos);    // tell servo to go to position in variable 'pos'
    thumbservo.write(180 - pos);    // tell servo to go to position in variable 'pos'
    delay(10);             // waits 15ms for the servo to reach the position
  }
  
}
void three()
{
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    indexservo.write(180 - pos);    // tell servo to go to position in variable 'pos'
    ringservo.write(pos);    // tell servo to go to position in variable 'pos'
    middleservo.write(180 -pos);    // tell servo to go to position in variable 'pos'
    pinkyservo.write( 180 - pos);    // tell servo to go to position in variable 'pos'
    thumbservo.write(180 - pos);    // tell servo to go to position in variable 'pos'
    delay(10);             // waits 15ms for the servo to reach the position
  }
  
}
void four()
{
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    indexservo.write(180 - pos);    // tell servo to go to position in variable 'pos'
    ringservo.write(pos);    // tell servo to go to position in variable 'pos'
    middleservo.write(180 -pos);    // tell servo to go to position in variable 'pos'
    pinkyservo.write( pos);    // tell servo to go to position in variable 'pos'
    thumbservo.write(180 - pos);    // tell servo to go to position in variable 'pos'
    delay(10);             // waits 15ms for the servo to reach the position
  }
  
}



void taskOne(void *pvParameters) {
  for (;;) {
    float temperature = mlx.readObjectTempC();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print("Temperature: ");
    display.print(temperature);
    display.print(" C");
    display.display();
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
  /* Print out the values */
    Serial.print("Acceleration X: ");
    Serial.print(a.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
    Serial.println(" m/s^2");

    Serial.print("Rotation X: ");
    Serial.print(g.gyro.x);
    Serial.print(", Y: ");
    Serial.print(g.gyro.y);
    Serial.print(", Z: ");
    Serial.print(g.gyro.z);
    Serial.println(" rad/s");

    Serial.print("Temperature: ");
    Serial.print(temp.temperature);
    Serial.println(" degC");

    Serial.println("");
      delay(1000);
  }
}

void taskTwo(void *pvParameters) {
  pinMode(35, INPUT);
  servo.attach(2);
  for (;;) {
    analogValue = analogRead(35);
    int angle = map(analogValue, 0, 4095, 0, 180);
    servo.write(angle);
    analogWrite(vib1, 125);

    Serial.print("Analog value: ");
    Serial.println(analogValue);
    delay(10);
  }
}

void setup() {
  Serial.begin(9600);

  //servo init
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  ringservo.setPeriodHertz(50);    // standard 50 hz servo
  middleservo.setPeriodHertz(50);    // standard 50 hz servo
  pinkyservo.setPeriodHertz(50);    // standard 50 hz servo
  thumbservo.setPeriodHertz(50);    // standard 50 hz servo
  indexservo.setPeriodHertz(50);    // standard 50 hz servo
  ringservo.attach(Ringpin, 500, 2400); // attaches the servo on pin 18 to the servo object
  middleservo.attach(Middlepin , 500, 2400); // attaches the servo on pin 18 to the servo object
  pinkyservo.attach(Pinkeypin, 500, 2400); // attaches the servo on pin 18 to the servo object
  thumbservo.attach(Thumbpin, 500, 2400); // attaches the servo on pin 18 to the servo object
  indexservo.attach(Indexpin, 500, 2400); // attaches the servo on pin 18 to the servo object

  pinMode(vib1, OUTPUT);
  pinMode(vib2, OUTPUT);
  pinMode(vib3, OUTPUT);


  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

    if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  WiFi.softAP("ESP32 WebServer");
  server.begin();

  xTaskCreatePinnedToCore(
    taskOne,
    "Task One",
    10000,
    NULL,
    1,
    NULL,
    0
  );

  xTaskCreatePinnedToCore(
    taskTwo,
    "Task Two",
    10000,
    NULL,
    1,
    NULL,
    1
  );
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");

    client.println("<html>");
    client.println("<head><title>ESP32 WebServer</title></head>");
    client.println("<body>");
    client.print("Temperature: ");
    client.print(mlx.readObjectTempC());
    client.println(" C<br>");
    client.print("Accelerometer: ");
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);
    client.print(ax);
    client.print(", ");
    client.print(ay);
    client.print(", ");
    client.println(az);
    client.print("Analog value: ");
    client.println(analogValue);
    client.println("</body>");
    client.println("</html>");

    delay(1);
    client.stop();
  }
}