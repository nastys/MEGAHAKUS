#ifndef MINITLAC_H
#define MINITLAC_H

#define STATE_SWITCH_OFFSET 0x1E3994
#define STATE_SWITCH_ORIGINAL {0xF3, 0x03, 0x01, 0x2A,    0x1F, 0x30, 0x00, 0x71,    0x40, 0x08, 0x00, 0x54}
#define STATE_SWITCH_PATCHED {submovlt, submovr, 0x80, 0x52,    gmmovl, gmmovr, 0x80, 0x52,    submovlb, submovr, 0x80, 0x52}

/*#define CURRENT_STATE_OFFSET 0xB2A2C640
#define CURRENT_STATE2_OFFSET 0xB2A2C65C*/

bool MINITLAC_injectGameSubState(unsigned long baseaddr, unsigned int state, unsigned int substate);

bool MINITLAC_restoreGameSubState(unsigned long baseaddr);

#endif // MINITLAC_H
