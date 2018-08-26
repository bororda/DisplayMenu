#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

typedef struct {
  byte parent;
  bool hasSubItems;
  String name;
} menuItem;
byte currentMenuItemId;
menuItem root = {120, 0, "Root"};
menuItem item1 = {0, 0, "ItemOne"};
menuItem item2 = {0, 1, "ItemTwoWithSubMenu"};
menuItem item3 = {2, 0, "SubItemTwoOne"};
menuItem item4 = {2, 0, "SubItemTwoTwo"};
menuItem item5 = {2, 0, "SubItemTwoThree"};
menuItem menu[7]={root, item1, item2, item3, item4, item5};


void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  display.clearDisplay(); // Clear the buffer.
  currentMenuItemId=1;
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  displayMenu();
}

void loop() {
  if (Serial.available()){
    switch (Serial.read()){
      case 'w'://up
      if(currentMenuItemId>0 
      && menu[currentMenuItemId-1].parent==menu[currentMenuItemId].parent){
        currentMenuItemId--;
        displayMenu();
        }
      break;
      case 's'://down
      if(currentMenuItemId<sizeof(menu)-1
      && menu[currentMenuItemId+1].parent==menu[currentMenuItemId].parent){
        currentMenuItemId++;
        displayMenu();
      }
	  break;
	  case 'a'://back
	  if(menu[currentMenuItemId].parent>0){
	    currentMenuItemId=menu[currentMenuItemId].parent;
     displayMenu();
	  }
	  break;
	  case 'd'://forward
	  if(menu[currentMenuItemId].hasSubItems){
		  for(int i=0;i<sizeof(menu);i++){
			  if(menu[i].parent==currentMenuItemId){
				  currentMenuItemId=i;
          displayMenu();
          break;
			  }
		  }
	  }
	  break;
    }
  }
  Serial.println(currentMenuItemId);
}

void displayMenu(){
	display.clearDisplay();
  for(int i=sizeof(menu);i>0;i--){
    if(i == currentMenuItemId+1 
    && menu[i].parent == menu[currentMenuItemId].parent){
      display.println(menu[currentMenuItemId].name);
      //display.println(menu[i].name);
    }
    else if(i == currentMenuItemId-1 
    && menu[i].parent == menu[currentMenuItemId].parent){
      //display.println(menu[i].name);
      display.println(menu[currentMenuItemId].name);
    }
  }
  display.display();
}

