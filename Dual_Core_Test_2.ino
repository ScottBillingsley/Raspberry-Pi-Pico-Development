/*
                            Dual Core Test
                        Vernon Billingley c2023
                          Raspberry Pi Pico
                         Arduino IDE 1.8.13

                  Test writing to an array using core1 and
                  read from and printing the array using core0..
                  Use the FIFO to pause core0 during the write cycle...


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

  Ring Oscillator random number generator code:
  https://deemocean.com/2022/10/02/rp2040-randomness-and-ring-oscillator/


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

/* Address of the Ring Oscillator bit */
#define ROSC_RANDOMBIT_OFFSET _u(0x0000001c)
#define ROSC_BASE _u(0x40060000)

/************************** Variables *****************************/
volatile uint16_t test_array[10];

uint8_t count = 0;

/* Allow the core0 to print the array */
volatile boolean Clear_to_Print = true;

/* The variable to hold the incoming data */
uint32_t eee;
/* The address of the variable to store the data */
uint32_t *foo = &eee;

/**************************  Functions ****************************/

/* Bits between 0 and 32 */
uint32_t ring_osc_rand(uint8_t bits) {
  static volatile uint32_t *randbit_reg = (uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);
  uint32_t random = 0;
  for (int i = 0; i < bits; i++)
  {
    unsigned int random_bit = 0x1 & (*randbit_reg);
    random = random << 1 | random_bit;
  }
  return random;
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
  /* Get a rnadom seed from the Ring Oscillator */
  uint32_t random_seed = ring_osc_rand(32);
  /* Intialize the RNG */
  srand(random_seed);

}/**************************  End Setup **************************/

/******************************************************************/
/*************************** Setup1 *******************************/
/******************************************************************/
void setup1() {
  /* Deley the startup a bit */
  delay(25);

}/**************************  End Setup1 **************************/

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {
  /* Check the FIFO has data */
  while (rp2040.fifo.available() > 0) {
    /* Get the data from the FIFO                        */
    /* rp2040.fifo.pop_nb writes directly to an address  */
    /* use a pointer to a variable the holds the address */
    /* of the variable to store to ie,                   */
    /* *foo holds the address of &eee                    */
    boolean this_bool = rp2040.fifo.pop_nb(foo);
    /* Set the boolean to the first bit */
    Clear_to_Print = eee & 0x01;
    if (this_bool == true) {
      if (Clear_to_Print == false) {
        Serial.println("    Waiting ");
      } else {
        Serial.println("    Running ");
      }
    }
  }

  /* If it's clear to print, print the array */
  if (Clear_to_Print == true) {
    for (uint8_t p = 0; p < 10; p ++) {
      Serial.println(test_array[p]);
      delay(5);
    }
  }
  /* After printing the array, wait for the next write cycle */
  /* to show the FIFO is working correctly                   */
  Clear_to_Print = false;

}/*************************** End Loop *****************************/

/******************************************************************/
/**************************** Loop1 *******************************/
/******************************************************************/
void loop1() {
  /* Get a random number from 0 to 255 */
  uint8_t this_random = (rand() % 255);
  /* Pause core0 to write the number to the array */
  rp2040.fifo.push(false);
  /* Print to the array */
  test_array[count] = this_random;
  /* Resume core0 */
  rp2040.fifo.push(true);
  /* Increment the count */
  count ++;
  if (count > 9) {
    count = 0;
  }
  /* Delay for a bit */
  delay(500);


}/*************************** End Loop1 ****************************/
