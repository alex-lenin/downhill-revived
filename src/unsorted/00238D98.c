/* UNIT 0x00238D98 0x00238F10 */
extern int D_0029E3D0,D_0029E3D4,D_0029E3D8,D_0029E3DC,D_0029E3E0,D_0029E3E8,D_0029E3F0,D_0029E3F4;
extern float D_0029E3FC,D_0029E400;
extern int func_00238F10(void);
extern int func_00243CE8(int);
void func_00238D98(int *out) {
    int old,current,delta,value,count;
    float rate;
    current=D_0029E3D8;
    old=D_0029E3DC;
    D_0029E3DC=current;
    delta=current-old;
    count=2;
    D_0029E3D0=delta;
    if(delta<=0) D_0029E3D0=1;
    if(D_0029E3D0>=3) D_0029E3D0=2;
    D_0029E3D4=D_0029E3F0/D_0029E3D0;
    if(D_0029E3D4<=0) D_0029E3D4=1;
    do {
        if(!(D_0029E3D4>D_0029E3F4)) break;
        if(out) *out=func_00243CE8(0)==0;
        else func_00243CE8(0);
        count--;
        value=D_0029E3D0+1;
        D_0029E3D0=value;
        D_0029E3D4=D_0029E3F0/value;
    } while(count>=0);
    rate=(float)D_0029E3D4;
    D_0029E3FC=rate;
    D_0029E400=1.0f/rate;
    D_0029E3E0+=*(volatile int *)0x10000800;
    *(volatile int *)0x10000800=0;
    D_0029E3E8=func_00238F10();
}
