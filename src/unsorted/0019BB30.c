/* UNIT 0x0019BB30 0x0019BBA8 */
typedef struct Buffer { int count; unsigned char data[1]; } Buffer;
extern void func_0019BBA8(Buffer *);
void func_0019BB30(Buffer *b, signed char *text) {
 signed char *p;
 unsigned char c; int count;
 func_0019BBA8(b);
 if (*text != '(') {
  p=text;
  do {
   c=*p;
   p++;
   count=b->count;
   b->data[count]=c;
   b->count = count+1;
  } while (*p != '(');
 }
}
