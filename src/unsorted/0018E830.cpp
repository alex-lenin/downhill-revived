/* UNIT 0x0018E830 0x0018E970 */
class Base {
public:
    int cursor;
    char pad4[0x4A8 - 0x4];
    int x4A8;
    char pad4AC[0x4B9 - 0x4AC];
    unsigned char selected;
    char pad4BA[0x4F8 - 0x4BA];
    virtual void Select();
    virtual void Deselect();
    virtual void v2();
    virtual void v3();
    virtual void Reselect();
    virtual void Refresh();
};

class A : public Base {
public:
    void func_0018E830();
    void func_0018DC20();
    A *func_0018EAC8(int index);
    void func_0018E770();
};

void A::func_0018E830()
{
    int old;
    A *current;
    A *prev;
    A *item;
    int i;

    x4A8 = 5;
    old = cursor;
    this->func_0018DC20();
    current = func_0018EAC8(cursor);
    this->func_0018E770();
    if (old != cursor) {
        prev = func_0018EAC8(old);
        if (prev != 0) {
            prev->Deselect();
            prev->selected = 0;
        }
        if (current != 0) {
            current->Select();
            current->selected = 1;
        }
    }
    if (current != 0) {
        if (!current->selected) {
            current->Select();
            current->selected = 1;
        } else {
            current->Reselect();
        }
    }
    for (i = 0; i < 12; i++) {
        if (i != cursor) {
            item = func_0018EAC8(i);
            if (item != 0) {
                item->Refresh();
            }
        }
    }
}
