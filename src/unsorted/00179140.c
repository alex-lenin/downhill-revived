/* UNIT 0x00179140 0x00179338 */
typedef struct Fade { char pad[0x4c]; float x4c; } Fade;
typedef struct Panel { char pad[8]; unsigned char x8; char pad9[3]; Fade *items[1]; } Panel;
typedef struct State { char pad[0x798c]; int x798c; } State;
typedef struct Obj { char pad[0xf60]; Panel *xf60; char padf64[0x200]; State *x1164; char pad1168[0x1cc]; float x1334; int x1338, x133c; } Obj;
typedef struct Globals { char pad[0x10c]; int x10c; char pad110[0x190]; int x2a0; } Globals;
extern Globals D_004B25A0;
extern int D_0043F348[];
extern int D_004C3778[];
extern unsigned char D_0032FC60[];
typedef struct Entry { char pad[0x770c]; int x770c; } Entry;
extern Entry D_004225A0;
extern void func_00218498(Panel *, int);
void func_00179140(Obj *obj) {
    Panel *panel = obj->xf60;
    float time;
    int count;
    int two;
    if (!panel) return;
    if ((unsigned)(D_0043F348[0]-2) < 3) goto active;
    if (D_0043F348[0] != 1) return;
active: {
        time = obj->x1334 - 1.0f;
        obj->x1334 = time;
        if (time < 0.0f) {
            if (D_004B25A0.x10c == 18 && (two = 2, D_004C3778[0] != two)) {
                if (D_004C3778[0] != 1) {
                    if (((Entry *)((char *)&D_004225A0 + D_004B25A0.x2a0 * 216))->x770c == 0xfffffff) count = 1;
                    else goto choose;
                } else {
choose:
                    count = D_0032FC60[0x31764] ? two : 1;
                }
            } else count = obj->x1164->x798c;
            if (count > 0) {
                count--;
                if (count != obj->x1338) {
                    obj->x133c = count;
                    obj->x1334 = 18.0f;
                    obj->x1338 = obj->xf60->x8;
                } else {
                    obj->x1334 = -1.0f;
                    func_00218498(obj->xf60,count);
                }
            }
        } else if (time > 12.0f) {
            panel->items[obj->x1338]->x4c = (time-12.0f) * 0.16666667f;
        } else if (time > 6.0f) {
            panel->items[obj->x133c]->x4c = (12.0f-time) * 0.16666667f;
            func_00218498(obj->xf60,obj->x133c);
        } else {
            panel->items[obj->x133c]->x4c = 1.0f;
            obj->x1338 = obj->x133c;
        }
    }
}
