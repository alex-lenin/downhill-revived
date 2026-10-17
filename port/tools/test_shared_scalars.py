"""Execute matching camera and GS bodies through the new shared scalar APIs."""
from pathlib import Path
import subprocess
import transform


def main():
    root=Path(__file__).resolve().parents[2];out=root/'build/port/runtime/shared-scalars';out.mkdir(parents=True,exist_ok=True)
    src=root/'src/unsorted'
    camera=(src/'001B2E80.cpp').read_text()+'''
extern "C" int D_0029D400=1;
extern "C" void func_001AE758(void *,int,float,float,float) {}
int main(void) {
 unsigned char storage[0x5A0]={0}; Settings settings={};
 settings.x4C=1;settings.x48=1;
 settings.x0=1;settings.x4=2;settings.x8=3;settings.xC=4;settings.x10=5;settings.x14=6;
 func_001B2E80(&settings,(Obj *)storage,2);
 for(int i=0;i<3;i++)if(*(float *)(storage+0x534+i*0x20)!=(i*2+2)*2)return 1;
 func_001B2E80(&settings,(Obj *)storage,-3);
 for(int i=0;i<3;i++)if(*(float *)(storage+0x534+i*0x20)!=(i*2+2)*2-(i*2+1)*3)return 2;
 for(int i=0;i<sizeof storage;i++)if((i<0x534||i>=0x578||((i-0x534)%0x20)>=4)&&storage[i])return 3;
 return 0;
}
'''
    particle=(src/'00228BE8.c').read_text()
    particle=particle.split('/* ---- merged from src/unsorted/0022B1F0.c ---- */',1)[1].split('INCLUDE_ASM("asm/nonmatchings/text", func_0022B258);',1)[0]
    particle+='''
static int fade_failures;
void func_0022B028(void *p,void *v,void *c,int *n,const float *offset,float *values,float velocityScale) {
 fade_failures+=p!=(void *)11||v!=(void *)22||c!=(void *)33||*n!=1||offset!=(void *)9||velocityScale!=0.375f||values[0]!=0.5f||values[1]!=0.5f||values[2]!=0.5f||values[3]!=0.75f;
}
void func_00223628__c6(void *a,void *b) {}
int main(void) {struct Object__c6 obj={0};obj.x2830=11;obj.x2834=22;obj.x2838=33;obj.x2850=1;func_0022B1F0__c6(&obj,9,0.375f,0.5f,0.75f);return fade_failures;}
'''
    blend=(src/'001B1698.c').read_text()+''.join((src/name).read_text().replace('#include "tween.h"','') for name in ['001B3170.c','001AE630.c'])+'''

static int blend_failures;
int func_002086C8(FloatTween *t,float *target,int frames,float start,float end) {
 blend_failures+=target==0 || t==0 || frames!=37 || start!=0.25f || end!=-0.5f;
 return 91;
}
int main(void) {
 RiderBlendController b={0};b.weight=0.25f;
 if(SetRiderBlendWeightFirst(&b,-0.5f,37)!=91)return 1;
 SetRiderBlendWeightFirstDiscard(&b,-0.5f,37);
 SetRiderBlendDiscard(&b,37,-0.5f);
 RiderSecondaryBlend secondary={0};secondary.weight=0.25f;
 RiderBodyBlend body={0};body.weight=0.25f;
 SetSecondaryBlendWeightFirst(&secondary,-0.5f,37);
 SetSecondaryBlendFloatFirst(-0.5f,&secondary,37);
 SetBodyBlendWeightFirst(&body,-0.5f,37);
 return blend_failures;
}
'''
    selector='''#include "animation_select.h"
static int selector_failures;
void func_00202E38(struct AnimationHandle *h,int mode,int flag,float duration) {
 selector_failures+=h!=(void *)0x1234 || mode!=3 || flag!=7 || duration!=-2.5f;
}
void func_00203590(struct AnimationHandle *h,int resource,int mode,int index) {
 selector_failures+=h!=(void *)0x1234 || resource!=0x4567 || mode!=3 || index!=7;
}
int main(void) {
 SelectAnimationTimeFirst((void *)0x1234,-2.5f,3,7);
 SelectAnimationLegacyAddress(0x1234,3,7,-2.5f);
 SelectAnimationResourceLegacy((void *)0x1234,0x4567,3,7);
 return selector_failures;
}
'''
    style=(src/'0018D7D0.c').read_text()+(src/'0018D800.c').read_text().replace('#include "ui_style.h"','')+'''

static int style_failures,style_calls;
void func_0020FB80(int slot,int x,int y,int w,int h) {
 style_failures+=slot!=0 || x!=42 || y!=17 || w!=-9 || h!=-3;style_calls++;
}
void func_0020F518(int slot,int r,int g,int b,int a) {
 style_failures+=slot!=0 || r!=11 || g!=22 || b!=33 || a!=128;style_calls++;
}
int main(void) {
 struct {unsigned before;UiStyle style;unsigned after;} record={0};
 record.before=0x12345678;record.after=0x87654321;
 if(func_0018D7D0(&record.style,42,17,-9,-3,11,22,33,128)!=&record.style)return 1;
 func_0018D800(&record.style);
 InitializeUiStyleDiscard(&record.style,42,17,-9,-3,11,22,33,128);
 func_0018D800(&record.style);
 return style_failures || style_calls!=4 || record.before!=0x12345678 || record.after!=0x87654321;
}
'''
    menu=''.join((src/name).read_text() for name in ['001848E0.c','001873C0.c','0018E3A8.c'])+'''

Desc D_0029DE30;
void func_0018D898(MenuItem *item,int id,int context) { item->x4=id;item->x8=0;item->xC=context; }
int main(void) {
 SharedMenu menu={0};MenuSelectionState state={0};
 SetMenuSelectionLegacy(&state,1,0);if(state.x10!=1 || state.xC)return 1;
 SetMenuSelectionLegacy(&state,0,0);if(state.x10 || state.xC)return 2;
 if(func_001873C0(&menu,37,1)!=1 || menu.items[0].x4!=37 || menu.items[0].xC!=(int)&D_0029DE30 || menu.items[0].x6!=1)return 3;
 if(func_001873C0(&menu,12,0)!=2 || menu.items[1].x4!=12 || menu.items[1].x6)return 4;
 func_0018E3A8(&menu,63,(void *)0x1234,7);
 return menu.x418!=3 || menu.items[2].x4!=63 || menu.items[2].xC!=0x1234 || menu.items[2].x6!=7;
}
'''
    probe='''#include "ground_probe.h"
float func_00211980(GroundProbeVector *p,GroundProbeVector *n,int flags,float height) {
 if(p->f[0]!=3 || flags!=7 || height!=2.5f)__builtin_trap();
 n->f[0]=0;n->f[1]=0;n->f[2]=1;n->f[3]=0;return 17.5f;
}
int main(void) {
 GroundProbeVector p={0},n={0};p.f[0]=3;
 for(int i=0;i<32;i++) {
  if(ProbeGroundLegacy(&p,&n,7,2.5f)!=17.5f)return 1;
  if(ProbeGroundHeightFirst(&p,&n,2.5f,7)!=17.5f)return 2;
  if(ProbeGroundHeightBetween(&p,2.5f,&n,7)!=17.5f)return 3;
 }
 return n.f[0] || n.f[1] || n.f[2]!=1 || n.f[3];
}
'''
    projection='''#include "track_projection.h"
int func_00119180(struct TrackProjectionState *track,struct Vector4 *pos,struct Vector4 *out,float *angles,int flags) {
 if(track!=(void *)1 || pos!=(void *)2 || out!=(void *)3 || flags!=7)__builtin_trap();
 angles[0]=1.25f;angles[1]=-2.5f;angles[2]=3.75f;return 19;
}
int main(void) {
 struct {unsigned before;TrackProjectionAngles angles;unsigned after;} record={0};
 record.before=0x12345678;record.after=0x87654321;
 ProjectTrackDiscard((void *)1,(void *)2,(void *)3,record.angles,7);
 ProjectTrackAddressDiscard((void *)1,(void *)2,(void *)3,(int)record.angles,7);
 return record.angles[0]!=1.25f || record.angles[1]!=-2.5f || record.angles[2]!=3.75f || record.before!=0x12345678 || record.after!=0x87654321;
}
'''
    derived=(src/'0018F210.cpp').read_text()+'''
char D_00286FB8[4],D_00287308[4];
static int base_calls;
SharedMenu *func_0018D9D0(SharedMenu *menu) {base_calls++;menu->x45C=37;return menu;}
int main(void) {
 struct {unsigned before;SharedDerivedMenu menu;unsigned after;} record={0};
 record.before=0x12345678;record.after=0x87654321;
 if(func_0018F210(&record.menu)!=&record.menu)return 3;
 if(base_calls!=1 || record.menu.base.x45C!=37 || record.menu.base.vptr!=D_00286FB8 || record.menu.member.vptr!=D_00287308)return 1;
 if(record.menu.base.x4A0!=2 || record.menu.base.x4A4!=3 || record.menu.base.x4C8!=(MenuLink *)&record.menu.member || record.menu.x500 || record.menu.member.x4!=1 || record.menu.member.x5!=1)return 2;
 return record.before!=0x12345678 || record.after!=0x87654321 || (char *)&record.menu.member-(char *)&record.menu!=0x504;
}
'''
    rtc=(src/'00193B00.c').read_text()+'''
static int reads;
int func_00248058(RtcClock *out) {
 if(!out)__builtin_trap();reads++;
 out->stat=0;out->second=0x17;out->minute=0x25;out->hour=0x09;out->pad=0;out->day=0x14;out->month=0x10;out->year=0x26;
 return 7;
}
int main(void) {
 struct {unsigned before;RtcClock clock;unsigned after;} record={0};
 record.before=0x12345678;record.after=0x87654321;
 for(int i=0;i<32;i++) {
  if(ReadRtcDiscard(&record.clock)!=7 || ReadRtcLegacy(&record.clock,123)!=7 || func_00193B00(&record.clock)!=7)return 1;
 }
 return reads!=96 || record.clock.second!=0x17 || record.clock.minute!=0x25 || record.clock.day!=0x14 || record.clock.month!=0x10 || record.clock.year!=0x26 || record.before!=0x12345678 || record.after!=0x87654321;
}
'''
    from recomp import ABI_SIGNATURES
    for name in ('func_00193B00','func_00248058'):
        sig=ABI_SIGNATURES[name]
        assert sig['ret']=='i32' and sig['args']==['i32'] and not sig['variadic'],sig
    # These integer-only APIs must also override stale source guesses.
    expected_si={
        '001BDDA0':('i32',2), '0014E9D0':('i32',0), '001B4618':('void',0),
        '00117A78':('void',4), '00117A98':('void',5), '0018F210':('i32',1),
        '00187808':('void',0), '0018F278':('void',0), '001790E8':('void',6),
        '00124B40':('void',1), '001B4A60':('i32',1),
    }
    import json, recomp
    saved_out=recomp.OUT
    try:
        recomp.OUT=str(out/'si-cache');cache=Path(recomp.OUT);cache.mkdir(exist_ok=True)
        (cache/'signatures-version.txt').write_text(recomp.SIGNATURE_VERSION)
        (cache/'signatures.json').write_text(json.dumps({'func_'+a:dict(ret='i32',args=['i32']*8,variadic=True) for a in expected_si}))
        actual=recomp.signatures(['func_'+a for a in expected_si])
        for addr,(ret,nargs) in expected_si.items():
            sig=actual['func_'+addr]
            assert sig['ret']==ret and sig['args']==['i32']*nargs and not sig['variadic'],sig
    finally:recomp.OUT=saved_out
    files={'camera.cpp':camera,'channel.c':(src/'001B3D58.c').read_text(),'packet.c':(src/'0023B220.c').read_text()+(src/'0023D4F8.c').read_text(),'particle.c':particle,'blend.c':blend,'selector.c':selector,'style.c':style,'menu.c':menu,'probe.c':probe,'projection.c':projection,'derived.cpp':derived,'rtc.c':rtc}
    for name,s in files.items():(out/name).write_text('#include "port.h"\n'+transform.transform(s.replace('#include "rtc.h"',(root/'include/rtc.h').read_text()).replace('#include "derived_menu.h"',(root/'include/derived_menu.h').read_text()).replace('#include "particle_update.h"',(root/'include/particle_update.h').read_text()).replace('#include "tween.h"',(root/'include/tween.h').read_text()).replace('#include "animation_select.h"',(root/'include/animation_select.h').read_text()).replace('#include "ui_style.h"',(root/'include/ui_style.h').read_text()).replace('#include "menu_items.h"',(root/'include/menu_items.h').read_text()).replace('#include "menu.h"',(root/'include/menu.h').read_text()).replace('#include "ground_probe.h"',(root/'include/ground_probe.h').read_text()).replace('#include "track_projection.h"',(root/'include/track_projection.h').read_text()),name.endswith('cpp'),'_',None))
    driver=out/'gs.c';driver.write_text('''#include "scalar_api.h"
#include <stdio.h>
int D_0029E41C,D_0029E420,D_0029E424,D_0029E428;
char D_0075BC70[0x1440] __attribute__((aligned(16)));
void func_001C2458(struct Scalar001C2458Obj **a,struct Scalar001C2458Obj **b,void *c,void *d,void *e,void *f,int flag,int mask,float x,float y,unsigned int z) {
 if(a!=(void *)1||b!=(void *)2||c!=(void *)3||d!=(void *)4||e!=(void *)5||f!=(void *)6||flag!=27||mask!=63||x!=1.25f||y!=-2.5f||z!=19)__builtin_trap();
}
int main(void) {
 ScalarBridge001C2458_1((void *)1,(void *)2,3,(void *)4,(void *)5,(void *)6,((ScalarUnsigned64)0xaabbccdd<<32)|27,1.25f,-2.5f,63,19);
 ScalarUnsigned64 q=0;func_0023B220(&q,0x12345678,0x23456789,0x3456789a);
 if(q!=(((ScalarUnsigned64)0x12345678<<32)|(((ScalarUnsigned64)0x23456789<<32)>>4)|0x3456789a))return 1;
 func_0023D4F8(11,22,33,44);
 ScalarUnsigned64 expected=11|((ScalarUnsigned64)22<<8)|((ScalarUnsigned64)33<<16)|((ScalarUnsigned64)44<<24)|((ScalarUnsigned64)0xfe00<<46);
 for(unsigned i=0;i<0x1440;i+=0x360)if(*(ScalarUnsigned64 *)(D_0075BC70+i)!=expected||*(ScalarUnsigned64 *)(D_0075BC70+i+0x170)!=expected)return 2;
 if(D_0029E41C!=11||D_0029E420!=22||D_0029E424!=33||D_0029E428!=44)return 3;
 puts("Shared scalar calls: real camera channels, rider argument slots and GS 64-bit packets pass");
 return 0;
}
''')
    flags=['clang','--target=i686-pc-windows-msvc','-fuse-ld=lld','-O1','-w','-msse2','-mfpmath=sse','-I'+str(root/'port/include'),'-I'+str(root/'include')]
    for names,exe in [(['camera.cpp','channel.c'],'camera.exe'),(['gs.c','packet.c'],'gs.exe'),(['particle.c'],'particle.exe'),(['blend.c'],'blend.exe'),(['selector.c'],'selector.exe'),(['style.c'],'style.exe'),(['menu.c'],'menu.exe'),(['probe.c'],'probe.exe'),(['projection.c'],'projection.exe'),(['derived.cpp'],'derived.exe'),(['rtc.c'],'rtc.exe')]:
        subprocess.run(flags+[str(out/n) for n in names]+['-o',str(out/exe)],check=True);subprocess.run([str(out/exe)],check=True)


if __name__=='__main__':main()
