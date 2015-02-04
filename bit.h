#ifndef BIT_H_
#define BIT_H_

#define BIT(x) (0x01 << (x))
#define readBit(REGISTER, BT) ((REGISTER) & BIT(BT))
#define setBit(REGISTER, BT) REGISTER |= (1<<BT)
#define toggleBit(REGISTER, BT) REGISTER ^= (1<<BT)
#define unsetBit(REGISTER, BT) REGISTER &= ~(1<<BT)

#endif
