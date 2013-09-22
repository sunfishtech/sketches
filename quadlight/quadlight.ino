// This example demonstrates CmdMessenger's callback  & attach methods
// For Arduino Uno and Arduino Duemilanove board (may work with other)

// Download these into your Sketches/libraries/ folder...

// CmdMessenger library available from https://github.com/dreamcat4/cmdmessenger
#include <Stripper.h>

#include <CmdMessenger.h>

// Base64 library available from https://github.com/adamvr/arduino-base64
//#include <Base64.h>

// Streaming4 library available from http://arduiniana.org/libraries/streaming/
//#include <Streaming.h>

#include <Adafruit_NeoPixel.h>

#define LEDPIN 6
#define SW1PIN 2
#define NUMLEDS 40
#define LSENSOR 0


// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstre
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, LEDPIN, NEO_GRB + NEO_KHZ800);
Stripper stripper = Stripper(&strip);


// Mustnt conflict / collide with our message payload data. Fine if we use base64 library ^^ above
char field_separator = '|';
char command_separator = '\n';
//char command_separator =';';


// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial, field_separator, command_separator);




// ------------------ C M D  L I S T I N G ( T X / R X ) ---------------------

// We can define up to a default of 50 cmds total, including both directions (send + recieve)
// and including also the first 4 default command codes for the generic error handling.
// If you run out of message slots, then just increase the value of MAXCALLBACKS in CmdMessenger.h

// Commands we send from the Arduino to be received on the PC
enum
{
  kCOMM_ERROR    = 000, // Lets Arduino report serial port comm error back to the PC (only works for some comm errors)
  kACK           = 001, // Arduino acknowledges cmd was received
  kARDUINO_READY = 002, // After opening the comm port, send this cmd 02 from PC to check arduino is ready
  kERR           = 003, // Arduino reports badly formatted cmd, or cmd not recognised

  kSW1           = 004,
  kLR1           = 005,
  // Now we can define many more 'send' commands, coming from the arduino -> the PC, eg
  // kICE_CREAM_READY,
  // kICE_CREAM_PRICE,
  // For the above commands, we just call cmdMessenger.sendCmd() anywhere we want in our Arduino program.

  kSEND_CMDS_END, // Mustnt delete this line
};


// Its also possible (above ^^) to implement some symetric commands, when both the Arduino and
// PC / host are using each other's same command numbers. However we recommend only to do this if you
// really have the exact same messages going in both directions. Then specify the integers (with '=')


// ---- U T I L  M E T H O D S ------
//reads a string in the form r,g,b and splits it into an array of integers
void readColor(byte *rgb){
  if(cmdMessenger.available()){
    char rgbStr[350] = {'\0'};
    cmdMessenger.copyString(rgbStr,350);
    if(rgbStr[0]){
      char *p = rgbStr;
      char *str;
      int idx = 0;
      while ((str = strtok_r(p, ",", &p)) != NULL) { 
        rgb[idx] = atoi(str);
        idx+=1;
      }
    }
  }
}

// ------------------ C A L L B A C K  M E T H O D S -------------------------

void rainbow()
{
   uint16_t wait = 20;
   if (cmdMessenger.available())
     wait = cmdMessenger.readInt();
   stripper.rainbow(wait);
}

void rainbowCycle()
{
  uint16_t wait = 20;
  if (cmdMessenger.available())
    wait = cmdMessenger.readInt();
   stripper.rainbowCycle(wait);
}

void colorCycle(){
  uint16_t wait = 5;
  if (cmdMessenger.available())
    wait = cmdMessenger.readInt();
   stripper.colorCycle(wait);
}

void scan(){
  uint16_t wait = 50;
  byte rgb[3] = {100,100,100};
  uint8_t fadeAmt = 5;
  uint16_t fadeWait = 20;
  
  readColor(rgb);
  if (cmdMessenger.available())
    wait = cmdMessenger.readInt();
  if (cmdMessenger.available())
    fadeAmt = cmdMessenger.readInt();
  if (cmdMessenger.available())
    fadeWait = cmdMessenger.readInt();
   stripper.scan(rgb, wait, fadeAmt, fadeWait);
}

void rain(){
  uint16_t wait = 50;
  byte rgb[3] = {100,100,100};
  uint8_t fadeAmt = 5;
  uint16_t fadeWait = 20;
  readColor(rgb);
  if (cmdMessenger.available())
    wait = cmdMessenger.readInt();
  if (cmdMessenger.available())
    fadeAmt = cmdMessenger.readInt();
   if (cmdMessenger.available())
     fadeWait = cmdMessenger.readInt();
  stripper.rain(rgb,wait, fadeAmt, fadeWait);
}

void setColor()
{
  unsigned int ledNo = 0; //0 = all
  byte rgb[3] = {0};
  unsigned long wait = 0;
 
  readColor(rgb);
  if (cmdMessenger.available()) ledNo = cmdMessenger.readInt();
  if (cmdMessenger.available()) wait = cmdMessenger.readInt();
  
  //uint32_t color = strip.Color(rgb[0],rgb[1],rgb[2]);
  if (ledNo < 1) {
    if (wait < 1) { 
      stripper.setColor(rgb);
    } else {
      stripper.colorWipe(rgb, wait);
    }
  } else {
    stripper.setPixel(ledNo, rgb);
  }
}

void setBrightness() {
  uint8_t bright = 255;
  if (cmdMessenger.available()) bright = cmdMessenger.readInt();
  stripper.brightness(bright);
}

void dim() {
  uint8_t bright = 100;
  uint8_t wait = 50;
  if (cmdMessenger.available()) bright = cmdMessenger.readInt();
  if (cmdMessenger.available()) wait = cmdMessenger.readInt();
  stripper.dim(bright, wait);
}

void setOff(){
  stripper.off();
}
// ------------------ D E F A U L T  C A L L B A C K S -----------------------

void arduino_ready()
{
  // In response to ping. We just send a throw-away Acknowledgement to say "im alive"
  cmdMessenger.sendCmd(kACK,"Arduino ready");
}

void unknownCmd()
{
  // Default response for unknown commands and corrupt messages
  cmdMessenger.sendCmd(kERR,"Unknown command");
}

// ------------------ E N D  C A L L B A C K  M E T H O D S ------------------



// ------------------ S E N S O R S-------------------------------------------
int buttonState;
int lastButtonState = LOW;
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50; 

int readButton(){
  int reading = digitalRead(SW1PIN);
  int result = -1;
  if (reading != lastButtonState){
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay){
    if (reading != buttonState){
      result = buttonState = reading;
    }
  }
  lastButtonState = reading;
  return result;
}

// ------------------ S E T U P ----------------------------------------------
// Commands we send from the PC and want to recieve on the Arduino.
// We must define a callback function in our Arduino program for each entry in the list below vv.
// They start at the address kSEND_CMDS_END defined ^^ above as 004
messengerCallbackFunction messengerCallbacks[] = 
{
  setColor,            //= 20
  rainbow,             //= 21
  rainbowCycle,        //= 22
  setBrightness,       //= 23
  setOff,              //= 24
  colorCycle,          //= 25
  scan,                //=26
  dim,                 //=27
  rain,                //=28
  NULL
};

void attach_callbacks(messengerCallbackFunction* callbacks)
{
  int i = 0;
  int offset = 20;
  while(callbacks[i])
  {
    cmdMessenger.attach(offset+i, callbacks[i]);
    i++;
  }
}

void setup() 
{
  pinMode(SW1PIN, INPUT);
  // Listen on serial connection for messages from the pc
  // Serial.begin(57600);  // Arduino Duemilanove, FTDI Serial
  Serial.begin(115200); // Arduino Uno, Mega, with AT8u2 USB

  // cmdMessenger.discard_LF_CR(); // Useful if your terminal appends CR/LF, and you wish to remove them
  //.print_LF_CR();   // Make output more readable whilst debugging in Arduino Serial Monitor
  
  // Attach default / generic callback methods
  cmdMessenger.attach(kARDUINO_READY, arduino_ready);
  cmdMessenger.attach(unknownCmd);

  // Attach my application's user-defined callback methods
  attach_callbacks(messengerCallbacks);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  arduino_ready();

  // blink
  pinMode(13, OUTPUT);
  
  randomSeed(analogRead(0));
}


// ------------------ M A I N ( ) --------------------------------------------

// Timeout handling
long timeoutInterval = 250; 
long previousMillis = 0;
int counter = 0;


void readLightSensor()
{
  int light = analogRead(LSENSOR);
  char msgArg[5];
  itoa(light,msgArg,5);
  cmdMessenger.sendCmd(kLR1,msgArg);
}  

void loop() 
{
  int sw1 = readButton();
  if (sw1 >= 0) {
    char msgArg[5];
    itoa(sw1,msgArg,5);
    cmdMessenger.sendCmd(kSW1,msgArg);
  }
  // Process incoming serial data, if any
  cmdMessenger.feedinSerialData();
  
  stripper.tick();
  delay(1);

  // handle timeout function, if any
  if (  millis() - previousMillis > timeoutInterval )
  {
    //readLightSensor();
    previousMillis = millis();
  }

  // Loop.
}





