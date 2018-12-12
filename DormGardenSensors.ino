#include <max6675.h>
//By: Camden Kronhaus 12/1/18


//water pins
const int waterInPin = 1; //analog input of water sensor
const int waterLED = 8;//water sensor LED pin
int waterState = LOW; //state of water sensor LED
int waterVal = 0; //output of water sensor

//Light Pins
const int RELAY1 = 11;
const int RELAY2 = 12;
int RELAYstate = HIGH;

//temp pins
const int tempSensor = 0;
const int tempLED = 7;//temp LED pin 
int tempState = LOW;//temp LED state

//PH Pins
const int phPin = 2; //analoginpin
int phSensorVal = 0; //sensorvalue
const int phLED = 5; // ph led indicator is out of range

unsigned long int avgPhValue; 
float b;
int buf[10],temp;


long long int counter = 0;//will keep track of when to turn on lights


void setup() {
  pinMode(tempLED, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(waterInPin, INPUT);
  pinMode(phPin, INPUT);
  pinMode(phLED, OUTPUT);
  pinMode(waterLED, OUTPUT);
  Serial.begin(9600);


}

void loop() {
  counter++;
  //TEMP SENSOR
  float degreesF;
  float voltage = 0;
  float degreesC;
  voltage = getVoltage(tempSensor); //calls function to return the volate of the temperature sensor reading
  degreesC =  (voltage) * 100.0; //voltage * 100.0 is equal to degrees in Celcius
  degreesF = degreesC * (9.0/5.0) + 32.0; //conversion to Farenheit
  Serial.print("Temp = ");
  Serial.println(degreesF);

  delay(100);

  if (degreesF >= 85 || degreesF <= 59){ // sets temperature range so that LED light state will be set to low or high accordingly
  tempState = HIGH;
  }else{
  tempState = LOW;
  }
 digitalWrite(tempLED, tempState);

  delay(1000);


//WATER SENSOR
  waterVal = analogRead(waterInPin); 
  Serial.print("Water Sensor = " ); 
  Serial.println(waterVal);

  delay(100);
  
  if (waterVal >= 70){ //A rough estimate of water sensor reading threshold for when water gets to low, sets LED state accordingly
    waterState = LOW;
  }else {
    waterState = HIGH;
  }

  digitalWrite(waterLED, waterState);

 delay(1000);

//PH SENSOR
for(int i=0;i<10;i++) //taken from PH store website, reads mutliple times, finds an average value for readings, and converts it to a pH reading
 { 
  buf[i]=analogRead(phPin);
  delay(10);
 }
 for(int i=0;i<9;i++)
 {
  for(int j=i+1;j<10;j++)
  {
   if(buf[i]>buf[j])
   {
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }
 avgPhValue=0;
 for(int i=2;i<8;i++)
    avgPhValue+=buf[i];
 float pHVol=(float)avgPhValue*5.0/1024/6;
 float phValue = -5.70 * pHVol + 21.34;
 Serial.print("pH Value = ");
 Serial.println(phValue);
 
 delay(20);


 if (phValue > 8 || phValue < 6.5){ //If pH falls out of designated range, set LED state accordingly
  digitalWrite(phLED, HIGH);
 }else if (phValue < 8 && phValue > 6.5){
   digitalWrite(phLED, LOW);
 }
 delay(10);
    


//LIGHT SWITCH
if (counter == 18461539) 
//counter keeps track of number of times looped through system, math used to ensure that condiiton will be
//met once after 12 hours of the system being on              
{
      RELAYstate = 0;
      digitalWrite(RELAY1, RELAYstate);           // Turns ON Relays
      digitalWrite(RELAY2, RELAYstate);
      Serial.println("Light ON");
 }
if (counter == 36923078)
//condition met after 24 hours of arduino being on, thne reset counter to 0 so that after 12 hours, condition for
//relay turning on will be met
 {
      RELAYstate = 1;
      digitalWrite(RELAY1, RELAYstate);
      digitalWrite(RELAY2, RELAYstate);
      Serial.println("Light OFF");      // Turns Relays Off
      counter = 0;

 }
    delay(20);
}


float getVoltage(int pin){
  return (analogRead(pin)) * .004882814;
}
