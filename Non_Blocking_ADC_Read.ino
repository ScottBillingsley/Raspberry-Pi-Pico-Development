/*
                         Non Blocking ADC Read 
                        Vernon Billingsley c2023
                          Raspberry Pi Pico
                         Arduino IDE 1.8.13

                   Setup the ADC to read in a non blocking mode....      

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
#include "hardware/adc.h"


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

#define ADC_MAX 3
/* GPIO26 */
#define ADC0 0
/* GPIO27 */
#define ADC1 1
/* GPIO28 */
#define ADC2 2


/************************** Init Includes *************************/

/************************** Variables *****************************/
/* Hold the current adc count */
uint8_t adc_count = 0;
/* Tell there is a new adc to service */
boolean new_adc = false;
/* Hold the ADC values */
uint16_t this_adc[ADC_MAX];
/* Hold the ADC to service */
uint8_t service_adc = 0;

/************************** Variables1 ****************************/
volatile boolean flip0 = false;

/**************************  Functions ****************************/
void read_adc_once(uint8_t channel) {
  /* Set the channel to read  */
  adc_select_input(channel);
  /* Start the conversion */
  hw_set_bits(&adc_hw->cs, ADC_CS_START_ONCE_BITS);
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

  /* Initialise the ADC hardware */
  adc_init();
  /* Initialise the ADC pins */
  for (uint8_t a = (ADC0 + 26); a <= (ADC2 + 26); a ++) {
    adc_gpio_init(a);
  }
  /* Start with ADC0 */
  read_adc_once(ADC0);


}/**************************  End Setup **************************/

/******************************************************************/
/*************************** Setup1 *******************************/
/******************************************************************/
void setup1() {

  pinMode(LED_BUILTIN, OUTPUT);

}/**************************  End Setup1 **************************/

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {
  /* Check the ADC each loop */
  if ((adc_hw->cs & ADC_CS_READY_BITS) >> 8 == true) {
    /* Store the results in the array */
    this_adc[adc_count] = (uint16_t) adc_hw->result;
    /* Store the current adc count */
    service_adc = adc_count;
    /* Increment the adc count */
    adc_count ++;
    if (adc_count == ADC_MAX) {
      adc_count = 0;
    }
    /* Start the next conversion */
    read_adc_once(adc_count);
    /* Tell there is a new adc */
    new_adc = true;
 }

  /* Service the ADC */
  if (new_adc == true) {
    switch (service_adc) {
      case 0:
        Serial.println(this_adc[0]);
        break;
      case 1:
        Serial.print("\t");
        Serial.println(this_adc[1]);
        break;
      case 2:
        Serial.print("\t");
        Serial.print("\t");
        Serial.println(this_adc[2]);
        break;
    }
    new_adc = false;
  }

  delay(125);


}/*************************** End Loop *****************************/

/******************************************************************/
/**************************** Loop1 *******************************/
/******************************************************************/
void loop1() {
  /* Change the boolean */
  flip0 = !flip0;
  /* Write to the pin */
  digitalWrite(LED_BUILTIN, flip0);
  delay(125);

}/*************************** End Loop1 ****************************/
