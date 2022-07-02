#include <PID_v1.h>

#define Aphase 2
#define Bphase 3
#define Ia A1
#define PWM 9

// biến cần để đọc encoder -> tốc độ rpm
int rpm;
int encoderCount = 0;
const int interrupt0 = 0;

// thời gian đọc và trích mẫu tốc độ
int timecho = 100;
unsigned long long thoigian;
unsigned long long hientai = 0;

// các thông số PID cho bộ PID tốc độ (vòng ngoài)
double speedSetpoint = 50.0, speedInput, speedOutput;
double speedKp = 0.8, speedKi = 4, speedKd = 0;

// các thông số PID cho bộ PID dòng điện (vòng trong)
double& currentSetpoint = speedOutput; 
double currentInput, currentOutput;
double currentKp = 0.8, currentKi = 2.5, currentKd = 0;

// 2 bộ PID mạch vòng dòng điện và mạch vòng tốc độ; điều khiển cascade  
PID currentPID(&currentInput, &currentOutput, &currentSetpoint, currentKp, currentKi, currentKd, DIRECT);
PID speedPID(&speedInput,&speedOutput,&speedSetpoint, speedKp, speedKi, speedKd, DIRECT);

void setup()
{
  Serial.begin(19200);
  
  pinMode(Aphase, INPUT_PULLUP);
  pinMode(Bphase, INPUT_PULLUP);
  attachInterrupt(interrupt0, countPulse, RISING);
  
  // thiết lập bộ PID dòng
  currentPID.SetMode(AUTOMATIC);
  currentPID.SetTunings(currentKp, currentKi, currentKd);
  
  // thiết lập bộ PID tốc độ
  speedPID.SetMode(AUTOMATIC);
  speedPID.SetTunings(speedKp, speedKi, speedKd);
  TCCR1B = TCCR1B & B11111000 | B00000010;  // Set tan so xung PWM fsw = 3kHz
  pinMode(PWM, OUTPUT);
}

// Hàm ngắt đếm xung cho encoder để đo tốc độ
void countPulse()
{
  if(digitalRead(Bphase) == LOW)
    encoderCount++;
  else 
    encoderCount--;
}



void loop()
{
  // đọc Encoder mỗi 100ms
  thoigian = millis();
  if (thoigian - hientai > timecho) //millis thay delay
  {    
    hientai = thoigian;
    rpm = int((encoderCount/(300.0*0.1))*60.0);    
        /*
         * Đĩa encoder có 300 xung, chúng ta đo được 120 xung/0.1s
         * vậy lấy 120/(300*0.1) = x vòng/s
         * ta được: x*60 = số vòng quay / phút (RPM)
         */
    encoderCount = 0;
  }

  // Tính toán bộ PID 
  speedInput = rpm;
  speedPID.Compute();
  currentInput = analogRead(Ia)/1023.0;
  currentPID.Compute();
  Serial.println(currentOutput);
  analogWrite(PWM, currentOutput);
}
