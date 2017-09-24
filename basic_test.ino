/* Infrared Thermometer MLX90614 (GY-906) [S034] : http://rdiot.tistory.com/56 [RDIoT Demo] */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <i2cmaster.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // LCD2004

void setup()
{
  lcd.init();  // initialize the lcd 
  lcd.backlight();
  lcd.print("start LCD2004");

  i2c_init(); //Initialise the i2c bus
  PORTC = (1 << PORTC4) | (1 << PORTC5);//enable pullups

  delay(1000);

  lcd.clear();
}

void loop()
{
 
  lcd.setCursor(0,0);
  lcd.print("S034:MLX90614 GY-906");

  int dev = 0x5A<<1;
  int data_low = 0;
  int data_high = 0;
  int pec = 0;
    
  i2c_start_wait(dev+I2C_WRITE);
  i2c_write(0x07);
    
  // read
  i2c_rep_start(dev+I2C_READ);
  data_low = i2c_readAck(); //Read 1 byte and then send ack
  data_high = i2c_readAck(); //Read 1 byte and then send ack
  pec = i2c_readNak();
  i2c_stop();
    
  //This converts high and low bytes together and processes temperature, MSB is a error bit and is ignored for temps
  double tempFactor = 0.02; // 0.02 degrees per LSB (measurement resolution of the MLX90614)
  double tempData = 0x0000; // zero out the data
  int frac; // data past the decimal point
   
  // This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
  tempData = (double)(((data_high & 0x007F) << 8) + data_low);
  tempData = (tempData * tempFactor)-0.01;
    
  float celcius = tempData - 273.15;
  float fahrenheit = (celcius*1.8) + 32;

  lcd.setCursor(0,1);
  lcd.print("Celcius=" + (String)celcius + "   ");

  lcd.setCursor(0,2);
  lcd.print("Fahrenheit=" + (String)fahrenheit + "  ");

  delay(1000); // wait a second before printing again
}
