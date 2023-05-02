#include <Adafruit_SSD1306.h>
#define OLED_ADDR   0x3C
#define OLED_WIDTH  128
#define OLED_HEIGHT 32

#define trigPin1 3
#define echoPin1 2
#define trigPin2 6
#define echoPin2 7
#define sound A0

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
Servo myservo1; 
// twelve servo objects can be created on most boards

bool gateOpen = true; // to keep track of gate state
int pos = 0;   // variable to store the servo position

unsigned long previousMillis = 0; // variable to store the last time the data was pushed
const long interval = 1000; // interval in milliseconds to push data

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo1.attach(10);

  Serial.begin (9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(13, OUTPUT);
    pinMode(11, OUTPUT);
  pinMode(5, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Clear the buffer
  display.clearDisplay();
  display.display();
}
void loop() {
  long duration, distance;
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration = pulseIn(echoPin1, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.print("Distance from sensor 1: ");
  Serial.print(distance);
  Serial.println(" cm");

  long duration2, distance2;
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2 / 2) / 29.1;
  Serial.print("Distance from sensor 2: ");
  Serial.print(distance2);
  Serial.println(" cm");

  display.clearDisplay(); // Clear the display buffer
  display.setCursor(0,0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  
  if (distance <= 7 || distance2 <= 7) {
     // Display "STOP" if the distance is less than or equal to 7
    if (!gateOpen) {
      gateOpen = true;
      myservo1.write(100);
      myservo.write(100);
      digitalWrite(13, 1);
      
    digitalWrite(11, HIGH); // turn on red LED
    digitalWrite(5, LOW); // turn off green LED
      Serial.println("Gate opening");
      delay(3500);
     
      
    } else {
      gateOpen = false;
      myservo1.write(0);
      myservo.write(0);
      digitalWrite(13, 1);
      digitalWrite(5, HIGH);
     digitalWrite(11, LOW);
      Serial.println("Gate closing");
      delay(3500);
     
      
    }
  }
 
  else {
    display.println("Distance: " + String(distance) + "cm"); // Display the distance if it's greater than 7
     
  }
  display.display();
}
