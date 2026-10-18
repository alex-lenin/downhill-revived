/* Complete VU0 0000..00B8 culling entry. The caller checks every instruction
   word and VI0 before entering, under the VU toward-zero / FTZ / DAZ MXCSR.
   No memory, divider or clipping instructions occur in this program.

   Its fixed schedule lets the five delayed flag reads use their actual
   producers: FMAND 0078/0080 see 0058/0060, FSAND 0090/0098 see 0070/0078,
   FMAND 00A8 sees 0088. Sticky flags include every FMAC, including ACC writes.
   Only the final current flags replace the input; D/I and all sticky bits
   remain. Masks preserve every untouched VF lane. No guest code is embedded. */
static unsigned native0_mac(__m128 v, unsigned mask)
{
    Upper up; fmac_flags4(&up, v, mask); return up.mac;
}
static unsigned native0_status(unsigned mac)
{
    return (mac & 15 ? 1 : 0) | (mac & 240 ? 2 : 0);
}
static __m128 native0_add(__m128 a, __m128 b) { return _mm_add_ps(operand4(a), operand4(b)); }
static __m128 native0_sub(__m128 a, __m128 b) { return _mm_sub_ps(operand4(a), operand4(b)); }
static __m128 native0_mul(__m128 a, __m128 b) { return _mm_mul_ps(operand4(a), operand4(b)); }
static __m128 native0_madd(__m128 a, __m128 b, __m128 acc)
{ return _mm_add_ps(operand4(acc), native0_mul(a,b)); }
static void native0_cull(ps2_cpu *cpu)
{
    Vu view; view.c=cpu;
    unsigned sticky=0, mac, sx, sy;
    __m128 v31=native0_add(vf4(&view,5),vf4(&view,4));
    store_fields(cpu->vf[31].f,_mm_castps_si128(v31),14);
    sticky |= native0_status(native0_mac(v31,14));
    __m128 radius=lane4(vf4(&view,5),3);
    __m128 offset=native0_add(_mm_setzero_ps(),radius);
    store_fields(cpu->vf[17].f,_mm_castps_si128(offset),8);
    sticky |= native0_status(native0_mac(offset,8));
    __m128 neg_radius=native0_mul(radius,_mm_set1_ps(1.0f));
    store_fields(cpu->vf[11].f,_mm_castps_si128(neg_radius),1);
    sticky |= native0_status(native0_mac(neg_radius,1));
    __m128 acc=native0_mul(vf4(&view,7),lane4(v31,0));
    sticky |= native0_status(native0_mac(acc,15));
    acc=native0_madd(vf4(&view,8),lane4(v31,1),acc);
    sticky |= native0_status(native0_mac(acc,15));
    __m128 v29=native0_madd(vf4(&view,9),lane4(v31,2),acc);
    store_fields(cpu->vf[29].f,_mm_castps_si128(v29),15);
    sticky |= native0_status(native0_mac(v29,15));
    acc=native0_mul(vf4(&view,15),lane4(v31,0));
    sticky |= native0_status(native0_mac(acc,15));
    acc=native0_madd(vf4(&view,16),lane4(v31,1),acc);
    sticky |= native0_status(native0_mac(acc,15));
    store_fields(cpu->vacc,_mm_castps_si128(acc),15);
    __m128 v30=native0_madd(vf4(&view,19),lane4(v31,2),acc);
    store_fields(cpu->vf[30].f,_mm_castps_si128(v30),15);
    sticky |= native0_status(native0_mac(v30,15));
    __m128 v27=native0_add(v29,offset);
    store_fields(cpu->vf[27].f,_mm_castps_si128(v27),14);
    mac=native0_mac(v27,14); cpu->vi[3]=mac & 0xc0; sticky |= native0_status(mac);
    __m128 v25=native0_sub(v29,neg_radius);
    store_fields(cpu->vf[25].f,_mm_castps_si128(v25),14);
    mac=native0_mac(v25,14); cpu->vi[4]=mac & 0xc0; sticky |= native0_status(mac);
    __m128 v28=native0_add(v30,offset);
    store_fields(cpu->vf[28].f,_mm_castps_si128(v28),14);
    mac=native0_mac(v28,14); sx=native0_status(mac)&2; sticky |= native0_status(mac);
    __m128 v26=native0_sub(v30,neg_radius);
    store_fields(cpu->vf[26].f,_mm_castps_si128(v26),14);
    mac=native0_mac(v26,14); sy=native0_status(mac)&2; sticky |= native0_status(mac);
    __m128 v10=native0_add(vf4(&view,10),lane4(v27,2));
    store_fields(cpu->vf[10].f,_mm_castps_si128(v10),8);
    mac=native0_mac(v10,8); sticky |= native0_status(mac);
    cpu->vi[6]=0xc0; cpu->vi[7]=0x80;
    cpu->vi[3]=mac & 0x80;
    cpu->vi[2]=sx | (native0_mac(v27,14)&0xc0) | cpu->vi[3];
    cpu->vi[5]=sy | cpu->vi[4];
    cpu->i=1.0f;
    cpu->vu0_ctrl[0]=(cpu->vu0_ctrl[0]&0xff0u) | native0_status(mac) | (sticky<<6);
    cpu->vu0_ctrl[2]&=0xffffffu;
    cpu->vu0_ctrl[1]=mac; cpu->vu0_ctrl[10]=0xc0;
}

/* 05E8..0758: transpose the local basis, then transform its three axes by
   each of four matrices. There are no flag reads, branches, memory accesses
   or divider operations. Retain the intermediate ACC and every sticky FMAC. */
static void native0_record(__m128 value, unsigned mask, unsigned *sticky, unsigned *mac)
{
    *mac=native0_mac(value,mask); *sticky |= native0_status(*mac);
}
static __m128 native0_transform(Vu *view, unsigned first, unsigned second, unsigned third,
                               __m128 axis, unsigned mask, unsigned *sticky, unsigned *mac)
{
    __m128 acc=native0_mul(vf4(view,first),lane4(axis,0));
    native0_record(acc,mask,sticky,mac);
    acc=native0_madd(vf4(view,second),lane4(axis,1),acc);
    native0_record(acc,mask,sticky,mac);
    store_fields(view->c->vacc,_mm_castps_si128(acc),mask);
    __m128 result=native0_madd(vf4(view,third),lane4(axis,2),acc);
    native0_record(result,mask,sticky,mac);
    return result;
}
static void native0_transform_basis(ps2_cpu *cpu)
{
    Vu view;view.c=cpu;unsigned sticky=0,mac=0;
    __m128 origin=native0_add(vf4(&view,4),vf4(&view,5));
    native0_record(origin,14,&sticky,&mac);
    store_fields(cpu->vf[31].f,_mm_castps_si128(origin),14);
    __m128 axis[3]={vf4(&view,1),vf4(&view,2),vf4(&view,3)};
    for(unsigned j=0;j<3;j++)store_fields(cpu->vf[6+j].f,_mm_castps_si128(axis[j]),15);
    __m128 v=native0_add(_mm_setzero_ps(),lane4(axis[2],0));
    native0_record(v,2,&sticky,&mac);store_fields(cpu->vf[1].f,_mm_castps_si128(v),2);
    v=native0_add(_mm_setzero_ps(),lane4(axis[2],1));
    native0_record(v,2,&sticky,&mac);store_fields(cpu->vf[2].f,_mm_castps_si128(v),2);
    v=native0_add(_mm_setzero_ps(),lane4(axis[0],2));
    native0_record(v,8,&sticky,&mac);store_fields(cpu->vf[3].f,_mm_castps_si128(v),8);
    v=native0_add(_mm_setzero_ps(),lane4(axis[1],0));
    native0_record(v,4,&sticky,&mac);store_fields(cpu->vf[1].f,_mm_castps_si128(v),4);
    v=native0_add(_mm_setzero_ps(),lane4(axis[0],1));
    native0_record(v,8,&sticky,&mac);store_fields(cpu->vf[2].f,_mm_castps_si128(v),8);
    /* 0628's ADD does not modify a source of the adjacent matrix multiply.
       Its flags are sticky only, so collect it before that multiply. */
    v=native0_add(_mm_setzero_ps(),lane4(axis[1],2));
    native0_record(v,4,&sticky,&mac);store_fields(cpu->vf[3].f,_mm_castps_si128(v),4);
    v=native0_transform(&view,9,10,11,axis[0],15,&sticky,&mac);
    store_fields(cpu->vf[21].f,_mm_castps_si128(v),15);
    v=native0_transform(&view,1,2,3,origin,14,&sticky,&mac);
    store_fields(cpu->vf[4].f,_mm_castps_si128(v),14);
    for(unsigned j=1;j<3;j++) {
        v=native0_transform(&view,9,10,11,axis[j],15,&sticky,&mac);
        store_fields(cpu->vf[21+j].f,_mm_castps_si128(v),15);
    }
    const unsigned matrices[3][4]={{13,14,15,25},{17,18,19,9},{12,16,20,28}};
    for(unsigned m=0;m<3;m++)for(unsigned j=0;j<3;j++) {
        v=native0_transform(&view,matrices[m][0],matrices[m][1],matrices[m][2],axis[j],15,&sticky,&mac);
        store_fields(cpu->vf[matrices[m][3]+j].f,_mm_castps_si128(v),15);
    }
    cpu->vu0_ctrl[0]=(cpu->vu0_ctrl[0]&0xff0u)|native0_status(mac)|(sticky<<6);
    cpu->vu0_ctrl[2]&=0xffffffu;
    cpu->vu0_ctrl[1]=mac;cpu->vu0_ctrl[10]=0x760;
}

static __m128 native0_dot3(Vu *view, unsigned first, unsigned second, unsigned third,
                          unsigned a, unsigned b, unsigned c, unsigned *sticky, unsigned *mac)
{
    __m128 acc=native0_mul(vf4(view,first),vf4(view,a));
    native0_record(acc,15,sticky,mac);
    acc=native0_madd(vf4(view,second),vf4(view,b),acc);
    native0_record(acc,15,sticky,mac);
    store_fields(view->c->vacc,_mm_castps_si128(acc),15);
    __m128 result=native0_madd(vf4(view,third),vf4(view,c),acc);
    native0_record(result,15,sticky,mac);
    return result;
}

/* 02E0..04E8: transformed bounding volumes. MR32 moves keep raw bits and
   execute after their pair's arithmetic reads. Final FMANDs observe the four
   separate 04A0/04A8/04B0/04B8 producers, not just the last MAC. */
static void native0_bounds(ps2_cpu *cpu)
{
    Vu view;view.c=cpu;unsigned sticky=0,mac=0;
    __m128 loaded=_mm_loadu_ps(cpu->vu0_mem[0].f);
    __m128 point=vf4(&view,4);
    __m128 v=native0_transform(&view,7,8,9,point,15,&sticky,&mac);
    store_fields(cpu->vf[17].f,_mm_castps_si128(v),15);
    store_fields(cpu->vf[10].f,_mm_castps_si128(loaded),15);
    v=native0_transform(&view,15,16,19,point,15,&sticky,&mac);
    store_fields(cpu->vf[18].f,_mm_castps_si128(v),15);
    v=native0_add(vf4(&view,17),lane4(loaded,0));
    native0_record(v,2,&sticky,&mac);store_fields(cpu->vf[17].f,_mm_castps_si128(v),2);
    for(unsigned axis=0;axis<3;axis++) {
        v=native0_dot3(&view,11,12,13,15,16,19,&sticky,&mac);
        store_fields(cpu->vf[28-axis].f,_mm_castps_si128(v),15);
        v=native0_dot3(&view,11,12,13,7,8,9,&sticky,&mac);
        store_fields(cpu->vf[25-axis].f,_mm_castps_si128(v),15);
        if(axis<2)for(unsigned r=11;r<=13;r++) {
            __m128 old=vf4(&view,r);
            __m128 rotated=_mm_shuffle_ps(old,old,_MM_SHUFFLE(0,3,2,1));
            store_fields(cpu->vf[r].f,_mm_castps_si128(rotated),15);
            __m128 z=native0_add(_mm_setzero_ps(),lane4(rotated,3));
            native0_record(z,2,&sticky,&mac);
            store_fields(cpu->vf[r].f,_mm_castps_si128(z),2);
        }
    }
    for(unsigned r=23;r<=28;r++) {
        v=_mm_and_ps(vf4(&view,r),_mm_castsi128_ps(_mm_set1_epi32(0x7fffffff)));
        store_fields(cpu->vf[r].f,_mm_castps_si128(v),14);
    }
    v=native0_add(vf4(&view,28),vf4(&view,27));
    native0_record(v,14,&sticky,&mac);store_fields(cpu->vf[22].f,_mm_castps_si128(v),14);
    v=native0_add(vf4(&view,25),vf4(&view,24));
    native0_record(v,14,&sticky,&mac);store_fields(cpu->vf[21].f,_mm_castps_si128(v),14);
    point=vf4(&view,5);
    v=native0_transform(&view,7,8,9,point,15,&sticky,&mac);
    store_fields(cpu->vf[28].f,_mm_castps_si128(v),15);
    v=native0_transform(&view,15,16,19,point,15,&sticky,&mac);
    store_fields(cpu->vf[27].f,_mm_castps_si128(v),15);
    v=native0_add(vf4(&view,22),vf4(&view,26));
    native0_record(v,14,&sticky,&mac);store_fields(cpu->vf[22].f,_mm_castps_si128(v),14);
    v=native0_add(vf4(&view,21),vf4(&view,23));
    native0_record(v,14,&sticky,&mac);store_fields(cpu->vf[21].f,_mm_castps_si128(v),14);
    v=native0_add(vf4(&view,28),vf4(&view,17));
    native0_record(v,14,&sticky,&mac);store_fields(cpu->vf[20].f,_mm_castps_si128(v),14);
    v=native0_add(vf4(&view,27),vf4(&view,18));
    native0_record(v,14,&sticky,&mac);store_fields(cpu->vf[19].f,_mm_castps_si128(v),14);
    v=native0_add(vf4(&view,20),vf4(&view,21));
    native0_record(v,14,&sticky,&mac);store_fields(cpu->vf[18].f,_mm_castps_si128(v),14);
    cpu->vi[3]=mac&0xc0;
    v=native0_add(vf4(&view,19),vf4(&view,22));
    native0_record(v,14,&sticky,&mac);store_fields(cpu->vf[17].f,_mm_castps_si128(v),14);
    cpu->vi[4]=mac&0xe0;
    v=native0_sub(vf4(&view,20),vf4(&view,21));
    native0_record(v,14,&sticky,&mac);store_fields(cpu->vf[27].f,_mm_castps_si128(v),14);
    cpu->vi[5]=mac&0xc0;
    v=native0_sub(vf4(&view,19),vf4(&view,22));
    native0_record(v,14,&sticky,&mac);store_fields(cpu->vf[28].f,_mm_castps_si128(v),14);
    cpu->vi[6]=mac&0xe0;cpu->vi[9]=0xe0;cpu->vi[10]=0xc0;
    cpu->vi[2]=cpu->vi[3]|cpu->vi[4];cpu->vi[5]|=cpu->vi[6];
    cpu->vu0_ctrl[0]=(cpu->vu0_ctrl[0]&0xff0u)|native0_status(mac)|(sticky<<6);
    cpu->vu0_ctrl[2]&=0xffffffu;
    cpu->vu0_ctrl[1]=mac;cpu->vu0_ctrl[10]=0x4f0;
}

/* 07B8..0800: squared length of a translated point. ACC.x keeps x*x+y*y;
   VF6.x gets its separate final addition of z*z. Other ACC/VF6 lanes survive. */
static void native0_length_squared(ps2_cpu *cpu)
{
    Vu view;view.c=cpu;unsigned sticky=0,mac=0;
    __m128 point=native0_add(vf4(&view,1),vf4(&view,4));
    native0_record(point,14,&sticky,&mac);store_fields(cpu->vf[3].f,_mm_castps_si128(point),14);
    __m128 one=native0_add(_mm_setzero_ps(),_mm_set1_ps(1.0f));
    native0_record(one,14,&sticky,&mac);store_fields(cpu->vf[7].f,_mm_castps_si128(one),14);
    __m128 squared=native0_mul(point,point);
    native0_record(squared,14,&sticky,&mac);store_fields(cpu->vf[5].f,_mm_castps_si128(squared),14);
    __m128 acc=native0_mul(point,lane4(point,0));
    native0_record(acc,8,&sticky,&mac);
    acc=native0_madd(one,lane4(squared,1),acc);
    native0_record(acc,8,&sticky,&mac);store_fields(cpu->vacc,_mm_castps_si128(acc),8);
    __m128 result=native0_madd(one,lane4(squared,2),acc);
    native0_record(result,8,&sticky,&mac);store_fields(cpu->vf[6].f,_mm_castps_si128(result),8);
    cpu->vu0_ctrl[0]=(cpu->vu0_ctrl[0]&0xff0u)|native0_status(mac)|(sticky<<6);
    cpu->vu0_ctrl[2]&=0xffffffu;
    cpu->vu0_ctrl[1]=mac;cpu->vu0_ctrl[10]=0x808;
}

/* 0808..0828: a direction transformed by three rows, with homogeneous W=1. */
static void native0_transform_direction(ps2_cpu *cpu)
{
    Vu view;view.c=cpu;unsigned sticky=0,mac=0;
    __m128 one=native0_add(_mm_set1_ps(1.0f),_mm_setzero_ps());
    native0_record(one,1,&sticky,&mac);store_fields(cpu->vf[6].f,_mm_castps_si128(one),1);
    __m128 result=native0_transform(&view,1,2,3,vf4(&view,4),14,&sticky,&mac);
    store_fields(cpu->vf[6].f,_mm_castps_si128(result),14);
    cpu->vu0_ctrl[0]=(cpu->vu0_ctrl[0]&0xff0u)|native0_status(mac)|(sticky<<6);
    cpu->vu0_ctrl[2]&=0xffffffu;
    cpu->vu0_ctrl[1]=mac;cpu->vu0_ctrl[10]=0x830;
}
