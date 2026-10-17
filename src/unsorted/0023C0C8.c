/* UNIT 0x0023C0C8 0x0023C1C8 */
extern void func_0024CF48(void *mpeg, void *work, int workSize);
extern int sceMpegAddCallback(void *mpeg, int type, void *callback, void *data);
extern void func_0023C230(void *mpeg);
extern void func_0023B248(void *p, int a, int b, int c, int d, int e);
extern int func_0023C6B8();
extern int func_0023C6E0();
extern int func_0023C710();
extern int func_0023C738();
extern int func_0023C760();

int func_0023C0C8(char *mpeg, void *work, int workSize, int a, int b, int c, int d, int e)
{
    func_0024CF48(mpeg, work, workSize);
    sceMpegAddCallback(mpeg, 0, func_0023C6B8, 0);
    sceMpegAddCallback(mpeg, 1, func_0023C6E0, 0);
    sceMpegAddCallback(mpeg, 2, func_0023C710, 0);
    sceMpegAddCallback(mpeg, 3, func_0023C738, 0);
    sceMpegAddCallback(mpeg, 5, func_0023C760, 0);
    func_0023C230(mpeg);
    func_0023B248(mpeg + 0x48, a, b, c, d, e);
    return 1;
}
