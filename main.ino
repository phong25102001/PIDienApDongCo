int count = 0;
int tempVoltage[101];
int mean;
float Kp = 0.8, Ki = 4.0;
int error;
int SetPoint = 0;
float eInt = 0, deltaT = 4.0/490;
int controlSignal; 


void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  long total = 0L;
  SetPoint = analogRead(A0);
  
  if (count<100)
    tempVoltage[count] = analogRead(A1);
  else
  {
    for (int i=0; i<count; i++)
        tempVoltage[i] = tempVoltage[i+1];
    tempVoltage[count] = analogRead(A1);
    count--;
  }
  
  for (int i=0; i<count; i++)
    total = total + tempVoltage[i];

  if (count>0)
    mean = total/count;
  count++;

  error = SetPoint-mean;
  eInt = eInt+error*deltaT;

  controlSignal = (int(Kp*error)+int(Ki*eInt))/4;
  
  if (controlSignal >= 255)
    controlSignal = 255;
  else if (controlSignal < 0)
    controlSignal = 0;   
  analogWrite(9,controlSignal);
  
  Serial.print(controlSignal);
  Serial.print(" ");
  Serial.print(SetPoint);
  Serial.print(" ");
  Serial.println(mean);
}
