
//#define DEBUG 1  //.................................................Debug


extern int IP_[4]; //variables for parsing IP from setup screen


// This get data from EEPROM so that we can send to VIEW screen on Nextion
//................................................................
//Get Settings
//................................................................
void get_Settings() {
  String temp_string = "";
  temp_string = "E1.31 -- IP Address:\\r" ;
  temp_string = temp_string + String(int(EEPROM.read(0)));
  temp_string = temp_string + ".";
  temp_string = temp_string + String(int(EEPROM.read(1)));
  temp_string = temp_string + ".";
  temp_string = temp_string + String(int(EEPROM.read(2)));
  temp_string = temp_string + ".";
  temp_string = temp_string + String(int(EEPROM.read(3)));
  temp_string = temp_string +  "\\r" ;
  temp_string = temp_string + "Port:" ;
  unsigned  int temp = (int(EEPROM.read(5)) * 256) + int(EEPROM.read(4));
  temp_string = temp_string + String(temp);
  temp_string = temp_string + " Size:" ;
  temp = ((EEPROM.read(7)) * 256) + (EEPROM.read(6));
  temp_string = temp_string + String(temp);
  temp_string = temp_string +  "\\r" ;
  temp_string = temp_string + "Universe:" ;
  temp = ((EEPROM.read(9)) * 256) + (EEPROM.read(8));
  temp_string = temp_string + String(temp);
  temp_string = temp_string +  "\\r" ;
  temp_string = temp_string + "Brightness: " ;
  temp_string = temp_string + String(int(EEPROM.read(10)));
  temp_string = temp_string +  "\\r" ;
  temp_string = temp_string + "3Wire --" ;
  temp_string = temp_string +  "\\r" ;
  temp_string = temp_string +  "Size:" ;
  temp_string = temp_string + String(int(EEPROM.read(20)));
  temp_string = temp_string +  "\\r" ;
  temp_string = temp_string + "WS2801 --  " ;
  temp_string = temp_string + "Size: " ;
  temp_string = temp_string + String(int(EEPROM.read(30)));
  temp_string = temp_string +  "\\r" ;
  temp_string = temp_string + "Renard --   \\r" ;
  temp_string = temp_string + "Baud: " ;
  temp_string = temp_string + String(int(EEPROM.read(40)));
  temp_string = temp_string + " Size: " ;
  temp_string = temp_string + String(int(EEPROM.read(43)));
  temp_string = temp_string +  "\\r" ;
  temp_string = temp_string + "Speed: \\r" ;
  temp_string = temp_string + "Man: " ;
  temp_string = temp_string + String(int(EEPROM.read(41)));
  temp_string = temp_string + " Auto: " ;
  temp_string = temp_string + String(int(EEPROM.read(42)));
  temp_string = temp_string +  "\\r" ;
  temp_string = temp_string + "Delay: " ;
  temp_string = temp_string + String(int(EEPROM.read(44)));
  temp_string = temp_string +  "\\r" ;


  char buffer[temp_string.length() + 1];
  temp_string.toCharArray(buffer, temp_string.length() + 1);
  txt_view_set.setText(buffer);
  txt_view_set.setText(buffer);
  delay(100);

#ifdef DEBUG  //...................only show if in debug mode
  Serial.println("Done......");
  Serial.println(temp_string.length());
#endif

}




//................................................................
//Parse IP
//................................................................
void parse_IP(String temp_IP, int i_where) {
  // int temp=temp_IP.length();

  int Index1 = temp_IP.indexOf('.');
  int Index2 = temp_IP.indexOf('.', Index1 + 1 );
  int Index3 = temp_IP.indexOf('.', Index2 + 1 );

  String firstValue = temp_IP.substring(0,  Index1);
  String secondValue = temp_IP.substring(Index1 + 1, Index2);
  String thirdValue = temp_IP.substring(Index2 + 1, Index3);
  String fourthValue = temp_IP.substring(Index3 + 1, temp_IP.length());



  IP_[0] = firstValue.toInt();
  IP_[1] = secondValue.toInt();
  IP_[2] = thirdValue.toInt();
  IP_[3] = fourthValue.toInt();


  Serial.println("IP INT: ");
  Serial.println(IP_[0]);
  Serial.println(IP_[1]);
  Serial.println(IP_[2]);
  Serial.println(IP_[3]);


  //Save IP...............................................................
  if (i_where == 0) {
    EEPROM.write(0, IP_[0]);
    EEPROM.write(1, IP_[1]);
    EEPROM.write(2, IP_[2]);
    EEPROM.write(3, IP_[3]);
    EEPROM.commit();
    delay(100);
#ifdef DEBUG  //...................only show if in debug mode
    Serial.println("IP Saved: ");
#endif
  }

}

// ................................................................
//Save to EEPROM
//................................................................
void save_entry(int i_loc, int i_data) {

  EEPROM.write(i_loc, i_data);
  EEPROM.commit();
  delay(100);
#ifdef DEBUG  //...................only show if in debug mode
  Serial.println("EPPROM Saved: ");
#endif
}



