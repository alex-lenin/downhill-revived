/* UNIT 0x001DDCA0 0x001DDCD0 */
extern char D_004C2120[];
extern void func_001DDC70(void *);
void func_001DDCA0(int enabled, int code) {
    if (code == 0xFFFF) {
        if (enabled != 0) {
            func_001DDC70(D_004C2120);
        }
    }
}
