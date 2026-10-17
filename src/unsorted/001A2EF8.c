/* UNIT 0x001A2EF8 0x001A30B8 */
#include "format.h"
#include "soft_double.h"
typedef struct { char pad[0x796C]; int x796C; char pad7970[0xEC]; int x7A5C; } Rider;
typedef struct { Rider *x0; int x4; float x8; int xC,x10; } Entry;
extern char D_0029D2F8[],D_0029D318[];
extern char *func_00158668(int);

extern void func_0020F518(int,int,int,int,int);
extern void func_0020F9A8(int,int,int,int,char *);
extern void func_0020FB80(int,int,int,int,int);

extern int func_00254D68(char *,const char *,...);
void func_001A2EF8(int unused,Entry *entry,int alpha,int y,int font,int index) {
 char buffer[32];
 double value;
 char *format;
 func_0020FB80(0,256,192,-91,-32);
 if(entry->x0->x7A5C==1) func_0020F518(0,88,152,73,alpha);
 else func_0020F518(0,80,100,80,alpha);
 func_00254D68(buffer,D_0029D2F8,index);
 func_0020F9A8(0,font,30,y,buffer);
 func_0020F9A8(0,font,70,y,func_00158668((entry->x0->x796C>>5)-1));
 FormatHmsDiscard(entry->x4, (char *)(buffer));
 func_0020F9A8(0,font,148,y,buffer);
 format=D_0029D318;
 value=func_001FD300(entry->x8);
 func_00254D68(buffer,format,value);
 func_0020F9A8(0,font,250,y,buffer);
 func_00254D68(buffer,D_0029D2F8,entry->x10);
 func_0020F9A8(0,font,340,y,buffer);
 func_00254D68(buffer,D_0029D2F8,entry->xC);
 func_0020F9A8(0,font,414,y,buffer);
}
