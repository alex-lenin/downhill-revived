/* UNIT 0x001C2880 0x001C2A90 .rodata=0x28FBB0:0x28FC10 */
typedef struct Obj { char pad0[0xC0]; float xC0,xC4,xC8; int xCC,xD0,xD4,xD8,xDC,xE0,xE4,xE8,xEC,xF0; float xF4; void *xF8; } Obj;
extern void func_001C3178(void *, void *, int, void *);         /* extern */
extern char D_00270980;
extern char D_00270A20;
extern char D_00270AC0;
extern char D_00270B60;
extern char D_00270C00;
extern char D_00270CA0;
extern char D_00270D40;
extern char D_00270DE0;
extern char D_00270E80;
extern char D_00270F20;
extern char D_00270FC0;
extern char D_00271060;
extern char D_00271100;
extern char D_002711A0;
extern char D_00271240;
extern char D_002712E0;
extern char D_00271380;
extern char D_00271420;
extern char D_002714C0;
extern char D_00271560[];
extern int D_004B2840[];

void func_001C2880(Obj *arg0) {
    arg0->xD0 = 0xF0;
    arg0->xD4 = 0;
    arg0->xF0 = 0;
    switch (D_004B2840[0]) {
    case 6:
        arg0->xF8 = &D_00270980;
        break;
    case 7:
        arg0->xF8 = &D_00270A20;
        break;
    case 8:
        arg0->xF8 = &D_00270AC0;
        break;
    case 9:
        arg0->xF8 = &D_00270B60;
        break;
    case 14:
        arg0->xF8 = &D_00270C00;
        break;
    case 15:
        arg0->xF8 = &D_00270CA0;
        break;
    case 17:
        arg0->xF8 = &D_00270D40;
        break;
    case 25:
        arg0->xF8 = &D_00270DE0;
        break;
    case 24:
        arg0->xF8 = &D_00270E80;
        break;
    case 16:
        arg0->xF8 = &D_00270F20;
        break;
    case 22:
        arg0->xF8 = &D_00270FC0;
        break;
    case 10:
        arg0->xF8 = &D_00271060;
        break;
    case 13:
        arg0->xF8 = &D_00271100;
        break;
    case 18:
        arg0->xF8 = &D_002711A0;
        break;
    case 11:
        arg0->xF8 = &D_00271240;
        break;
    case 5:
        arg0->xF8 = &D_002712E0;
        break;
    case 2:
        arg0->xF8 = &D_00271380;
        break;
    case 3:
        arg0->xF8 = &D_00271420;
        break;
    case 12:
        arg0->xF8 = &D_002714C0;
        break;
    case 20:
        arg0->xF8 = &D_00271560;
        break;
    default:
        arg0->xF8 = 0;
        break;
    }
    func_001C3178(arg0, arg0, 0, &arg0->xE4);
    arg0->xEC = 0xBE;
    arg0->xF4 = 0.055f;
    arg0->xC0 = -3.8f;
    arg0->xC4 = 3.0f;
    arg0->xC8 = 1.11f;
    arg0->xD8 = 0;
    arg0->xDC = 0;
    arg0->xE0 = 0;
    arg0->xCC = 0;
    arg0->xE8 = 0;
}
