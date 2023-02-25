/*
                         Pin Interrupt Test
                        Vernon Billingsley c2023
                          Raspberry Pi Pico
                         Arduino IDE 1.8.13

                   Setup amd test the arduino attach interrupt
                   function...SDK code also included..

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

  SDK interrupt code from:
  https://forums.raspberrypi.com/viewtopic.php?t=314186

             _|_                         330
  GPIO2 _____o o______        GPIO6 -----\/\/---- LED ---- GND
                      |
                     GND
             _|_                         330
  GPIO3 _____o o______        GPIO7 -----\/\/---- LED ---- GND
                      |
                     GND

*/
/************************ Includes ********************************/

/************************* Defines ********************************/
#if !defined(LED_BUILTIN)
#define LED_BUILTIN     25
#endif

#define DEBUG 0

#if DEBUG > 0
#define dprint(expression) Serial.print("# "); Serial.print( #expression ); Serial.print( ": " ); Serial.println( expression )
#define dshow(expression) Serial.print( expression )
#else
#define dprint(expression)
#define dshow(expression)
#endif

/* GPIO2, chip pin 4 */
#define BUTTON1 2
/* GPIO3, chip pin 5 */
#define BUTTON2 3
/* GPIO6, chip pin 9 */
#define LED1  6
/* GPIO7, chip pin 10 */
#define LED2  7

/************************** Variables *****************************/

/**************************  Functions ****************************/
//void gpio_callback(uint gpio, uint32_t events) {
//  if(gpio == BUTTON1){
//    digitalWrite(LED1, digitalRead(BUTTON1));
//  }
//  if(gpio == BUTTON2){
//    digitalWrite(LED2, digitalRead(BUTTON2));
//  }
//}

void button_1() {
  digitalWrite(LED1, digitalRead(BUTTON1));
}
void button_2() {
  digitalWrite(LED2, digitalRead(BUTTON2));
}

/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  if (DEBUG > 0) {
    Serial.begin(115200);
    while (!Serial);
    Serial.print(F(BOARD_NAME)); Serial.print(" @ "); Serial.print(F_CPU); Serial.println(" mHz");
  }

  pinMode(BUTTON1, INPUT);

  pinMode(LED1, OUTPUT);


  //gpio_set_irq_enabled_with_callback(BUTTON1, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  // gpio_set_irq_enabled(BUTTON2, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

  attachInterrupt(BUTTON1, button_1, CHANGE);


}/**************************  End Setup **************************/

/******************************************************************/
/*************************** Setup1 *******************************/
/******************************************************************/
void setup1() {

  pinMode(BUTTON2, INPUT);

  pinMode(LED2, OUTPUT);

  attachInterrupt(BUTTON2, button_2, CHANGE);

}/**************************  End Setup1 **************************/

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {


}/*************************** End Loop *****************************/

/******************************************************************/
/**************************** Loop1 *******************************/
/******************************************************************/
void loop1() {


}/*************************** End Loop1 ****************************/
