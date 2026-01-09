#define LED 13
#define BAUDRATE 57600
#define DEBUGOUTPUT 0

#define enA 5 //Enable1 Pin enA 
#define in1 6 //Motor1  Pin in1 
#define in2 7 //Motor1  Pin in2 

#define in3 8 //Motor2  Pin in3 
#define in4 9 //Motor2  Pin in4 
#define enB 10//Enable2 Pin enB 

#define powercontrol 10

#define yAxis_pin A0 // Joysticks Y-axis
#define xAxis_pin A1 // Joysticks X-axis

#define led1 A2
#define led2 A3
#define led3 A4
#define led4 A5

#define button_pin 2 
#define mode_led 3 

int Mode=0;

// checksum variables
byte generatedChecksum = 0;
byte checksum = 0; 
int payloadLength = 0;
byte payloadData[64] = {0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;

// system variables
long lastReceivedPacket = 0;
boolean bigPacket = false;

int Speed = 255;  
int timer=0, flag=0, Set=0, Run=3;

//////////////////////////
// Microprocessor Setup //
//////////////////////////

void setup() {// put your setup code here, to run once
pinMode(enA, OUTPUT); // declare as output for L298 Pin enA 
pinMode(in1, OUTPUT); // declare as output for L298 Pin in1 
pinMode(in2, OUTPUT); // declare as output for L298 Pin in2 
pinMode(in3, OUTPUT); // declare as output for L298 Pin in3   
pinMode(in4, OUTPUT); // declare as output for L298 Pin in4 
pinMode(enB, OUTPUT); // declare as output for L298 Pin enB 

pinMode(LED, OUTPUT); 

pinMode(mode_led, OUTPUT);

pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);
pinMode(led3, OUTPUT);
pinMode(led4, OUTPUT);

pinMode(xAxis_pin, INPUT);
pinMode(yAxis_pin, INPUT);
pinMode(button_pin, INPUT_PULLUP);
 
Serial.begin(BAUDRATE);           // USB
}

////////////////////////////////
// Read data from Serial UART //
////////////////////////////////
byte ReadOneByte(){
  int ByteRead;
  while(!Serial.available());
  ByteRead = Serial.read();

#if DEBUGOUTPUT  
  Serial.print((char)ByteRead);   // echo the same byte out the USB serial (for debug purposes)
#endif

  return ByteRead;
}

/////////////
//MAIN LOOP//
/////////////
void loop() {

if(digitalRead(button_pin)==0){
Stop();
Mode = !Mode;  
delay(1000);
}

digitalWrite(mode_led, Mode);

if(Mode==1){leds();
int xAxis = analogRead(xAxis_pin); // Read Joysticks X-axis
int yAxis = analogRead(yAxis_pin); // Read Joysticks Y-axis

// If joystick stays in middle the motors are not moving
if(xAxis > 470 && xAxis <550 && yAxis > 470 && yAxis <550){Stop(); Speed=150;}

if (xAxis > 470 && xAxis <550){    
// Y-axis used for left and right control
if (yAxis < 470)turnRight();
if(yAxis > 550)turnLeft();
if(Speed<255)Speed++;

}else{
if (yAxis > 470 && yAxis <550){    
if (xAxis < 470)forword();
if (xAxis > 550)backword();
if(Speed<255)Speed++;
}else{
if(xAxis < 470)forword();
if(xAxis > 550)backword();
if(Speed<255)Speed++;  
  } 
 }
}
else{  Speed=180;
  // Look for sync bytes
  if(ReadOneByte() == 170) {
    if(ReadOneByte() == 170) {
        payloadLength = ReadOneByte();
        
      
        if(payloadLength > 169) //Payload length can not be greater than 169
        return;
        generatedChecksum = 0;        
        for(int i = 0; i < payloadLength; i++) 
        {  
        payloadData[i] = ReadOneByte();            //Read payload into memory
        generatedChecksum += payloadData[i];
        }   

        checksum = ReadOneByte();                      //Read checksum byte from stream      
        generatedChecksum = 255 - generatedChecksum;   //Take one's compliment of generated checksum

        if(checksum == generatedChecksum) {    
          poorQuality = 200;
          attention = 0;
          meditation = 0;

          for(int i = 0; i < payloadLength; i++) {// Parse the payload
          switch (payloadData[i]) {
          case 2:
            i++;            
            poorQuality = payloadData[i];
            bigPacket = true;            
            break;
          case 4:
            i++;
            attention = payloadData[i];                        
            break;
          case 5:
            i++;
            meditation = payloadData[i];
            break;
          case 0x80:
            i = i + 3;
            break;
          case 0x83:
            i = i + 25;      
            break;
          default:
            break;
          } // switch
        } // for loop

#if !DEBUGOUTPUT

        // *** Add your code here ***

        if(bigPacket){
          if(poorQuality == 0)
             digitalWrite(LED, HIGH);
          else
            digitalWrite(LED, LOW);
          
 Serial.print("PoorQuality: ");
 Serial.print(poorQuality, DEC);
 Serial.print(" Attention: ");
 Serial.print(attention, DEC);
 Serial.print(" Time since last packet: ");
 Serial.print(millis() - lastReceivedPacket, DEC);
 lastReceivedPacket = millis();
 Serial.print("\n");

if(poorQuality<10){Stop();  
Serial.println(" ");
if(timer>0){timer=timer-1;}  
}else{timer=Run;
if(Set==0){Serial.print("F"); forword();}
if(Set==1){Serial.print("B"); backword();}
if(Set==2){Serial.print("L"); turnLeft();}
if(Set==3){Serial.print("R"); turnRight(); }
} 

if(timer==0){
timer=Run;  
Set = Set+1;
if(Set>3){Set=0;}  
leds();
}
          
             
        }
#endif        
        bigPacket = false;        
      }
      else {
        // Checksum Error
      }  // end if else for checksum
    } // end if read 0xAA byte
  } // end if read 0xAA byte
}

analogWrite(enA, Speed); // Write The Duty Cycle 0 to 255 Enable Pin A for Motor1 Speed 
analogWrite(enB, Speed); // Write The Duty Cycle 0 to 255 Enable Pin B for Motor2 Speed
}



void forword(){  //forword
digitalWrite(in1, HIGH); //Right Motor forword Pin 
digitalWrite(in2, LOW);  //Right Motor backword Pin 
digitalWrite(in3, LOW);  //Left Motor backword Pin 
digitalWrite(in4, HIGH); //Left Motor forword Pin 
}

void backword(){ //backword
digitalWrite(in1, LOW);  //Right Motor forword Pin 
digitalWrite(in2, HIGH); //Right Motor backword Pin 
digitalWrite(in3, HIGH); //Left Motor backword Pin 
digitalWrite(in4, LOW);  //Left Motor forword Pin 
}

void turnRight(){ //turnRight
digitalWrite(in1, LOW);  //Right Motor forword Pin 
digitalWrite(in2, HIGH); //Right Motor backword Pin  
digitalWrite(in3, LOW);  //Left Motor backword Pin 
digitalWrite(in4, HIGH); //Left Motor forword Pin 
}

void turnLeft(){ //turnLeft
digitalWrite(in1, HIGH); //Right Motor forword Pin 
digitalWrite(in2, LOW);  //Right Motor backword Pin 
digitalWrite(in3, HIGH); //Left Motor backword Pin 
digitalWrite(in4, LOW);  //Left Motor forword Pin 
}

void Stop(){ //stop
digitalWrite(in1, LOW); //Right Motor forword Pin 
digitalWrite(in2, LOW); //Right Motor backword Pin 
digitalWrite(in3, LOW); //Left Motor backword Pin 
digitalWrite(in4, LOW); //Left Motor forword Pin 
}

void leds(){
digitalWrite(led1, LOW);
digitalWrite(led2, LOW);
digitalWrite(led3, LOW);
digitalWrite(led4, LOW);

if(Set==0){digitalWrite(led1, HIGH);}
if(Set==1){digitalWrite(led2, HIGH);}
if(Set==2){digitalWrite(led3, HIGH);}
if(Set==3){digitalWrite(led4, HIGH);}
}
