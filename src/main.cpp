#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_MPU6050.h>
#include <ESP32Servo.h>

// OLED display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
bool release = true;

// MLX90614 temperature sensor setup
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// MPU6050 accelerometer/gyroscope setup
Adafruit_MPU6050 mpu;

Servo ringservo;   // create servo object to control a ringservo
Servo middleservo; // create servo object to control a middleservo
Servo pinkyservo;  // create servo object to control a pinkyservo
Servo thumbservo;  // create servo object to control a thumbservo
Servo indexservo;  // create servo object to control a indexservo

//reset the flags
bool grabbed = false;
bool released = false;

TaskHandle_t core1TaskHandle = NULL; // Handle to the task running on the primary core

//filter parameters for emg sensors
#define SAMPLE_RATE 500
#define INPUT_PIN 14
#define BUFFER_SIZE 128
int circular_buffer[BUFFER_SIZE];
int data_index, sum;
bool check = true;

int pos = 0; // variable to store the servo position

//Assign pins for servos
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int Middlepin = 25;
int Ringpin = 26;
int Pinkeypin = 27;
int Thumbpin = 32;
int Indexpin = 33;


int signall;
int envelop;

//EMG Filter 
float EMGFilter(float input)
{
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - 0.05159732 * z1 - 0.36347401 * z2;
    output = 0.01856301 * x + 0.03712602 * z1 + 0.01856301 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -0.53945795 * z1 - 0.39764934 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - 0.47319594 * z1 - 0.70744137 * z2;
    output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.00211112 * z1 - 0.74520226 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  return output;
}

//Envelop for EMG
int getEnvelop(int abs_emg)
{
  sum -= circular_buffer[data_index];
  sum += abs_emg;
  circular_buffer[data_index] = abs_emg;
  data_index = (data_index + 1) % BUFFER_SIZE;
  return (sum / BUFFER_SIZE) * 2;
}

//function for grabbing 
void grab()
{
  indexservo.write(150); // tell servo to go to position in variable 'pos'
  delay(20);
  ringservo.write(0); // tell servo to go to position in variable 'pos'
  delay(20);
  middleservo.write(180); // tell servo to go to position in variable 'pos'
  pinkyservo.write(20);   // tell servo to go to position in variable 'pos'
  delay(20);
  thumbservo.write(0); // tell servo to go to position in variable 'pos'
  delay(20);
  Serial.println("---------------------grabbing finished------------------------------------");
}

//function for releasing
void releasee()
{
  indexservo.write(30); // tell servo to go to position in variable 'pos'
  delay(20);
  ringservo.write(180); // tell servo to go to position in variable 'pos'
  delay(20);
  middleservo.write(0);  // tell servo to go to position in variable 'pos'
  pinkyservo.write(170); // tell servo to go to position in variable 'pos'
  delay(20);
  thumbservo.write(170); // tell servo to go to position in variable 'pos'
  delay(20);
  Serial.println("---------------------releasing finished------------------------------------");
}

//Primary core code
void core1Task(void *pvParameters)
{
  while (1)
  {

    static unsigned long past = 0;
    unsigned long present = micros();
    unsigned long interval = present - past;
    past = present;
    // Run timer
    static long timer = 0;
    timer -= interval;

    // Sample and get envelop-*
    if (timer < 0)
    {
      timer += 1000000 / SAMPLE_RATE;
      int sensor_value = analogRead(INPUT_PIN);
      signall = EMGFilter(sensor_value);
      envelop = getEnvelop(abs(signall));
    }
    // Read the temperature from the MLX90614 sensor
    float temperature = mlx.readObjectTempC();

    // Read the accelerometer and gyroscope values from the MPU6050 sensor
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Print the temperature and accelerometer/gyroscope values to the serial monitor
    // Serial.print("Temperature: ");
    // Serial.print(temperature);
    // Serial.print(" C\t");
    // Serial.print("Accel: ");
    // Serial.print(a.acceleration.x);
    // Serial.print(", ");
    // Serial.print(a.acceleration.y);
    // Serial.print(", ");
    // Serial.print(a.acceleration.z);
    // Serial.print("\tGyro: ");
    // Serial.print(g.gyro.x);
    // Serial.print(", ");
    // Serial.print(g.gyro.y);
    // Serial.print(", ");
    // Serial.println(g.gyro.z);

    // Display the temperature on the OLED display
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print((int)temperature);
    display.print((char)247); // degree symbol
    display.print(" C");
    display.display();
    if (temperature > 50)
    {
      analogWrite(13, 255);
    }
    else
    {
      analogWrite(13, 0);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  ringservo.setPeriodHertz(50);             // standard 50 hz servo
  middleservo.setPeriodHertz(50);           // standard 50 hz servo
  pinkyservo.setPeriodHertz(50);            // standard 50 hz servo
  thumbservo.setPeriodHertz(50);            // standard 50 hz servo
  indexservo.setPeriodHertz(50);            // standard 50 hz servo
  ringservo.attach(Ringpin, 500, 2400);     // attaches the servo on pin 18 to the servo object
  middleservo.attach(Middlepin, 500, 2400); // attaches the servo on pin 18 to the servo object
  pinkyservo.attach(Pinkeypin, 500, 2400);  // attaches the servo on pin 18 to the servo object
  thumbservo.attach(Thumbpin, 500, 2400);   // attaches the servo on pin 18 to the servo object
  indexservo.attach(Indexpin, 500, 2400);   // attaches the servo on pin 18 to the servo object

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(SSD1306_WHITE);

  // Initialize the MLX90614 temperature sensor
  mlx.begin();

  // Initialize the MPU6050 accelerometer/gyroscope
  // Try to initialize!
  if (!mpu.begin())
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1)
    {
      delay(10);
    }
  }

  // Create a task to run on the primary core
  xTaskCreatePinnedToCore(
      core1Task,        // Function to run on the primary core
      "Core 1 Task",    // Name of the task
      10000,            // Stack size (in bytes)
      NULL,             // Task parameters
      1,                // Priority
      &core1TaskHandle, // Task handle
      1                 // Core to run the task on (0 = primary core)
  );
}


void loop()
{
  if (check)
  {
    delay(5000);
    check = false;
  }
  if (envelop > 70)
  {
    Serial.print("--------------------------------grab-------------------------");
    if (!grabbed)
    {
      grab();
      grabbed = true;
      released = false;
    }
  }

  else
  {
    Serial.print("--------------------------------release-------------------------");

    if (!released)
    {
      releasee();
      released = true;
      grabbed = false;
    }
  }
  Serial.print(signall);
  Serial.print(",");
  Serial.println(envelop);
}