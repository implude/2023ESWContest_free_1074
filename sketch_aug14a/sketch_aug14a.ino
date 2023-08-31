const float e = 2.7182;
const int UNITCOUNT 6;

int mic[UNITCOUNT] = {A0, A1, A2, A3, A4, A5};
int vib[UNITCOUNT] = {13, 12, 11, 10, 9, 8};
int threshHold[UNITCOUNT] = {100, 90, 90, 90, 90, 90};
int base[UNITCOUNT] = {50, 60, 70, 80, 120, 120};

struct Unit
{
  int micPin;
  int vibPin;

  int outputVib;//vibration strength to signal
  int avgVolume;//average of previous sounds detected
  int curVolume;//current sound detected

  int threshHold;
  int base;
  
};

Unit units[UNITCOUNT];

int deltaMS;

void setup() {
  Serial.begin(9600);

  for(int i=0;i<UNITCOUNT;i++)
  {// unit setup
    units[i].micPin=mic[i];
    pinMode(mic[i], INPUT);
    units[i].vibPin=vib[i];
    pinMode(vib[i], OUTPUT);
    
    units[i].avgVolume = analogRead(units[i].micPin);
    units[i].threshHold = threshHold[i];
    units[i].base = base[i];
  }

  deltaMS = 10;
}


void loop() {

  //time gap between each loop (seconds)
  float deltaTime = ((float)deltaMS)/1000;

  for(int i=0;i<UNITCOUNT;i++)
  {
    units[i].curVolume = analogRead(units[i].micPin);
    
    units[i].outputVib = units[i].curVolume - units[i].avgVolume;

    // change avgVolume following the [y = 1/e^x] graph
    units[i].avgVolume *= 1.0/pow(e,deltaTime);
    units[i].avgVolume += units[i].curVolume*deltaTime;

    if(units[i].outputSound > units[i].threshHold)
    {// sound stronger than threshHold
      analogWrite(units[i].vibPin, units[i].outputVib+units[i].base);
    }
    else
    {
      analogWrite(units[i].vibPin,0);
    }
    

  }

  delay(deltaMS);
}
