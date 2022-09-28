#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);// Configure software serial port
//String dataMessage;
String TextForSms;
char incomingChar;// Create variable to store incoming SMS characters

int trigPin = 9;  // Arduino pin tied to trigger pin
int echoPin =10; // Arduino pin tied to echo pin

int redled = 3; // Red pin of the RGB LED
int greenled = 5; // Green pin of the RGB LED
int blueled = 6; // Blue pin of the RGB LED

//For Percentage
float duration, distance; // variable where the distance measured with the sonar is stored
float const TankHeight = 10.07; // Actual height of the polytank(outerpart)
float const TankRaduis = 5.0;
float MaxWaterLevel, GapbwSonar, ActualVolume, ActualDistance, PercentageL, PercentageU,sonarReadings, AverageReadings;
//For Average
float height=0, average=0, litres=0, volume=0, sum=0;

float arr1[6];
float arr2[6];
int i;
int j;

char number[]="+233501311364"; //Destination number

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //starts serial communication, so that the Arduino can send out command through the USB connection. 
  SIM900.begin(9600);
  pinMode(trigPin, OUTPUT); //Sets the trigPin as output
  pinMode(echoPin, INPUT); //sets the echopin as input
  pinMode(redled, OUTPUT); //sets as output
  pinMode(greenled, OUTPUT); // sets as output
  pinMode(blueled, OUTPUT); //sets as output
  digitalWrite(redled, LOW);  //sets the pin Low(0v or GND)
  digitalWrite(greenled, LOW); 
  digitalWrite(blueled, LOW);  
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(1000);
  // Set module to send SMS data to serial out upon receipt 
  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(1000);
  SIM900.println("AT+CMGD=1,4"); // delete all SMS
  delay(5000);// Give time to your GSM shield log on to network 
  Serial.print("Ready..."); 

  MaxWaterLevel = 0.745*TankHeight;// Maximum water the thank can hold
  GapbwSonar = TankHeight-MaxWaterLevel; //Gab between max Water level and the sonar
}

void sendSMS(String message){
  Serial.println("Sending SMS.....");     
  SIM900.println("AT+CMGF=1\r"); // AT command to send SMS message
  delay(1000);
  SIM900.println("AT+CMGS = \"+233501311364\""); // recipient's mobile number, in international format
  delay(1000);
  SIM900.println(message);// message to send
  delay(1000);
  SIM900.println((char)26);// End AT command with a ^Z, ASCII code 26
  delay(1000); 
  SIM900.println();
  delay(1000); // give module time to send SMS
   // turn off module
  Serial.println("SMS sent");   
}

void loop() {
  // put your main code here, to run repeatedly:
  height = dist();
  Serial.println(height); 
  ActualDistance = height-GapbwSonar;//the level of water lost or used
  ActualVolume= MaxWaterLevel-ActualDistance;// the level of water left in the tank   
  //level of water USED in percentage  
  PercentageU = (ActualDistance/MaxWaterLevel*100);
  Serial.println(PercentageU);  
  //level of water LEFT in percentage  
  PercentageL = (ActualVolume/MaxWaterLevel*100);
  Serial.println(PercentageL);  
  AverageReadings = Avg();
  Serial.println(AverageReadings);  
  delay(1000); 

  if (SMSRequest()){  
      TextForSms = ("Percentage Used: " + String(PercentageU) +"% " + "Percentage Left: " + String(PercentageL) +"% " + "Average use per week: "+ String(AverageReadings) +"L ");
      delay(2000);      
      Serial.println(TextForSms);
      sendSMS(TextForSms);// Send the SMS text message
      delay(5000);
      TextForSms =" ";
      SIM900.println();
      // Give module time to send SMS
      delay(5000);       
  }
  delay(10);
   //Level indication led
  if (PercentageL >= 100){
    TextForSms = "The water Tank is full.";
    Serial.println(TextForSms);    
    sendSMS(TextForSms);
    delay(5000);
    TextForSms = " ";    
    digitalWrite(greenled, HIGH); 
    delay(10000); // The led switches on for 10 seconds
    digitalWrite(greenled, LOW);
    //delay(1000); // The led switches on for 5 seconds    
   
  }else if (PercentageL >20 && PercentageL <100){
    TextForSms = "There is water in the Tank.";
    Serial.println(TextForSms);    
    sendSMS(TextForSms);
    delay(5000);     
    TextForSms = " ";         
    digitalWrite(blueled, HIGH); 
    delay(10000); // The led switches on for 60 seconds
    digitalWrite(blueled, LOW);  
   
  }else if(PercentageL>= 5 && PercentageL <=20){
    TextForSms = "The water level is critically low.";
    Serial.println(TextForSms);    
    sendSMS(TextForSms); 
    delay(5000);  
    TextForSms = " ";         
    // The led switches on for 10 seconds 
    digitalWrite(redled, HIGH);
    delay(5000); 
    digitalWrite(redled, LOW);
    
  }else if(PercentageL < 5){
    TextForSms = "The water tank is Empty!!!";
    Serial.println(TextForSms);    
    sendSMS(TextForSms);
    delay(5000);   
    TextForSms = " ";       
    // The led switches on for 10 seconds 
    digitalWrite(redled, HIGH);
    delay(10000); 
    digitalWrite(redled, LOW);
  } 
}

//Function to store the distance value
float dist() {
  //Clears the trigPin
  digitalWrite(trigPin,LOW);
  delay(5000); // Wait 5s between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  digitalWrite(trigPin,HIGH);
  delay(5000);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin,HIGH);
  distance = (duration/2)/29.1;
  return(distance);
}


//Function for average
float Avg() {
  //Averae usage of water for a week in litres
  for( j=0; j<=6; j++){
    for( i=0; i<=6; i++){  
      sonarReadings= dist() -GapbwSonar;         
      arr1[i] = sonarReadings;
      delay(5000);//reapeats this action every 24h for 7 days (86400000)   Its 5 seconds in the code because of time   
    }
    arr2[j] = arr1[i] - arr1[i-1];//actual usuage(height) at the end of every 24h
  } 
  
  for(j=0; j<=6; j++){
	sum = sum +arr2[j];//adding all usage for the 7 days
  }
  litres=sum*0.001;
  volume=3.14*TankRaduis*TankRaduis*litres;
  //average usage for the 7 days 
	average = volume/7;
  sum=0;
  return(average);
}


boolean SMSRequest() {
  if(SIM900.available() >0) {
    incomingChar=SIM900.read();
    if(incomingChar=='S') {
      delay(10);
      Serial.print(incomingChar);
      incomingChar=SIM900.read();
      if(incomingChar =='T') {
        delay(10);
        Serial.print(incomingChar);
        incomingChar=SIM900.read();
        if(incomingChar=='A') {
          delay(10);
          Serial.print(incomingChar);
          incomingChar=SIM900.read();
          if(incomingChar=='T') {
            delay(10);
            Serial.print(incomingChar);
            incomingChar=SIM900.read();
            if(incomingChar=='E') {
              delay(10);
              Serial.print(incomingChar);
              Serial.print("...Request Received \n");
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}
