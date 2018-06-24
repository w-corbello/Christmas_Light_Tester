//#define DEBUG 1  //.................................................Debug


//variables:___________________________________________________

int cur_chnl;

unsigned int Max_Cnls_ws2801;
unsigned int Brightness_ws2801;
unsigned int Step_Auto_ws2801;
unsigned int Step_Man_ws2801;
unsigned int Man_Delay_ws2801;



//unsigned int Baud_Rate;

extern int act_value;
extern int pattern_renard;

extern boolean r_WS2801;

extern int pattern_ws2801;
boolean setup_once_1 = false;




//;***********************************************************
//;Clear ALL
//;***********************************************************
void clear_all_1() {                                              //Clear all channels
  Ren_Comd();
  for (int t = 0; t < Max_Cnls_ws2801; t++) {
    Serial.write(0);
  }
}



//
//*****************************************************************
//Run ws2801 through renard
//*****************************************************************

void run_auto_ws2801(int i_setting) {


  clear_all_1();
  Ren_Comd();
  for (cur_chnl = 0; cur_chnl < Max_Cnls_ws2801 - 3 ; cur_chnl = cur_chnl + 3) {
    if (i_setting == 0) {
      Serial.write(0);
      Serial.write(0);
      Serial.write(0);
    } else if (i_setting == 1) {              //red.................
      Serial.write(Brightness_ws2801);
      Serial.write(0);
      Serial.write(0);
    } else if (i_setting == 2) {              //green.................
      Serial.write(0);
      Serial.write(Brightness_ws2801);
      Serial.write(0);
    } else if (i_setting == 3 ) {              //blue.................
      Serial.write(0);
      Serial.write(0);
      Serial.write(Brightness_ws2801);
    }
  }
}



// Calibrate
//*****************************************************************
//Max_Cnls_ws2801, Step_Man_ws2801, Man_Delay_ws2801
//*****************************************************************

void ws2801_calib() {

  clear_all();                                  // clear all channels
  Ren_Comd();
  Serial.write(Brightness_ws2801);              //red.................
  Serial.write(0);
  Serial.write(0);

  Serial.write(0);                              //green.................
  Serial.write(Brightness_ws2801);
  Serial.write(0);
  Serial.write(0);
  Serial.write(Brightness_ws2801);
  Serial.write(0);

  Serial.write(0);                             //blue.................
  Serial.write(0);
  Serial.write(Brightness_ws2801);
  Serial.write(0);
  Serial.write(0);
  Serial.write(Brightness_ws2801);
  Serial.write(0);
  Serial.write(0);
  Serial.write(Brightness_ws2801);



  delay(2000);
}









void run_ws2801() {

  if (setup_once_1 == false) {
    Serial.end();
    delay(100);
    Max_Cnls_ws2801 = EEPROM.read(30);
    if (Max_Cnls_ws2801 == 255) {
      Max_Cnls_ws2801 = 100;
    }
    Brightness_ws2801 = EEPROM.read(31);
    if (Brightness_ws2801 == 255) {
      Brightness_ws2801 = 120;
    }

    Step_Auto_ws2801 = 10;
    Step_Man_ws2801 = 10;
    Man_Delay_ws2801 = 10;

    if (Serial) {
      Serial.end();
    }
    Serial.begin(230400);                                       // We Set baud to that of PIC16F1825 chip here
    setup_once_1 = true;

#ifdef DEBUG  //...................only show if in debug mode
    Serial.print("Size......");
    Serial.println(Max_Cnls_ws2801);
    Serial.print("Brightness......");
    Serial.println("Size......");
    Serial.println(Max_Cnls_ws2801);
#endif



  }



  switch (pattern_ws2801) {
    case 0:
      run_auto_ws2801(0);
      r_WS2801 = false;
      break;
    case 1:
      run_auto_ws2801(1);
      break;
    case 2:
      run_auto_ws2801(2);
      break;
    case 3:
      run_auto_ws2801(3);
      break;
    case 4:
      ws2801_calib();
      break;

  }
  delay(2000);



}
