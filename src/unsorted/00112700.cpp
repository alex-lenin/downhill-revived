/* UNIT 0x00112700 0x00112758 */
inline int min_value(int a, int b) { return b < a ? b : a; }
class Obj {
public:
    char pad[0x3D4];
    int x3D4[4];
    int func_00112758(int unused, int value);
    void func_00112700(int index, int value);
};
void Obj::func_00112700(int index, int value) {
    int amount = func_00112758(index, value);
    int total = x3D4[index] + amount;
    x3D4[index] = min_value(total, 100);
}


