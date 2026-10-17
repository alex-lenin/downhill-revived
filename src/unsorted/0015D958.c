/* UNIT 0x0015D958 0x0015DB78 .rodata=0x28A420:0x28A440 */
typedef struct Record { int value; short x4, x6; unsigned char x8,x9,xA,xB; char name[1]; } Record;
extern char D_0029C770[];
extern char D_00277270[][30];
extern char *func_00158650(int);
extern void func_0020F518(int,int,int,int,int);
extern void func_0020F9A8(int,int,int,int,const char *);
extern int func_00254D68(char *,const char *,...);
void func_0015D958(int y,int x,Record *record,char *title,int font,int time,int selected) {
    char buffer[32];
    func_0020F518(0,110,110,200,150);
    func_0020F9A8(0,font,x,y,title);
    func_00254D68(buffer,"%02x-%02x",record->x9,record->x8);
    if (selected) func_0020F518(0,220,220,0,128);
    else func_0020F518(0,100,120,100,128);
    func_0020F9A8(0,font,x+0x42,y,record->name);
    func_0020F9A8(0,font,x+0x84,y,buffer);
    func_0020F9A8(0,font,x+0xBC,y,func_00158650(record->x4));
    func_0020F9A8(0,font,x+0xFC,y,D_00277270[record->x6]);
    func_00254D68(buffer,D_0029C770,record->xB);
    func_0020F9A8(0,font,x+0x166,y,buffer);
    if (time) {
        int value = record->value;
        int seconds = value % 60;
        int minutes;
        int hours;
        value /= 60;
        minutes = value % 60;
        value /= 60;
        hours = value % 60;
        func_00254D68(buffer,"%d:%02d:%02d",hours,minutes,seconds);
    } else func_00254D68(buffer,D_0029C770,record->value);
    func_0020F9A8(0,font,x+0x18E,y,buffer);
}
