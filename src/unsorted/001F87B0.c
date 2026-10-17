/* UNIT 0x001F87B0 0x001F8920 */
#include "animation.h"
typedef AnimationHandle Sound;
typedef struct Obj { char pad[0x500]; int x500, x504, x508, x50C, x510; } Obj;
typedef struct Global { char pad[0x290]; int x290; } Global;
extern Global D_004B25A0;
void func_001F87B0(Obj *self)
{
    Sound first, second, third;
    self->x500 = 50;
    self->x504 = 50;
    self->x508 = -1;
    self->x50C = 60;
    first.x8 = 0;
    func_00203590(&first, 0x2588, 0, D_004B25A0.x290 / 32 + 3);
    self->x510 = (self->x510 + 1) % 4;
    func_002039B0(&first, 1);
    second.x8 = 0;
    func_00203590(&second, 0x23D7, 0, 1);
    func_002039B0(&second, 1);
    third.x8 = 0;
    func_00203590(&third, D_004B25A0.x290 + 16, 0, 250);
    func_00202E38(&third, 1, 1, 300.0f);
    func_00203590(&third, 0x25EE, 0, 250);
    func_00202E38(&third, 1, 1, 300.0f);
    func_00203590(&third, 0x384, 0, 250);
    func_00202E38(&third, 1, 1, 300.0f);
}
