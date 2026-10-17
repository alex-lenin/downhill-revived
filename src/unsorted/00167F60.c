/* UNIT 0x00167F60 0x00168198 */
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F518(int,int,int,int,int);
extern void func_0020F9A8(int,int,int,int,char *);
extern int func_00254D68(char *,char *,...);
extern int D_0026E9F0[];
extern char D_0028ACC8[];
extern int D_0026EA08[];
extern char D_0028ACE8[];
extern int D_0026EA38[];
extern char D_0028AD10[];
extern int D_0026EA50[];
extern char D_0028AD30[];
extern int D_0026EA68[];
extern char D_0028AD58[];
extern int D_0026EA80[];
extern char D_0028AD78[];
extern int D_0026EA20[];
extern char D_0028AD98[];
extern int D_0026EA98[];
extern char D_0028ADB8[];
extern int D_0026EAB0[];
extern char D_0028ADD0[];
extern int D_0026EB88[];
extern char D_0029C818[], D_0029C820[];
void func_00167F60(int unused,int kind,int arg2,int arg3,int index) {
    char text[80];
    int *value; char *format;
    func_0020FB80(0,0x120,0xe0,-0x6f,-0x20);
    func_0020F518(0,0x6e,0x6e,0xc8,arg3);
    if (kind == 0) { func_00254D68(text,D_0028ACC8,D_0026E9F0[index]); }
    else if (kind == 1) { func_00254D68(text,D_0028ACE8,D_0026EA08[index]); }
    else if (kind == 2) { func_00254D68(text,D_0028AD10,D_0026EA38[index]); }
    else if (kind == 11) { func_00254D68(text,D_0028AD30,D_0026EA50[index]); }
    else if (kind == 13) { func_00254D68(text,D_0028AD58,D_0026EA68[index]); }
    else if (kind == 14) { func_00254D68(text,D_0028AD78,D_0026EA80[index]); }
    else if (kind == 10) { func_00254D68(text,D_0028AD98,D_0026EA20[index]); }
    else if (kind == 16) { func_00254D68(text,D_0028ADB8,D_0026EA98[index]); }
    else if (kind == 17) { func_00254D68(text,D_0028ADD0,D_0026EAB0[index]); }
    else { func_00254D68(text,D_0029C818,D_0026EB88[kind]); }
    func_0020F9A8(0,0,30,arg2,text);
    func_0020F518(0,88,152,73,arg3);
    func_0020F9A8(0,0,400,arg2,D_0029C820);
}
