#include <EEPROM.h>

#include "nextion_1.h"
#include "remote_1.h"
#include "Artnet_Sender.h"
#include "renard.h"
#include "ws2801.h"
#include "monitor.h"

#include "FastLED.h"

//#define DEBUG 1  //.................................................Debug


//Wifi settings.....................................................................................WIFI Here
const char* ssid = ".......";                                                        // CHANGE FOR YOUR SETUP
const char* password = "........";                                                    // CHANGE FOR YOUR SETUP
//Wifi settings.....................................................................................WIFI Here


//////////////////////////////////////////////////
#define BRIGHTNESS  60                            // 3 wire FastLed
#define NUM_LEDS 100                              // 3 wire FastLed

// Data pin that led data will be written out over
#define DATA_PIN 12                              // 3 wire FastLed


int string_size = 0;                            // variable we change for number of LEDS max is NUM_LEDS
int brightness = 50;
//////////////////////////////////////////////////
CRGB leds[NUM_LEDS];

unsigned int act_int = 0;
String act_string = "";
int act_value = 0;
String act_ip_add = "";

unsigned int display_page = 0;
unsigned int page_element = 0;
unsigned int last_element = 0;
int IP_[4];
int pattern_type = 0;
int pattern_renard = 0;
int pattern_ws2801 = 0;

//boolean show_result = false;


boolean r_E131 = false;
boolean r_3wire = false;
boolean r_WS2801 = false;
boolean r_renard = false;
boolean r_monitor = false;
boolean r_monitor_unicast = false;
boolean do_once = false;
boolean do_once_0 = false;

extern boolean setup_once;
extern IPAddress ip;

//................................................................
//setup
//................................................................
void setup() {
  Serial.begin(115200);

  nextionSerial.begin(115200);
  nex.init();
  nextionSerial.println(main_menu.show());

  EEPROM.begin(126);
  ArtDmxBuffer_1[(125 + total_slots)];

  host = {EEPROM.read(0), EEPROM.read(1), EEPROM.read(2), EEPROM.read(3)};
  udp_Port = ((EEPROM.read(5)) * 256) + (EEPROM.read(4));
  total_slots = ((EEPROM.read(7)) * 256) + (EEPROM.read(6));
  if (total_slots >= 512) {
    total_slots = 512;
  }
  act_universe = ((EEPROM.read(9)) * 256) + (EEPROM.read(8));
  i_bright = EEPROM.read(10);


  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  string_size = EEPROM.read(20);
  brightness = EEPROM.read(22);
  FastLED.setBrightness(50);      //brightness

  pinMode(0, INPUT);
  pinMode(14, OUTPUT);
  newNTP = true;
  cascade_pat = 0;

  digitalWrite(14, 1);

  delay(2000);
}

//......................................................................................
//Check Nextion serial output
//......................................................................................
void check_interface() {

  boolean data_receieved = false;
  char input[8];
  int n = 0;
  while (nextionSerial.available() > 1) {                         // Wait till there is something
    input[n] = nextionSerial.read();                              // Then: Get it
    delay(20);
    n++;
    data_receieved = true;
    if (n >= 8) {
      while (nextionSerial.available()) {
        nextionSerial.read();
      }
      break;
    }
  }


  if (data_receieved == true) {
    for (int t = 0; t < n; t++) {
      if (input[t] == 0x66) {
        display_page = input[t + 1];
        page_element = 0;
        data_receieved = false;
        break;
      }
    }
    for (int t = 0; t < n; t++) {
      if (input[t] == 0x65 ) {
        display_page = input[t + 1];
        page_element = input[t + 2];
        data_receieved = false;
        break;
      }
    }
#ifdef DEBUG  //...................only show if in debug mode
    Serial.print(display_page);
    Serial.print(";");
    Serial.println(page_element);
#endif

  }
  // display_page = what page we are on , page_element = ID of button, text etc on Nextion screen.
  //--------------------------------------------------------------------page == 0
  if (display_page == 0) {
    if (do_once_0 == false) {
      r_E131 = false;
      r_3wire = false;
      r_WS2801 = false;
      r_renard = false;
      do_once_0 = true;
    }
  }

  //--------------------------------------------------------------------page == 1
  if (display_page == 1) {
    if (do_once == false) {
#ifdef DEBUG  //...................only show if in debug mode
      Serial.println("Reset Do Once");
#endif
      do_once = true;
    }
  }
  //--------------------------------------------------------------------page == 2
  if (display_page == 2) {
    if (last_element != page_element) {
      switch (page_element) {
        case 0:                                                      // run_e131 - Page
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 2, on Page 2");
#endif
          conn_wifi();
          pattern_type = 0;
          r_E131 = true;
          last_element = page_element;
          break;



        case 3:                                                      // run_e131 - STOP
          Serial.println("Page 2, Stop");
          pattern_type = 0;
          r_E131 = true;
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 2, Stop");
#endif
          last_element = page_element;
          break;

        case 6:                                                     // run_e131 - RED
          r_E131 = true;
          pattern_type = 2;
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 2, RED");
#endif
          last_element = page_element;
          break;
        case 7:                                                     // run_e131 - GREEN
          r_E131 = true;
          pattern_type = 3;
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 2, GREEN");
#endif
          last_element = page_element;
          break;
        case 8:                                                   // run_e131 - BLUE
          r_E131 = true;
          pattern_type = 4;
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 2, BLUE");
#endif
          last_element = page_element;
          break;
        case 9:                                                  // run_e131 - START STEP
          r_E131 = true;
          pattern_type = 1;
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 2, STEP");
#endif
          last_element = page_element;
          break;
        case 10:                                                  // run_e131 - Connect to wifi
          conn_wifi();
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 2, Connect WiFi");
#endif
          last_element = page_element;
          break;
        case 11:                                                  // run_e131 - START STEP

#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 2, Disconnect WiFi");
#endif
          disconnect_wifi();
          last_element = page_element;
          break;
      }
    }
  }




  //--------------------------------------------------------------------page == 3
  if (display_page == 3) {
    if (last_element != page_element) {
      act_value = v_value_3.getValue();
      delay(50);
      char buffer[20];
      switch (page_element) {
        case 13:                                                                     // set_E131 - IP
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 3, IP");
#endif
          if (v_ip_add_3.getText(buffer, 20))
            act_ip_add = buffer;
          if (act_ip_add != "") {
#ifdef DEBUG  //...................only show if in debug mode
            Serial.println("IP: ");
            Serial.println(act_ip_add);
#endif
            parse_IP(act_ip_add, 0);
          }

          last_element = page_element;
          break;
        case 14:                                                                      // set_E131 - PORT
          if (act_value == 0) {
            act_value = v_value_3.getValue();
            delay(50);
          }
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 3, Port");
          Serial.println(act_value);
#endif
          save_entry(4, lowByte(act_value));    //E1.31  returned save to EEPROM
          save_entry(5, highByte(act_value));   //E1.31  returned save to EEPROM

          last_element = page_element;
          break;
        case 15:                                    // set_E131 - UNIVERSE
          if (act_value == 0) {
            act_value = v_value_3.getValue();
          }
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 3, Universe");
          Serial.println(act_value);
#endif
          save_entry(8, lowByte(act_value));    //E1.31  returned save to EEPROM
          save_entry(9, highByte(act_value));   //E1.31  returned save to EEPROM
          last_element = page_element;
          break;
        case 16:                                                                          // set_E131 - SIZE
          if (act_value == 0) {
            act_value = v_value_3.getValue();
            delay(50);
          }
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 3, Size");
          Serial.println(act_value);
#endif
          save_entry(6, lowByte(act_value));    //E1.31  returned save to EEPROM
          save_entry(7, highByte(act_value));   //E1.31  returned save to EEPROM
          last_element = page_element;
          break;

        case 25:                                                                          // set_E131 - Brightness
          if (act_value == 0) {
            act_value = v_value_3.getValue();
            delay(50);
          }
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 3, Brightness");
          Serial.println(act_value);
#endif
          save_entry(10, act_value);    //E1.31  returned save to EEPROM
          last_element = page_element;
          break;
      }
    }
  }
  //--------------------------------------------------------------------page == 4
  if (display_page == 4) {
    if (last_element != page_element) {
      act_value = v_value_4.getValue();
      delay(50);
      switch (page_element) {
        case 13:                                // set_3wire - Universe
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 4, Size");       //3 Wire Size returned save to EEPROM
#endif
          save_entry(20, act_value);
          last_element = page_element;
          break;
        case 21:                                // set_3wire - Brightness
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 4, Brightness");       //3 Wire Brightness returned save to EEPROM
#endif
          save_entry(22, act_value);
          last_element = page_element;
          break;
      }
    }
  }
  //--------------------------------------------------------------------------------------------  page == 5
  if (display_page == 5) {
    if (last_element != page_element) {
      switch (page_element) {
        case 3:                                // run_3wire - RED
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 5, RED");
#endif
          last_element = page_element;
          pattern_type = 2;
          r_3wire = true;
          break;
        case 4:                                // run_3wire - GREEN
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 5, GREEN");
#endif
          last_element = page_element;
          pattern_type = 3;
          r_3wire = true;
          break;
        case 5:                                // run_3wire - BLUE
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 5, BLUE");
#endif
          last_element = page_element;
          pattern_type = 4;
          r_3wire = true;
          break;
        case 6:                                // run_3wire - STEP
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 5, STEP");
#endif
          last_element = page_element;
          pattern_type = 1;
          r_3wire = true;
          break;
        case 7:                                // run_3wire - STOP
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 5, STOP");
#endif
          last_element = page_element;
          pattern_type = 0;
          r_3wire = true;
          break;
      }
    }
  }
  //--------------------------------------------------------------------page == 6
  if (display_page == 6) {
    if (last_element != page_element) {
      act_value = v_value_6.getValue();
      delay(50);
      if (act_value == 0) {
        act_value = v_value_6.getValue();
        delay(50);
      }
      switch (page_element) {
        case 13:                                // set_ws2801 - Size
          save_entry(30, act_value);            //WS2801 Size returned, save to EEPROM
          last_element = page_element;

#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 6, Size");
          Serial.println("Size");
          Serial.println(act_value);
#endif

          break;
        case 21:                                // set_ws2801 - Brightness
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 6, Brightness");       //WS2801 brightnes sreturned,  save to EEPROM
#endif
          save_entry(31, act_value);
          last_element = page_element;
          break;
      }
    }
  }

  //--------------------------------------------------------------------page == 7
  if (display_page == 7) {
    if (last_element != page_element) {
      switch (page_element) {
        case 3:                                // run_ws2801 - RED
          pattern_ws2801 = 1;
          r_WS2801 = true;
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 7, RED");
#endif
          last_element = page_element;
          break;
        case 4:                                // run_ws2801 - GREEN
          pattern_ws2801 = 2;
          r_WS2801 = true;
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 7, GREEN");
#endif
          last_element = page_element;
          break;
        case 5:                                // run_ws2801 - BLUE
          pattern_ws2801 = 3;
          r_WS2801 = true;
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 7, BLUE");
#endif
          last_element = page_element;
          break;
        case 6:                                // run_ws2801 - Calibrate
          pattern_ws2801 = 4;
          r_WS2801 = true;
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 7, STEP");
#endif
          last_element = page_element;
          break;
        case 7:                                // run_ws2801 - STOP
          pattern_ws2801 = 0;
          r_WS2801 = true;
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 7, STOP");
#endif
          last_element = page_element;
          break;
      }
    }
  }
  //--------------------------------------------------------------------page == 8
  if (display_page == 8) {
    if (last_element != page_element) {
      act_value = v_value_8.getValue();
      delay(50);
      switch (page_element) {
        case 13:                                // set_renard - BAUD
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 8, BAUD");
#endif
          save_entry(40, act_value);
          last_element = page_element;
          break;
        case 15:                                // set_renard - MAN
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 8, MAN");
#endif
          last_element = page_element;
          save_entry(41, act_value);
          break;
        case 16:                                // set_renard - DELAY
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 8, DELAY");
#endif
          save_entry(44, act_value);
          last_element = page_element;
          break;
        case 17:                                // set_renard - SIZE
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 8, SIZE");
#endif
          save_entry(43, act_value);
          last_element = page_element;
          break;
        case 18:                                // set_renard - AUTO
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 8, AUTO");
#endif
          last_element = page_element;
          save_entry(42, act_value);
          break;
      }
    }
  }

  //----------------------------------------------------------------------------page == 10
  if (display_page == 10) {
    if (last_element != page_element) {
      switch (page_element) {
        case 3:                                // run_renard - manual run
          last_element = page_element;
          act_value = v_value_10.getValue();
          if (act_value == 0) {
            act_value = v_value_10.getValue();
            delay(50);
          }

          pattern_renard = 1;
          r_renard = true;
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 10, Manual");
          Serial.println(act_value);

#endif
          break;
        case 4:                                // run_renard -auto run
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 10, Auto");
#endif
          last_element = page_element;
          pattern_renard = 2;
          r_renard = true;
          break;
        case 5:                                // run_renard -stop
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 10, Stop");
#endif
          last_element = page_element;
          clear_all();
          setup_once = false;
          r_renard = false;
          break;
        case 12:                                // run_renard -up
          last_element = page_element;
          act_value = v_value_10.getValue();
          if (act_value == 0) {
            act_value = v_value_10.getValue();
            delay(50);
          }
          pattern_renard = 1;
          r_renard = true;
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 10, Man Up");
          Serial.println(act_value);
#endif
          break;
        case 13:                                //run_renard -down
          last_element = page_element;
          act_value = v_value_10.getValue();
          if (act_value == 0) {
            act_value = v_value_10.getValue();
            delay(50);
          }
          r_renard = true;
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 10, Man Down");
          Serial.println(act_value);
#endif
          pattern_renard = 1;
          r_renard = true;
          break;
        case 14:                                //run_renard -dim all
          last_element = page_element;
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 10, Dim All");
#endif
          pattern_renard = 3;
          r_renard = true;

          break;
      }
    }
  }
  //--------------------------------------------------------------------page == 12
  if (display_page == 12) {

    if (do_once == true) {
      switch (page_element) {

        case 0:
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 12, Show Settings");
#endif

          get_Settings();
          act_value = 0;
          act_string = "";
          do_once = false;
          break;
        case 2:
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 12, Reset Do Once");
#endif

          do_once = true;
          break;
      }
    }
  }

  //--------------------------------------------------------------------page == 14
  if (display_page == 14) {

    if (last_element != page_element) {
      switch (page_element) {
        case 10:
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 14, Unicast");
#endif
          r_monitor_unicast = true;
          r_monitor = true;
          last_element = page_element;
          break;
        case 11:

          r_monitor_unicast = false;
          act_value = num_universe.getValue();
          delay(20);
          act_value = num_universe.getValue();
          delay(20);
          r_monitor = true;
          last_element = page_element;
          break;
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 14, Multicast");
          Serial.print("Multicast Universe: ");
          Serial.println(act_value);
#endif

        case 12:
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 14, Back");
#endif
          r_monitor = false;
          stop_monitor();
          last_element = page_element;
          break;
        case 13:
#ifdef DEBUG  //...................only show if in debug mode
          Serial.println("Page 14, Stop");
#endif
          r_monitor = false;
          stop_monitor();
          last_element = page_element;
          break;





      }
    }
  }

  //......................................................................................
  //End of Check interface
  //......................................................................................
}


//......................................................................................
//Run three wire using FastlLED
//......................................................................................
void run_three_wire() {
  if (pattern_type == 0) {
    string_size = EEPROM.read(20);
#ifdef DEBUG  //...................only show if in debug mode
    Serial.println("pattern_type == 0");
#endif
    for (int i_led = 0; i_led < string_size; i_led++) {
      leds[i_led] = CRGB::Black;
    }
    FastLED.show();
    delay(1000);
    r_3wire = false;
  }



  if (pattern_type == 1) {
    leds[0] = CRGB(255, 0, 0);
    leds[1] = CRGB(0, 255, 0);
    leds[2] = CRGB(0, 255, 0);
    leds[3] = CRGB(0, 0, 255);
    leds[4] = CRGB(0, 0, 255);
    leds[5] = CRGB(0, 0, 255);
    FastLED.show();
    delay(1000);
  }

  if (pattern_type == 2) {
    string_size = EEPROM.read(20);
    for (int i_led = 0; i_led < string_size; i_led++) {
      leds[i_led] = CRGB::Red;
    }
    FastLED.show();
    delay(1000);
  }

  if (pattern_type == 3) {
    string_size = EEPROM.read(20);
    for (int i_led = 0; i_led < string_size; i_led++) {
      leds[i_led] = CRGB::Green;
    }
    FastLED.show();
    delay(1000);
  }

  if (pattern_type == 4) {
    string_size = EEPROM.read(20);
    for (int i_led = 0; i_led < string_size; i_led++) {
      leds[i_led] = CRGB::Blue;
    }
    FastLED.show();
    delay(1000);
  }



}

//................................................................
//loop
//................................................................
void loop()  {

  check_interface();

  if (r_3wire == true) {                                 //r_3wire................................................................
    run_three_wire();
    boolean do_once_0 = false;
  }

  if (r_E131 == true) {                                 //r_E131................................................................
    run_artnet();
    boolean do_once_0 = false;
  }

  if (r_WS2801 == true) {                               //r_WS2801................................................................
    run_ws2801();
    boolean do_once_0 = false;
  }

  if (r_renard == true) {                               //r_renard................................................................
    act_value = v_value_10.getValue();
    delay(50);
    run_renard();
    boolean do_once_0 = false;
  }
  if (r_monitor == true) {                               //r_monitor................................................................
    loop_monitor();
    boolean do_once_0 = false;
  }



}
