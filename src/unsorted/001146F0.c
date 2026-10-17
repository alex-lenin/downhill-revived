/* UNIT 0x001146F0 0x001148E8 */
typedef struct { char pad0[0x796c]; int x796c; } Actor;
typedef struct { char pad0[0x58]; Actor *actor; char pad5c[0x50]; int duration; } State;
typedef struct { char pad0[0x5bb4]; int x5bb4; char pad5bb8[0x24]; int x5bdc; } Settings;
extern Settings D_00398C40;
void func_001146F0(State *state) {
 int scale;
 state->duration=0;
 switch(state->actor->x796c) {
 case 0xa0: state->duration=1200; break;
 case 0xc0: state->duration=240; break;
 case 0x40: state->duration=60; break;
 case 0x60: state->duration=360; break;
 case 0x80: state->duration=420; break;
 case 0x20: state->duration=300; break;
 case 0x120: state->duration=420; break;
 case 0x160: state->duration=1200; break;
 case 0xe0: state->duration=60; break;
 case 0x100: state->duration=60; break;
 case 0x140: state->duration=60; break;
 case 0x180: state->duration=60; break;
 case 0x1a0: state->duration=60; break;
 case 0x1c0: case 0x1e0: default: state->duration=600; break;
 }
 if(D_00398C40.x5bb4==0) scale=D_00398C40.x5bdc/2;
 else if(D_00398C40.x5bb4==2) scale=D_00398C40.x5bdc/2+50;
 else scale=D_00398C40.x5bdc/2+25;
 state->duration-=state->duration*scale/100;
}
