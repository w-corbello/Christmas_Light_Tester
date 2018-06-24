


//variables:___________________________________________________
int current_chnl;

unsigned int Max_Cnls;
unsigned int Int_Step_Auto;
unsigned int Int_Step_Man;
unsigned int Man_Delay;
//unsigned int DIM_ON;
unsigned int Baud_Rate;

extern int act_value;
extern int pattern_renard;
boolean setup_once = false;

//;***********************************************************
//;Check Variable for Command code      0x7D, 0x7E and 0x7F
//;***********************************************************

void run_renard();


int ChkOpCode(int opcode) {
  int result;

  if (opcode == 0x7D) {
    result = 0x7C;
    return result;
  }

  if (opcode == 0x7E) {
    result = 0x7C;
    return result;
  }

  if (opcode == 0x7F) {
    result = 0x80;
    return result;
  }
  result = opcode;
  return result;

}


//****************************************************************
// functions:


void Ren_Comd() {                                     //Ren_Comd
  Serial.write(0x7E);                                //Command Start byte for Renard
  Serial.write(0x80);                                //Command byte for Renard
}




void fill_empty_channels(int ichnl) {               //fills channels not being worked on with 0x00
  for (int t = 0; t < ichnl; t++) {
    Serial.write(0);
  }
}



void clear_all() {                                  //Clear all channels
  Ren_Comd();
  for (int t = 0; t < Max_Cnls; t++) {
    Serial.write(0);
  }
}






// manual test
//*****************************************************************
//Max_Cnls, Int_Step_Man, Man_Delay
//*****************************************************************

void run_man() {
  int res;

  clear_all();
  for (current_chnl = 0; current_chnl < act_value; current_chnl++) {
    for (int t = 0; t < 255; t = t + Int_Step_Man) {
    Ren_Comd();
    fill_empty_channels(act_value - 1);
    res =  ChkOpCode(t);
    Serial.write(res);
    delay(Man_Delay * 5);

    }
  }
}




// Auto test
//*****************************************************************
//Max_Cnls, Int_Step_Man, Man_Delay
//*****************************************************************

void run_auto() {
  int res;


  for (current_chnl = 0; current_chnl < Max_Cnls; current_chnl++) {
    clear_all();
    for (int t = 0; t < 255; t = t + Int_Step_Auto) {

      Ren_Comd();
      fill_empty_channels(current_chnl);
      res =  ChkOpCode(t);
      Serial.write(res);
      delay(Man_Delay * 5);

    }
  }


}


// Auto test
//*****************************************************************
//Max_Cnls, Int_Step_Man, Man_Delay
//*****************************************************************

void dim_all() {
  current_chnl = 0;
  clear_all();                                  // clear all channels
  Ren_Comd();
  for (int t = 0; t < Max_Cnls + 1; t++) {
    Serial.write(115);
  }

  delay(Man_Delay * 5);
}









void run_renard() {

  if (setup_once == false) {
    Serial.end();
    delay(100);
    Max_Cnls = EEPROM.read(43);
    if (Max_Cnls == 255) {
      Max_Cnls = 32;
    }
    Int_Step_Auto = EEPROM.read(42);
    if (Int_Step_Auto == 255) {
      Int_Step_Auto = 10;
    }
    Int_Step_Man = EEPROM.read(41);
    if (Int_Step_Man == 255) {
      Int_Step_Man = 10;
    }
    Man_Delay = EEPROM.read(44);
    if (Man_Delay == 255) {
      Man_Delay = 10;
    }
    //DIM_ON = EEPROM.read(4);
    // if (DIM_ON == 255) {
    //   DIM_ON = 100;
    // }
    Baud_Rate = EEPROM.read(40);
    if (Baud_Rate >= 5) {
      Baud_Rate = 0;
    }


    if (Baud_Rate == 0) {
      Serial.begin(57600);
    } else if (Baud_Rate == 1) {
      Serial.begin(115200);
    } else if (Baud_Rate == 2) {
      Serial.begin(230400);
    } else if (Baud_Rate == 3) {
      Serial.begin(460800);
    }
    setup_once = true;
  }



  switch (pattern_renard) {
    case 0:
      break;
    case 1:
      run_man();
      break;
    case 2:
      run_auto();
      break;
    case 3:
      dim_all();
      break;
  }
  delay(Man_Delay * 5);



}
