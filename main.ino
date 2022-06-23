#include <PID_v1.h>

int count = 0;
int tempVoltage[101];
int mean;
// tinh trung binh 100 gia tri de uoc luong dien ap Vo

double Setpoint;
double Input;
double Output;
double Kp = 0.8, Ki = 2.5, Kd = 0;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup()
{
  Serial.begin(19200);
  myPID.SetMode(AUTOMATIC);
  myPID.SetTunings(Kp, Ki, Kd);
  myPID.SetOutputLimits(0,1023);
  
  TCCR1B = TCCR1B & B11111000 | B00000010;  // Set tan so xung PWM 3kHz
  pinMode(9, OUTPUT);
  
  
  //Setpoint = map(analogRead(A0),0,1023,0,255);
  //analogWrite(9,Setpoint);
  //delay(5000);
  //myPID.SetSampleTime(0.07);
}

void loop()
{
  Setpoint = analogRead(A0);
  
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
    mean = int(float(total)/count); // mean gan bang Vo
  count++;
  
  Input = double(mean); //doc dien ap vao scale de phu hop voi tin hieu ra
  myPID.Compute();
  analogWrite(9,Output/4);          // ghi tin hieu dieu khien PWM ra chan so 9
  Serial.print(Setpoint);
  Serial.print(" ");
  Serial.println(Input);
}
