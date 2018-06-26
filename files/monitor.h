
#include <E131.h>
extern int act_value;
extern const char* ssid;
extern const char* password;
extern boolean r_monitor_unicast;
extern boolean r_monitor;
boolean do_once_3 = true;
int ret_count = 0;
String  e131_returned = "";


unsigned long time_between = millis();
unsigned long timeNow = 0;
unsigned long timeLast = 0;


E131 e131;

//#define DEBUG 1  //.................................................Debug
void stop_monitor() {
  if (Serial) {
    Serial.end();
  }
  do_once_3 = true;
  String temp = "Stopped: ";
  char buffer[temp.length() + 1];
  temp.toCharArray(buffer, temp.length() + 1);
  txt_sacn_6.setText(buffer);

  temp = "";
  buffer[temp.length() + 1];
  temp.toCharArray(buffer, temp.length() + 1);
  txt_sacn_7.setText(buffer);
  txt_sacn_8.setText(buffer);


}


void setup_monitor() {
  Serial.begin(115200);
  delay(10);

  if (timeLast == 0) {
    timeNow = millis();       // the number of milliseconds that have passed since boot
    timeLast = timeNow;
  }


  String temp = "";
  char buffer[temp.length() + 1];
  temp.toCharArray(buffer, temp.length() + 1);
  txt_sacn_1.setText(buffer);
  txt_sacn_2.setText(buffer);
  txt_sacn_3.setText(buffer);
  txt_sacn_4.setText(buffer);
  txt_sacn_5.setText(buffer);
  txt_sacn_6.setText(buffer);
  txt_sacn_7.setText(buffer);
  delay(20);
  txt_sacn_1.setText(buffer);
  txt_sacn_2.setText(buffer);
  txt_sacn_3.setText(buffer);
  txt_sacn_4.setText(buffer);
  txt_sacn_5.setText(buffer);
  txt_sacn_6.setText(buffer);
  txt_sacn_7.setText(buffer);


  temp = "Connecting....";
  buffer[temp.length() + 1];
  temp.toCharArray(buffer, temp.length() + 1);
  txt_sacn_8.setText(buffer);
  delay(1000);

  if (r_monitor_unicast == true) {
    e131.begin(ssid, password);               /* via Unicast on the default port */


    String temp = " Unicast: ";
    char buffer[temp.length() + 1];
    temp.toCharArray(buffer, temp.length() + 1);
    txt_sacn_6.setText(buffer);

    temp = " Listening on: ";
    buffer[temp.length() + 1];
    temp.toCharArray(buffer, temp.length() + 1);
    txt_sacn_7.setText(buffer);

    temp = WiFi.localIP().toString();
    buffer[temp.length() + 1];
    temp.toCharArray(buffer, temp.length() + 1);
    txt_sacn_8.setText(buffer);


  }
  if (r_monitor_unicast == false) {
    if (act_value != 0) {
      e131.beginMulticast(ssid, password, act_value); /* via Multicast for Universe for entered value*/
    } else {
      e131.beginMulticast(ssid, password, 1); /* via Multicast for Universe 1 */
    }



    temp = " Multicast: ";
    char buffer[temp.length() + 1];
    temp.toCharArray(buffer, temp.length() + 1);
    txt_sacn_6.setText(buffer);

    String temp = " Universe: ";
    temp = temp + String(act_value);
    buffer[temp.length() + 1];
    temp.toCharArray(buffer, temp.length() + 1);
    txt_sacn_7.setText(buffer);

    temp = "";
    buffer[temp.length() + 1];
    temp.toCharArray(buffer, temp.length() + 1);
    txt_sacn_8.setText(buffer);


  }


}


void loop_monitor() {
  if (do_once_3 == true) {
    setup_monitor();
    do_once_3 = false;
  }

  /* Parse a packet */
  uint16_t num_channels = e131.parsePacket();


  String temp = " Waiting......... ";



  /* Process channel data if we have it */
  if (num_channels) {
    timeNow = millis();       // the number of milliseconds that have passed since boot
    time_between = timeNow - timeLast;
    timeLast = timeNow;

    String temp = " Univ: ";
    temp =  temp + String(e131.universe);
    char buffer[temp.length() + 1];
    temp.toCharArray(buffer, temp.length() + 1);
    txt_sacn_1.setText(buffer);
    delay(10);
    txt_sacn_1.setText(buffer);

    temp = " # Chnls: ";
    temp = temp + String(num_channels);
    buffer[temp.length() + 1];
    temp.toCharArray(buffer, temp.length() + 1);
    txt_sacn_2.setText(buffer);
    delay(10);
    txt_sacn_2.setText(buffer);

    temp = " # Packets: ";
    temp = temp + String(e131.stats.num_packets);
    buffer[temp.length() + 1];
    temp.toCharArray(buffer, temp.length() + 1);
    txt_sacn_3.setText(buffer);
    delay(10);
    txt_sacn_3.setText(buffer);

    temp = " # Errors: ";
    temp = temp +  String(e131.stats.packet_errors);
    buffer[temp.length() + 1];
    temp.toCharArray(buffer, temp.length() + 1);
    txt_sacn_4.setText(buffer);
    delay(10);
    txt_sacn_4.setText(buffer);
    temp = " mSec: ";
    temp = temp +  String(time_between);
    buffer[temp.length() + 1];
    temp.toCharArray(buffer, temp.length() + 1);
    txt_sacn_5.setText(buffer);
    delay(10);
    txt_sacn_5.setText(buffer);


#ifdef DEBUG  //...................only show if in debug mode
    Serial.printf("Universe %u / %u Channels | Packet#: %u / Errors: %u / CH1: %u\n",
                  e131.universe,              // The Universe for this packet
                  num_channels,               // Number of channels in this packet
                  e131.stats.num_packets,     // Packet counter
                  e131.stats.packet_errors,   // Packet error counter
                  e131.data[0]);              // Dimmer data for Channel 1
#endif
  }








  if (r_monitor == false) {
    stop_monitor();
    delay (250);
  }


  // delay (250);
}
