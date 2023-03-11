/*
                          ADC Map Function
                        Vernon Billingsley c2023
                          Raspberry Pi Pico
                         Arduino IDE 1.8.13

                  A non divison map function to map the ADC returns..
                  Also a LP filter to steady the ADC returns before
                  mapping...

                  Function time aprox 2 uS...

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

#define DEBUG 0

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

#define O_PIN 15

/************************** Init Includes *************************/

/************************** Variables *****************************/
/* Array to hold the ADC values */
uint16_t adc_array[ADC_MAX];

/* Slope variable for the map function */
float slope = 0.0;
/* Input start from */
int16_t in_start = 15; /* After filtering */
/* Input end */
int16_t in_end = 4090; /* After filtering */
/* Output map start */
int16_t out_start = 10;
/* Output map end */
int16_t out_end = 1760;

boolean new_adc = false;

/*About the filter  */
/* .125 * 256 = 32  */
/* .187 * 256 = 47  */
unsigned long alpha = 47;
/*Store the filtered sample */
unsigned long adc_filtered[ADC_MAX];
/*Store the previous filtered sample */
unsigned long f_v[5][3];

/************************** Variables1 ****************************/
volatile boolean flip0 = false;

/**************************  Functions ****************************/
void read_adc_once(uint8_t channel) {
  /* Set the channel to read  */
  adc_select_input(channel);
  /* Start the conversion */
  hw_set_bits(&adc_hw->cs, ADC_CS_START_ONCE_BITS);
}

float get_slope(int16_t input_start, int16_t input_end, int16_t output_start, int16_t output_end){
  float slope = 1.0 * ((float)output_end - (float)output_start) / ((float)input_end - (float)input_start);
  return slope;
}

int16_t adc_map(int16_t this_m){
  float output = (float) out_start + slope * (this_m - in_start);
  return (int16_t) output;
}

/* An integer math low pass filter for smooting the ADC reads */
void filter(byte ary_num) {
  /*Calculate the new value */
  //f_v[ary_num][1] = (float)alpha * adc_array[ary_num] + (1 - alpha) * f_v[ary_num][0];
  f_v[ary_num][1] = (((adc_array[ary_num] << 8) * alpha) + ((256 - alpha) * ( f_v[ary_num][0] << 8))) >> 16;
  /*Store the old value */
  f_v[ary_num][0] = f_v[ary_num][1];

  /*Store the filtered value */
  adc_filtered[ary_num] = f_v[ary_num][1];
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

  /* Used for oscope speed checking */
  pinMode(O_PIN, OUTPUT);

  /* Initialise the ADC hardware */
  adc_init();
  /* Initialise the ADC pins */
  for (uint8_t a = (ADC0 + 26); a <= (ADC2 + 26); a ++) {
    adc_gpio_init(a);
  }
  /* Start with ADC0 */
  read_adc_once(ADC0);
  /* Pre define the slope to avoid division.. */
  slope = get_slope(0, 4096, 10, 1100);

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
      /* Check the ADC each loop  */
    if ((adc_hw->cs & ADC_CS_READY_BITS) >> 8 == true) {
      digitalWrite(O_PIN, HIGH);
      /* Keep a running average */
      adc_array[ADC0] = (adc_array[ADC0] + (uint16_t) adc_hw->result) >> 1;
      /* Filter the ADC, results in adc_filtered[adc#] */
      filter(ADC0);
      /* Start the next conversion */
      read_adc_once(ADC0);
      /* Print the results.. */
      new_adc = true;
      /* Toggle pin for oscope timeing check */
      digitalWrite(O_PIN, LOW);
    }

    if(new_adc == true){

      Serial.print(adc_array[ADC0]);
      Serial.print("\t");
      Serial.print(adc_filtered[ADC0]);
      Serial.print("\t");      
      int16_t mapped_adc = adc_map(adc_filtered[ADC0]);
      Serial.println(mapped_adc);
      new_adc = false;
    }

  delay(25);

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
