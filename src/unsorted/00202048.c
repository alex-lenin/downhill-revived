/* UNIT 0x00202048 0x00202348 -fno-opt-stack */
#include "sound.h"
extern int D_0027946C[],D_00279428[],D_002794A0[],D_00279468[],D_00279450[];
extern int *D_00279448[];
extern SoundCommandReturn *D_00279458[];
extern char D_004C8980[],D_004C8940[],D_00297860[],D_00297BB8[],D_00297C18[];
extern void func_002016E8(void *,int);
extern int sceSifCheckStatRpc(void *);
extern void func_00254D08(char *,...);
extern int func_002013F8(void);
extern void func_0025A960(int);
extern void func_0025CF88(void *,int,int,void *,int,void *,int,void *,int);
extern int func_00202348(void);
void func_00202048(int command,int length,void *data,SoundCompletionCallback value,SoundCallbackData extra) {
 int restored=0;
 int size,waits,i;
 short *out;
 if(!D_0027946C[0] && !D_00279428[0] && !length && !value) {
  func_002016E8(D_004C8980,1);
  while(sceSifCheckStatRpc(D_004C8940)) {
   if(!D_002794A0[0]) func_00254D08(D_00297860);
   func_002013F8();
   func_0025A960(0);
  }
  func_0025CF88(D_004C8940,command,1,0,0,D_004C8980,12,0,0);
  return;
 }
 waits=0;
 size=length+4;
 if(size&3) size+=4-size%4;
 while(*D_00279448[D_00279468[0]]==256 || D_00279450[D_00279468[0]]<size) {
  if(D_0027946C[0]) { D_0027946C[0]=0; restored=1; }
  func_002013F8();
  if(1==waits && !D_002794A0[0]) func_00254D08(D_00297BB8,D_00279468[0],*D_00279448[D_00279468[0]]);
  waits++;
 }
 if(waits && !D_002794A0[0]) func_00254D08(D_00297C18,waits);
 if(restored) D_0027946C[0]=1;
 out=(short *)((char *)D_00279448[D_00279468[0]]-(D_00279450[D_00279468[0]]-4096));
 *out++=command;
 *out++=length;
 for(i=0;i<length;i++) ((unsigned char *)out)[i]=((unsigned char *)data)[i];
 D_00279450[D_00279468[0]]-=size;
 { SoundCommandReturn **commands=&D_00279458[D_00279468[0]];
 int **counts=&D_00279448[D_00279468[0]];
 (*commands)[**counts].callback=value;
 (*commands)[**counts].data=extra; }
 func_00202348();
}
