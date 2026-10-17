/* UNIT 0x0016DD40 0x0016DE90 */
typedef struct Obj {
 char pad0[0x250]; char x250[0x230]; char x480[0x230];
 unsigned long **x6B0; char pad6B4[10]; unsigned char x6BE,x6BF;
} Obj;
extern void func_0016D488(Obj *,void *);
extern int func_0016E508(Obj *,void *,float);
extern void func_0016EE40(Obj *);
void func_0016DD40(Obj *obj) {
 unsigned long **p; unsigned long *flags; int count; void *other; unsigned long mask, bit;
 mask=0xFDFFFFFFFFFFFFFFUL;
 p=obj->x6B0;
 for(count=9;count>=0;count--) { flags=*p++; if(flags) *flags &= mask; }
 other=obj->x250;
 func_0016D488(obj,other);
 if(!obj->x6BE) obj->x6BE=func_0016E508(obj,other,-1.5f);
 other=obj->x480;
 func_0016D488(obj,other);
 if(!obj->x6BF) obj->x6BF=func_0016E508(obj,other,1.5f);
 if(obj->x6BE && obj->x6BF) func_0016EE40(obj);
 bit=0x0200000000000000UL;
 mask=0xFDFFFFFFFFFFFFFFUL;
 p=obj->x6B0;
 for(count=9;count>=0;count--) { flags=*p++; if(flags) *flags=(*flags & mask)|bit; }
}
