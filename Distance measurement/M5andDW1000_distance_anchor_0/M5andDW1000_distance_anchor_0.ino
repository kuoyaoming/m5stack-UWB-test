//#include <M5Stack.h>
#include <M5Core2.h>
String DATA = " ";  //Used to store distance data .  
int UWB_MODE = 1;   //Used to set UWB mode .  

int UWB_B_NUMBER = 0;

hw_timer_t *timer = NULL;
int timer_flag = 0;
int base_flag = 0;
uint32_t timer_data = 0;
static void IRAM_ATTR Timer0_CallBack(void);

//Data display.  数据显示
void UWB_display(){
    if (UWB_B_NUMBER == 1){
      M5.Lcd.drawString(DATA, 60, 90, 4);//Prompt information  (Base station 0 only) 
    } 
}

//UI display
void UWB_ui_display() {
  M5.Lcd.drawString("UWB", 130, 0, 4); //UI
  M5.Lcd.drawString(String(UWB_B_NUMBER), 130, 80, 8);


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

  if (timer_data == 0 || timer_data > 70){   //Indicates successful or lost connection with Tag  
    if (Serial2.available()){
      delay(2);
      DATA = Serial2.readString();
      DATA = "set up successfully!";
      timer_data = 1;
      timer_flag = 1;
    }
    else if (timer_data > 0 && Serial2.available() == 0 ){
      DATA = "Can't find the tag!!!";
      timer_flag = 0;
    }
  }
}

//AT command
void UWB_setupmode(){
  
    for (int b = 0; b < 2; b++)
    {
      delay(50);
      Serial2.write("AT+anchor_tag=1,");      //Set the base station 
      Serial2.print(UWB_B_NUMBER);            //UWB_B_NUMBER is base station ID0~ID3
      Serial2.write("\r\n");
      delay(1);
      delay(50);
      if (b == 0) {
        Serial2.write("AT+RST\r\n");
      }
    }
    UWB_clear();
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

void setup()
{
  M5.begin();
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
  
}
