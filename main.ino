
int count = 0;
int tempVoltage[101];
int mean;
double Kp = 0.8;
int error;
int SetPoint = 0;

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
  analogWrite(9,int(Kp*error)/4);
  Serial.print(SetPoint);
  Serial.print(" ");
  Serial.println(mean);
}
