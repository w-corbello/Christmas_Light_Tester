
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//#define DEBUG 1  //.................................................Debug


unsigned int total_slots = 510;
unsigned int act_universe = 100;
unsigned int cascade_pat = 0;
unsigned int pat_shift = 0;
unsigned int int_seconds = 0;
unsigned long int_timeNow = 0;
unsigned long int_timeLast = 0;
unsigned long send_udp = 0;
unsigned int udp_Port = 5568;
unsigned int i_bright = 126;

extern int pattern_type;
extern boolean r_E131;
boolean conn_wifi_once = false;

extern NextionText txt_run_e131;


extern const char* ssid;
extern const char* password;


boolean newNTP = false;





IPAddress host(192, 168, 1, 49);




const int sec_1[52] PROGMEM = {0x00, 0x10, 0x00, 0x00, 0x41, 0x53, 0x43, 0x2D, 0x45, 0x31,
                               0x2E, 0x31, 0x37, 0x00, 0x00, 0x00, 0x72, 0x6E, 0x00, 0x00,
                               0x00, 0x04, 0x7B, 0x38, 0x31, 0x37, 0x63, 0x34, 0x66, 0x65,
                               0x66, 0x2D, 0x32, 0x39, 0x34, 0x36, 0x2D, 0x34, 0x72, 0x58,
                               0x00, 0x00, 0x00, 0x02, 0x6A, 0x6F, 0x65, 0x79, 0x54, 0x65,
                               0x73, 0x74
                              };

//.....................................................    vvvv  iniverse  (113) & (114)
const int sec_2[19] = {0x64, 0x00, 0x00, 0x46, 0x00, 0x00, 0x01, 0x72, 0x0B, 0x02,
                       0xA1, 0x00, 0x00, 0x00, 0x01, 0x01, 0x83, 0x00
                      };



//Artnet PACKET
byte ArtDmxBuffer_1[637]; //137 + slots +3




WiFiUDP Udp;


// connect to wifi – returns true if successful or false if not.............................
boolean ConnectWifi(void) {
  boolean state = true;
  int i = 0;



  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false;
      break;
    }
    i++;
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(14, 0);
    delay(100);
    conn_wifi_once = true;
    txt_run_e131.setText("Connected..");
    delay(100);
    txt_run_e131.setText("Connected..");
    Udp.begin(udp_Port);                  //change to artnet "PORT"...................
    total_slots =((EEPROM.read(7)) * 256) + (EEPROM.read(6));
    i_bright = int(EEPROM.read(10));
  } else {
    Serial.println("");
    Serial.println("Connection failed.");
    digitalWrite(14, 1);
    delay(100);
    conn_wifi_once = false;
  }

  return state;
}


void construct_arnet_packet() {

  for (int t = 0; t < 52; t++) {
    ArtDmxBuffer_1[t] = sec_1[t];
  }

  for (int t = 52; t < 52 + 56 ; t++) {
    ArtDmxBuffer_1[t] = 0x00;
  }

  ArtDmxBuffer_1[108] = 0x64; //priority (0-200, default 100)
  ArtDmxBuffer_1[109] = 0x00; //Synchronization name
  ArtDmxBuffer_1[110] = 0x00; //Synchronization name
  ArtDmxBuffer_1[111] = 0x46; //Sequence Number
  ArtDmxBuffer_1[112] = 0x00; //Options
  ArtDmxBuffer_1[113] = highByte(act_universe); // universe high byte
  ArtDmxBuffer_1[114] = lowByte(act_universe); // universe low byte
  ArtDmxBuffer_1[115] = 0x72; //Flags and Length
  ArtDmxBuffer_1[116] = 0x0B; //Flags and Length
  ArtDmxBuffer_1[117] = 0x02; //Vector
  ArtDmxBuffer_1[118] = 0xA1; //Address Type & Data Type
  ArtDmxBuffer_1[119] = 0x00; //First Property (high byte)
  ArtDmxBuffer_1[120] = 0x00; //First Property (low byte)
  ArtDmxBuffer_1[121] = 0x00; //Address Increment (high byte)
  ArtDmxBuffer_1[122] = 0x01; //Address Increment (low byte)
  ArtDmxBuffer_1[123] = highByte(total_slots);  //total slots in packet(universe size) high byte
  ArtDmxBuffer_1[124] = lowByte(total_slots);  //total slots in packet(universe size) high byte
  ArtDmxBuffer_1[125] = 0x00; //actual start for data in slots



  if (pattern_type == 0) {
    for (int t = 0; t < total_slots - 3;  t = t + 3) {
      ArtDmxBuffer_1[126 + t + 0] = 0x00;
      ArtDmxBuffer_1[126 + t + 1] = 0x00;
      ArtDmxBuffer_1[126 + t + 2] = 0x00;
    }
    r_E131 = false;

  }


  if (pattern_type == 1) {
    ArtDmxBuffer_1[126 ] = 0xFF;
    ArtDmxBuffer_1[127 ] = 0x00;
    ArtDmxBuffer_1[128 ] = 0x00;

    ArtDmxBuffer_1[129 ] = 0x00;
    ArtDmxBuffer_1[130 ] = 0xFF;
    ArtDmxBuffer_1[131 ] = 0x00;
    ArtDmxBuffer_1[132 ] = 0x00;
    ArtDmxBuffer_1[133 ] = 0xFF;
    ArtDmxBuffer_1[134 ] = 0x00;

    ArtDmxBuffer_1[135 ] = 0x00;
    ArtDmxBuffer_1[136 ] = 0x00;
    ArtDmxBuffer_1[137 ] = 0xFF;
    ArtDmxBuffer_1[138 ] = 0x00;
    ArtDmxBuffer_1[139 ] = 0x00;
    ArtDmxBuffer_1[140 ] = 0xFF;
    ArtDmxBuffer_1[141 ] = 0x00;
    ArtDmxBuffer_1[142 ] = 0x00;
    ArtDmxBuffer_1[143 ] = 0xFF;

    for (int t = 0; t < total_slots - 3;  t++) {
      ArtDmxBuffer_1[144 + t] = 0x00;
    }


  }

  if (pattern_type == 2) {
    for (int t = 0; t < total_slots;  t = t + 3) {
      ArtDmxBuffer_1[126 + t + 0] = 0x7F;
      ArtDmxBuffer_1[126 + t + 1] = 0x00;
      ArtDmxBuffer_1[126 + t + 2] = 0x00;

    }

  }

  if (pattern_type == 3) {
    for (int t = 0; t < total_slots;  t = t + 3) {
      ArtDmxBuffer_1[126 + t + 0] = 0x00;
      ArtDmxBuffer_1[126 + t + 1] = 0x7F;
      ArtDmxBuffer_1[126 + t + 2] = 0x00;
    }

  }

  if (pattern_type == 4) {
    for (int t = 0; t < total_slots;  t = t + 3) {
      ArtDmxBuffer_1[126 + t + 0] = 0x00;
      ArtDmxBuffer_1[126 + t + 1] = 0x00;
      ArtDmxBuffer_1[126 + t + 2] = 0x7F;
    }

  }


}

void disconnect_wifi() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFi.disconnect();
    digitalWrite(14, 1);
    conn_wifi_once = false;
  }

}




// --------------------------------------------------------------------------------------
void doClock() {

  int_timeNow = millis() / 100;      // the number of milliseconds that have passed since boot

  if (newNTP == true) {
    int_timeLast = int_timeNow - int_seconds;
    newNTP = false;
  }



  int_seconds = int_timeNow - int_timeLast;    //used for heart beat ans one second clock

  if (int_seconds >= 5) {
    send_udp = send_udp + 1;
    int_timeLast = int_timeNow;
    digitalWrite(2, !digitalRead(2));
  }


  if (send_udp >= 3) {
    //Serial.println("Send UDP...");
    construct_arnet_packet();

    Udp.beginPacket(host, udp_Port);             //change host and port...............................................................
    Udp.write(ArtDmxBuffer_1, 125 + total_slots);
    Udp.endPacket();
    send_udp = 0;
  }
}






// --------------------------------------------------------------------------------------



void conn_wifi() {
  if (conn_wifi_once == false) {
    ConnectWifi();




  }
}


void run_artnet() {


  doClock();

  if (digitalRead(0) == LOW) {
    pattern_type = pattern_type + 1;
    delay(1000);
    if (pattern_type == 4) {
      pattern_type = 0;

    }
  }




}




