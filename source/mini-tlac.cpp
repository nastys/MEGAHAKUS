#include <dmntcht.h>
#include "mini-tlac.h"

bool MINITLAC_injectGameSubState(unsigned long baseaddr, unsigned int state, unsigned int substate)
{
    const unsigned int multiplier=0x20;

    unsigned short submov=static_cast<unsigned short>(multiplier*substate);
    unsigned char submovlt=(submov & 0xFF)+0x13;
    unsigned char submovlb=(submov & 0xFF)+0x1;
    unsigned char submovr=(submov >> 8) & 0xFF;

    unsigned short gmmov=static_cast<unsigned short>(multiplier*state);
    unsigned char gmmovl=(gmmov & 0xFF);
    unsigned char gmmovr=(gmmov >> 8) & 0xFF;


    const unsigned char buffer[]=STATE_SWITCH_PATCHED;
    return dmntchtWriteCheatProcessMemory(baseaddr+STATE_SWITCH_OFFSET, &buffer, sizeof(buffer));
}

bool MINITLAC_restoreGameSubState(unsigned long baseaddr)
{
    const unsigned char buffer[]=STATE_SWITCH_ORIGINAL;
    return dmntchtWriteCheatProcessMemory(baseaddr+STATE_SWITCH_OFFSET, &buffer, sizeof(buffer));
}
