/* UNIT 0x0017C960 0x0017C9B8 */
extern void func_0017CB10(void *);
typedef struct { char entries[0x580]; int x580; } Obj;
void func_0017C960(Obj *obj) {
 char *ptr=(char *)obj;
 do {func_0017CB10(ptr); ptr+=0x2C;} while((int)ptr<(int)obj+0x580);
 obj->x580=0;
}
