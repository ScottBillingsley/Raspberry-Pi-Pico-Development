/*
                    Serial Wombat Encoder Test
                     Vernon Billingsley c2023
                        Raspberry Pi Pico
                        Arduino IDE 1.8.13 

                Setup and test the Serial Wombat in the 
                encoder mode on I2C...Display the results
                incrementing and decrementing a variable...                             

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission
    notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

               Raspberry Pi Pico

               -----------------
              |      |USB|      |
  TXD 0 GPIO0 -|1              40|-  VBUS
  RXD 0 GPIO1 -|2              39|-  Vsys
       GND   -|3              38|-  GND
       GPIO2 -|4              37|-  EN
       GPIO3 -|5              36|-  3.3v Out
       GPIO4 -|6              35|-  ADC Ref
       GPIO5 -|7              34|-  GPIO28  ADC2
       GND   -|8              33|-  GND AGND
       GPIO6 -|9              32|-  GPIO27  ACD1
       GPIO7 -|10             31|-  GPIO26
       GPIO8 -|11             30|-  Run/Reset
       GPIO9 -|12             29|-  GPIO22
       GND   -|13             28|-  GND
      GPIO10 -|14             27|-  GPIO21
      GPIO11 -|15             26|-  GPIO20
      GPIO12 -|16             25|-  GPIO19
      GPIO13 -|17             24|-  GPIO18
       GND   -|18             23|-  GND
      GPIO14 -|19             22|-  GPIO17
      GPIO15 -|20             21|-  GPIO16
               -----------------

  Board json..
  https://github.com/earlephilhower/arduino-pico

  Board Settings:
  Raspberry Pi Pico
  2MB (on FS)
  133 mHz
  Small (-Os) (standard)
  RTTI: disabled
  Stack Protector: disabled
  C++ Exceptions: disabled
  Debug Port: disabled
  Debug Level: none
  USB Stack: "Pico SDK"
  IP Stack: "IPv4 Only"
  Upload Method: "Default (UF2)"

  https://www.serialwombat.com/
  
*/
/************************ Includes ********************************/
#include <SerialWombat.h>

/************************* Defines ********************************/
#if !defined(LED_BUILTIN)
#define LED_BUILTIN     25
#endif

#define DEBUG 1

#if DEBUG > 0
#define dprint(expression) Serial.print("# "); Serial.print( #expression ); Serial.print( ": " ); Serial.println( expression )
#define dshow(expression) Serial.print( expression )
#else
#define dprint(expression)
#define dshow(expression)
#endif

/* Define the Wire pins */
/* GPIO12 Chip pin 16  */
#define THIS_SDA0 12
/* GPIO13 Chip pin 17  */
#define THIS_SCL0 13

SerialWombatChip sw6C;    //Declare a Serial Wombat chip
SerialWombatQuadEnc qeBasic(sw6C);

/************************** Variables *****************************/
/* Set the encoder position to the center */
uint16_t encoder_center = 32768;
/* Store the incoming data */
uint16_t i2c_data;
/* The variable to increment */
int16_t my_count;
/* Tell there is new data */
boolean new_data = false;

/**************************  Functions ****************************/

/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  if (DEBUG > 0) {
    Serial.begin(115200);
    while (!Serial);
    Serial.print(F(BOARD_NAME)); Serial.print(" @ "); Serial.print(F_CPU); Serial.println(" mHz");
  }

  /* Set the Wire pins */
  Wire.setSDA(THIS_SDA0);
  Wire.setSCL(THIS_SCL0);
  /* Start the Wire */
  Wire.begin();
  /* Initialize the Serial Wombat */
  sw6C.begin(Wire, 0x6C);
  /* Tell the Wombat the encoder pins */
  qeBasic.begin(0, 1);

}/**************************  End Setup **************************/

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {
  /* Poll the Serial Wombat */
  i2c_data = qeBasic.read();
  /* Rest the count to center after each read */
  qeBasic.write(encoder_center);

  if (i2c_data > encoder_center) {
    my_count ++;
    new_data = true;
  }
  if (i2c_data < encoder_center) {
    my_count --;
    if (my_count < 0) {
      my_count = 0;
    }
    new_data = true;
  }

  if (new_data == true) {
    Serial.println(my_count);
    new_data = false;
  }

  /* Delay a little */
  delay(50);

}/*************************** End Loop *****************************/
