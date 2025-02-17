#include <DYPlayerArduino.h>
#include <Arduino.h>

DY::Player player;

int analogPin = A0; 
int sensorPin = 3;
int Mp3Busy = 4; //de Mp3 module geeft via deze pin aan dat er op dit moment muziek speelt
int InterruptAllowed = 0; //de tekst "dankuwel" mag niet worden, de random melodietjes wel
int randomint = 11; // random getal als integer.
char str[6];
String SongToPlay;

#define INTERVAL_MESSAGE1 60000
unsigned long time_1 = 0;

int A0val = 0; //Store analog valuie of pin A0
volatile int sensorState; // store pin interrupt value

void setup() {
  player.begin();
  Serial.begin(9600);
  player.setVolume(8); 
  randomSeed(analogRead(A1));
  InterruptAllowed = 1;
  attachInterrupt(digitalPinToInterrupt(sensorPin), sensorInterrupt, RISING);
  Serial.println("End_Void_Setup");
}

void loop() {  
  // alles wat deze loop doet is afentoe een random mp3tje afspelen.
  if(millis() >= time_1 + INTERVAL_MESSAGE1){
    time_1 +=INTERVAL_MESSAGE1;
    if (digitalRead(Mp3Busy)){ //Wanneer er op dit moment geen muziek speelt dan..
      InterruptAllowed = 1;  //wanneer iemand papier in de mond doet dan mag deze mp3 worden afgebroken
      Serial.println("Play random song");
      //player.playSpecifiedDevicePath(DY::Device::Sd,"/00008.mp3");
    }
  }  
}

void sensorInterrupt () {
  sensorState = digitalRead(sensorPin);
  Serial.println("Interrupt triggered");
  if ((digitalRead(Mp3Busy)) || InterruptAllowed){
    InterruptAllowed = 0;
    Serial.println("PlayRandom MP3");
    SongToPlay = randommp3();
    Serial.println("SongToPlay = "+SongToPlay);
    //player.playSpecifiedDevicePath(DY::Device::Sd,"/HBG1.mp3"); // Werkt OK
      player.playSpecifiedDevicePath(DY::Device::Sd,'SongToPlay');
  }
}

String randommp3 () {
  //maakt een random getal aan en zal dit random getal omzetten naar /000xx.mp3 waarbij xx het random getal is
  randomint = random(1,41);
  snprintf(str, 6, "%05d", randomint);
  return (String)"/"+str+".mp3"; //+"\0";
}
