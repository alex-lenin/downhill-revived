/* UNIT 0x0019BED0 0x0019BF38 */
extern void func_001ACFA0(void *, int);
extern void func_001AD580(void *, int, float, float);
extern void func_00136030(void *, int);
void func_0019BED0(char **p) {
    char *obj = *p;
    char *part = obj + 0x4580;
    func_001ACFA0(part, 1);
    func_001AD580(part, 0, 0.0f, 1.0f);
    *(int *)(obj + 0x48B0) = 0;
    obj[0x4D50] = 0;
    func_00136030(obj + 0x40C0, 2);
}
