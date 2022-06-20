int bientro = 0;
int count = 0;
int tempVoltage[101];
int mean;

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  long total = 0L;
  bientro = analogRead(A0);
  
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
  analogWrite(9,bientro/4);
  Serial.print(bientro);
  Serial.print(" ");
  Serial.println(mean);
}
