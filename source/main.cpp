#define TESLA_INIT_IMPL // If you have more than one file using the tesla header, only define this in the main one
#include <tesla.hpp>    // The Tesla Header
#include <dmntcht.h>
#include <sstream>
#include "constants.h"

static DmntCheatProcessMetadata metadata;
static bool initialized=false;

bool debugService_isRunning()
{
    Handle handle;
    SmServiceName service=smEncodeName("dmnt:cht");
    bool isRunning=R_FAILED(smRegisterService(&handle, service, false, 1));
    svcCloseHandle(handle);
    if(!isRunning) smUnregisterService(service);
    return isRunning;
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
    return;
}

class M39OLGUI : public tsl::Gui {
public:
    M39OLGUI(u8 arg1, u8 arg2, bool arg3) {}

    // Called when this Gui gets loaded to create the UI
    // Allocate all your elements on the heap. libtesla will make sure to clean them up when not needed anymore
    virtual tsl::elm::Element* createUI() override {
        auto rootFrame = new tsl::elm::OverlayFrame("MEGAHAKUS 1.0.0", "For MEGA39's 1.0.3");
        auto list = new tsl::elm::List();
        if (initialized&&debugService_isRunning()&&metadata.title_id==0x100F3100DA46000)
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

            // "Debug mode"
            // Get current state
            unsigned char dwguibuffer[1];
            if(!dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + DW_GUI_DRAW_OFFSET, dwguibuffer, 1))
            {
                // Create item
                auto *debug_mode = new tsl::elm::ToggleListItem("Debug mode ", dwguibuffer[0]==dw_gui_enabled);

                // Set listener function
                debug_mode->setStateChangedListener(debug_mode_toggle);

                // Add item
                list->addItem(debug_mode);
            }
        }
        else list->addItem(new tsl::elm::ListItem("Please start the game."));

        rootFrame->setContent(list);

        return rootFrame;
    }

    // Called once every frame to update values
    virtual void update() override {

    }

    // Called once every frame to handle inputs not handled by other UI elements
    virtual bool handleInput(u64 keysDown, u64 keysHeld, touchPosition touchInput, JoystickPosition leftJoyStick, JoystickPosition rightJoyStick) override {
        return false;   // Return true here to singal the inputs have been consumed
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
    }  // Callet at the end to clean up all services previously initialized

    virtual void onShow() override {}    // Called before overlay wants to change from invisible to visible state
    virtual void onHide() override {}    // Called before overlay wants to change from visible to invisible state

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
        return initially<M39OLGUI>(1, 2, true);  // Initial Gui to load. It's possible to pass arguments to its constructor like this
    }
};

int main(int argc, char **argv) {
    return tsl::loop<M39OL>(argc, argv);
}

