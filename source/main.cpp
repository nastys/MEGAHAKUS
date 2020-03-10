#define TESLA_INIT_IMPL // If you have more than one file using the tesla header, only define this in the main one
#include <tesla.hpp>    // The Tesla Header
#include <dmntcht.h>
#include <sstream>
#include "constants.h"

static DmntCheatProcessMetadata metadata;
static bool initialized=false;
static tsl::elm::ListItem *vp_itm;
static tsl::elm::ToggleListItem *poll_disable;
static bool cursor_boundaries=true;
static bool debug_mode_enabled=false;

bool VPincrease(u64 button)
{
    if(button==KEY_A||button==KEY_RIGHT||button==KEY_LSTICK_RIGHT||button==KEY_RSTICK_RIGHT||button==KEY_X||button==KEY_LEFT||button==KEY_LSTICK_LEFT||button==KEY_RSTICK_LEFT||button==KEY_MINUS||button==KEY_PLUS)
    {
        unsigned int vpnum;
        if(!dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + VP_AMOUNT_OFFSET_MAIN, &vpnum, sizeof(vpnum)))
        {
            if(button==KEY_X||button==KEY_LEFT||button==KEY_LSTICK_LEFT||button==KEY_RSTICK_LEFT)
            {
                vpnum-=1000;
                if(vpnum>9999999) vpnum = 0;
            }
            else if(button==KEY_MINUS)
                vpnum = 0;
            else if(button==KEY_PLUS)
                vpnum = 9999999;
            else
            {
                vpnum+=1000;
                if(vpnum>9999999) vpnum = 9999999;
            }
            dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + VP_AMOUNT_OFFSET_MAIN, &vpnum, sizeof(vpnum));
            vp_itm->setText("VP: "+std::to_string(vpnum));
        }
        return true;
    }
    return false;
}

bool debugService_isRunning()
{
    Handle handle;
    SmServiceName service=smEncodeName("dmnt:cht");
    bool isRunning=R_FAILED(smRegisterService(&handle, service, false, 1));
    svcCloseHandle(handle);
    if(!isRunning) smUnregisterService(service);
    return isRunning;
}

void inputpoll_toggle(bool state)
{
    unsigned char buf[4];
    if(state)
    {
        buf[0]=input_poll_disabled[0];
        buf[1]=input_poll_disabled[1];
        buf[2]=input_poll_disabled[2];
        buf[3]=input_poll_disabled[3];
    }
    else
    {
        buf[0]=input_poll_enabled[0];
        buf[1]=input_poll_enabled[1];
        buf[2]=input_poll_enabled[2];
        buf[3]=input_poll_enabled[3];
    }
    dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + INPUT_POLL_OFFSET, &buf, sizeof(buf));
    return;
}

void recording_toggle(bool state)
{
    unsigned char buf[4];
    if(state)
    {
        buf[0]=recording_enabled[0];
        buf[1]=recording_enabled[1];
        buf[2]=recording_enabled[2];
        buf[3]=recording_enabled[3];
    }
    else
    {
        buf[0]=recording_disabled[0];
        buf[1]=recording_disabled[1];
        buf[2]=recording_disabled[2];
        buf[3]=recording_disabled[3];
    }
    dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + ENABLE_RECORDING_OFFSET, &buf, sizeof(buf));
    return;
}

void disable_npr(bool state)
{
    char buffer[9];
    if(state) strcpy(buffer, add_param_redirect.c_str());
    else strcpy(buffer, add_param_original.c_str());
    dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + ADD_PARAM_OFFSET, buffer, sizeof(buffer));
    return;
}

void debug_mode_toggle(bool state)
{
    unsigned char buf1[1];
    unsigned char buf2[4];
    if(state)
    {
        buf1[0]=dw_gui_enabled;
        buf2[0]=show_sprites_enabled[0];
        buf2[1]=show_sprites_enabled[1];
        buf2[2]=show_sprites_enabled[2];
        buf2[3]=show_sprites_enabled[3];
    }
    else
    {
        buf1[0]=dw_gui_disabled;
        buf2[0]=show_sprites_disabled[0];
        buf2[1]=show_sprites_disabled[1];
        buf2[2]=show_sprites_disabled[2];
        buf2[3]=show_sprites_disabled[3];
    }
    dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + DW_GUI_UPDATE_OFFSET, buf1, sizeof(buf1));
    dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + DW_GUI_DRAW_OFFSET, buf1, sizeof(buf1));
    dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + SHOW_SPRITES_OFFSET, buf2, sizeof(buf2));

    poll_disable->setState(state);
    inputpoll_toggle(state);
    debug_mode_enabled=state;
    return;
}

/*void cur_map_toggle(bool state)
{
    unsigned char buf;
    if(state)
        buf=mouse_map_touchscreen;
    else
        buf=mouse_map_default;
    dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + MOUSE_MAP_OFFSET, &buf, sizeof(buf));
    return;
}

void cur_map_disable(bool state)
{
    unsigned char buf[4];
    if(state)
    {
        buf[0]=mouse_map_disabled[0];
        buf[1]=mouse_map_disabled[1];
        buf[2]=mouse_map_disabled[2];
        buf[3]=mouse_map_disabled[3];
    }
    else
    {
        buf[0]=mouse_map_enabled[0];
        buf[1]=mouse_map_enabled[1];
        buf[2]=mouse_map_enabled[2];
        buf[3]=mouse_map_enabled[3];
    }
    dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + MOUSE_PREMAP_OFFSET, &buf, sizeof(buf));
    return;
}*/

void cursor_boundaries_toggle(bool state)
{
    cursor_boundaries=state;
    return;
}

bool bid_match()
{
    const unsigned char build_id_size=20;
    const unsigned char expected_build_id[build_id_size]={0xFA, 0xE8, 0xAA, 0x77, 0x8E, 0x4C, 0xE6, 0x12, 0xCE, 0x4E, 0x76, 0x55, 0x73, 0x9B, 0xB9, 0x4F, 0xF9, 0xD2, 0xCB, 0x11};
    for(unsigned char i=0; i<build_id_size; i++)
        if(metadata.main_nso_build_id[i]!=expected_build_id[i])
            return false;
    return true;
}

class M39OLGUI : public tsl::Gui {
public:
    M39OLGUI(u8 arg1, u8 arg2, bool arg3) {}

    // Called when this Gui gets loaded to create the UI
    // Allocate all your elements on the heap. libtesla will make sure to clean them up when not needed anymore
    virtual tsl::elm::Element* createUI() override {
        auto rootFrame = new tsl::elm::OverlayFrame("MEGAHAKUS 2.0.0", "For MEGA39's 1.0.3");
        auto list = new tsl::elm::List();
        if (initialized&&debugService_isRunning()&&metadata.title_id==0x100F3100DA46000&&bid_match())
        {
            // "Disable NPR"
            // Get current state
            char adpbuffer[10];
            if(!dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + ADD_PARAM_OFFSET, adpbuffer, 9))
            {
                adpbuffer[9]='\0';

                // Create item
                auto *no_npr = new tsl::elm::ToggleListItem("Disable NPR", adpbuffer==add_param_redirect);

                // Set listener function
                no_npr->setStateChangedListener(disable_npr);

                // Add item
                list->addItem(no_npr);
            }

            // "VP"
            // Get current amount
            unsigned int vpnum;
            if(!dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + VP_AMOUNT_OFFSET_MAIN, &vpnum, sizeof(vpnum)))
            {
                // Create item
                vp_itm = new tsl::elm::ListItem("VP: "+std::to_string(vpnum));

                // Set listener function
                vp_itm->setClickListener(VPincrease);

                // Add item
                list->addItem(vp_itm);
            }

            // "Debug mode"
            // Get current state
            unsigned char dwguibuffer[1];
            if(!dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + DW_GUI_DRAW_OFFSET, dwguibuffer, 1))
                debug_mode_enabled=dwguibuffer[0]==dw_gui_enabled;

            // Create item
            auto *debug_mode = new tsl::elm::ToggleListItem("Debug mode", debug_mode_enabled);

            // Set listener function
            debug_mode->setStateChangedListener(debug_mode_toggle);

            // Add item
            list->addItem(debug_mode);


            // "Cursor to touchscreen"
            // Get current state
            /*unsigned char cursorbuffer;
            if(!dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + MOUSE_MAP_OFFSET, &cursorbuffer, 1))
            {
                // Create item
                auto *cur_map = new tsl::elm::ToggleListItem("Cursor to touch screen", cursorbuffer==mouse_map_touchscreen);

                // Set listener function
                cur_map->setStateChangedListener(cur_map_toggle);

                // Add item
                list->addItem(cur_map);
            }*/

            // "Disable input polling"
            // Get current state
            unsigned char pollbuffer;
            if(!dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + INPUT_POLL_OFFSET, &pollbuffer, 1))
            {
                // Create item
                poll_disable = new tsl::elm::ToggleListItem("Disable input polling", pollbuffer==input_poll_disabled[0]);

                // Set listener function
                poll_disable->setStateChangedListener(inputpoll_toggle);

                // Add item
                list->addItem(poll_disable);
            }

            // "Disable mouse polling"
            // Get current state
            /*unsigned char mousebuffer;
            if(!dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + MOUSE_PREMAP_OFFSET, &mousebuffer, 1))
            {
                // Create item
                auto *mouse_disable = new tsl::elm::ToggleListItem("Disable mouse polling", mousebuffer==mouse_map_disabled[0]);

                // Set listener function
                mouse_disable->setStateChangedListener(cur_map_disable);

                // Add item
                list->addItem(mouse_disable);
            }*/

            // "Cursor boundaries"
            auto *cur_bound_itm = new tsl::elm::ToggleListItem("Cursor boundaries", cursor_boundaries);
            cur_bound_itm->setStateChangedListener(cursor_boundaries_toggle);
            list->addItem(cur_bound_itm);

            // "Enable recording"
            // Get current state
            unsigned char recbuffer[1];
            if(!dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + ENABLE_RECORDING_OFFSET, recbuffer, 1))
            {
                // Create item
                auto *enable_rec_itm = new tsl::elm::ToggleListItem("Enable recording", recbuffer[0]==recording_enabled[0]);

                // Set listener function
                enable_rec_itm->setStateChangedListener(recording_toggle);

                // Add item
                list->addItem(enable_rec_itm);
            }
        }
        else
        {
            auto *not_started = new tsl::elm::ListItem("Please start MEGA39's 1.0.3.");
            list->addItem(not_started);
        }

        rootFrame->setContent(list);
        return rootFrame;
    }

    // Called once every frame to update values
    virtual void update() override {
    }

    // Called once every frame to handle inputs not handled by other UI elements
    virtual bool handleInput(u64 keysDown, u64 keysHeld, touchPosition touchInput, JoystickPosition leftJoyStick, JoystickPosition rightJoyStick) override {
        if(debug_mode_enabled&&(leftJoyStick.dx!=0||leftJoyStick.dy!=0||rightJoyStick.dx!=0||rightJoyStick.dy!=0||keysDown&KEY_ZR||keysDown&KEY_ZL))
        {
            DivaInputState dis;
            if(!dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + INPUTSTATE_P0_OFFSET, &dis, sizeof(dis)))
            {
                if(keysHeld&KEY_L||keysHeld&KEY_R||keysHeld&KEY_LSTICK||keysHeld&KEY_RSTICK)
                {
                    if(leftJoyStick.dx>10000||rightJoyStick.dx>10000)
                    {
                        if(!cursor_boundaries||dis.MouseX<1270) dis.MouseX+=10;
                        else dis.MouseX=1279;
                    }
                    else if(leftJoyStick.dx<-10000||rightJoyStick.dx<-10000)
                    {
                        if(!cursor_boundaries||dis.MouseX>10) dis.MouseX-=10;
                        else dis.MouseX=0;
                    }
                    if(leftJoyStick.dy<-10000||rightJoyStick.dy<-10000)
                    {
                        if(!cursor_boundaries||dis.MouseY<710) dis.MouseY+=10;
                        else dis.MouseY=719;
                    }
                    else if(leftJoyStick.dy>10000||rightJoyStick.dy>10000)
                    {
                        if(!cursor_boundaries||dis.MouseY>10) dis.MouseY-=10;
                        else dis.MouseY=0;
                    }
                }
                else
                {
                    if(leftJoyStick.dx>10000||rightJoyStick.dx>10000)
                    {
                        if(!cursor_boundaries||dis.MouseX<1278) dis.MouseX+=2;
                        else dis.MouseX=1279;
                    }
                    else if(leftJoyStick.dx<-10000||rightJoyStick.dx<-10000)
                    {
                        if(!cursor_boundaries||dis.MouseX>2) dis.MouseX-=2;
                        else dis.MouseX=0;
                    }
                    if(leftJoyStick.dy<-10000||rightJoyStick.dy<-10000)
                    {
                        if(!cursor_boundaries||dis.MouseY<718) dis.MouseY+=2;
                        else dis.MouseY=719;
                    }
                    else if(leftJoyStick.dy>10000||rightJoyStick.dy>10000)
                    {
                        if(!cursor_boundaries||dis.MouseY>2) dis.MouseY-=2;
                        else dis.MouseY=0;
                    }
                }

                static bool RlastPressed=false;
                static bool RlastReleased=false;
                if(keysDown&KEY_ZR||keysHeld&KEY_ZR) for(int i=0; i<10000; i++)
                {
                    dis.SetBit(102, 0, DivaInputState::Type_Released);
                    dis.SetBit(102, 1, DivaInputState::Type_Tapped);
                    dis.SetBit(102, 1, DivaInputState::Type_Down);
                    RlastPressed=true;
                }
                else if(RlastPressed) for(int i=0; i<10000; i++)
                {
                    dis.SetBit(102, 0, DivaInputState::Type_Tapped);
                    dis.SetBit(102, 0, DivaInputState::Type_Down);
                    dis.SetBit(102, 1, DivaInputState::Type_Released);
                    RlastPressed=false;
                    RlastReleased=true;
                }
                else if(RlastReleased) for(int i=0; i<10000; i++)
                {
                    dis.SetBit(102, 0, DivaInputState::Type_Released);
                    RlastReleased=false;
                }

                static bool LlastPressed=false;
                static bool LlastReleased=false;
                if(keysDown&KEY_ZL||keysHeld&KEY_ZL) for(int i=0; i<10000; i++)
                {
                    dis.SetBit(100, 0, DivaInputState::Type_Released);
                    dis.SetBit(100, 1, DivaInputState::Type_Tapped);
                    dis.SetBit(100, 1, DivaInputState::Type_Down);
                    LlastPressed=true;
                }
                else if(LlastPressed) for(int i=0; i<10000; i++)
                {
                    dis.SetBit(100, 0, DivaInputState::Type_Tapped);
                    dis.SetBit(100, 0, DivaInputState::Type_Down);
                    dis.SetBit(100, 1, DivaInputState::Type_Released);
                    LlastPressed=false;
                    LlastReleased=true;
                }
                else if(LlastReleased) for(int i=0; i<10000; i++)
                {
                    dis.SetBit(100, 0, DivaInputState::Type_Released);
                    LlastReleased=false;
                }

                dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + INPUTSTATE_P0_OFFSET, &dis, sizeof(dis));
            }
            return true;
        }

        return false;   // Return true here to signal the inputs have been consumed
    }
};

class M39OL : public tsl::Overlay {
public:
                                             // libtesla already initialized fs, hid, pl, pmdmnt, hid:sys, set:sys and the SD card
    virtual void initServices() override
    {
        if (debugService_isRunning())
        {
            dmntchtInitialize();
            dmntchtForceOpenCheatProcess();
            dmntchtGetCheatProcessMetadata(&metadata);

            initialized=true;
        }
    }  // Called at the start to initialize all services necessary for this Overlay
    virtual void exitServices() override
    {
        dmntchtExit();
        initialized=false;
    }  // Called at the end to clean up all services previously initialized

    virtual void onShow() override {}    // Called before overlay wants to change from invisible to visible state
    virtual void onHide() override {}    // Called before overlay wants to change from visible to invisible state

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
        return initially<M39OLGUI>(1, 2, true);  // Initial Gui to load. It's possible to pass arguments to its constructor like this
    }
};

int main(int argc, char **argv) {
    return tsl::loop<M39OL>(argc, argv);
}

