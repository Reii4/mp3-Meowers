#include <Servo.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
//define used libraries and pin numbers
const int servoSwitch = 9;
const int servoPin = 3 ;
const int playSwitch = 6;
const int skipSwitch = 5;
const int RX = 10;
const int TX = 11;

// define servo object and buttonStates
Servo myServo; 
int servoSwitchState = 0;
int playSwitchState = 0;
int skipSwitchState =0;

//define DFPlayer object and transmit/recieve relationship
DFRobotDFPlayerMini myDFPlayer;
SoftwareSerial softSerial(RX,TX);

//define counting indices for tracking mp3s
int songNum = 3;
int index = 1;
int pauseState = myDFPlayer.readState();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  // set switch pints to input 
  pinMode(servoSwitch, INPUT);
  pinMode(playSwitch, INPUT);
  pinMode(skipSwitch, INPUT);
  myServo.attach(servoPin);

  //perform internal error check and report.
  if (!myDFPlayer.begin(softSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"))
  }
  //default opening set up
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(10);  //Set volume value. From 0 to 30
  
}

void loop() {
  // put your main code here, to run repeatedly:

  //constantly read button states
  servoSwitchState = digitalRead(servoSwitch);
  playSwitchState = digitalRead(playSwitch);
  skipSwitchState = digitalRead(skipSwitch);

  // servoMovement on button press
  if (servoSwitchState == HIGH) 
  {
    
    for(int i = 90; i < 135; i++){myServo.write(i); delay(100);}
    for(int i = 135; i > 45; i--){myServo.write(i); delay(100);}
    servoSwitchState = LOW;
    myServo.write(90);
  }

  //play or pause on button press
  if(playSwitchState == HIGH)
  {
    if(myDFPlayer.readState() != pauseState)
    {
      myDFPlayer.pause();
    }
    else{
        myDFPlayer.play(index);  //Play the first mp3
    }
  }

  //skip to next or loop back to first song on buttonpress
  if(skipSwitchState == HIGH)
  {
    myDFPlayer.next();
    index++;
    if(index > 3) { index = 1;}
  }
}


// built in premade cases to read errors
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}