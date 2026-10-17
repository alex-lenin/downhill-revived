/* UNIT 0x0018FEC0 0x0018FF10 */
typedef struct Entry { int x4, x8; char xC; char pad[3]; int x10; } Entry;
typedef struct Obj { int x0; Entry entries[5]; int count; } Obj;
void func_0018FEC0(Obj *p, int a, int b, int c, char d) {
    p->entries[p->count].x10 = a;
    p->entries[p->count].x4 = b;
    p->entries[p->count].x8 = c;
    p->entries[p->count].xC = d;
    p->count++;
}
