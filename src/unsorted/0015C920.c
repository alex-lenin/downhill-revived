/* UNIT 0x0015C920 0x0015C990 */
struct State { char pad[0x35C]; char x35C; char pad35D[0x17]; char x374[1]; };
struct Obj { char pad[0x44]; char x44; char pad45[4]; char x49; };
extern struct State D_004B25A0;
extern struct Obj D_0043F9F0;
extern void func_001B8B10(struct State *);
extern void func_001A8CE8(struct Obj *, int);
extern void func_001C5B80(void *);
extern void func_001C6628(void *);
void func_0015C920(void) {
 struct Obj *obj=&D_0043F9F0;
 struct State *state=&D_004B25A0;
 void *p;
 state->x35C=1; obj->x44=1;
 p=state->x374;
 obj->x49=0;
 func_001B8B10(state);
 func_001A8CE8(obj,1);
 func_001C5B80(p); func_001C6628(p);
}
