/* UNIT 0x0017B5C8 0x0017B670 */
struct Obj { char pad[0x1280]; int x1280; };
extern void func_0023DED0(void);
extern void func_0023D650(int, int *, int *);
extern void func_0020F9A8(int, int, int, int, int);
void func_0017B5C8(struct Obj *obj, int arg, float x, float y) {
    int width, height;
    func_0023DED0();
    func_0023D650(obj->x1280, &width, &height);
    func_0020F9A8(0, obj->x1280, (int)((width >> 1) * x), (int)((height >> 1) * y), arg);
}
