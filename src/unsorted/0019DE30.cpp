/* UNIT 0x0019DE30 0x0019DE60 */
class Object {
public:
    char pad0[0x610];
    int x610, x614;
    char pad618[0xD];
    char x625;
    void func_0018DB20();
    void func_0019DE30();
};
void Object::func_0019DE30() {
    x625 = 1;
    x610 = 0;
    x614 = 0;
    this->func_0018DB20();
}
