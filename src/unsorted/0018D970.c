/* UNIT 0x0018D970 0x0018D9C8 */
struct Pair { short x0; short x2; };
extern int func_0018D860(struct Pair *);
extern void func_0018D930(int, int, int, int);
void func_0018D970(int obj, struct Pair *pair) {
    int x0, x2, result;
    x0 = pair->x0;
    x2 = pair->x2;
    result = func_0018D860(pair);
    func_0018D930(obj, x0, x2, result);
}



