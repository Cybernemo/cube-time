#include "MPU9250.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

unsigned long front_time = 0;
unsigned long back_time = 0;
unsigned long left_time = 0;
unsigned long right_time = 0;
unsigned long up_time = 0;
unsigned long down_time = 0;
unsigned long ms_time = 0;
unsigned long cycle_time = 0;

MPU9250 myIMU;

#define SerialDebug true  // Set to true to get Serial output for debugging
void setup() {
  Wire.begin();
  Serial.begin(38400);

// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  display.clearDisplay();
  myIMU.initMPU9250();
  


}

void loop() {
  cycle_time = millis() - ms_time; 
  ms_time = millis();
  
  myIMU.readByte(MPU9250_ADDRESS, INT_STATUS);
  myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values
  myIMU.getAres();

  // Now we'll calculate the accleration value into actual g's
  // This depends on scale being set
  myIMU.ax = (float)myIMU.accelCount[0]*myIMU.aRes; // - accelBias[0];
  myIMU.ay = (float)myIMU.accelCount[1]*myIMU.aRes; // - accelBias[1];
  myIMU.az = (float)myIMU.accelCount[2]*myIMU.aRes; // - accelBias[2];

  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  float acc = 0.8;
  if(myIMU.az > acc){
    up_time = up_time + cycle_time;
    display.println("UP");
    display.println(up_time/60000);
    display.print("minutes");
  }else if(myIMU.az < -acc){
    down_time = down_time + cycle_time;
    display.println("DOWN");
    display.println(down_time/60000);
    display.print("minutes");
  }else if(myIMU.ay > acc){
    front_time = front_time + cycle_time;
    display.println("FRONT");
    display.println(front_time/60000);
    display.print("minutes");
  }else if(myIMU.ay < -acc){
    back_time = back_time + cycle_time;
    display.println("BACK");
    display.println(back_time/60000);
    display.print("minutes");
  }else if(myIMU.ax > acc){
    left_time = left_time + cycle_time;
    display.println("LEFT");
    display.println(left_time/60000);
    display.print("minutes");
  }else if(myIMU.ax < -acc){
    right_time = right_time + cycle_time;
    display.println("RIGHT");
    display.println(right_time/60000);
    display.print("minutes");
  }else{
    display.setCursor(0,24);
    display.print("NOT A FACE");
  }
  

  display.display();
  display.clearDisplay();

  // Print acceleration values in milligs!
//  Serial.print("X-acceleration: "); Serial.print(1000*myIMU.ax);
//  Serial.print(" mg ");

}
