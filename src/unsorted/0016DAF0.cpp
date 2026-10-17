/* UNIT 0x0016DAF0 0x0016DD40 */
struct Buffer;
typedef struct Value { int x[4]; } Value;
typedef struct Buffer { Value x0; Value values[1]; } Buffer;
typedef struct Actor { char pad0[8]; int x8; char padC[0x34]; int x40[60]; struct Buffer *x130; } Actor;
typedef struct Obj { char pad0[0x208]; char *x208; char pad20C[0x4A8]; int x6B4, x6B8; char pad6BC[4]; char x6C0; } Obj;
extern "C" void func_00126B20(Actor *,int), func_00203DC8(Value *,int), func_002038E0(Value *,int);
extern "C" float func_00203C28(Value *), func_001385A8(void *);
extern "C" int func_00204230(Value *), func_00222BD8(int,int), func_0016E998(Obj *,Actor *);
static inline Value *get_value(Actor *actor, int idx) { int offset = actor->x40[idx] * 16 + 16; return (Value *)((char *)actor->x130 + offset); }
static inline int test_done(Value *a, Value *b, Value *value) { return func_00203C28(a) >= 1.0f || (*b = *value, !func_00204230(b)); }
extern "C" int func_0016DAF0(Obj *obj, Actor *actor) {
    int result = 0;
    Value *value;
    Value temp0, temp1, temp2;
    int count;
    int cmd;
    unsigned char done;
    if (actor) {
        cmd = actor->x8;
        value = get_value(actor, cmd);
        switch (cmd) {
        case 1:
            func_00126B20(actor,7);
            cmd = actor->x8;
            temp0 = *get_value(actor, cmd);
            func_00203DC8(&temp0,0);
            break;
        case 7:
            temp0 = *value;
            done = test_done(&temp0,&temp1,value);
            if (done) {
                obj->x6B8++;
                if (obj->x6B8 >= 16) {
                    obj->x6B8 = 0;
                    obj->x6B4++;
                    count = 6;
                    if (obj->x208) count = (int)(func_001385A8(obj->x208 + 0x40C0) * 6.0f);
                    count += func_00222BD8(1,2);
                    if (obj->x6B4 < count) {
                        temp2 = *value;
                        func_002038E0(&temp2,1);
                    } else {
                        obj->x6B4 = 0;
                        result = 1;
                        func_00126B20(actor,1);
                    }
                }
            }
            break;
        default:
            if (!func_0016E998(obj,actor)) obj->x6C0 = 1;
            break;
        }
    }
    return result;
}
