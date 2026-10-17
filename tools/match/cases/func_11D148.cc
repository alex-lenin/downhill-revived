struct A {
    int x0, x4, x8; char pad[0x26c - 0xc]; short x26c;
    int func_11D148();
};
int A::func_11D148() {
    switch (x8) {
    case 2: case 3: case 4: return x26c >= 6;
    case 5: return 1;
    case 0: case 1: case 6: case 7: case 8: case 9:
    default: return 0;
    }
}
