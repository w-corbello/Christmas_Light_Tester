

#include <Nextion.h>
#include <NextionPage.h>
#include <NextionText.h>
#include <NextionNumber.h>
#include <NextionVariableNumeric.h>
#include <NextionVariableString.h>
#include <SoftwareSerial.h>



SoftwareSerial nextionSerial(5, 4); // RX, TX   Software serial so that UART TX  can run Renard and WS2801

Nextion nex(nextionSerial);
NextionPage main_menu(nex, 0, 0, "main_menu");                   // (nex, "pagenumber", "IDnumber", "variableName")




// page 0
// page 1
// page 2
NextionText txt_run_e131(nex, 2, 1, "txt_run_e131");
// page 3
NextionVariableNumeric v_value_3(nex, 3, 23, "v_value");
NextionVariableString v_ip_add_3(nex, 3, 24, "v_ip_add");
// page 4
NextionVariableNumeric v_value_4(nex, 4, 21, "v_value");
// page 5
// page 6
NextionVariableNumeric v_value_6(nex, 6, 20, "v_value");
// page 7
// page 8
NextionVariableNumeric v_value_8(nex, 8, 24, "v_value");
// page 9
// page 10
NextionVariableNumeric v_value_10(nex, 10, 11, "v_value");
//page 12
NextionText txt_view_set(nex, 12, 2, "txt_view_set");
//page 13
//page 14
NextionText txt_sacn_1(nex, 14, 2, "txt_sacn_1");
NextionText txt_sacn_2(nex, 14, 3, "txt_sacn_2");
NextionText txt_sacn_3(nex, 14, 4, "txt_sacn_3");
NextionText txt_sacn_4(nex, 14, 5, "txt_sacn_4");
NextionText txt_sacn_5(nex, 14, 6, "txt_sacn_5");
NextionText txt_sacn_6(nex, 14, 7, "txt_sacn_6");
NextionText txt_sacn_7(nex, 14, 8, "txt_sacn_7");
NextionText txt_sacn_8(nex, 14, 9, "txt_sacn_8");
NextionNumber num_universe(nex, 14, 14, "num_universe");
