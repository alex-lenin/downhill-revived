/* UNIT 0x0013D608 0x0013D6F8 */
struct Anim {
    unsigned int x0;
    char pad4[0xB - 0x4];
    unsigned char xB;
};

struct Owner;

class Part {
public:
    virtual void Update(Owner *owner);
};

struct Owner {
    char pad0[0x51C];
    Anim *anims[10];
    Part *parts[10];
};

extern int D_0029E4A8;
extern "C" void func_00218498(Anim *anim, int frame);

extern "C" void func_0013D608(Owner *owner, int index, int frame, int last)
{
    int i;
    Anim *anim;
    Part *part;
    Part **slot;
    Part **base;

    if (last + 1 != 0) {
    base = owner->parts;
    slot = (Part **)((index << 3) + (int)base);
    i = -~last;
    do {
        anim = ((Anim **)slot)[-10];
        if (anim != 0) {
            if ((anim->x0 & 0xFFFC0000) == 0x96340000) {
                func_00218498(anim, frame);
            } else {
                func_00218498(anim, (frame + D_0029E4A8) % anim->xB);
            }
        }
        part = *slot;
        if (part != 0) {
            part->Update(owner);
        }
        ++slot;
    } while (--i != 0);
    }
}
