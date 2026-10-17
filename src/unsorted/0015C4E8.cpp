/* UNIT 0x0015C4E8 0x0015C600 */
struct Game;

class Input {
public:
    Game *x0;
    virtual void v0();
    virtual void v1();
    virtual int IsPressed();
};

class Repeater : public Input {
public:
    int x8;
    void func_0015C4E8();
};

struct Game {
    char pad0[0x63DC];
    char x63DC[4];
    Input x63E0;
    Input x63E8;
    char pad63F0[4];
    Input x63F4;
    char pad63FC[0x10];
    Input x640C;
};

extern "C" int func_00238F48(void);
extern "C" void func_0015BFF8(void *list, Input *input);

void Repeater::func_0015C4E8()
{
    Game *game = x0;
    Input *input = &game->x63E0;
    void *list = game->x63DC;
    int elapsed = func_00238F48() - x8;

    if (input->IsPressed()) {
        func_0015BFF8(list, input);
    } else if (!IsPressed() && elapsed > 16) {
        input = &game->x640C;
        if (input->IsPressed()) {
            func_0015BFF8(list, input);
        } else {
            input = &game->x63E8;
            if (input->IsPressed()) {
                func_0015BFF8(list, input);
            } else {
                input = &game->x63F4;
                if (input->IsPressed()) {
                    func_0015BFF8(list, input);
                }
            }
        }
    }
}
