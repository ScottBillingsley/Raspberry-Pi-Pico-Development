/*
                          Clock Hands Test
                        Vernon Billingsley c2023
                          Raspberry Pi Pico
                         Arduino IDE 1.8.13

                  Display the clock face on a Nextion Display...


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

/* Define the screen size */
#define NEXTION_WIDTH 800
#define NEXTION_HEIGHT 480


/************************** Variables *****************************/
/* Set the rotation for the screen hands */
uint16_t rotation = 270;
/* Get the working screen cordinates */
uint16_t screen_center_x = NEXTION_WIDTH / 2;
uint16_t screen_center_y = NEXTION_HEIGHT / 2;
/* Set the radius of the clock circle */
uint16_t circle_radius = ((float)screen_center_y * .75);
uint16_t second_hand_radius = ((float)screen_center_y * .65);
uint16_t minute_hand_radius = ((float)screen_center_y * .60);
uint16_t hour_hand_radius = ((float)screen_center_y * .50);

/* Use to clear the screen */
String clear_screen = "cls BLACK";
/* The end transmit string */
String end_char = String(char(0xff)) + String(char(0xff)) + String(char(0xff));

uint16_t seconds = rotation;
uint16_t minutes = rotation;
uint16_t hours = rotation;

const char* colors[] = {
  "BLACK",
  "BLUE",
  "BROWN",
  "GREEN",
  "YELLOW",
  "RED",
  "GRAY",
  "WHITE",
};

/* Sine table for 0 to 360 degrees */
const double sin_table[] = {
  0.0000, 0.0175, 0.0349, 0.0523, 0.0698, 0.0872, 0.1045, 0.1219, 0.1392, 0.1564, 0.1736, 0.1908, 0.2079, 0.2250, 0.2419, 0.2588, 0.2756, 0.2924, 0.3090, 0.3256, 0.3420, 0.3584, 0.3746, 0.3907, 0.4067, 0.4226, 0.4384, 0.4540, 0.4695, 0.4848, 0.5000, 0.5150, 0.5299, 0.5446, 0.5592, 0.5736, 0.5878, 0.6018, 0.6157, 0.6293, 0.6428, 0.6561, 0.6691, 0.6820, 0.6947, 0.7071, 0.7193, 0.7314, 0.7431, 0.7547, 0.7660, 0.7771, 0.7880, 0.7986, 0.8090, 0.8192, 0.8290, 0.8387, 0.8480, 0.8572, 0.8660, 0.8746, 0.8829, 0.8910,
  0.8988, 0.9063, 0.9135, 0.9205, 0.9272, 0.9336, 0.9397, 0.9455, 0.9511, 0.9563, 0.9613, 0.9659, 0.9703, 0.9744, 0.9781, 0.9816, 0.9848, 0.9877, 0.9903, 0.9925, 0.9945, 0.9962, 0.9976, 0.9986, 0.9994, 0.9998, 1.0000, 0.9998, 0.9994, 0.9986, 0.9976, 0.9962, 0.9945, 0.9925, 0.9903, 0.9877, 0.9848, 0.9816, 0.9781, 0.9744, 0.9703, 0.9659, 0.9613, 0.9563, 0.9511, 0.9455, 0.9397, 0.9336, 0.9272, 0.9205, 0.9135, 0.9063, 0.8988, 0.8910, 0.8829, 0.8746, 0.8660, 0.8572, 0.8480, 0.8387, 0.8290, 0.8192, 0.8090, 0.7986,
  0.7880, 0.7771, 0.7660, 0.7547, 0.7431, 0.7314, 0.7193, 0.7071, 0.6947, 0.6820, 0.6691, 0.6561, 0.6428, 0.6293, 0.6157, 0.6018, 0.5878, 0.5736, 0.5592, 0.5446, 0.5299, 0.5150, 0.5000, 0.4848, 0.4695, 0.4540, 0.4384, 0.4226, 0.4067, 0.3907, 0.3746, 0.3584, 0.3420, 0.3256, 0.3090, 0.2924, 0.2756, 0.2588, 0.2419, 0.2250, 0.2079, 0.1908, 0.1736, 0.1564, 0.1392, 0.1219, 0.1045, 0.0872, 0.0698, 0.0523, 0.0349, 0.0175, -0.0000, -0.0175, -0.0349, -0.0523, -0.0698, -0.0872, -0.1045, -0.1219, -0.1392, -0.1564, -0.1736, -0.1908,
  -0.2079, -0.2250, -0.2419, -0.2588, -0.2756, -0.2924, -0.3090, -0.3256, -0.3420, -0.3584, -0.3746, -0.3907, -0.4067, -0.4226, -0.4384, -0.4540, -0.4695, -0.4848, -0.5000, -0.5150, -0.5299, -0.5446, -0.5592, -0.5736, -0.5878, -0.6018, -0.6157, -0.6293, -0.6428, -0.6561, -0.6691, -0.6820, -0.6947, -0.7071, -0.7193, -0.7314, -0.7431, -0.7547, -0.7660, -0.7771, -0.7880, -0.7986, -0.8090, -0.8192, -0.8290, -0.8387, -0.8480, -0.8572, -0.8660, -0.8746, -0.8829, -0.8910, -0.8988, -0.9063, -0.9135, -0.9205, -0.9272, -0.9336, -0.9397, -0.9455, -0.9511, -0.9563, -0.9613, -0.9659,
  -0.9703, -0.9744, -0.9781, -0.9816, -0.9848, -0.9877, -0.9903, -0.9925, -0.9945, -0.9962, -0.9976, -0.9986, -0.9994, -0.9998, -1.0000, -0.9998, -0.9994, -0.9986, -0.9976, -0.9962, -0.9945, -0.9925, -0.9903, -0.9877, -0.9848, -0.9816, -0.9781, -0.9744, -0.9703, -0.9659, -0.9613, -0.9563, -0.9511, -0.9455, -0.9397, -0.9336, -0.9272, -0.9205, -0.9135, -0.9063, -0.8988, -0.8910, -0.8829, -0.8746, -0.8660, -0.8572, -0.8480, -0.8387, -0.8290, -0.8192, -0.8090, -0.7986, -0.7880, -0.7771, -0.7660, -0.7547, -0.7431, -0.7314, -0.7193, -0.7071, -0.6947, -0.6820, -0.6691, -0.6561,
  -0.6428, -0.6293, -0.6157, -0.6018, -0.5878, -0.5736, -0.5592, -0.5446, -0.5299, -0.5150, -0.5000, -0.4848, -0.4695, -0.4540, -0.4384, -0.4226, -0.4067, -0.3907, -0.3746, -0.3584, -0.3420, -0.3256, -0.3090, -0.2924, -0.2756, -0.2588, -0.2419, -0.2250, -0.2079, -0.1908, -0.1736, -0.1564, -0.1392, -0.1219, -0.1045, -0.0872, -0.0698, -0.0523, -0.0349, -0.0175, 0.0000,
};
/* Cosine table for 0 to 360 degrees */
const double cos_table[] = {
  1.0000, 0.9998, 0.9994, 0.9986, 0.9976, 0.9962, 0.9945, 0.9925, 0.9903, 0.9877, 0.9848, 0.9816, 0.9781, 0.9744, 0.9703, 0.9659, 0.9613, 0.9563, 0.9511, 0.9455, 0.9397, 0.9336, 0.9272, 0.9205, 0.9135, 0.9063, 0.8988, 0.8910, 0.8829, 0.8746, 0.8660, 0.8572, 0.8480, 0.8387, 0.8290, 0.8192, 0.8090, 0.7986, 0.7880, 0.7771, 0.7660, 0.7547, 0.7431, 0.7314, 0.7193, 0.7071, 0.6947, 0.6820, 0.6691, 0.6561, 0.6428, 0.6293, 0.6157, 0.6018, 0.5878, 0.5736, 0.5592, 0.5446, 0.5299, 0.5150, 0.5000, 0.4848, 0.4695, 0.4540,
  0.4384, 0.4226, 0.4067, 0.3907, 0.3746, 0.3584, 0.3420, 0.3256, 0.3090, 0.2924, 0.2756, 0.2588, 0.2419, 0.2250, 0.2079, 0.1908, 0.1736, 0.1564, 0.1392, 0.1219, 0.1045, 0.0872, 0.0698, 0.0523, 0.0349, 0.0175, -0.0000, -0.0175, -0.0349, -0.0523, -0.0698, -0.0872, -0.1045, -0.1219, -0.1392, -0.1564, -0.1736, -0.1908, -0.2079, -0.2250, -0.2419, -0.2588, -0.2756, -0.2924, -0.3090, -0.3256, -0.3420, -0.3584, -0.3746, -0.3907, -0.4067, -0.4226, -0.4384, -0.4540, -0.4695, -0.4848, -0.5000, -0.5150, -0.5299, -0.5446, -0.5592, -0.5736, -0.5878, -0.6018,
  -0.6157, -0.6293, -0.6428, -0.6561, -0.6691, -0.6820, -0.6947, -0.7071, -0.7193, -0.7314, -0.7431, -0.7547, -0.7660, -0.7771, -0.7880, -0.7986, -0.8090, -0.8192, -0.8290, -0.8387, -0.8480, -0.8572, -0.8660, -0.8746, -0.8829, -0.8910, -0.8988, -0.9063, -0.9135, -0.9205, -0.9272, -0.9336, -0.9397, -0.9455, -0.9511, -0.9563, -0.9613, -0.9659, -0.9703, -0.9744, -0.9781, -0.9816, -0.9848, -0.9877, -0.9903, -0.9925, -0.9945, -0.9962, -0.9976, -0.9986, -0.9994, -0.9998, -1.0000, -0.9998, -0.9994, -0.9986, -0.9976, -0.9962, -0.9945, -0.9925, -0.9903, -0.9877, -0.9848, -0.9816,
  -0.9781, -0.9744, -0.9703, -0.9659, -0.9613, -0.9563, -0.9511, -0.9455, -0.9397, -0.9336, -0.9272, -0.9205, -0.9135, -0.9063, -0.8988, -0.8910, -0.8829, -0.8746, -0.8660, -0.8572, -0.8480, -0.8387, -0.8290, -0.8192, -0.8090, -0.7986, -0.7880, -0.7771, -0.7660, -0.7547, -0.7431, -0.7314, -0.7193, -0.7071, -0.6947, -0.6820, -0.6691, -0.6561, -0.6428, -0.6293, -0.6157, -0.6018, -0.5878, -0.5736, -0.5592, -0.5446, -0.5299, -0.5150, -0.5000, -0.4848, -0.4695, -0.4540, -0.4384, -0.4226, -0.4067, -0.3907, -0.3746, -0.3584, -0.3420, -0.3256, -0.3090, -0.2924, -0.2756, -0.2588,
  -0.2419, -0.2250, -0.2079, -0.1908, -0.1736, -0.1564, -0.1392, -0.1219, -0.1045, -0.0872, -0.0698, -0.0523, -0.0349, -0.0175, 0.0000, 0.0175, 0.0349, 0.0523, 0.0698, 0.0872, 0.1045, 0.1219, 0.1392, 0.1564, 0.1736, 0.1908, 0.2079, 0.2250, 0.2419, 0.2588, 0.2756, 0.2924, 0.3090, 0.3256, 0.3420, 0.3584, 0.3746, 0.3907, 0.4067, 0.4226, 0.4384, 0.4540, 0.4695, 0.4848, 0.5000, 0.5150, 0.5299, 0.5446, 0.5592, 0.5736, 0.5878, 0.6018, 0.6157, 0.6293, 0.6428, 0.6561, 0.6691, 0.6820, 0.6947, 0.7071, 0.7193, 0.7314, 0.7431, 0.7547,
  0.7660, 0.7771, 0.7880, 0.7986, 0.8090, 0.8192, 0.8290, 0.8387, 0.8480, 0.8572, 0.8660, 0.8746, 0.8829, 0.8910, 0.8988, 0.9063, 0.9135, 0.9205, 0.9272, 0.9336, 0.9397, 0.9455, 0.9511, 0.9563, 0.9613, 0.9659, 0.9703, 0.9744, 0.9781, 0.9816, 0.9848, 0.9877, 0.9903, 0.9925, 0.9945, 0.9962, 0.9976, 0.9986, 0.9994, 0.9998, 1.0000,
};

/**************************  Functions ****************************/
void screen_setup() {
  /* Clear the screen */
  Serial1.print(clear_screen + end_char);
  delay(1);
  /* Draw the face of the clock */
  draw_circle(screen_center_x, screen_center_y, circle_radius, 3);
  /* Place dots on the hour marks */
  for (uint16_t a = 0; a < 360; a += 30) {
    int16_t this_x = screen_center_x + (circle_radius * cos_table[a]);
    int16_t this_y = screen_center_y + (circle_radius * sin_table[a]);
    draw_dot(this_x, this_y, 5, 4);
  }

}/* End screen setup */

/* Draw a circle centerd at x,y with a radius r and color c */
void draw_circle(uint16_t circle_x, uint16_t circle_y, uint16_t circle_r, uint16_t circle_c) {
  /* Convert input ints to a string */
  String this_x = String(circle_x);
  String this_y = String(circle_y);
  String this_r = String(circle_r);
  String this_c = String(colors[circle_c]);
  String out_string = "cir " + this_x + "," + this_y + "," + this_r + "," + this_c + end_char;
  /* Send the the display */
  Serial1.print(out_string);
}
/* Draw a dot centerd at x,y with a radius r and filled with color c */
void draw_dot(uint16_t dot_x, uint16_t dot_y, uint16_t dot_r, uint16_t dot_c) {
  /* Convert input ints to a string */
  String this_x = String(dot_x);
  String this_y = String(dot_y);
  String this_r = String(dot_r);
  String this_c = String(colors[dot_c]);
  String out_string = "cirs " + this_x + "," + this_y + "," + this_r + "," + this_c + end_char;
  /* Send the the display */
  Serial1.print(out_string);
}

/* Draw a line on the screen */
void draw_line(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, uint16_t color) {
  String from_x = String(start_x);
  String from_y = String(start_y);
  String to_x = String(end_x);
  String to_y = String(end_y);
  String this_c = String(colors[color]);
  String out_string = "line " + from_x + "," + from_y + "," + to_x + "," + to_y + "," + this_c + end_char;
  /* Send the the display */
  Serial1.print(out_string);
}

int16_t get_x(uint16_t x_center, uint16_t radius, uint16_t degree) {
  return x_center + (radius * cos_table[degree]);
}

int16_t get_y(uint16_t y_center, uint16_t radius, uint16_t degree) {
  return y_center + (radius * sin_table[degree]);
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
  /* Use Serial1 for output to the Nextion..  */
  /* Set the Nextion for 115200 buad serial   */
  /* in the Program.s tab in the editor..     */
  Serial1.begin(115200);
  while (!Serial1) {
    /* Wait whille serail connects */
  }

  /* Setup the screen to start */
  screen_setup();


}/**************************  End Setup **************************/

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {

  /* Draw the hands */
  /* Clear the second hand*/
  draw_line(screen_center_x, screen_center_y, get_x(screen_center_x, second_hand_radius, seconds), get_y(screen_center_y, second_hand_radius, seconds), 0);
  /* Clear the minute hand */
  draw_line(screen_center_x, screen_center_y, get_x(screen_center_x, minute_hand_radius, minutes), get_y(screen_center_y, minute_hand_radius, minutes), 0);
  /* Clear the hour hand */
  draw_line(screen_center_x, screen_center_y, get_x(screen_center_x, hour_hand_radius, hours), get_y(screen_center_y, hour_hand_radius, hours), 0);

  seconds += 6;
  if (seconds >= 360) {
    seconds = 0;
  }
  if (seconds == rotation) {
    minutes += 6;
    if (minutes % 72 == 0) {
      hours += 6;
    }
  }
  if (minutes >= 360) {
    minutes = 0;
  }
  if (hours >= 360) {
    hours = 0;
  }

  /* Draw the hour hand */
  draw_line(screen_center_x, screen_center_y, get_x(screen_center_x, hour_hand_radius, hours), get_y(screen_center_y, hour_hand_radius, hours), 7);

  /* Draw minute hand */
  draw_line(screen_center_x, screen_center_y, get_x(screen_center_x, minute_hand_radius, minutes), get_y(screen_center_y, minute_hand_radius, minutes), 6);

  /* Draw second hand */
  draw_line(screen_center_x, screen_center_y, get_x(screen_center_x, second_hand_radius, seconds), get_y(screen_center_y, second_hand_radius, seconds), 5);


  delay(999);

}/*************************** End Loop *****************************/
