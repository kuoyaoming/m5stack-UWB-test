//#include <M5Stack.h>
#include <M5Core2.h>
String DATA = " ";  //Used to store distance data .  
int UWB_MODE = 2;   //Used to set UWB mode .  

int UWB_T_UI_NUMBER_2 = 0; // flag bit 
int UWB_T_UI_NUMBER_1 = 0;
int UWB_T_NUMBER = 0;
int UWB_B_NUMBER = 0;

hw_timer_t *timer = NULL;
int timer_flag = 0;
int base_flag = 0;
uint32_t timer_data = 0;
static void IRAM_ATTR Timer0_CallBack(void);

//Data display.  
void UWB_display(){
  switch (UWB_MODE){
    case 0: //Tag mode 
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
    break;
    case 1: //Base station mode 
    if (UWB_B_NUMBER == 1){
      M5.Lcd.drawString(DATA, 60, 90, 4);//Prompt information  (Base station 0 only) 
    }
    break;
  }
}

//UI display
void UWB_ui_display() {
  M5.Lcd.drawString("UWB", 130, 0, 5); //UI
  M5.Lcd.drawString("Tag", 50, 210, 4);
  M5.Lcd.drawString("Base", 130, 210, 4);
  M5.Lcd.drawString("Reset", 220, 210, 4);

  switch (UWB_MODE){
    case 0:
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
    break;
    case 1: //Base station mode UI display  
    M5.Lcd.drawString("Base station", 80, 50, 4 );
    M5.Lcd.drawString(String(UWB_B_NUMBER), 240, 50, 4 ); //UWB_B_NUMBER
    if (UWB_B_NUMBER == 0){
      M5.Lcd.drawString("loading......", 100, 90, 4 );
    }
    else{
      M5.Lcd.fillRect(0, 90, 340, 30, BLACK);
    }

    break;
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
  switch (UWB_MODE){
    case 0:
    if (Serial2.available()){
      delay(20);
      UWB_T_NUMBER = (Serial2.available() / 11);      //Count the number of Base stations  
      delay(20);
      if (UWB_T_NUMBER != UWB_T_UI_NUMBER_1 || UWB_T_UI_NUMBER_2 == 0){    //Changing the UI display  
        UWB_ui_display();
        UWB_T_UI_NUMBER_1 = UWB_T_NUMBER;
        UWB_T_UI_NUMBER_2 = 1;
      }
      DATA = Serial2.readString();
      delay(2);
      timer_flag = 0;
      timer_data = 1;
      break;
    }else{
      timer_flag = 1;
    }
    if (timer_data == 0 || timer_data > 8){    //Count the number of Base stations  
      if(timer_data==9){M5.Lcd.fillRect(210, 50, 50, 30, BLACK);}
      DATA = "  0 2F   ";
      timer_flag = 0;
    }
    break;
    case 1:
    if (timer_data == 0 || timer_data > 70){   //Indicates successful or lost connection with Tag 
      if (Serial2.available()){
        delay(2);
        DATA = Serial2.readString();
        DATA = "set up successfully!";
        timer_data = 1;
        timer_flag = 1;
        break;
      }
      else if (timer_data > 0 && Serial2.available() == 0 ){
        DATA = "Can't find the tag!!!";
        timer_flag = 0;
        break;
      }
    }
    break;
  }
}

//AT command
void UWB_setupmode(){
  switch (UWB_MODE) {
    case 0:
      for (int b = 0; b < 2; b++)
      { //Repeat twice to stabilize the connection
        delay(50);
        Serial2.write("AT+anchor_tag=0\r\n");     //Set up the Tag 
        delay(50);
        Serial2.write("AT+interval=5\r\n");        //Set the calculation precision, the larger the response is, the slower it will be
        delay(50);                                     
        Serial2.write("AT+switchdis=1\r\n");        //Began to distance 
        delay(50);
        if (b == 0) {
          Serial2.write("AT+RST\r\n");            //RESET 
        }
      }
      UWB_clear();
      break;
    case 1:
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
      break;
  }
}


void UWB_Keyscan() {
  if (M5.BtnA.isPressed())
  {
    UWB_MODE = 0;
    UWB_setupmode();
    UWB_clear();
    UWB_ui_display();
    UWB_T_UI_NUMBER_2 = 0;
  }
  if (M5.BtnB.isPressed())
  {
    UWB_MODE = 1;
    if (UWB_B_NUMBER == 4)
    {
      UWB_B_NUMBER = 0;
    }
    UWB_setupmode();
    UWB_clear();
    UWB_ui_display();
    UWB_B_NUMBER++;
  }
  if (M5.BtnC.isPressed())
  {
    Serial2.write("AT+RST\r\n");
    UWB_setupmode();
    UWB_clear();
    UWB_ui_display();
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

void setup()
{
  M5.begin();
  //M5.Power.begin();
  //Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial2.begin(115200);
  delay(100);
  UWB_Timer();
  UWB_ui_display();
}

void loop()
{
  M5.update();
  UWB_Keyscan();
  UWB_readString();
  UWB_display();
  //Serial.println(UWB_MODE);
  Serial.println("base:");
  Serial.println(UWB_T_NUMBER);
  
}
