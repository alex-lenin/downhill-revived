/* UNIT 0x0015A890 0x0015A968 */
typedef struct Obj { char pad[8]; char *x8; char padC[0x3C]; char x48[16]; } Obj;
extern char D_002AF2C0[];
extern int func_0011C168(void *,void *,float *,float);
extern void func_00204358(void *,int);
void func_0015A890(Obj *obj) {
 float distance; void *sound;
 if(func_0011C168(D_002AF2C0,obj->x8+0x10,&distance,400.0f)) {
  sound=obj->x48;
  if(distance<40.0f) {func_00204358(sound,0);return;}
  if(distance<80.0f) {func_00204358(sound,1);return;}
  if(distance<160.0f) {func_00204358(sound,2);return;}
  func_00204358(sound,3);
 }
}
