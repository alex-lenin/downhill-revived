/* UNIT 0x00201180 0x002013F8 -fno-opt-stack */
typedef struct 
{
  void *x0;
  void *x4;
  int pad8;
} Pair;
typedef struct 
{
  int x0;
  int pad4;
  unsigned long x8;
} State;
typedef struct 
{
  int x0;
  char pad4[0xc];
  int x10;
} Packet;
typedef struct 
{
  char pad[0x24];
  int x24;
} Client;
extern Pair D_00279448;
extern Pair D_00279458;
extern Pair D_00279460;
extern int D_00279450[];
extern int D_00279470[];
extern int D_002794A0[];
extern State D_00279478;
extern Client D_004C8940;
extern Client D_004CD3C0;
extern int D_004C8B00[];
extern int D_004C9B00[];
extern int D_004CD400[];
extern char D_004CAB00[];
extern char D_004CBB00[];
extern char D_004CCB00[];
extern char D_004CCF40[];
extern char D_002977A0[];
extern char D_002977C8[];
extern volatile Packet D_004CD380;
extern void func_0025C7D0(int);
extern int func_0025CDB8(void *, int, int);
extern void func_00254D08(void *, void *, int);
extern int func_00201E90(int, int, void *);
void func_00201180(int flags)
{
  struct 
  {
    Packet *packet;
    int flags;
  } args;
  int delay;
  D_00279448.x0 = D_004C8B00;
  D_00279448.x4 = D_004C9B00;
  D_00279460.x0 = D_004CCB00;
  D_00279460.x4 = D_004CCF40;
  D_00279458.x0 = D_004CAB00;
  D_00279458.x4 = D_004CBB00;
  if (flags & 2)
  {
    D_002794A0[0] = 1;
  }
  else
  {
    D_002794A0[0] = 0;
  }
  func_0025C7D0(0);
  do
  {
    if (func_0025CDB8(&D_004C8940, 0x123456, 0) < 0)
    {
      func_00254D08(D_002977A0, D_002977C8, 0xb2);
      for (;;)
      {
      }

    }
    for (delay = 10000; delay != (-1); delay--)
    {
    }

  }
  while (D_004C8940.x24 == 0);
  D_00279470[0] = 0;
  D_00279478.x0 = 0;
  D_00279478.x8 = 0;
  D_004CD400[0] = 0;
  do
  {
    if (func_0025CDB8(&D_004CD3C0, 0x123457, 0) < 0)
    {
      func_00254D08(D_002977A0, D_002977C8, 0xc7);
      for (;;)
      {
      }

    }
    for (delay = 10000; delay != (-1); delay--)
    {
    }

  }
  while (D_004CD3C0.x24 == 0);
  D_004CD380.x0 = 0;
  D_00279450[0] = 0xffc;
  D_00279450[1] = 0xffc;
  D_004C8B00[0] = 0;
  D_004C9B00[0] = 0;
  D_004CD380.x10 = 0;
  args.packet = &D_004CD380;
  args.flags = flags;
  func_00201E90(0, 8, &args);
}
