/*
                          Dual Core Test 3
                        Vernon Billingley c2023
                          Raspberry Pi Pico
                          Arduino IDE 1.8.13

                   Use RPi_Pico_TimerInterrupt to blink an LED
                   One on core0 and one on core1...

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

*/
/************************ Includes ********************************/
/* https://github.com/khoih-prog/RPI_PICO_TimerInterrupt  */
#include "RPi_Pico_TimerInterrupt.h"

/************************* Defines ********************************/
#ifndef TIMER_INTERRUPT_DEBUG
#define TIMER_INTERRUPT_DEBUG         1
#endif
#ifndef _TIMERINTERRUPT_LOGLEVEL_
#define _TIMERINTERRUPT_LOGLEVEL_     4
#endif

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

/* Define the LED pins */
#define LED1 4
#define LED2 9

/************************** Init Includes *************************/
/* Setup core0 RPI_PICO_Timer */
RPI_PICO_Timer ITimer1(1);
/* Setup core1 RPI_PICO_Timer */
RPI_PICO_Timer ITimer2(2);

/************************** Variables *****************************/
/* core0 timer mS */
uint16_t TIMER1_INTERVAL_MS = 250;
/* core1 timer mS */
uint16_t TIMER2_INTERVAL_MS = 125;
/* Boolean for core0 LED */
volatile boolean flip0 = false;
/* Boolean for core1 LED */
volatile boolean flip1 = false;


/**************************  Functions ****************************/
/* The ISR handler for timer 1 */
bool TimerHandler1(struct repeating_timer *t) {
  (void) t;
  /* Change the boolean */
  flip0 = !flip0;
  /* Write to the pin */
  digitalWrite(LED1, flip0);
  
  return true;
}

/* The ISR handler for timer 2 */
bool TimerHandler2(struct repeating_timer *t) {
  (void) t;
  /* Change the boolean */
  flip1 = !flip1;
  /* Write to the pin */
  digitalWrite(LED2, flip1);
  
  return true;
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

  /************************* Setup Pins ***************************/
  pinMode(LED1, OUTPUT);

  /* Attach the Timer1 interrupt */
  bool timer1_debug = ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1);


}/**************************  End Setup **************************/

/******************************************************************/
/*************************** Setup1 *******************************/
/******************************************************************/
void setup1() {

  /************************* Setup Pins ***************************/
  pinMode(LED2, OUTPUT);

  /* Attach the Timer2 interrupt */
  bool timer2_debug = ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS * 1000, TimerHandler2);


}/**************************  End Setup1  *************************/

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {

  delay(100);

}/*************************** End Loop *****************************/

/******************************************************************/
/**************************** Loop1 *******************************/
/******************************************************************/
void loop1() {
  
  delay(100);
  
}/*************************** End Loop1 ****************************/
