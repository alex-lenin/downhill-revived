/* UNIT 0x0018D610 0x0018D638 */
class A {
public:
    char pad0[0xD];
    unsigned char xD;
    void func_0018D610();
    void func_0018D7B0();
};

void A::func_0018D610()
{
    if (xD) {
        this->func_0018D7B0();
    }
}
