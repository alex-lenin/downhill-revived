/* UNIT 0x001168A0 0x001169F8 */
struct Base { char pad[0x796c]; int x796C; };
struct Obj { char pad[0x58]; struct Base *x58; char p5c[12]; float x68; };
void func_001168A0(struct Obj *obj) {
 int value;
 obj->x68=0.0f;
 value=obj->x58->x796C;
 if(value==0x100) goto twelve;
 if(value<0x101) {
  if(value==0x80) goto ten;
  if(value<0x81) {
   if(value==0x40) goto fifteen;
   if(value<0x41) { if(value==0x20) goto ten; else goto fallback; }
   else { if(value==0x60) goto ten; else goto fallback; }
  } else {
   if(value==0xc0) goto fifteen;
   if(value<0xc1) { if(value==0xa0) goto seven; else goto fallback; }
   else { if(value==0xe0) goto twelve; else goto fallback; }
  }
 } else {
  if(value==0x180) goto twelve;
  if(value<0x181) {
   if(value==0x140) goto twelve;
   if(value<0x141) { if(value==0x120) goto ten; else goto fallback; }
   else { if(value==0x160) goto seven; else goto fallback; }
  } else {
   if(value==0x1c0) goto fallback;
   if(value<0x1c1) { if(value==0x1a0) goto twelve; else goto fallback; }
   else goto fallback;
  }
 }
 ten: obj->x68=10.0f; return;
 fifteen: obj->x68=15.0f; return;
 twelve: obj->x68=12.0f; return;
 seven: obj->x68=7.0f; return;
 fallback: obj->x68=2.0f;
}
