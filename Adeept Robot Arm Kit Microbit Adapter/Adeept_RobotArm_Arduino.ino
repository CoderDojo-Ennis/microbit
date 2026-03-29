#include <Servo.h>
#include <NeoSWSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET     4
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

// Servo pins
const int servopin1 = 9;
const int servopin2 = 6;
const int servopin3 = 5;
const int servopin4 = 3;
const int servopin5 = 11;

// Servo objects
Servo servo1;
bool servo1Attached = false;

Servo servo2;
bool servo2Attached = false;

Servo servo3;
bool servo3Attached = false;

Servo servo4;
bool servo4Attached = false;

Servo servo5;
bool servo5Attached = false;

// neoSerial( RX pin, TX pin )
NeoSWSerial swSerial(8, 2);

void setup() {
  // Serial setup
  swSerial.begin(9600);
  Serial.begin(9600);
  
  // Display setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);//Sets the font display color
  display.clearDisplay();

  delay(50);
  Serial.println("Ready for commands like: S2=45;");

  //Set the font size
  display.setTextSize(2);
  //Set the display location
  display.setCursor(30,20);
  //String displayed
  display.print("Ready");
  //Began to show
  display.display();

  // Attach servos once
  // servo1.attach(servopin1);
  // servo2.attach(servopin2);
  // servo3.attach(servopin3);
  // servo4.attach(servopin4);
  // servo5.attach(servopin5);

  // Initialize all servos to 90°
  // servo1.write(90);
  // servo2.write(90);
  // servo3.write(90);
  // servo4.write(90);
  // servo5.write(90);
}

const int BUF_SIZE = 20;
char inputBuffer[BUF_SIZE];
int bufferIndex = 0;

void loop() {
  while (swSerial.available() > 0) {
    char c = swSerial.read();
    // 1. Check for termination character ';'
    if (c == ';') {
      inputBuffer[bufferIndex] = '\0'; // Null-terminate the string
      parseCommand(inputBuffer);      // Process the buffer
      bufferIndex = 0;                // Reset for next command

    } 
    // 2. Fill the buffer (prevent overflow)
    else if (c != '\r' && c != '\n' && c != ' ') {
      if (bufferIndex < BUF_SIZE - 1) {
        inputBuffer[bufferIndex++] = c;
      }
    }
  }
}

void parseCommand(char* buf) {
  int index, angle;
  // 3. Extract numbers using pattern "S[number]=[number]"
  // sscanf returns the number of successfully filled variables
  if (sscanf(buf, "S%d=%d", &index, &angle) == 2) {
    
    Serial.print("ID: "); Serial.println(index);
    Serial.print("Angle: "); Serial.println(angle);

    moveServo(index, angle);
  } else {

    display.clearDisplay();
    display.setTextSize(2);
    // Line1
    display.setCursor(30,5);
    display.print("Error");
    // Line2
    display.setTextSize(1);
    display.setCursor(0,25);
    display.print("Parse failed on!");
    display.display();
  }
}

void moveServo(int index, int angle) {
  angle = constrain(angle, 0, 180);
  bool operationFailed = false;

  switch (index) {
    case 1:
      servo1.write(angle);
      if(!servo1Attached) {
        servo1Attached = true;
        servo1.attach(servopin1);
      }
      break;
    case 2:
      servo2.write(angle);
      if(!servo2Attached) {
        servo2Attached = true;
        servo2.attach(servopin2);
      }
      break;
    case 3: 
      servo3.write(angle);
      if(!servo3Attached) {
        servo3Attached = true;
        servo3.attach(servopin3);
      }
      break;
    case 4:
      servo4.write(angle); 
      if(!servo4Attached) {
        servo4Attached = true;
        servo4.attach(servopin4);
      }
      break;
    case 5:
      servo5.write(angle); 
      if(!servo5Attached) {
        servo5Attached = true;
        servo5.attach(servopin5);
      }
      break;
    default:
      operationFailed = true;
  }

  if(operationFailed){
    Serial.println("Invalid servo index!");

    display.clearDisplay();
    display.setTextSize(2);
    // Line1
    display.setCursor(30,5);
    display.print("Error");
    // Line2
    display.setTextSize(1);
    display.setCursor(0,25);
    display.print("Invalid servo index!");
    display.display();
  } else {
    Serial.print("OK ! Servo: ");
    Serial.print(index);
    Serial.print(", Angle: ");
    Serial.println(angle);
    display.clearDisplay();
    display.setTextSize(3);
    // Line1
    display.setCursor(50,5);
    display.print("OK");
    // Line2
    display.setTextSize(1);
    display.setCursor(5,35);
    display.print("Servo: ");
    display.print(index);
    display.print(" Angle: ");
    display.print(angle);
    display.display();
  }
}

