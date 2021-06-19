/*  
 * ArdMixer
 * baptistedelphin.fr
 * June 2021
 */
 
int motor1A = 5;
int motor1B = 6;

int speed = 255;

int fader1pin = A0;
int fader1val = 0;
int fader1vol = 0;

bool mute = false;

unsigned long start_millis = 0;
int interval = 50;

int master_volume = 100;

void setup() 
{
    Serial.begin(9600);
    //Serial.println("ArdMixer");

    delay(500);

    faderTo(100,fader1pin, motor1A, motor1B);
    faderTo(0,fader1pin, motor1A, motor1B);
    faderTo(100,fader1pin, motor1A, motor1B);

    delay(500);
}

void loop() 
{
    if (Serial.available() > 0)
    {
        String data = Serial.readStringUntil('\n');
        //Serial.print("> ");
        //Serial.println(data);
        
        if (data.startsWith("SET_MASTER_VOL"))
        {
            int new_master_val = getValue(data, ' ', 1).toInt();
            //Serial.print("New MASTER volume received : ");
            //Serial.println(new_master_val);
            
            if(new_master_val > 100)
                new_master_val = 100;
            else if(new_master_val < 0)
                new_master_val = 0;

            master_volume = new_master_val;
                
            faderTo(new_master_val,fader1pin, motor1A, motor1B);
        }
        else if (data == "SET_MASTER_MUTE")
        {
            mute = true;
            faderTo(0,fader1pin, motor1A, motor1B);
        }       
        else if (data == "SET_MASTER_UNMUTE")
        {
            mute = false;
            faderTo(master_volume,fader1pin, motor1A, motor1B);
        }
    }

    if(!mute)
    {
        fader1val = analogRead(fader1pin);
        fader1vol = map(fader1val, 0, 1014, 0, 100);
        if(fader1vol > master_volume+1 || fader1vol < master_volume-1)
        {
            
            //fader1vol = map(fader1val, 0, 1014, 0, 100);
            if(fader1vol%5 == 0 && (millis()-start_millis)>interval)
            {
                start_millis = millis();
                Serial.print("MASTER_VOL ");  
                Serial.println(fader1vol);
                master_volume = fader1vol;
            }         
            
        }
    }
    
}

void faderTo(int percent, int faderPin, int motorPinA, int motorPinB)
{
    int analogVal = map(percent, 0, 100, 0, 1014);
    while (analogRead(faderPin) != analogVal)
    {
        if(analogRead(faderPin) > analogVal)
        {
            analogWrite(motorPinA, speed);
            analogWrite(motorPinB, 0);
        }
        else
        {
            analogWrite(motorPinA, 0);
            analogWrite(motorPinB, speed);
        }
    }

    analogWrite(motorPinA, 0);
    analogWrite(motorPinB, 0);
    
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
