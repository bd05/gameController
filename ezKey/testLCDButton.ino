/*
  OLED Menu System
  
  with help from:
  https://forum.pjrc.com/threads/24350-Menu-system-on-Teensy3-and-OLED-(SSD1306)
  OLED Library from Adafruit
  MenuSystem library from https://github.com/jonblack/arduino-menusystem
  Button library from http://playground.arduino.cc/Code/Button#Download
*/

#include <MenuSystem.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>

//DEFINES
#define OLED_DC 11
#define OLED_CS 12
#define OLED_CLK 10
#define OLED_MOSI 9
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Menu variables
MenuSystem ms;
Menu mm("");
Menu mu1("Up");
MenuItem mu1_mi1("Easy");
MenuItem mu1_mi2("Medium");
MenuItem mu1_mi3("Hard");
Menu mu2("Down");
MenuItem mu2_mi1("Easy");
MenuItem mu2_mi2("Medium");
MenuItem mu2_mi3("Hard");
Menu mu3("Left/Right");
MenuItem mu3_mi1("Easy");
MenuItem mu3_mi2("Medium");
MenuItem mu3_mi3("Hard");

Button UpBtn = Button(5,PULLUP);
Button DownBtn = Button(4,PULLUP);
Button SelectBtn = Button(3,PULLUP);
Button BackBtn = Button(2,PULLUP);

bool bRanCallback = false;
bool bForward = true;
int line = 0;             // variable for setting display line

// Menu callback function
// In this example all menu items use the same callback.

void on_upEasy_selected(MenuItem* p_menu_item)
{
  //Serial.println("DISPLAY1 Selected");
  display.setCursor(0,55);
  display.print("upEasy Selected");
  bRanCallback = true;
  bForward = true;
}
void on_upMedium_selected(MenuItem* p_menu_item)
{
  //Serial.println("DISPLAY2 Selected");
  display.setCursor(0,55);
  display.print("upMedium Selected");
  //bRanCallback = false;
  bForward = true;
}
void on_upHard_selected(MenuItem* p_menu_item)
{
  //Serial.println("DISPLAY3 Selected");
  display.setCursor(0,55);
  display.print("upHard Selected");
  bRanCallback = false;
  bForward = true;
}

void on_downEasy_selected(MenuItem* p_menu_item)
{
  //Serial.println("SETTINGS1 Selected");
  display.setCursor(0,55);
  display.print("downEasy Selected");
  bRanCallback = true;
  bForward = true;
}
void on_downMedium_selected(MenuItem* p_menu_item)
{
  //Serial.println("SETTINGS2 Selected");
  display.setCursor(0,55);
  display.print("downMedium Selected");
  bRanCallback = false;
  bForward = true;
}
void on_downHard_selected(MenuItem* p_menu_item)
{
  //Serial.println("SETTINGS3 Selected");
  display.setCursor(0,55);
  display.print("downHard Selected");
  //bRanCallback = false;
  bForward = true;
}

void on_horizEasy_selected(MenuItem* p_menu_item)
{
  //Serial.println("SETTINGS1 Selected");
  display.setCursor(0,55);
  display.print("horizEasy Selected");
  bRanCallback = true;
  bForward = true;
}
void on_horizMedium_selected(MenuItem* p_menu_item)
{
  //Serial.println("SETTINGS2 Selected");
  display.setCursor(0,55);
  display.print("horizMedium Selected");
  bRanCallback = false;
  bForward = true;
}
void on_horizHard_selected(MenuItem* p_menu_item)
{
  //Serial.println("SETTINGS3 Selected");
  display.setCursor(0,55);
  display.print("horizHard Selected");
  //bRanCallback = false;
  bForward = true;
}

// Standard arduino functions

void setup()
{
  //Serial.begin(9600);
  
// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC); 
  display.display();     // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer
  
// Menu setup
  mm.add_menu(&mu1);
  mu1.add_item(&mu1_mi1, &on_upEasy_selected);
  mu1.add_item(&mu1_mi2, &on_upMedium_selected);
  mu1.add_item(&mu1_mi3, &on_upHard_selected);
  mm.add_menu(&mu2);
  mu2.add_item(&mu2_mi1, &on_downEasy_selected);
  mu2.add_item(&mu2_mi2, &on_downMedium_selected);
  mu2.add_item(&mu2_mi3, &on_downHard_selected);
  mm.add_menu(&mu3);
  mu3.add_item(&mu3_mi1, &on_horizEasy_selected);
  mu3.add_item(&mu3_mi2, &on_horizMedium_selected);
  mu3.add_item(&mu3_mi3, &on_horizHard_selected);
  ms.set_root_menu(&mm);
}

void loop()
{
//OLED set up
  display.display();    
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  line=0; //line variable reset
  //Serial.println("");
  
  
// Display Title
  //display.setCursor(0,0);
  //display.println("DIFFICULTY");
  
// Display the menu
  Menu const* cp_menu = ms.get_current_menu();
  MenuComponent const* cp_menu_sel = cp_menu->get_selected();
  for (int i = 0; i < cp_menu->get_num_menu_components(); ++i)
  {
    MenuComponent const* cp_m_comp = cp_menu->get_menu_component(i);
    //Serial.print(cp_m_comp->get_name());
    display.setCursor(30,line);
    display.print(cp_m_comp->get_name());
    
    if (cp_menu_sel == cp_m_comp){
      //Serial.print("<<< ");
      display.setCursor(0,line);
      display.print("> ");
    }
    line=line+10;
    //Serial.println("");
  } 
 
// read the state of the pushbutton value:
  if(UpBtn.isPressed()){    
    ms.prev(); 
  } 
  
  if(DownBtn.isPressed()){   
    ms.next();  
  } 

  if(SelectBtn.isPressed()){   
    ms.select();  
  } 
  
  if(BackBtn.isPressed()){   
    ms.back();  
  } 
 
  
// Wait for two seconds so the output is viewable
  delay(500);
}
