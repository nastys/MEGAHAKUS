#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
#include "structs.h"

#define GAME_TITLE_ID 0x100F3100DA46000

// Disable NPR
#define ADD_PARAM_OFFSET 0x99BA41
const std::string add_param_redirect="adp_force";
const std::string add_param_original="add_param";

// VP
#define VP_AMOUNT_OFFSET_MAIN 0x4D0E69F8

// Debug mode
// Thanks to samyuu for the addresses and the InputState struct
#define SHOW_SPRITES_OFFSET 0x1CDD58
const unsigned char show_sprites_enabled[]={0xF3, 0x03, 0x00, 0x2A};
const unsigned char show_sprites_disabled[]={0xE0, 0x07, 0x00, 0x32};
#define DW_GUI_UPDATE_OFFSET 0x2DAB2F
#define DW_GUI_DRAW_OFFSET 0x2DAB47
const unsigned char dw_gui_enabled=0x37;
const unsigned char dw_gui_disabled=0x36;

// InputState pointer
#define INPUTSTATE_P0_OFFSET 0xE5A000

// Disable input poll
#define INPUT_POLL_OFFSET 0x1CD968
const unsigned char input_poll_enabled[]={0x36, 0xF8, 0xFF, 0x97};
const unsigned char input_poll_disabled[]={0x1F, 0x20, 0x03, 0xD5};

// Map mouse to touchscreen
/*#define MOUSE_MAP_OFFSET 0x1CB83D
const unsigned char mouse_map_default=0xA1;
const unsigned char mouse_map_touchscreen=0xE9;

// Disable mouse polling
#define MOUSE_PREMAP_OFFSET 0x1CB834
const unsigned char mouse_map_enabled[]={0x88, 0x00, 0x00, 0x54};
const unsigned char mouse_map_disabled[]={0x04, 0x00, 0x00, 0x14};*/

// Resolution scale
// Thanks to SwigS-27 for the offset
#define RES_SCALE_OFFSET 0xB2D191BC

// Enable recording
// Using somewhatlurker's method
#define ENABLE_RECORDING_OFFSET 0x61EBA0
const unsigned char recording_enabled[]={0x1F, 0x20, 0x03, 0xD5};
const unsigned char recording_disabled[]={0x41, 0x00, 0x00, 0x36};

#endif // CONSTANTS_H
