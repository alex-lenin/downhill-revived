/* UNIT 0x00187328 0x00187358 */
extern char D_003E3BD0[];
extern void *func_00187008(void *obj);

/* static initializer for the global object at D_003E3BD0 */
void func_00187328(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        func_00187008(D_003E3BD0);
    }
}
