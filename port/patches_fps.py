"""Port-only real-step corrections. Retail sources/checks remain untouched.

Every edit is guarded at runtime; 60 takes the pre-unlock arithmetic. Timestamp
storage stays integer: high-rate animation timestamps use 1/1024-field units.
"""
from pathlib import Path
import re


def add(patches):
    root = Path(__file__).resolve().parents[1] / 'src' / 'unsorted'

    def edit(unit, old, new):
        paths = list(root.glob(unit + '.*'))
        assert len(paths) == 1, unit
        patches.setdefault('unsorted/' + paths[0].name, []).append(('text', old, new))

    def entry(unit, name, code):
        path = next(root.glob(unit + '.*'))
        text = path.read_text()
        m = re.search(r'\b' + name + r'\([^;{}]*\)\s*\{', text)
        assert m, name
        edit(unit, m[0], m[0] + '\n' + code + '\n')

    # Input edges must expire after each real update, even when the integer
    # legacy clock is unchanged. Keep timers/cooldowns in their original units.
    patches.setdefault('unsorted/001909A8.c', []).append(('re',
        (r'func_00238F48\(\)',2),'port_input_stamp(func_00238F48())'))
    edit('0021F8E0','int next=n-1;',
         'int next=n-(port_fps_high() ? port_whole_step() : 1);')
    edit('0021F8E0','++D_00660D60[k]',
         '(D_00660D60[k]+=port_fps_high() ? port_whole_step() : 1)')
    edit('0017EB00','obj->counts[index]--;',
         'obj->counts[index]-=port_fps_high() ? port_whole_step() : 1;')
    edit('0017E440','else obj->x3EC++;',
         'else obj->x3EC+=port_fps_high() ? port_whole_step() : 1;')

    # Reset must take place after the original initializer too.
    edit('00238A68', '*(volatile int *)0x10000800 = 0;',
         '*(volatile int *)0x10000800 = 0;\n    if (port_fps_high()) port_clock_reset();')
    entry('00238C00', 'func_00238C00',
          '    if (port_fps_high()) { port_clock_wait(result); return 1; }')
    entry('00238D98', 'func_00238D98',
          '    if (port_fps_high()) { port_clock_step(); return; }')
    entry('0011A100', 'func_0011A100','    port_fps_race_step();')
    edit('00135A30','func_0012DDC0(o->x158);',
         'func_0012DDC0(o->x158); port_fps_contact_recovery(o);')
    edit('001336F8','func_00231C10(obj);','port_fps_force_window(obj,1); func_00231C10(obj);')

    # AI's rotating expensive-query slot and seven-second leader refresh are
    # field schedules, not integration work. A held slot must not run its
    # queries repeatedly on the intervening fractional updates.
    edit('0011A100', '++race->x501B4',
         '(race->x501B4 += port_fps_high() ? port_whole_step() : 1)')
    edit('0011A100', '--race->x501C0',
         '(race->x501C0 -= port_fps_high() ? port_whole_step() : 1)')
    for unit,expression in (
            ('0010EC80','*(int *)(obj->x0 + 0x797C) == D_002AF2C0.x501B4'),
            ('0010F0D8','obj->x0->x797C == D_002AF2C0.x501B4'),
            ('0010FB00','world->x797C==D_002AF2C0.x501B4')):
        edit(unit,expression,'((!port_fps_high() || port_whole_step()) && ('+expression+'))')
    edit('00112360','o->x0->x797C != D_002AF2C0.x501B4',
         '(port_fps_high() && !port_whole_step()) || o->x0->x797C != D_002AF2C0.x501B4')
    # Failed trick selections can be retried immediately: the cooldown is
    # zero until a choice succeeds. Polling that RNG on fractional updates
    # raises the probability of starting a trick and shifts later AI rolls.
    entry('00110870','func_00110870',
          '    if (port_fps_high() && !port_whole_step()) return;')
    edit('0010C940','else if(func_00222BD8(0,1))',
         'else if(port_fps_high() && !port_whole_step()) return;\n else if(func_00222BD8(0,1))')

    for unit in ('00136790', '00136B40', '00136C20', '00136F80'):
        path = next(root.glob(unit + '.*'))
        text = path.read_text()
        # Do not rewrite the accessor declaration.
        calls = list(re.finditer(r'func_00238F38\(\)', text))
        assert len(calls) == 1, (unit, len(calls))
        edit(unit, 'func_00238F38()', '(port_fps_high() ? 60.0f : func_00238F38())')
    edit('00136038', 'SCALE(o->x70,o->x70,0.9f);',
         'SCALE(o->x70,o->x70,port_decay(0.9f)); port_fps_force_window(o,0);')
    # 00231CC0 applies these tire vectors directly to linear/angular impulse,
    # not the force accumulator. A fixed Coulomb cap per contact update would
    # give N/60 times the friction per second. Scale the cap, not the
    # unconstrained collision impulse needed to cancel relative motion.
    edit('00132CE0','pos = &a->x50;',
         'if (port_fps_high()) friction*=port_step();\n    pos = &a->x50;')
    edit('00161B10','bike->x434 > D_0029C7D0',
         'bike->x434 > (port_fps_high() ? D_0029C7D0*port_step() : D_0029C7D0)')
    # Sliding tire modes bypass the Coulomb cap above: these are sustained
    # per-contact linear/angular impulses, not impact restitution.
    edit('0016AFC0','VecScale(out, &sum, D_0029C864);',
         'VecScale(out, &sum, port_fps_high() ? D_0029C864*port_step() : D_0029C864);')
    edit('0016ABC8','scale=func_0016AD98(D_0026EBE8,obj->xC);',
         'scale=func_0016AD98(D_0026EBE8,obj->xC); if (port_fps_high()) scale*=port_step();')
    edit('0016ACE8','"f"(-func_0016AD98(D_0026EC18, obj->xC))',
         '"f"(-func_0016AD98(D_0026EC18, obj->xC)*(port_fps_high() ? port_step() : 1.0f))')
    edit('00136F80','n = obj->x158->x7B00 - 1;',
         'n = obj->x158->x7B00 - (port_fps_high() ? port_whole_step() : 1);')
    # The yaw/pitch velocities are applied with dt downstream (00137C78/E68),
    # but their acceleration and release were still per-update constants.
    edit('00137950', 'float step = accel * input;',
         'float step = accel * input; if (port_fps_high()) step*=port_step();')
    edit('00137950', 'float next = vel * decay;', 'float next = vel * port_decay(decay);')
    edit('00137B70', 'obj->x454 * pitchInput * scale',
         'obj->x454 * pitchInput * scale * (port_fps_high() ? port_step() : 1.0f)')
    edit('00137B70', 'obj->x240 * obj->x470', 'obj->x240 * port_decay(obj->x470)')
    edit('00137300', 'VecScale(specialDir, specialDir, D_0029C3B0);',
         'VecScale(specialDir, specialDir, port_decay(D_0029C3B0));')
    edit('00137300', 'specialScale = specialOne - D_0029C3B0;',
         'specialScale = specialOne - port_decay(D_0029C3B0);')
    edit('00137300', 'dir = &obj->x270;',
         'if (port_fps_high()) scale=port_decay(scale);\n    dir = &obj->x270;')
    edit('001AF5E8', 'amount = amount * 0.05f + old * 0.95f;',
         'amount = port_fps_high() ? old+(amount-old)*port_blend(0.05f) : amount * 0.05f + old * 0.95f;')
    # Remaining manual object/UI motion found outside the accessor callers.
    for unit,expression in [('001926E0','obj->x8->xA0+=0.015f;'),
                            ('001926E0','obj->x8->xA8+=0.003f;'),
                            ('0017C168','(*item)->x4c += 0.05f;')]:
        value=expression.rstrip(';').split('+=')[1].strip()
        edit(unit,expression,expression.replace(value,'(port_fps_high() ? '+value+'*port_step() : '+value+')'))
    edit('001926E0','v.v[1]=speed;', 'v.v[1]=port_fps_high() ? speed*port_step() : speed;')
    edit('0017C130','child->x10 += obj->x1288;',
         'child->x10 += port_fps_high() ? obj->x1288*port_step() : obj->x1288;')
    edit('0017C130','obj->x1314--;','obj->x1314-=port_fps_high() ? port_whole_step() : 1;')
    edit('0017C168','--obj->x1318','(obj->x1318-=port_fps_high() ? port_whole_step() : 1)')
    edit('0016FC00','obj->x224 += 0.03f;',
         'obj->x224 += port_fps_high() ? 0.03f*port_step() : 0.03f;')
    edit('0016FC00','diff < obj->x224',
         'diff < (port_fps_high() ? obj->x224*port_step() : obj->x224)')
    edit('0016FC00','obj->x1F0->x10.z -= obj->x224;',
         'obj->x1F0->x10.z -= port_fps_high() ? obj->x224*port_step() : obj->x224;')
    edit('00142940','w = length * 0.9992f + D_0029DD3C * 1.2f - obj->pos[2] * -0.008f / length;',
         'w = port_fps_high() ? length*port_decay(0.9992f)+(D_0029DD3C*1.2f-obj->pos[2]*-0.008f/length)*(port_particle_sum(0.9992f)/0.9992f) : length * 0.9992f + D_0029DD3C * 1.2f - obj->pos[2] * -0.008f / length;')
    edit('001B0410','obj->x20++;','obj->x20+=port_fps_high() ? port_whole_step() : 1;')
    for unit,expression in [('001B4688','--obj->x8'),('001B4718','--obj->timer')]:
        edit(unit,expression,'('+expression[2:]+'-=port_fps_high() ? port_whole_step() : 1)')
    edit('001B4900','D_00437340.x508--;',
         'D_00437340.x508-=port_fps_high() ? port_whole_step() : 1;')
    for field in ('x14','x18'):
        edit('001634E0','drain += gauge->'+field+';',
             'drain += port_fps_high() ? gauge->'+field+'*port_step() : gauge->'+field+';')
    # Despite its provisional decomp name "dt", component +2110 is 1.0
    # in both 60 and 144 live state, not seconds/fields per real step.
    # Recovery and base depletion therefore need the same time conversion.
    edit('001634E0','gauge->fill * gauge->x24 * gauge->x28 * gauge->game->dt',
         'gauge->fill * gauge->x24 * gauge->x28 * gauge->game->dt * (port_fps_high() ? port_step() : 1.0f)')
    edit('001634E0','gauge->drain * gauge->game->dt;',
         'gauge->drain * gauge->game->dt * (port_fps_high() ? port_step() : 1.0f);')
    for unit,expression in [('001737F0','obj->xC->pos.z += 0.5f;'),
                            ('00173CB0','obj->xC->pos[2]+=0.5f;'),
                            ('00173CB0','obj->xC->pos[2]-=0.5f;')]:
        edit(unit,expression,expression.replace('0.5f','(port_fps_high() ? 0.5f*port_step() : 0.5f)'))
    # All seven retail callers apply a partial delta quaternion every update
    # (camera attitude / ground-following actors). This is the shared nlerp
    # toward identity, rather than a one-shot asset-space quaternion scale.
    edit('00223578','{\n\n    __asm__ volatile ("lqc2 $vf11, 0(%0)"',
         '{\n    if (port_fps_high()) scale=port_blend(scale);\n    __asm__ volatile ("lqc2 $vf11, 0(%0)"')
    edit('00165B90','obj->x1A8++',
         '(port_fps_high() ? (obj->x1A8+=port_whole_step())-port_whole_step() : obj->x1A8++)')
    edit('00148F60','progress = obj->x2F0 + obj->x2F4;',
         'progress = obj->x2F0 + (port_fps_high() ? obj->x2F4*port_step() : obj->x2F4);')
    edit('00148F60','obj->x2F0 += obj->x2F4;',
         'obj->x2F0 += port_fps_high() ? obj->x2F4*port_step() : obj->x2F4;')
    patches['unsorted/00148F60.cpp'].append(('re',(r'obj->x2F0 -= obj->x2F8',2),
        'obj->x2F0 -= port_fps_high() ? obj->x2F8*port_step() : obj->x2F8'))
    patches['unsorted/00148F60.cpp'].append(('re',(r'obj->xA4 \* obj->x300',3),
        '(obj->xA4 * obj->x300 * (port_fps_high() ? port_step() : 1.0f))'))
    # Two held directions, one release decay.
    patches.setdefault('unsorted/00138658.cpp', []).append(
        ('re', (r'rate\*0\.03f', 2), 'rate*(port_fps_high() ? 0.03f*port_step() : 0.03f)'))
    edit('00138658', 'o->x488*=0.95f;', 'o->x488*=port_decay(0.95f);')
    edit('00135A30', '++o->x4A4;',
         'if (port_fps_high()) o->x4A4=port_air_tick(o,o->x4A4); else ++o->x4A4;')
    edit('00135A30', 'if (o->x4A4 == 1)',
         'if (port_fps_high() ? port_air_first(o,o->x4A4) : o->x4A4 == 1)')
    edit('00135A30', 'o->x48C *= D_0029C30C;', 'o->x48C *= port_decay(D_0029C30C);')
    for unit, expression, body in [('00136038','(float)o->x4a4','o'),
                                    ('00138658','(float)o->x4A4','o'),
                                    ('00137E68','(float)obj->x4a4','obj')]:
        path=next(root.glob(unit+'.*'))
        count=path.read_text().count(expression)
        patches.setdefault('unsorted/'+path.name,[]).append(('re',(re.escape(expression),count),
            '(port_fps_high() ? port_air_age(%s,%s) : %s)' %
            (body,expression.removeprefix('(float)'),expression)))
    edit('00148F60', 'angle=(float)obj->x364*0.017453294f;',
         'angle=port_counter_fields(obj->x364)*0.017453294f;')
    # The pedal kick is a per-update impulse: spread it over the real steps.
    edit('00136790', 'len=strength*D_0029C31C;',
         'len=strength*D_0029C31C; if (port_fps_high()) len*=port_step();')
    edit('0012F6A0', '--obj->x7A74', '(obj->x7A74 -= port_fps_high() ? port_whole_step() : 1)')
    edit('0012F5D0', 'obj->x7a3c += speed;',
         'obj->x7a3c += port_fps_high() ? speed*port_step() : speed;')
    edit('001AD2B8', 'ctrl->x34 += 0.05f;',
         'ctrl->x34 += port_fps_high() ? 0.05f*port_step() : 0.05f;')
    for unit in ('001AD2B8', '00157950', '00239778', '002398C0'):
        path = next(root.glob(unit + '.*'))
        text = path.read_text()
        # Accessor calls on executable lines, never declarations.
        for line in text.splitlines():
            if 'func_00238F60()' in line and not line.lstrip().startswith('extern'):
                edit(unit, line, line.replace('func_00238F60()',
                     '(port_fps_high() ? port_step() : (float)func_00238F60())'))
    for unit, args in [('00235EA8', ('rate', 'minimum')), ('00235F38', ('rate', 'minStep'))]:
        entry(unit, 'func_'+unit,
              '    if (port_fps_high()) { %s=port_blend(%s); %s*=port_step(); }' % (args[0],args[0],args[1]))
    # Raw helpers now do the correction once; avoid the rate-aware helper
    # correcting it a second time, while keeping its original 30/60 paths.
    for unit, callee in [('00236088','00235EA8'),('00236130','00235F38')]:
        entry(unit, 'func_'+unit,
              '    if (port_fps_high()) return func_%s(fparg0,fparg1,fparg2,fparg3);' % callee)

    # True fractional animation evaluation without changing any retail layout.
    for unit in ('00202D78', '002065C0'):
        edit(unit, 'D_0029E160 += func_00238F60();',
             'D_0029E160 += port_animation_scale()>1 ? port_animation_step() : func_00238F60();')
    edit('00202E00', 'p->x4->x1C * (float)(D_0029E160[0] - p->x4->x14)',
         'p->x4->x1C * port_animation_fields((float)(D_0029E160[0] - p->x4->x14))')
    edit('00202E00', '(float)p->x4->x14 * p->x4->x1C',
         'port_animation_fields((float)p->x4->x14) * p->x4->x1C')
    edit('00202E00', 'elapsed = (float)(now - (unsigned int)p->x4->x14);',
         'elapsed = port_animation_fields((float)(now - (unsigned int)p->x4->x14));')
    patches['unsorted/00202E00.cpp'].append(('re', (r'\* 16\.66667f', 5),
                                           '* (port_animation_scale()>1 ? 16.66667f/1024 : 16.66667f)'))
    edit('00206440', 'frame = D_0029E160;', 'frame = D_0029E160 / port_animation_scale();')
    # Execute all skeleton updates at high rates (the original scheduler skips
    # expensive animation updates by field period for distant models).
    edit('00206440', 'int apply = state == current;', 'int apply = port_fps_high() || state == current;')
    # Existing EE-float patch in patches.py runs first.
    edit('00208708', 'ps2_fdiv((float)(func_00202DA8() - obj->startTick), (float)obj->duration)',
         'ps2_fdiv(port_animation_fields((float)(func_00202DA8() - obj->startTick)), (float)obj->duration)')
    # Tween callbacks otherwise remain scheduled on integer field boundaries,
    # even with a fractional clock. Only the float-tween trampoline runs on
    # every real step; ordinary gameplay callbacks retain their 60-Hz timers.
    edit('00236C60', 'extern unsigned int func_00238F48(void);',
         'extern unsigned int func_00238F48(void);\nextern unsigned int func_00263AC8(void *);')
    edit('00236C60', 'if (now >= timer->due)',
         'if (now >= timer->due || (port_fps_high() && timer->callback == func_00263AC8))')
    # Disable recording and playback above 60: skipping the recorder alone
    # would leave its caller incrementing count over uninitialized samples.
    entry('001A8DE8', 'func_001A8DE8',
          '    if (port_fps_high()) return 1;')
    entry('00138E08', 'func_00138E08',
          '    if (port_fps_high()) return; /* high-rate ghosts disabled */')
    edit('0020CDF0', 'p->rise*(float)dt', 'p->rise*(port_fps_high() ? port_step() : (float)dt)')
    edit('0020CDF0', 'p->fall*(float)dt', 'p->fall*(port_fps_high() ? port_step() : (float)dt)')
    edit('00154058', 'obj->x30.z -= fvar;',
         'obj->x30.z -= port_fps_high() ? fvar*port_step()*port_step() : fvar;')
    edit('0021F210', 'func_00238F30() == 0x3C', '(port_fps_high() || func_00238F30() == 0x3C)')
    entry('00239A80', 'func_00239CE0',
          '    if (port_fps_high()) { port_ragdoll_step(sys->particles,sys->count,sizeof(Particle),D_0029DC74); return; }')
    edit('00239A80', 'scale = -(dist * 0.99f);', 'scale = -(dist * port_blend(0.99f));')
    patches['unsorted/00239A80.c'].append(('re',(r'D_0029DC84 \* 0\.5f',2),'port_blend(D_0029DC84) * 0.5f'))
    for field in ('xDD8','xDD0','xDE4'):
        edit('001A4F70', 'obj->%s++;' % field,
             'obj->%s+=port_fps_high() ? port_whole_step() : 1;' % field)
    edit('001A4F70', 'obj->xDF8 = VLength(&normal);',
         'obj->xDF8 = VLength(&normal); if (port_fps_high()) obj->xDF8/=port_step();')
    edit('001A4F70', 'Scale__c4(&lateral, &lateral, D_0029D34C);',
         'Scale__c4(&lateral, &lateral, port_decay(D_0029D34C));')
    for constant in ('D_0029D3A8','D_0029D3AC'):
        edit('001A4F70', '"f"(%s)' % constant,
             '"f"(port_fps_high() ? %s*port_step()*port_step() : %s)' % (constant,constant))
    edit('00161B10', 'VSCALE(delta,&delta,1.0f/count);',
         'VSCALE(delta,&delta,1.0f/(port_fps_high() ? port_step() : (float)count));')
    edit('00161B10', '} else effect->x2724--;',
         '} else effect->x2724-=port_fps_high() ? port_whole_step() : 1;')
    # Further source audit: timers next to code which never queried the clock.
    # Allocation counts, loop cursors, item counts and RNG search indexes are
    # deliberately not included in this table.
    for unit, field in [('001100D0','obj->x3BC'),('001103B0','obj->x3BC'),
                         ('0012CF68','obj->x7bc8'),('00155208','p->xB4')]:
        edit(unit, field+'++;', field+'+=port_fps_high() ? port_whole_step() : 1;')
    for unit, field in [('0012CF68','obj->x7bc0'),('00155380','obj->timer'),
                         ('00142508','obj->timer'),('001425E0','o->x64'),
                         ('00147BD8','obj->timer'),('0012AD60','obj->x180'),
                         ('0013DF28','obj->xCE0'),('00161B10','effect->x27F8'),
                         ('0011FF38','o->x25E')]:
        edit(unit, field+'--;', field+'-=port_fps_high() ? port_whole_step() : 1;')
    for unit,field in [('001207F8','obj->x262'),('0015B018','obj->x1E8'),
                        ('001517D0','obj->xA3C')]:
        edit(unit, '--'+field, '('+field+'-=port_fps_high() ? port_whole_step() : 1)')
    edit('00161B10', 'if (++effect->x276E == 0x79)',
         'if ((!port_fps_high() || port_whole_step()) && (effect->x276E+=port_fps_high() ? port_whole_step() : 1) == 0x79)')
    edit('00154058', 'obj->x2C++;', 'obj->x2C+=port_fps_high() ? port_whole_step() : 1;')
    edit('001425E0', '(target-value)*0.125f', '(target-value)*port_blend(0.125f)')
    edit('00147BD8', 'obj->phase += obj->step;',
         'obj->phase += port_fps_high() ? obj->step*port_step() : obj->step;')
    for sign in ('+','-'):
        edit('00147BD8', 'obj->amount'+sign+'0.01f',
             'obj->amount'+sign+'(port_fps_high() ? 0.01f*port_step() : 0.01f)')
    # Keep integer lifetime storage and special asm ABIs; make all floating
    # motion/emission calculations fractional at the producer entry points.
    edit('00228BE8', 'pt.f[3] = count;', 'pt.f[3] = port_fps_high() ? port_step() : (float)count;')
    edit('00228BE8', 'invCount = 1.0f / (float)count;',
         'invCount = 1.0f / (port_fps_high() ? port_step() : (float)count);')
    edit('00228BE8', 'obj->x2768 += obj->x272C / (minSize + sizeRange * (baseAlpha + spread * 0.5f));',
         'obj->x2768 += obj->x272C * (port_fps_high() ? port_step() : 1.0f) / (minSize + sizeRange * (baseAlpha + spread * 0.5f));')
    edit('00228BE8', 'left=*life-1;', 'left=*life-(port_fps_high() ? port_whole_step() : 1);')
    # The specialized float-step particle path already receives f20 from the
    # generated dispatcher. Convert its frame damping to fractional decay.
    edit('00228BE8', 'values[3]=1.0f-obj->x2778;', 'values[3]=port_decay(1.0f-obj->x2778);')
    # NPC event rolls consume RNG only on whole fields. Preserve their original
    # probability and RNG draw frequency, rather than doubling ambient events.
    for path in root.glob('*.*'):
        text=path.read_text()
        pattern=r'func_00222BD8\(0,\s*100\)\s*==\s*42'
        count=len(re.findall(pattern,text))
        if count:
            patches.setdefault('unsorted/'+path.name, []).append(('re',(pattern,count),
                '( (!port_fps_high() || port_whole_step()) && func_00222BD8(0,100)==42 )'))
