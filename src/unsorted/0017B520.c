/* UNIT 0x0017B520 0x0017B5C8 */
struct Obj {
    char pad0[0x1280];
    int x1280;
};

extern int func_0023DED0(void);
extern void func_0023D650(int handle, int *width, int *height);
extern void func_0020F9F0(int a, int handle, int x, int y, char *text);

void func_0017B520(struct Obj *obj, char *text, float scaleX, float scaleY)
{
    int width;
    int height;

    func_0023DED0();
    func_0023D650(obj->x1280, &width, &height);
    func_0020F9F0(0, obj->x1280, (int)((width >> 1) * scaleX),
                  (int)((height >> 1) * scaleY), text);
}
