/* UNIT 0x00242FB8 0x00243198 */
typedef struct Random { unsigned int state[624]; int index; } Random;
void func_00242FB8(Random *rng, unsigned int seed)
{
 unsigned int y, last;
 rng->state[0]=seed;
 for(rng->index=1; rng->index<624; rng->index++) {
  rng->state[rng->index]=(int)((unsigned long)(rng->state[rng->index-1]^(rng->state[rng->index-1]>>30))*0x6c078965U)+rng->index;
 }
 for(rng->index=0; rng->index<227; rng->index++) {
  unsigned int value;
  y=(rng->state[rng->index]&0x80000000U)|(rng->state[rng->index+1]&0x7fffffffU);
  value=rng->state[rng->index+397]^(y>>1);
  if(y&1) value^=0x9908b0dfU;
  rng->state[rng->index]=value;
 }
 for(;rng->index<623;rng->index++) {
  unsigned int value;
  y=(rng->state[rng->index]&0x80000000U)|(rng->state[rng->index+1]&0x7fffffffU);
  value=rng->state[rng->index-227]^(y>>1);
  if(y&1) value^=0x9908b0dfU;
  rng->state[rng->index]=value;
 }
 y=(rng->state[623]&0x80000000U)|(rng->state[0]&0x7fffffffU);
 last=rng->state[396]^(y>>1);
 if(y&1) last^=0x9908b0dfU;
 rng->state[623]=last;
 rng->index=0;
}
