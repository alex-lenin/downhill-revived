/* UNIT 0x001126A8 0x00112700 */
inline int min_value(int a, int b) { return b < a ? b : a; }
class Obj {
public:
    char pad[0x3C4];
    int x3C4[4];
    int func_00112758(int unused, int value);
    void func_001126A8(int index, int value);
};
void Obj::func_001126A8(int index, int value) {
    int amount = func_00112758(index, value);
    int total = x3C4[index] + amount;
    x3C4[index] = min_value(total, 100);
}

