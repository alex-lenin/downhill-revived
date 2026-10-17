/* UNIT 0x001669D8 0x00166B30 */
typedef struct Anim {
    char pad0[0x64];
} Anim;

typedef struct Obj {
    char pad0[0x120];
    int x120;
    char pad124[0x1B8 - 0x124];
    Anim anims[6]; /* 0x1B8 */
} Obj;

extern void func_001AC810(Anim *anim, float value);
extern void func_001ACB70(Anim *anim, float value);
extern void func_001ACAE0(Anim *anim, float value);

void func_001669D8(Obj *obj, float value)
{
    func_001AC810(&obj->anims[0], value);
    func_001ACB70(&obj->anims[0], 0.0f);
    func_001ACAE0(&obj->anims[0], 0.5f);
    func_001AC810(&obj->anims[2], value);
    func_001ACB70(&obj->anims[2], 0.0f);
    func_001ACAE0(&obj->anims[2], 0.5f);
    if (obj->x120 == 0) {
        func_001AC810(&obj->anims[1], value);
        func_001ACB70(&obj->anims[1], 0.0f);
        func_001ACAE0(&obj->anims[1], 0.5f);
        func_001AC810(&obj->anims[3], value);
        func_001ACB70(&obj->anims[3], 0.0f);
        func_001ACAE0(&obj->anims[3], 0.5f);
        func_001AC810(&obj->anims[4], value);
        func_001ACB70(&obj->anims[4], 0.0f);
        func_001ACAE0(&obj->anims[4], 0.5f);
        func_001AC810(&obj->anims[5], value);
        func_001ACB70(&obj->anims[5], 0.0f);
        func_001ACAE0(&obj->anims[5], 0.5f);
    }
}
