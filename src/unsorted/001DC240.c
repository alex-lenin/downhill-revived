/* UNIT 0x001DC240 0x001DC2D8 */
typedef struct { char pad[0x2580]; int x2580[4]; int x2590; int x2594[4]; int x25A4[4]; int x25B4[4]; int x25C4; } Obj;
extern void func_001DC8D8(void *);
void func_001DC240(Obj *obj) {
 char *cur=(char *)obj; char *end=(char *)obj+0x2580;
 int *a,*b,*c,*d; int count;
 do { func_001DC8D8(cur);cur+=0x60; } while((int)cur<(int)end);
 d=obj->x2594;c=obj->x25B4;b=obj->x25A4;a=obj->x2580;count=3;
 do { *a=0;count--;*b=0;a++;*c=0;b++;*d=0;c++;d++; }while(count>=0);
 obj->x2590=0;obj->x25C4=0;
}
