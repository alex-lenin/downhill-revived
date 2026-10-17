/* UNIT 0x001817E0 0x001818B0 */
int func_001817E0(void *arg0, int arg1) {
 int result;
 if(arg1==0x4EC) result=4;
 else if(arg1==0x50B) result=16;
 else if(arg1==0x55A || arg1==0x56E || arg1==0x58A || arg1==0x590 || arg1==0x596 || arg1==0x59C || arg1==0x5A2 || arg1==0x5A8 || arg1==0x5AE || arg1==0x5B4) result=8;
 else if(arg1==0x574 || arg1==0x580) result=32;
 else if(arg1==0x514) result=64;
 else if(arg1==0x57A || arg1==0x516) result=128;
 else result=arg1==0x584 ? 256 : 0;
 return result;
}
