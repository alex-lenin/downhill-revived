/* UNIT 0x001EACF0 0x001EAD60 */
typedef struct Obj { char pad[0xF0]; unsigned char xF0,xF1; } Obj;
extern void func_001EAF38(void *);
extern void func_001D3040(void *);
void func_001EACF0(Obj *obj) {
    char *p;
    char *end;
    char *other;
    if(obj->xF0) {
        obj->xF1=0;
        other=(char *)obj+0x100;
        p=(char *)obj;
        end=p+0xF0;
        do { func_001EAF38(p); p+=0x50; } while((int)p<(int)end);
        func_001D3040(other);
    }
}
