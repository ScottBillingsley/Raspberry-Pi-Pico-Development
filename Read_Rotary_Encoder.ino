/*
                         Read Rotary Encoder
                        Vernon Billingsley c2023
                          Raspberry Pi Pico
                         Arduino IDE 1.8.13

               An example of how to read a KY040 rotary encoder        
               using pin change interrupt...

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

  Code example modified from:
  https://www.reddit.com/r/raspberrypipico/comments/pacarb/sharing_some_c_code_to_read_a_rotary_encoder/

*/
/************************ Includes ********************************/

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

/* GPIO20, chip pin 26, 102n cap to ground to debounce */
#define ENCODER_CLK 20
/* GPIO19, chip pin 25 102n cap tp ground to debounce */
#define ENCODER_DAT 19
/* GPIO18, chip pin 24 102n cap to ground to debounce */
#define ENCODER_BUT 18


/************************** Variables *****************************/
/* Hold the encoder count */
int encoder_count = 0;
boolean new_encoder_value = false;
boolean encoder_button = false;

/**************************  Functions ****************************/

void encoder_callback(uint gpio, uint32_t events) {

  static bool ccw_fall = false;
  static bool cw_fall = false;
  uint8_t encoder_value = 0;

  /* Get the state of all the pins, and mask off the three encoder pins.. */
  /* Change the right shift to match your setup... Pins 18, 19, and 20    */
  /* in this example..                                                    */
  encoder_value = (gpio_get_all() >> 18) & 0b111;

  if (gpio == ENCODER_CLK) {
    if (!cw_fall && (encoder_value == 0x03)) {
      cw_fall = true;
    }
    if (ccw_fall && (encoder_value == 0x01)) {
      cw_fall = 0;
      ccw_fall = 0;
      encoder_count --;
      new_encoder_value = true;
    }
  }
  if (gpio == ENCODER_DAT) {
    if (!ccw_fall && (encoder_value == 0x05)) {
      ccw_fall = 1;
    }
    if (cw_fall && (encoder_value == 0x01)) {
      cw_fall = false;
      ccw_fall = false;
      encoder_count ++;
      new_encoder_value = true;
    }
  }
  if (gpio == ENCODER_BUT) {
    if (encoder_value == 0x06) {
      encoder_button = true;
    }
  }
}/* End encoder_callback */



/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  if (DEBUG > 0) {
    Serial.begin(115200);
    while (!Serial);
    Serial.print(F(BOARD_NAME)); Serial.print(" @ "); Serial.print(F_CPU); Serial.println(" mHz");
  }

  /* Setup the encoder pins */
  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DAT, INPUT_PULLUP);
  pinMode(ENCODER_BUT, INPUT_PULLUP);

  /* Setup the interrupt callback */
  gpio_set_irq_enabled_with_callback(ENCODER_BUT, GPIO_IRQ_EDGE_FALL, true, &encoder_callback);
  gpio_set_irq_enabled(ENCODER_CLK, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(ENCODER_DAT, GPIO_IRQ_EDGE_FALL, true);

  Serial.println("Rotary Encoder Test..");


}/**************************  End Setup **************************/

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {

  if (new_encoder_value == true) {

    Serial.println(encoder_count);

    new_encoder_value = false;
  }
  if (encoder_button == true) {

    Serial.println("Button..");

    encoder_button = false;
  }


}/*************************** End Loop *****************************/
