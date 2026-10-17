/* UNIT 0x0019A530 0x0019A728 */
struct State { char pad[32]; int x20; };
extern struct State D_003F52D0;
extern signed char D_0026F938[][12];
extern char *D_0026F7F8[];
extern char D_0028E530[], D_0028E548[], D_0029D158[], D_0029D160[];
extern void func_0020F518(int,int,int,int,int);
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F9A8(int,int,int,int,char *);
extern int func_00254D68(char *,char *,...);
void func_0019A530(void *self, int index) {
    char buf[48];
    int count, i, y, next;
    signed char *list;
    list = D_0026F938[index];
    count = 0;
    while (list[count] != 40) count++;
    y = 47;
    func_0020F518(0,80,120,80,170);
    i = 0;
    func_0020FB80(0,256,192,-83,-32);
    func_00254D68(buf,D_0028E530,count);
    func_0020F9A8(0,D_003F52D0.x20,92,20,buf);
    func_0020FB80(0,224,192,-75,-32);
    func_0020F9A8(0,D_003F52D0.x20,92,35,D_0029D158);
    func_0020F9A8(0,D_003F52D0.x20,176,35,D_0028E548);
    func_0020F518(0,110,110,200,170);
    while(list[i] != 40) {
        next = i + 1;
        func_00254D68(buf,D_0029D160,next);
        func_0020F9A8(0,D_003F52D0.x20,112,y,buf);
        func_0020F9A8(0,D_003F52D0.x20,136,y,D_0026F7F8[list[i]]);
        i = next;
        y += 10;
    }
}
