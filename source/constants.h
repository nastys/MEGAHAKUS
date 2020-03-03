#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
#include "structs.h"

// "Disable NPR" 1.0.3
// Redirects add_pararm to adp_force
#define ADD_PARAM_OFFSET 0x99BA41
const std::string add_param_redirect="adp_force";
const std::string add_param_original="add_param";

// "Debug mode" 1.0.3
// Thanks to samyuu for the original (1.0.2) addresses
#define SHOW_SPRITES_OFFSET 0x1CDD58
const unsigned char show_sprites_enabled[]={0xF3, 0x03, 0x00, 0x2A};
const unsigned char show_sprites_disabled[]={0xE0, 0x07, 0x00, 0x32};
#define DW_GUI_UPDATE_OFFSET 0x2DAB2F
#define DW_GUI_DRAW_OFFSET 0x2DAB47
const unsigned char dw_gui_enabled=0x37;
const unsigned char dw_gui_disabled=0x36;

#endif // CONSTANTS_H
