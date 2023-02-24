/*
                            ADC Read Test
                        Vernon Billingsley c2023
                          Raspberry Pi Pico
                         Arduino IDE 1.8.13

                 Setup and read the ADC on core0 and control
                 the bllink delay of LED on core 1..                                    

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
       GPIO7 -|10             31|-  GPIO26  ADC0
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

/* GPIO2, Chip pin 4 for Oscope test */
#define OSCOPE 2

/************************** Variables *****************************/
uint8_t blink_delay = 125;
/* Boolean to tell core1 it's safe to read the variable */
volatile boolean clear_to_read = true;
/* The variable to hold the incoming data */
uint32_t ctr_bool;
/* The address of the variable to store the data */
uint32_t *ctr_address = &ctr_bool;
/* The bool to control the LED blink */
volatile boolean flip = false;
/* Hold the current adc reading */
uint16_t current_adc0;
/* Hold the new adc reading */
uint16_t new_adc0;


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

}/**************************  End Setup **************************/

/******************************************************************/
/*************************** Setup1 *******************************/
/******************************************************************/
void setup1() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(OSCOPE, OUTPUT);


}/**************************  End Setup1 **************************/

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {
  /* Read the ADC */
  new_adc0 = analogRead(A0);
  if(new_adc0 != current_adc0){
    /* Store the new adc reading */
    current_adc0 = new_adc0;
    /* Convert to 8 bit numner 0 to 256 */
    uint8_t this_read = (current_adc0 >> 4);
    /* Limit the short delay time */
    if(this_read < 10){
      this_read = 10;
    }
    /* Store the delay time */
      /* Before writing to the variable */
      /* Send a pause to core1         */
      rp2040.fifo.push(false);
      blink_delay = this_read;    
      /* Resume core1 after variable write */
      rp2040.fifo.push(true);    
  }
  delay(1);

}/*************************** End Loop *****************************/ 

/******************************************************************/
/**************************** Loop1 *******************************/
/******************************************************************/
void loop1() {
    /* Check the clear to read boolean before reading the array */
    /* Check the FIFO has data */
    while (rp2040.fifo.available() > 0) {
      boolean this_bool = rp2040.fifo.pop_nb(ctr_address);
      if (this_bool == true) {
        clear_to_read = ctr_bool & 0x01;
        digitalWrite(OSCOPE, clear_to_read);
      }
    }
    flip = !flip;
  /* Write to the pin */
  digitalWrite(LED_BUILTIN, flip);
  if(clear_to_read == true){
    delay(blink_delay);    
  }

}/*************************** End Loop1 ****************************/  
