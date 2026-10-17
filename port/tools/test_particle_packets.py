"""Execute shared matrix/particle packet APIs through the real native bodies."""
from pathlib import Path
import subprocess
import transform


def main():
    root=Path(__file__).resolve().parents[2]
    out=root/'build/port/runtime/particle-packets';out.mkdir(parents=True,exist_ok=True)
    for name in ('002239B0.c','00223A38.c','00223A68.c'):
        source=(root/'src/unsorted'/name).read_text()
        source=source.replace('#include "matrix_packets.h"',(root/'include/matrix_packets.h').read_text()).replace('#include "vector.h"',(root/'include/vector.h').read_text())
        (out/name).write_text('#include "port.h"\n'+transform.transform(source,False,'_',None))
    driver=out/'driver.c'
    driver.write_text(r'''#include "matrix_packets.h"
#include "ps2asm.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int ps2_vu0_run(ps2_cpu *cpu,unsigned address) { abort(); }
int main(void) {
 struct {unsigned before[4];Matrix4 matrix;unsigned after[4];} output={0};
 Matrix4 input={0}, identity={0};unsigned char overlap[96] __attribute__((aligned(16)));
 for(int i=0;i<4;i++){output.before[i]=0x12345678;output.after[i]=0x87654321;}
 input.row[0].x=2;input.row[0].y=-3;input.row[1].y=5;input.row[2].z=7;input.row[3].w=1;
 func_00223A38(&output.matrix,&input);
 if(memcmp(&output.matrix,&input,64))return 1;
 CopyMatrixAddressLegacy((int)&output.matrix,&input);
 func_00223A68(&identity);
 for(int row=0;row<4;row++)for(int col=0;col<4;col++)if(((float *)&identity.row[row])[col]!=(float)(row==col))return 2;
 IdentityMatrixAddressLegacy((int)&output.matrix);
 if(memcmp(&output.matrix,&identity,64))return 3;
 func_002239B0(&output.matrix,&input,&identity);
 if(memcmp(&output.matrix,&input,64))return 4;
 if(ComposeMatrixResultLegacy(&output.matrix,&output.matrix,&identity)!=(float *)&output.matrix || memcmp(&output.matrix,&input,64))return 5;
 for(int i=0;i<96;i++)overlap[i]=(unsigned char)i;
 func_00223A38((Matrix4 *)(overlap+16),(Matrix4 *)overlap);
 for(int i=0;i<64;i++)if(overlap[16+i]!=i)return 6;
 for(int i=0;i<4;i++)if(output.before[i]!=0x12345678 || output.after[i]!=0x87654321)return 7;
 if(ps2_asm_problems())return 8;
 puts("Shared matrix packet calls: copy overlap, identity, composition and 64-byte canaries pass");
 return 0;
}
''')
    exe=out/'test.exe'
    flags=['clang','--target=i686-pc-windows-msvc','-fuse-ld=lld','-O1','-w','-msse2','-mfpmath=sse','-I'+str(root/'port/include'),'-I'+str(root/'include'),'-I'+str(root/'port/runtime')]
    subprocess.run(flags+[str(out/n) for n in ('002239B0.c','00223A38.c','00223A68.c','driver.c')]+[str(root/'port/runtime/ps2asm.c'),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)
    descriptor=(root/'include/particle_descriptor.h').read_text()+r'''
#include <string.h>
#include <stddef.h>
#include <stdio.h>
static void *want_pos,*want_matrix,*want_desc,*want_offset;
int func_0022D1B0(struct Vector4 *pos,struct Matrix4 *matrix,ParticleEmitterDescription *description,struct Vector4 *offset) {
 if(pos!=want_pos || matrix!=want_matrix || description!=want_desc || offset!=want_offset)__builtin_trap();
 return description->type==0x39?-1:17;
}
int main(void) {
 struct {unsigned before[4];ParticleEmitterDescription description;unsigned after[4];} record={0};
 want_pos=(void *)1;want_matrix=(void *)2;want_desc=&record.description;want_offset=(void *)3;
 for(int i=0;i<4;i++){record.before[i]=0x12345678;record.after[i]=0x87654321;}
 if(sizeof(record.description)!=0xB0 || offsetof(ParticleEmitterDescription,x58)!=0x58 || offsetof(ParticleEmitterDescription,x60)!=0x60 || offsetof(ParticleEmitterDescription,xA0)!=0xA0)return 1;
 record.description.x58.all=((ParticleDescriptorFlags)1<<32)|((ParticleDescriptorFlags)1<<36);
 if(!record.description.x58.bits.fixed || !record.description.x58.bits.active)return 2;
 record.description.x58.bits.active=0;
 if(record.description.x58.all!=((ParticleDescriptorFlags)1<<32))return 3;
 for(int i=0;i<16;i++)record.description.x60[i]=(float)(i+1);
 record.description.type=0x39;if(SpawnParticleDescription(want_pos,want_matrix,want_desc,want_offset)!=-1)return 4;
 record.description.type=0x4C;if(SpawnParticleDescriptionDiscard(want_pos,want_matrix,want_desc,want_offset)!=17)return 5;
 for(int i=0;i<16;i++)if(record.description.x60[i]!=(float)(i+1))return 6;
 for(int i=0;i<4;i++)if(record.before[i]!=0x12345678 || record.after[i]!=0x87654321)return 7;
 puts("Shared particle descriptor: complete matrix, high DI flags, following pointers and result pass");return 0;
}
'''
    descriptor_path=out/'descriptor.c';descriptor_path.write_text('#include "port.h"\n'+transform.transform(descriptor,False,'_',None))
    descriptor_exe=out/'descriptor.exe'
    subprocess.run(flags+[str(descriptor_path),'-o',str(descriptor_exe)],check=True)
    subprocess.run([str(descriptor_exe)],check=True)


    # Execute the actual two spray bodies against bounded synthetic buffers.
    emitter_source=(root/'src/unsorted/0023E7C8.c').read_text()
    spray=emitter_source[:emitter_source.index('void func_0023E7C8(')]
    for name in ('func_0023F650','func_0023FAE8'):
        start=emitter_source.index('void '+name+'(');brace=emitter_source.index('{',start)
        spray+=emitter_source[start:transform.close_paren(transform.code_mask(emitter_source),brace)+1]+'\n'
    spray=spray.replace('#include "particle_spray.h"',(root/'include/particle_spray.h').read_text()).replace('#include "particle.h"',(root/'include/particle.h').read_text()).replace('#include "vector.h"',(root/'include/vector.h').read_text())
    spray+=r'''
#include "ps2asm.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
int ps2_vu0_run(ps2_cpu *cpu,unsigned address) { abort(); }
int D_002694D0[]={0x80402010};
int D_0027F6C0[]={0,85,170};
int main(void) {
 struct {unsigned before[4];Vec data[176];unsigned after[4];} vertices={0};
 struct {unsigned before[4];Vec data[86];unsigned after[4];} velocities={0};
 struct {unsigned before[4];short data[86];unsigned after[4];} lifetimes={0};
 struct {unsigned before[4];int data[98];unsigned after[4];} colors={0};
 Obj emitter={0};Vec position={0};position.f[0]=11;position.f[1]=22;position.f[2]=33;position.f[3]=1;
 for(int i=0;i<4;i++){vertices.before[i]=velocities.before[i]=lifetimes.before[i]=colors.before[i]=0x12345678;vertices.after[i]=velocities.after[i]=lifetimes.after[i]=colors.after[i]=0x87654321;}
 emitter.x2830=vertices.data;emitter.x2834=velocities.data;emitter.x2838=lifetimes.data;emitter.x283C=colors.data;
 emitter.x272C=1;emitter.x2730=0.25f;emitter.x273C=4;emitter.x2740=2;emitter.x2744=3;emitter.x27DA=4;emitter.x2854=4;
 emitter.x26B0.f[0]=emitter.x2640.f[0]=1;emitter.x26C0.f[1]=emitter.x2650.f[1]=1;emitter.x26D0.f[2]=emitter.x2660.f[2]=1;
 emitter.x2690.f[0]=emitter.x2630.f[0]=11;emitter.x2690.f[1]=emitter.x2630.f[1]=22;emitter.x2690.f[2]=emitter.x2630.f[2]=33;
 for(int kind=0;kind<2;kind++) {
  emitter.x285C=emitter.x285D=emitter.x285E=0;
  for(int i=0;i<2;i++) {
   if(kind)EmitParticleSprayVariantLegacy(&emitter,&position,-999);else EmitParticleSprayLegacy(&emitter,&position,999);
   if(emitter.x285C!=i+1 || emitter.x285D!=i+1 || emitter.x285E!=i+1 || emitter.x2850!=i+1)return 1;
   for(int j=0;j<3;j++) {
    if(lifetimes.data[i+j*42]!=4)return 2;
    if(colors.data[i+j*48]!=(kind?(j==0?0x80242460:(j==1?0x80246024:0x80603030)):0x80402010))return 3;
    for(int k=0;k<4;k++)if(!isfinite(vertices.data[i*2+j*85].f[k]) || !isfinite(velocities.data[i+j*42].f[k]))return 4;
   }
  }
 }
 for(int i=0;i<4;i++)if(vertices.before[i]!=0x12345678 || vertices.after[i]!=0x87654321 || velocities.before[i]!=0x12345678 || velocities.after[i]!=0x87654321 || lifetimes.before[i]!=0x12345678 || lifetimes.after[i]!=0x87654321 || colors.before[i]!=0x12345678 || colors.after[i]!=0x87654321)return 5;
 if(ps2_asm_problems())return 6;
 puts("Real spray emitters: three sections, finite vertices, colors, lifetimes, counts and buffer canaries pass");return 0;
}
'''
    spray_path=out/'spray.c';spray_path.write_text('#include "port.h"\n'+transform.transform(spray,False,'_',None))
    spray_exe=out/'spray.exe'
    subprocess.run(flags+[str(spray_path),str(root/'port/runtime/ps2asm.c'),'-o',str(spray_exe)],check=True)
    subprocess.run([str(spray_exe)],check=True)

    unit=(root/'src/unsorted/00215B68.c').read_text();start=unit.index('int func_00216E28(');brace=unit.index('{',start)
    prepare_body=unit[start:transform.close_paren(transform.code_mask(unit),brace)+1]
    prepared='''#include "vif_transforms.h"
#include "matrix_packets.h"
#include <string.h>
#include <stdio.h>
typedef VifTransformSource Obj216E28;
typedef VifTransformState State216E28;
Matrix4 matrices[4],view;
Matrix4 *D_0029E1A0=&matrices[0],*D_0029E1A4=&matrices[1],*D_0029E1A8=&matrices[2],*D_0029E1AC=&matrices[3];
float *func_0022FD90(void) { return (float *)&view; }
void func_00223950(void *out,float *a,float *b) { memcpy(out,b,64); }
void func_00216EF0(void *out,const float *a,const float *b) { memcpy(out,b,64); }
'''+prepare_body+'''
int main(void) {
 Obj216E28 object={0};State216E28 state={0};
 struct {unsigned before[4];Vector4 vector;unsigned after[4];} result={0};
 object.x10=1;object.x14=2;object.x18=3;state.x20=4;state.x24=5;state.x28=6;
 for(int i=0;i<4;i++){object.matrix[i*5]=1;result.before[i]=0x12345678;result.after[i]=0x87654321;}
 for(int i=0;i<16;i++){state.x70[i]=(float)(i+1);state.xB0[i]=(float)(i+21);((float *)&view)[i]=(float)(i+41);}
 if(func_00216E28(&object,&state,&result.vector)!=1 || PrepareVifTransformsDiscard(&object,&state,&result.vector)!=1 || PrepareVifTransformsAddress((int)&object,&state,&result.vector)!=1)return 1;
 if(result.vector.x!=5 || result.vector.y!=7 || result.vector.z!=9 || result.vector.w!=0)return 2;
 if(memcmp(matrices[0].row,object.matrix,64) || memcmp(matrices[1].row,state.x70,64) || memcmp(matrices[2].row,state.xB0,64) || memcmp(&matrices[3],&view,64))return 3;
 for(int i=0;i<4;i++)if(result.before[i]!=0x12345678 || result.after[i]!=0x87654321)return 4;
 puts("Real VIF transform preparation: full input/output vectors, initialized native padding lane, matrices and status pass");return 0;
}
'''
    prepared=prepared.replace('#include "vif_transforms.h"',(root/'include/vif_transforms.h').read_text()).replace('#include "matrix_packets.h"',(root/'include/matrix_packets.h').read_text()).replace('#include "vector.h"',(root/'include/vector.h').read_text())
    prepare_path=out/'prepare.c';prepare_path.write_text('#include "port.h"\n'+transform.transform(prepared,False,'_',None))
    apply_source=(root/'src/unsorted/00221888.c').read_text().replace('#include "vector.h"',(root/'include/vector.h').read_text())
    apply_path=out/'apply.c';apply_path.write_text('#include "port.h"\n'+transform.transform(apply_source,False,'_',None))
    prepare_exe=out/'prepare.exe'
    # Reuse the no-microprogram stub from a separate tiny runtime shim.
    shim=out/'shim.c';shim.write_text('#include "ps2asm.h"\n#include <stdlib.h>\nint ps2_vu0_run(ps2_cpu *p,unsigned a) {abort();}\n')
    subprocess.run(flags+[str(prepare_path),str(apply_path),str(out/'00223A38.c'),str(shim),str(root/'port/runtime/ps2asm.c'),'-o',str(prepare_exe)],check=True)
    subprocess.run([str(prepare_exe)],check=True)


if __name__=='__main__':main()
