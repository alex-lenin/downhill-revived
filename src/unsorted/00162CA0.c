/* UNIT 0x00162CA0 0x00162D18 */
typedef struct Entry { float x0; float x4,x8; float xC; int x10; unsigned char x14; char pad[3]; float x18; int x1C; } Entry;
extern Entry D_00768098[];
extern float func_00162D18(int);
void func_00162CA0(void) {
 Entry *p=D_00768098; int i=0; int index;
 do { p->x8=0.0f; p->x0=100.0f; p->x4=0.0f; p->x10=0; p->x14=0;
 p->xC=func_00162D18(i); i++; p->x18=100.0f; p->x1C=0; p++;
 } while(i<32);
}
