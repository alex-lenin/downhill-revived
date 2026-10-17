/* UNIT 0x0018AA90 0x0018AAE8 */
struct Entry { char pad0[0x1c]; short x1C; };
class Obj {
public:
    char pad0[0x4f8];
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8(Entry *);
    void func_00187438(Entry *);
    void func_0018AA90(int);
};
void Obj::func_0018AA90(int index) {
    Entry *entry = (Entry *)(index * 16 + (unsigned int)this);
    int type = entry->x1C;
    type ^= 31;
    if (type == 0) {
        slot8(entry);
    } else {
        func_00187438(entry);
    }
}


