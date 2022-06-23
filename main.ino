#include <PID_v1.h>

int count = 0;
int tempVoltage[101];
int mean;
// tinh trung binh 100 gia tri de uoc luong dien ap Vo

double Setpoint;
double Input;
double Output;
double Kp = 0.8, Ki = 2, Kd = 0;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup()
{
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  myPID.SetMode(AUTOMATIC);
  myPID.SetTunings(Kp, Ki, Kd);
  
  Setpoint = map(analogRead(A0),0,1023,0,255);
  analogWrite(9,Setpoint);
  delay(5000);
 // myPID.SetSampleTime(1);
}

void loop()
{
  Setpoint = map(analogRead(A0),0,1023,0,255);
  
  long total = 0L;
  
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
    mean = total/count; // mean gan bang Vo
  count++;
  
  Input = map(mean,0,1023,0,255); //doc dien ap vao scale de phu hop voi tin hieu ra
  myPID.Compute();
  analogWrite(9,Output);          // ghi tin hieu dieu khien PWM ra chan so 9

  Serial.print(Setpoint);
  Serial.print(" ");
  Serial.println(Input);
}
