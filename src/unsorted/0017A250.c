/* UNIT 0x0017A250 0x0017A328 */
extern char D_0029C9C8[];
extern int func_00254D68(char *,char *,int);
extern int func_00215B58();
extern int func_0017C830(void *,int);
extern void func_0017C8A8(void *,void *,int);
void func_0017A250(char *self,int value,int a,int b) {
 char text[16]; int digit; int result;
 func_00254D68(text,D_0029C9C8,value);
 digit=(signed char)text[1]-'0';
 if((unsigned int)digit>=10) digit=0;
 result=func_0017C830(self,func_00215B58(a));
 func_0017C8A8(self,self+(0xD34+digit*16),result);
 digit=(signed char)text[0]-'0';
 if((unsigned int)digit>=10) digit=0;
 if(!digit) digit=10;
 result=func_0017C830(self,func_00215B58(b));
 func_0017C8A8(self,self+(0xD34+digit*16),result);
}
