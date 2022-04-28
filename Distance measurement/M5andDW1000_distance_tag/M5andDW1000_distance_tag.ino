//#include <M5Stack.h>
#include <M5Core2.h>
String DATA = " ";  //Used to store distance data .  
int UWB_MODE = 0;   //Used to set UWB mode .  
int UWB_T_UI_NUMBER_2 = 0; // flag bit 
int UWB_T_UI_NUMBER_1 = 0;
int UWB_T_NUMBER = 0;

hw_timer_t *timer = NULL;
int timer_flag = 0;
int base_flag = 0;
uint32_t timer_data = 0;
static void IRAM_ATTR Timer0_CallBack(void);

//Data display.  
void UWB_display(){
 
  if (UWB_T_NUMBER > 0 && UWB_T_NUMBER < 5){
    int c = UWB_T_NUMBER;
    int b = 4 - UWB_T_NUMBER;
    while (c > 0){
      c--;
      M5.Lcd.drawString(DATA.substring(2 + c * 11, 3 + c * 11), 37, 50 + c * 40, 4 );//Tag the serial number 
      M5.Lcd.drawString(DATA.substring(4 + c * 11, 8 + c * 11), 210, 50 + c * 40, 4); //Distance 
    }
    while (b > 0){
      b--;
      M5.Lcd.fillRect(0, 170 - b * 40, 340, 30, BLACK);
    }
  }  
}

void UWB_setupmode(){
 
    for (int b = 0; b < 2; b++)
    { //Repeat twice to stabilize the connection
      delay(50);
      Serial2.write("AT+anchor_tag=0\r\n");     //Set up the Tag 
      delay(50);
      Serial2.write("AT+interval=30\r\n");        //Set the calculation precision, the larger the response is, the slower it will be
      delay(50);                                    
      Serial2.write("AT+switchdis=1\r\n");        //Began to distance 
      delay(50);
      if (b == 0) {
        Serial2.write("AT+RST\r\n");            //RESET 
      }
    }
    UWB_clear();   
}

//UI display
void UWB_ui_display() {
  M5.Lcd.drawString("UWB  Tag", 100, 0, 4); //UI

  if (UWB_T_NUMBER > 0 && UWB_T_NUMBER < 5){   //Tag mode UI display  
    int c = UWB_T_NUMBER;
    int b = 4 - UWB_T_NUMBER;
    while (c > 0 ){
      c--;
      M5.Lcd.drawString("B", 20, 50 + c * 40, 4 );
      M5.Lcd.drawString("Distance:", 80, 50 + c * 40, 4 );
      M5.Lcd.drawString("M", 280, 50 + c * 40, 4);
    }

    while (b > 0){
      b--;
      M5.Lcd.fillRect(0, 170 - b * 40, 340, 30, BLACK);
    }
  }   
}
//  Display and data clear.  
void UWB_clear() {
  if (Serial2.available()){
    delay(3);
    DATA = Serial2.readString();
  }
  DATA = "";
  timer_flag = 0;
  timer_data = 0;
  M5.Lcd.fillRect(0, 50, 340, 150, BLACK);
}
//Read UART data  
void UWB_readString(){
 
  if (Serial2.available()){
    delay(50);
    UWB_T_NUMBER = (Serial2.available() / 11);      //Count the number of Base stations  
    delay(50);
    if (UWB_T_NUMBER != UWB_T_UI_NUMBER_1 || UWB_T_UI_NUMBER_2 == 0){    //Changing the UI display  
      UWB_ui_display();
      UWB_T_UI_NUMBER_1 = UWB_T_NUMBER;
      UWB_T_UI_NUMBER_2 = 1;
    }
    DATA = Serial2.readString();
    delay(2);
    timer_flag = 0;
    timer_data = 1;
  }else{
    timer_flag = 1;
  }
  if (timer_data == 0 || timer_data > 8){    //Count the number of Base stations  提示与基站0断连（测试）
    if(timer_data==9){M5.Lcd.fillRect(210, 50, 50, 30, BLACK);}
    DATA = "  0 2F   ";
    timer_flag = 0;
  }
}


void UWB_Timer()
{
  timer = timerBegin(0, 80, true);              //Timer setting 
  timerAttachInterrupt(timer, Timer0_CallBack, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

static void IRAM_ATTR Timer0_CallBack(void)      //Timer function 
{
  if (timer_flag == 1)
  {
    timer_data++;
    if (timer_data == 4294967280)
    {
      timer_data = 1;
    }
  }
  else
  {
    timer_data = 0;
  }
}

static inline void fps(const int seconds){
  // Create static variables so that the code and variables can
  // all be declared inside a function
  static unsigned long lastMillis;
  static unsigned long frameCount;
  static unsigned int framesPerSecond;
  
  // It is best if we declare millis() only once
  unsigned long now = millis();
  frameCount ++;
  if (now - lastMillis >= seconds * 1000) {
    framesPerSecond = frameCount / seconds;
    Serial.print("FPS : ");
    Serial.println(framesPerSecond);
    frameCount = 0;
    lastMillis = now;
  }
}

void setup()
{
  M5.begin();
  //M5.Power.begin();
  //Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial2.begin(115200);
  delay(100);
  UWB_Timer();
  UWB_setupmode();
  UWB_ui_display();
}

void loop()
{
  M5.update();
  UWB_readString();
  UWB_display();  
  Serial.print(DATA);
  fps(3); 
}
