/* UNIT 0x00206DD8 0x00206E20 */
typedef struct {
    char pad[0x2c00];
    unsigned char x2c00[128];
    unsigned char x2c80;
} Obj;
extern Obj D_004CD480;
void func_00206DD8(int a,int b) {
    int i;
    unsigned char *p;
    Obj *obj;
    if(b==65535 && a) {
        obj=&D_004CD480;
        i=127;
        p=&obj->x2c00[127];
        do {
            *p=i;
            i--;
            p--;
        } while(i>=0);
        obj->x2c80=0;
    }
}
