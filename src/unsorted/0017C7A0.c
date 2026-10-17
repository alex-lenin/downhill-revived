/* UNIT 0x0017C7A0 0x0017C830 */
extern char D_0029C988[];
extern int func_00254D68(char *, char *, int);
extern int func_00215B58(int);
extern int func_0017C830(char *, int);
extern void func_0017C8A8(char *, char *, int);
void func_0017C7A0(char *obj, int arg, int number) {
 char text[16]; int digit; int value;
 if (number < 0) digit=10;
 else { func_00254D68(text,D_0029C988,number); digit=text[0]-'0'; if((unsigned int)digit>=10) digit=0; }
 value=func_00215B58(arg); value=func_0017C830(obj,value);
 func_0017C8A8(obj,obj+0xD34+digit*16,value);
}
