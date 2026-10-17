/* UNIT 0x00220768 0x002208E8 */
extern unsigned char D_0029E2A8;
struct Input { unsigned char pad[8]; unsigned char x8[24]; };
extern struct Input D_00660B80[];
int func_00220768(int button,int port) {
 int index=port;
 unsigned char result=0;
 if(!((unsigned char *)&D_0029E2A8)[index]) goto done;
 {
 switch(button) {
 case 0x400:button=8;break;
 case 0x100:button=10;break;
 case 0x800:button=9;break;
 case 0x200:button=11;break;
 case 0x4000:button=6;break;
 case 0x8000:button=7;break;
 case 0x2000:button=5;break;
 case 0x1000:button=4;break;
 case 0x80:button=1;break;
 case 0x20:button=0;break;
 case 0x10:button=2;break;
 case 0x40:button=3;break;
 default:button=-1;break;
 }
 if(button>=0) {
 result=D_00660B80[index].x8[button];
 if(__builtin_fabsf((float)result)<2.0f) result=0;
 }
 }
 done:
 return result;
}
