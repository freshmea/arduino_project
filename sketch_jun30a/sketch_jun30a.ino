
int melody[] = {2637, 2637, 0, 2637, 0, 2093, 2637, 0, 3136,  0, 0, 0, 1568, 0, 0, 0,
         2093, 0, 0, 1568, 0, 0, 1319, 0, 0, 1760, 0, 1976, 0, 1480, 1760, 0,
         1568, 2637, 3136, 3520, 0, 2794, 3136, 0, 2637, 0, 2093, 2349, 3951, 0, 0,
         2093, 0, 0, 3136, 0, 0, 1319, 0, 0, 1760, 0, 3951, 0, 1865, 1760, 0,
         3136, 2637, 3136, 3520, 0, 2794, 3136, 0, 2637, 0, 2093, 2349, 3951, 0, 0};
//int tempo[] = {12, 12, 12, 12, 12, 12, 12, 12};

void setup() {
  // put your setup code here, to run once:
   pinMode(2, INPUT);
   Serial.begin(9600);
}

void loop() {
  int a;
  a = digitalRead(2);
  Serial.println(a);
  
for (int i = 0; i < 78; i++) {
    tone(13, melody[i], 40);  //tone함수를 이용해 8번핀으로 주파수 연주
    delay(100);    
    noTone(13);      //음 종료
  }
  
}
