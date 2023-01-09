#include<Servo.h>
#include<LiquidCrystal.h>

LiquidCrystal lcd1(3, 4, 7, 8, 9, 10);

Servo servo_left;
Servo servo_right;
Servo servo_bulb;

int ldr_pin = A2;

int servo_pos=90;
int servo_pos2=90;
int servo_bulb_pos=90;

int panel_open = 0;
int value_ldr = 0;

int rain = 0;
int dummy_water_height = 0;

int SolarPin = A1;
int adcValue = 0;
int offsetVoltage = 2500; //half of the voltage
double adcVoltage = 0;
double currentValue = 0;
int sensitivity = 66;

int bulb_open = 0;

const int trigPin = 11;
const int echoPin = 12;

long duration;
int distance;

void setup() {
  servo_right.attach(5);
  servo_left.attach(6);
  servo_bulb.attach(13);
  
  pinMode(2, INPUT);

  lcd1.begin(32, 2);

  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
}

void loop() {
  bool servo_positioned = 0;
  adcValue = analogRead(SolarPin);
  adcVoltage = (adcValue / 1024.0) * 5000;
  currentValue = ( (adcVoltage - offsetVoltage) / sensitivity );
  int loadVoltage = currentValue * 12;
  int power = loadVoltage * currentValue * currentValue;
  
  lcd1.setCursor(0, 1);
  lcd1.print(power);
  lcd1.print("W|");
  
  
  int rain = digitalRead(2);
  value_ldr = analogRead(ldr_pin);

  //no rain and sunny weather
  if(rain == 0 && panel_open == 0 && value_ldr > 300){
    lcd1.clear();
    
    lcd1.setCursor(0, 0);
    lcd1.print("Not Raining| ");
    lcd1.setCursor(13, 0);
    lcd1.print("Sunny| ");
    lcd1.setCursor(20, 0);
    if(dummy_water_height < 200){
      lcd1.print("Tank Full  ");
    }
    if(dummy_water_height > 200){
      lcd1.print("Tank Normal");
    }
    if(!servo_positioned){
      for(servo_pos=90; servo_pos<=180; servo_pos+=1, servo_pos2-=1){
        servo_right.write(servo_pos);
        servo_left.write(servo_pos2);
        delay(100);
        lcd1.setCursor(0, 1);
        lcd1.print(power);
        lcd1.print("W|");
        lcd1.setCursor(4, 1);
        lcd1.print("Panel Opening|");
  
        lcd1.setCursor(18, 1);
        lcd1.print("Solar Charging");
        
      } 
      servo_positioned = 1; 
    }
    
    lcd1.setCursor(0, 0);
    lcd1.print("Not Raining| ");
    lcd1.setCursor(13, 0);
    lcd1.print("Sunny| ");
    lcd1.setCursor(20, 0);
    
    if(dummy_water_height < 200){
      lcd1.print("Tank Full  ");
    }
    if(dummy_water_height > 200){
      lcd1.print("Tank Normal");
    }
    lcd1.setCursor(0, 1);
    lcd1.print(power);
    lcd1.print("W|");
    lcd1.setCursor(4, 1);
    lcd1.print("Panel Opened |");
    lcd1.setCursor(18, 1);
    lcd1.print("Solar Charging");
    
    panel_open=1;
  }

  //raining and panel is opened
  if(rain == 1  && panel_open == 1 ) {
    lcd1.clear();
    
    lcd1.setCursor(0, 0);
    lcd1.print("  Raining  | ");
    lcd1.setCursor(13, 0);
    if(value_ldr > 300){
      lcd1.print("Sunny| ");
      lcd1.setCursor(17, 0);
    }if(value_ldr < 300){
      lcd1.print("Shady| ");
      
    }
    
    if(dummy_water_height > 200){
        lcd1.setCursor(20, 0);
        lcd1.print("Tank Normal");
    }
    
    for(servo_pos=180; servo_pos>=90; servo_pos -= 1, servo_pos2 += 1){
      servo_right.write(servo_pos);
      servo_left.write(servo_pos2);
      delay(100);
      lcd1.setCursor(0, 1);
      lcd1.print(power);
      lcd1.print("W|");
      lcd1.setCursor(4, 1);
      lcd1.print("Panel Closing|");
      
    }  
      
      //lcd1.clear();
      lcd1.setCursor(0, 0);
      lcd1.print("  Raining  | ");
      
      
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      dummy_water_height= duration*0.034/2;
      
      
      lcd1.setCursor(13, 0);
      if(value_ldr > 300){
        lcd1.print("Sunny| ");
        lcd1.setCursor(17, 0);
      }else{
        lcd1.print("Shady| ");
        lcd1.setCursor(21, 0);
        lcd1.setCursor(18, 1);
        lcd1.print("  Not Charging");
      }
      
      if(dummy_water_height > 200){
        lcd1.setCursor(20, 0);
        lcd1.print("Tank Normal");
      }
      lcd1.setCursor(0, 1);
      lcd1.print(power);
      lcd1.print("W|");
      lcd1.setCursor(4, 1);
      lcd1.print("Panel Closed |");
      panel_open=0;
      
      if(dummy_water_height < 200){
          lcd1.setCursor(20, 0);
          lcd1.print("Tank Full  ");
        }
        lcd1.setCursor(18, 1);
        lcd1.print(" Valve Opened ");
        servo_bulb.write(180);
        
        lcd1.setCursor(0, 1);
        lcd1.print(power);
        lcd1.print("W|");
        lcd1.setCursor(18, 1);
        lcd1.print(" Valve Closed ");
        servo_bulb.write(90);
      
   }

  //no sun and no rain and panel is closed   
  if(value_ldr < 300 && rain == 0 && panel_open == 0){
    //lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Not Raining| ");
    lcd1.setCursor(20, 0);
    if(dummy_water_height < 200){
      lcd1.print("Tank Full  ");
      
    }else{
      lcd1.print("Tank Normal");
    }
    lcd1.setCursor(13, 0);
    lcd1.print("Shady| ");
    lcd1.setCursor(18, 1);
    lcd1.print("  Not Charging");
    if(panel_open == 0){
      for(servo_pos=90; servo_pos<=180; servo_pos+=1, servo_pos2-=1){
        servo_right.write(servo_pos);
        servo_left.write(servo_pos2);
        delay(100);
        lcd1.setCursor(0, 1);
        lcd1.print(power);
        lcd1.print("W|");
        lcd1.setCursor(4, 1);
        lcd1.print("Panel Opening|");
        panel_open = 1;
      }
      lcd1.setCursor(0, 1);
      lcd1.print(power);
      lcd1.print("W|");
      lcd1.setCursor(4, 1);
      lcd1.print("Panel Opened |");
    }else{
      servo_right.write(servo_pos);
      servo_left.write(servo_pos2);
    }
    lcd1.setCursor(0, 1);
    lcd1.print(power);
    lcd1.print("W|");
    lcd1.setCursor(4, 1);
    lcd1.print("Panel Opened |");
    
    delay(1000);
    
  }
  
  if( (value_ldr < 300 && panel_open == 0) || (value_ldr > 300 && panel_open == 1) || (rain == 1  && panel_open == 0) || (rain == 0  && panel_open == 1) ){
      servo_right.write(servo_pos);
      servo_left.write(servo_pos2);
      if(value_ldr < 300){
        lcd1.setCursor(13, 0);
        lcd1.print("Shady| ");
        lcd1.setCursor(18, 1);
        lcd1.print("  Not Charging");
        
      }else{
        lcd1.setCursor(13, 0);
        lcd1.print("Sunny| ");
      }
      if(rain == 1  && panel_open == 0){
        //lcd1.clear();
        lcd1.setCursor(0, 0);
        lcd1.print("  Raining  | ");
        lcd1.setCursor(0, 1);
        lcd1.print(power);
        lcd1.print("W|");
        lcd1.setCursor(4, 1);
        lcd1.print("Panel Closed |");
        lcd1.setCursor(13, 0);
        if(value_ldr > 300){
          lcd1.print("Sunny| ");
        }else{
          lcd1.print("Shady| ");
        }
        
        if(dummy_water_height < 200){
          lcd1.setCursor(20, 0);
          lcd1.print("Tank Full  ");
          lcd1.setCursor(0, 1);
          lcd1.print(power);
          lcd1.print("W|");
          lcd1.setCursor(18, 1);
          delay(1000);
          lcd1.print(" Valve Opened ");
          delay(1000);
          servo_bulb.write(180);
          
        }else{
          lcd1.setCursor(20, 0);
          lcd1.print("Tank Normal");
          lcd1.setCursor(18, 1);
          delay(1000);
          lcd1.print(" Valve Closed ");
          delay(1000);
          servo_bulb.write(90);
        }

        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        duration = pulseIn(echoPin, HIGH);
        dummy_water_height= duration*0.034/2;

        delay(100);
      }
      
  }
}
