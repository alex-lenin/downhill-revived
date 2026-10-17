/* UNIT 0x0016D350 0x0016D488 */
struct Obj { char pad[0x6B0]; unsigned long **x6B0; char pad6B4[10]; unsigned char x6BE,x6BF; };
extern void func_0016D488(struct Obj *, void *);
extern unsigned char func_0016E678(struct Obj *, void *);
extern void func_0016EE40(struct Obj *);
void func_0016D350(struct Obj *obj) {
 unsigned long **p; int count; unsigned long mask, bit;
 mask=0xFDFFFFFFFFFFFFFFUL;
 p=obj->x6B0;
 for(count=9;count>=0;count--,p++) { if(*p) **p &= mask; }
 if(obj->x6BE==0) { void *first=(char *)obj+0x250; func_0016D488(obj,first); obj->x6BE=func_0016E678(obj,first); }
 if(obj->x6BF==0) { void *second=(char *)obj+0x480; func_0016D488(obj,second); obj->x6BF=func_0016E678(obj,second); }
 else if(obj->x6BE) func_0016EE40(obj);
 bit=0x0200000000000000UL; mask=0xFDFFFFFFFFFFFFFFUL;
 p=obj->x6B0;
 for(count=9;count>=0;count--,p++) { if(*p) **p = (**p & mask) | bit; }
}
