/* UNIT 0x0021E920 0x0021E9A8 */
typedef unsigned int U128 __attribute__((mode(TI)));
typedef union { U128 wide; unsigned long halves[2]; unsigned int words[4]; } Packet;
int func_0021E920(Packet *p,int x,int y,int w,int h) { p->halves[0]=((unsigned long)0xC00D<<46)|0x8001; p->halves[1]=0x551; x+=0x7E0; ++p; p->words[0]=0; p->words[1]=0; p->words[2]=0; p->words[3]=0; y+=0x7E0; ++p; p->wide=0; p->words[0]=x<<4; p->words[1]=y<<4; p->words[2]=0xFFFF28; ++p; p->wide=0; p->words[0]=(x+w)<<4; p->words[1]=(y+h)<<4; p->words[2]=0xFFFF28; return 4; }
