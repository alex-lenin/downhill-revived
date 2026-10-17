/* UNIT 0x0018DFA8 0x0018DFC8 */
typedef struct Obj {
    char pad0[0x4CC];
    int x4CC[10];
    int count;
} Obj;

void func_0018DFA8(Obj *obj, int value)
{
    obj->x4CC[obj->count] = value;
    obj->count++;
}
