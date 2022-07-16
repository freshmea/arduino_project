#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library

// LCD 선언
LCDWIKI_KBV my_lcd(ILI9486,40,38,39,-1,41); //model,cs,cd,wr,rd,reset


//색깔 선언
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//게임 변수 선언
#define TIME_LIMIT  12 
#define LIFE        5

struct color_t{
  int a;
  int b;
  int c;
};

struct color_t oval; //original value
struct color_t tval; //temporary value


//전역변수 선언
int a;
int diff;
int sdiff;
int life = LIFE;
int score[] ={0,0,0,0,0};
extern volatile unsigned long timer0_millis;

String rank[10]={"A A A", "B B B", "C C C", "D D D", "E E E", "F F F", "G G G", "H H H", "I I I", "J J J"};
int rankscore[10]= {800, 700, 600, 500, 400, 300, 200, 100, 100, 100};

int melody1[] = {2637, 2637, 0, 2637, 0, 2093, 2637, 0, 3136,  0, 
                 0, 0, 1568, 0, 0, 0, 2093, 0, 0, 1568, 
                 0, 0, 1319, 0, 0, 1760, 0, 1976, 0, 1480, 
                 1760, 0, 1568, 2637, 3136, 3520, 0, 2794, 3136, 0, 
                 2637, 0, 2093, 2349, 3951, 0, 0, 2093, 0, 0, 
                 3136, 0, 0, 1319, 0, 0, 1760, 0, 3951, 0, 
                 1865, 1760, 0, 3136, 2637, 3136, 3520, 0, 2794, 3136, 
                 0, 2637, 0, 2093, 2349, 3951, 0, 0};
int tempo1[] = {12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
          12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
          9, 9, 9, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
          12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
          9, 9, 9, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12};
          
int melody2[] = {392, 659, 587, 523, 392, 0, 392, 659, 587, 523, 
                440, 0, 440, 698, 659, 587, 494, 0, 784, 784, 
                698, 587, 659, 523};
int tempo2[] = {25, 25, 25, 25, 75, 25,
    25, 25, 25, 25, 75, 25,
    25, 25, 25, 25, 75, 25,
    25, 25, 25, 25, 50, 25, 25};

int melody3[] = {440, 440, 0, 0, 440, 0, 349, 0, 523, 0, 440, 0, 349, 0, 523, 0, 440, 0, 659, 0,
         659, 0, 659, 0, 698, 0, 523, 0, 784, 0, 698, 0, 523, 0, 440, 0, 349, 349, 349,
         466, 698, 622, 587, 523, 932, 698, 622, 587, 523, 932, 698, 622, 587, 622, 523};
int tempo3[] = {50, 20, 50, 20, 50, 20, 40, 5, 20, 5, 
                60, 10, 40, 5, 20, 5, 60, 80, 50, 20, 
                50, 20, 50, 20, 40, 5, 20, 5, 60, 10,
                40, 5, 20, 5, 60, 40, 21, 21, 21, 128,
                128, 21, 21, 21, 128, 64, 21, 21, 21, 128, 
                64, 21, 21, 21, 128};



//함수선언
//sound effect 함수 선언
void spacegun(int maximum){
  for(int i=0; i<maximum;i++){
    digitalWrite(13, HIGH);
    delayMicroseconds(i);
    digitalWrite(13, LOW);
    delayMicroseconds(i);
  }
}

void randsound(int maximum){
  tone(13, random(maximum, 10*maximum));
  delay(maximum);
  noTone(13);
}

void fibonacci(int maximum){
  long fib = 1;
  long fib1 = 1;
  long fib2 = 2;
  for(int i=0; i<maximum; i++){
    fib = fib1+fib2;
    fib1 = fib2;
    fib2 = fib;
    tone(13, fib);
    delay(200);
  }
  noTone(13);
}


//기본 폰트 설정
void font(int f){
  if (f == 0){
    my_lcd.Set_Text_colour(RED);
    my_lcd.Set_Text_Back_colour(WHITE);
    my_lcd.Set_Text_Size(3);
  }
  if (f == 1){
    my_lcd.Set_Text_colour(BLACK);
    my_lcd.Set_Text_Back_colour(WHITE);
    my_lcd.Set_Text_Size(6);

  }
  if (f == 2){
  my_lcd.Set_Text_colour(GREEN);
  my_lcd.Set_Text_Back_colour(WHITE);
  my_lcd.Set_Text_Size(4);
  }
}


//랜덤색 함수 선언
struct color_t setocolor(void){
  randomSeed(analogRead(A5));
  oval.a=random(0, 255);
  oval.b=random(0, 255);
  oval.c=random(0, 255);
  my_lcd.Set_Draw_color(oval.a,oval.b,oval.c);
  my_lcd.Fill_Rectangle(my_lcd.Get_Display_Width()/2, 30, my_lcd.Get_Display_Width()-10, my_lcd.Get_Display_Height()-10);
  return oval;
}


//버튼 누르기전 멈춤 함수 선언
void halt(void){
  a = digitalRead(2);
    while(a == 0){
      a = digitalRead(2);
      if (a ==1){
        spacegun(300);
      }
  }
}

//배경 음악 실행
void music(int n){
  a = digitalRead(2);
    while(a == 0){
      a = digitalRead(2);
      //스타워즈
      if (n==0){
      for (int i = 0; i < 55; i++) {
        a = digitalRead(2);
        tone(13, melody3[i], tempo3[i]*7);  //tone함수를 이용해 8번핀으로 주파수 연주 
        delay(tempo3[i]*7);  
        if ( a==1){
          break;
        }
      }
      }
      //마리오
      if (n==1){
      for (int i = 0; i < 78; i++) {
        a = digitalRead(2);
        tone(13, melody1[i], tempo1[i]*14);  //tone함수를 이용해 8번핀으로 주파수 연주 
        delay(tempo1[i]*14);  
        if ( a==1){
          break;
        }
      }
      }
      //캐롤
      if (n==2){
      for (int i = 0; i < 24; i++) {
        a = digitalRead(2);
        tone(13, melody2[i], tempo2[i]*7);  //tone함수를 이용해 8번핀으로 주파수 연주 
        delay(tempo2[i]*7);  
        if ( a==1){
          break;
        }
      }
      }
      noTone(13);      //음 종료
      if (a ==1){
        spacegun(300);
      }
  }
}

//기회 감소 함수 선언
void minuslife(void){
    score[LIFE-life] = 200-sdiff;
    life -= 1;
    
    my_lcd.Fill_Screen(WHITE);
    my_lcd.Set_Text_Size(10);
    my_lcd.Print_String("My Goal ", 40, 60+10);
    my_lcd.Print_String(String(200-sdiff), 220, 60+120);
    
    font(0);
    delay(100);
    
    halt();
    my_lcd.Fill_Screen(WHITE);
    oval = setocolor(); 
    timer0_millis=0;
}


//시작화면
void startscreen(void){
  my_lcd.Fill_Screen(WHITE);
  delay(100);
  my_lcd.Fill_Screen(0, 255, 255);
  my_lcd.Set_Draw_color(255, 0, 0);
  my_lcd.Fill_Round_Rectangle(my_lcd.Get_Display_Width()/2-1-150+1, my_lcd.Get_Display_Height()/2-1-120+1, my_lcd.Get_Display_Width()/2-1+150-1, my_lcd.Get_Display_Height()/2-1+120-1,15);
  my_lcd.Set_Text_colour(0, 255, 255);   
  font(0);
  my_lcd.Set_Text_Mode(1);
  my_lcd.Set_Text_Size(6);
  my_lcd.Set_Text_colour(YELLOW);
  my_lcd.Print_String("Color", CENTER, my_lcd.Get_Display_Height()/2-90);
  my_lcd.Print_String("Training", CENTER, my_lcd.Get_Display_Height()/2-20);
  my_lcd.Print_String("Machine", CENTER, my_lcd.Get_Display_Height()/2+50);
  my_lcd.Set_Text_Mode(0);
  
  delay(100);   
  music(0);
  my_lcd.Fill_Screen(WHITE);
  
  font(1);
  my_lcd.Set_Text_Size(35);
  my_lcd.Set_Text_colour(RED);
  my_lcd.Print_String("3", 150, my_lcd.Get_Display_Height()/2-110);
  delay(1000);
  my_lcd.Print_String("2", 150, my_lcd.Get_Display_Height()/2-110);
  delay(1000);
  my_lcd.Print_String("1", 150, my_lcd.Get_Display_Height()/2-110);
  delay(1000);
  my_lcd.Fill_Screen(WHITE);
  my_lcd.Set_Text_Size(13);
  my_lcd.Print_String("START!", 20, my_lcd.Get_Display_Height()/2-60);
  delay(1000);
  my_lcd.Fill_Screen(WHITE);
  oval = setocolor();
  timer0_millis =0;
  life = LIFE;
}

void setup() {
  Serial.begin(9600);
  my_lcd.Init_LCD();
  my_lcd.Fill_Screen(WHITE);  
  my_lcd.Set_Rotation(1); 
  my_lcd.Set_Text_Mode(0);
  font(0);
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), swInterrupt, FALLING);
  noTone(13);
  //시작화면
  startscreen();
  oval = setocolor();
  timer0_millis =0;
}


void loop() {
  
  struct color_t val;
  //val 읽어들이기
  val.a = analogRead(A2);
  val.b = analogRead(A1);
  val.c = analogRead(A3);
  //val 255 기준
  val.a = val.a/4;
  val.b = val.b/4;
  val.c = val.c/4;
  //목표 값과 val 값 차이 계산
  diff = 0;
  sdiff = 0 ;
  diff = diff+abs(oval.a-val.a);
  diff = diff+abs(oval.b-val.b);
  diff = diff+abs(oval.c-val.c);
  sdiff = diff;
  if (sdiff > 400){
    sdiff = 400;
  }
  sdiff = sdiff/2;
  diff= diff/2;

  //현재 색사각형 그리기
  my_lcd.Set_Draw_color(val.a,val.b,val.c);
  my_lcd.Fill_Rectangle(10, 30, my_lcd.Get_Display_Width()/2, my_lcd.Get_Display_Height()-10);

  font(0);
  //현재 색 정보 표시
//  my_lcd.Print_String(String(val.a)+" ", 20, 30);
//  my_lcd.Print_String(String(val.b)+" ", 120, 30);
//  my_lcd.Print_String(String(val.c)+" ", 220, 30);

  //기본 정보 표시(점수, 남은시간, 기회 
//  my_lcd.Print_String("SCORE: :"+String(200-sdiff)+" "+"Time: "+String(TIME_LIMIT-timer0_millis/1000)+"Life: "+life+"  ", 0, 0);
  my_lcd.Print_String("Time: "+String(TIME_LIMIT-timer0_millis/1000)+"      Life: "+life+"  ", 50, 5);
  //5초 시간이 남은 것 카운트
  if (timer0_millis > (TIME_LIMIT*1000-5000) ){
    //5초 카운트 글자 사이즈 20, 색상, 글자 배경색
    my_lcd.Set_Text_Size(20);
    my_lcd.Set_Text_colour(WHITE);
    my_lcd.Set_Text_Back_colour(oval.a,oval.b,oval.c);
    
    my_lcd.Print_String(String(TIME_LIMIT-timer0_millis/1000), my_lcd.Get_Display_Width()/2+60, 60+60);

    //생상 재설정
    font(0);
  }


  //버튼을 눌렀거나 시간이 다 되었을때
  a = digitalRead(2);
  if ( a == 1 or timer0_millis>TIME_LIMIT*1000){
    minuslife();
  }

  //게임이 끝났을 때
  if (life == 0){
    my_lcd.Fill_Screen(WHITE);
    
    //총 점수 계산
    int totalscore = 0;
    for (int i=0;i<LIFE;i++){
      totalscore += score[i];
    }
    
    font(1);
    my_lcd.Print_String("Total:"+String(totalscore), 70, 80);

    font(2);
    my_lcd.Print_String("Your color level", 60, 150);

    font(0);
    //점수대 별 등급
    if ( totalscore > 900){
      my_lcd.Set_Text_Size(4);
      my_lcd.Print_String("Andy Warhol", 100, 230); 
    } else if (totalscore > 800){
      my_lcd.Set_Text_Size(5);
      my_lcd.Print_String("Hawk Eye", 100, 230);
    } else if (totalscore > 700){
      my_lcd.Set_Text_Size(4);
      my_lcd.Print_String("Head Designer", 100, 230);
    } else if (totalscore > 600){
      my_lcd.Set_Text_Size(6);
      my_lcd.Print_String("Designer", 100, 230);    
    } else if (totalscore > 500){
      my_lcd.Set_Text_Size(5);
      my_lcd.Print_String("Dog Eye", 100, 230);      
    } else {
      my_lcd.Set_Text_Size(7);
      my_lcd.Print_String("Client", 100, 230);   
    } 

    font(0);
    delay(100);
    music(2);
    delay(200);

    // 이니셜 입력
    my_lcd.Fill_Screen(WHITE);
    a = digitalRead(2);
    String ini;
    while ( a == 0){
      a = digitalRead(2);
      val.a = analogRead(A2);
      val.b = analogRead(A1);
      val.c = analogRead(A3);
      val.a = val.a/40+65;
      val.b = val.b/40+65;
      val.c = val.c/40+65;
      char c1=val.a;
      char c2=val.b;
      char c3=val.c;

      font(2);
      my_lcd.Print_String("Enter your name: ",  50, 60);
      my_lcd.Set_Text_Size(8);
      my_lcd.Set_Text_colour(BLUE);
      my_lcd.Print_String(String(c1), 100+60, 150);
      my_lcd.Print_String(String(c2), 160+60, 150);
      my_lcd.Print_String(String(c3), 220+60, 150); 
      ini =  String(c1)+" "+String(c2)+" "+String(c3);
      if (a ==1){
        spacegun(300);
      }
    }

    //랭크갱신하기
    int tempscore;
    String tempini;
    for(int i = 0 ;i<10;i++){
      if(totalscore >rankscore[i]){
        tempscore = rankscore[i];
        tempini = rank[i];
        rankscore[i] = totalscore;
        rank[i] = ini;
        totalscore = tempscore;
        ini = tempini;
      }
        
      
    }
    //랭킹 보여주기
    font(1);
    
    delay(100);
    my_lcd.Fill_Screen(WHITE);
    my_lcd.Set_Text_Size(6);
    my_lcd.Print_String("Ranking ", 130, 20);
    my_lcd.Set_Text_colour(BLACK);
    my_lcd.Set_Text_Size(4);
    for(int i = 0;i<5;i++){
      my_lcd.Print_String(String(i+1)+". "+rank[i]+": "+String(rankscore[i]), 60, 110+30*i);
    }
    delay(100);
    music(1);
    
    
    


    //시작화면
    startscreen();
    

  }
}

void swInterrupt(){
  a=1;
}
