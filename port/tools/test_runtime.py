"""Build and run the inline asm runtime tests (port/runtime/test_ps2asm.c) as a 32-bit Windows
program. Needs LLVM (clang-cl) and Visual Studio's x86 libraries; run port/tools/build.py first,
the test parses every ps2_asm() call site found in build/port/src.
usage: python port/tools/test_runtime.py"""
import glob, os, re, subprocess, sys, json

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OUT = os.path.join(ROOT, 'build', 'port', 'runtime')
STR = r'"(?:\\.|[^"\\])*"'
CALL = re.compile(r'ps2_asm\(((?:%s\s*)+),\s*("[^"]*"),\s*(\d+)' % STR)

# A DMA kick must survive all three native C MMIO lvalue forms.
from transform import dma_stores
for source in ('*(volatile unsigned int *)0x10009000 = 0x145;',
               'HW(0x10009000) = 0x145;',
               '#define D1_CHCR (*(volatile unsigned int *)0x10009000)\nD1_CHCR = 0x145;'):
    result = dma_stores(source)
    assert 'ps2_store32(0x10009000, 0x145);' in result, result
assert dma_stores('if (D1_CHCR == 0x145) wait();') == 'if (D1_CHCR == 0x145) wait();'

os.makedirs(OUT, exist_ok=True)
from test_float_abi import main as check_float_abi
check_float_abi()
from test_scalar_guard import main as check_scalar_guard
check_scalar_guard()
from test_quad_abi import main as check_quad_abi
check_quad_abi()
from test_scalar_dispatch import main as check_scalar_dispatch
check_scalar_dispatch()
from test_format_abi import main as check_format_abi
check_format_abi()
from test_shared_scalars import main as check_shared_scalars
check_shared_scalars()
from test_particle_packets import main as check_particle_packets
check_particle_packets()
from test_scalar_abi import main as check_scalar_abi
check_scalar_abi()
from test_particle_float import main as check_particle_float
check_particle_float()
from test_ready_ratio import main as check_ready_ratio
check_ready_ratio()
from test_tween_float import main as check_tween_float
check_tween_float()
from test_terrain_float import main as check_terrain_float
check_terrain_float()
from test_slab_float import main as check_slab_float
check_slab_float()
from test_proximity_float import main as check_proximity_float
check_proximity_float()
from test_npc_distance_float import main as check_npc_distance_float
check_npc_distance_float()
from test_rider_step import main as check_rider_step
check_rider_step()
from test_blend_float import main as check_blend_float
check_blend_float()
from test_x87 import main as check_x87
check_x87()
from test_virtual_scale import main as check_virtual_scale
check_virtual_scale()
from test_fiber_float import main as check_fiber_float
check_fiber_float()
from test_recomp import prepare
recomp_test = prepare(OUT)
from test_recomp import check_forwarders
thunk_test = check_forwarders(OUT)
from test_data import check
check(OUT)
from test_abi import prepare as prepare_abi
abi_tests = prepare_abi(OUT)
from test_integer_abi import prepare as prepare_integer_abi
integer_abi_tests = prepare_integer_abi(OUT)
from test_member_abi import prepare as prepare_member_abi
member_test = prepare_member_abi(OUT)
with open(os.path.join(ROOT, 'port', 'gs', 'raster.hlsl'), encoding='utf-8') as f:
    shader = f.read()
with open(os.path.join(OUT, 'gs_shader.h'), 'w', encoding='utf-8') as f:
    f.write('static const char gs_shader[] = ' + json.dumps(shader) + ';\n')
    f.write('static const char gs_shader_path[] = ' + json.dumps(os.path.join(ROOT, 'port', 'gs', 'raster.hlsl')) + ';\n')
sites = set()
for f in glob.glob(os.path.join(ROOT, 'build', 'port', 'src', '**', '*.c*'), recursive=True):
    for m in CALL.finditer(open(f, encoding='utf-8').read()):
        sites.add((' '.join(m.group(1).split('\n')), m.group(2), int(m.group(3))))
if not sites:
    sys.exit('no ps2_asm() calls in build/port/src: run port/tools/build.py first')
with open(os.path.join(OUT, 'all_templates.inc'), 'w', encoding='utf-8', newline='\n') as f:
    for t, c, n in sorted(sites):
        f.write('{%s, %s, %d},\n' % (t, c, n))

vswhere = r'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe'
vs = subprocess.run([vswhere, '-latest', '-property', 'installationPath'], capture_output=True,
                    text=True).stdout.strip()
exe = os.path.join(OUT, 'test_ps2asm.exe')
rt = os.path.join(ROOT, 'port', 'runtime')
cmd = os.path.join(OUT, 'test.cmd')
with open(cmd, 'w') as f:
    f.write('@set "PATH=%%PATH%%;%s"\n' % os.path.dirname(vswhere))
    f.write('@call "%s\\VC\\Auxiliary\\Build\\vcvarsall.bat" x86 >nul\n' % vs)
    # SSE math, no contraction: single-precision results that don't depend on the optimizer
    f.write('clang-cl -m32 /nologo /O2 -Wno-deprecated-declarations -msse2 -mfpmath=sse /clang:-ffp-contract=off '
            '/I"%s" /I"%s" "%s\\ps2asm.c" "%s\\vu0_micro.c" "%s\\vu0_interp.c" "%s\\test_ps2asm.c" /Fe:"%s"\n'
            % (rt, OUT, rt, rt, rt, rt, exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % exe)
    recomp_exe = os.path.join(OUT, 'test_callbacks.exe')
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 -msse2 -mfpmath=sse /clang:-ffp-contract=off '
            '/I"%s" "%s" /Fe:"%s"\n' % (rt, recomp_test, recomp_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % recomp_exe)
    abi_exe = os.path.join(OUT, 'test_abi.exe')
    thunk_exe = os.path.join(OUT, 'test_thunk.exe')
    f.write('@if errorlevel 1 exit /b 1\n')
    alias_asm = os.path.join(OUT, 'code-alias.s')
    f.write('clang --target=i686-pc-windows-msvc -c "%s" -o "%s.o"\n' % (alias_asm, alias_asm))
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 /I"%s" "%s" "%s.o" /Fe:"%s"\n' % (rt, thunk_test, alias_asm, thunk_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % thunk_exe)
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 -Wno-deprecated-non-prototype %s /Fe:"%s"\n'
            % (' '.join('"%s"' % p for p in abi_tests), abi_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % abi_exe)
    gs = os.path.join(ROOT, 'port', 'gs')
    integer_exe = os.path.join(OUT, 'test_integer_abi.exe')
    f.write('@if errorlevel 1 exit /b 1\n')
    for source in integer_abi_tests:
        f.write('clang --target=i686-pc-windows-itanium -c -O2 -malign-double -I"%s" "%s" -o "%s.o"\n'
                % (os.path.join(ROOT, 'port/include'), source, source))
        f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang --target=i686-pc-windows-msvc -fuse-ld=lld -O2 -msse2 -mfpmath=sse -ffp-contract=off %s "%s/ps2asm.c" "%s/vu0_interp.c" -o "%s"\n'
            % (' '.join('"%s.o"' % p for p in integer_abi_tests), rt, rt, integer_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % integer_exe)
    gs_exe = os.path.join(OUT, 'test_gs.exe')
    member_exe = os.path.join(OUT, 'test_member_abi.exe')
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang --target=i686-pc-windows-itanium -c -O2 -fno-exceptions -I"%s" "%s" -o "%s.o"\n'
            % (os.path.join(ROOT, 'port/include'), member_test, member_test))
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang --target=i686-pc-windows-msvc -fuse-ld=lld "%s.o" -o "%s"\n' % (member_test, member_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % member_exe)
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 -Wno-deprecated-declarations -msse2 -mfpmath=sse /clang:-ffp-contract=off '
            '"%s\\memory.c" "%s\\gs.c" "%s\\display.c" "%s\\test_gs.c" /Fe:"%s"\n' % (gs, gs, gs, gs, gs_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % gs_exe)
    capture_exe = os.path.join(OUT, 'test_capture.exe')
    capture_dir = os.path.join(OUT, 'capture')
    os.makedirs(capture_dir, exist_ok=True)
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 -Wno-deprecated-declarations "%s/memory.c" "%s/gs.c" "%s/display.c" "%s/capture.c" "%s/test_capture.c" /Fe:"%s"\n'
            % (gs, gs, gs, gs, gs, capture_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s" "%s"\n' % (capture_exe, capture_dir))
    f.write('@if errorlevel 1 exit /b 1\n"%s" "%s" 6\n' % (capture_exe, capture_dir))
    vu_exe = os.path.join(OUT, 'test_vu1.exe')
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 -Wno-deprecated-declarations -msse2 -mfpmath=sse /clang:-ffp-contract=off '
            '"%s\\vu0_interp.c" "%s\\test_vu1.c" "%s\\memory.c" "%s\\gs.c" /Fe:"%s"\n'
            % (rt, rt, gs, gs, vu_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % vu_exe)
    vif_exe = os.path.join(OUT, 'test_vif.exe')
    hle = os.path.join(ROOT, 'port', 'hle')
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 -Wno-deprecated-declarations -msse2 -mfpmath=sse /clang:-ffp-contract=off /I"%s" '
            '"%s\\vu0_interp.c" "%s\\vif.c" "%s\\graphics.c" "%s\\test_vif.c" "%s\\memory.c" "%s\\gs.c" /Fe:"%s"\n'
            % (rt, rt, hle, hle, hle, gs, gs, vif_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % vif_exe)

    queue_exe = os.path.join(OUT, 'test_graphics_queue.exe')
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 -Wno-deprecated-declarations /DGRAPHICS_QUEUE_TEST /I"%s" '
            '"%s/graphics_queue.c" "%s/test_graphics_queue.c" /Fe:"%s"\n' % (rt, hle, hle, queue_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % queue_exe)
    f.write('@if errorlevel 1 exit /b 1\n"%s" inline\n' % queue_exe)
    queued_vif_exe = os.path.join(OUT, 'test_queued_vif.exe')
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 -Wno-deprecated-declarations -msse2 -mfpmath=sse /clang:-ffp-contract=off /DGRAPHICS_QUEUE_TEST /I"%s" '
            '"%s/vu0_interp.c" "%s/vif.c" "%s/graphics.c" "%s/graphics_queue.c" "%s/test_vif.c" "%s/memory.c" "%s/gs.c" /Fe:"%s"\n'
            % (rt, rt, hle, hle, hle, hle, gs, gs, queued_vif_exe))
    f.write('@if errorlevel 1 exit /b 1\n@set DHD_MTVU=1\n"%s"\n@set DHD_MTVU=\n' % queued_vif_exe)

    autopad_exe = os.path.join(OUT, 'test_autopad.exe')
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 -Wno-deprecated-declarations /I"%s" "%s\\autopad.c" "%s\\test_autopad.c" /Fe:"%s"\n'
            % (rt, hle, hle, autopad_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % autopad_exe)

    options_exe = os.path.join(OUT, 'test_game_options.exe')
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 -Wno-deprecated-declarations /I"%s" "%s\\game_options.c" "%s\\test_game_options.c" /Fe:"%s"\n'
            % (hle, hle, hle, options_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % options_exe)

    keyboard_exe = os.path.join(OUT, 'test_keyboard.exe')  # keyboard layout and steering ramp (finding 059)
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 /I"%s" "%s\\keyboard.c" "%s\\test_keyboard.c" /Fe:"%s"\n'
            % (hle, hle, hle, keyboard_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % keyboard_exe)

    # SDL3 starts and enumerates the controllers (none needed); gamepad.c's smoke test
    import sdl3
    sdl_lib, sdl_include = sdl3.build()
    gamepad_exe = os.path.join(OUT, 'test_gamepad.exe')
    f.write('clang-cl -m32 /nologo /O2 -w /I"%s" /I"%s" "%s\\gamepad.c" "%s\\test_gamepad.c" "%s" %s /Fe:"%s"\n'
            % (hle, sdl_include, hle, hle, sdl_lib, ' '.join(sdl3.SYSTEM_LIBS), gamepad_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % gamepad_exe)

    sif_exe = os.path.join(OUT, 'test_sif.exe')
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 /I"%s" "%s\\sif.c" "%s\\test_sif.c" /Fe:"%s"\n'
            % (rt, hle, hle, sif_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % sif_exe)

    timer_exe = os.path.join(OUT, 'test_timer.exe')
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 "%s\\timer.c" "%s\\test_timer.c" /Fe:"%s"\n'
            % (hle, hle, timer_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % timer_exe)

    sound_exe = os.path.join(OUT, 'test_sound.exe')
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 /DSND_NO_HOST /I"%s" "%s\\sound.c" "%s\\test_sound.c" /Fe:"%s"\n'
            % (hle, hle, hle, sound_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % sound_exe)

    gpu_exe = os.path.join(OUT, 'test_gpu.exe')
    f.write('@if errorlevel 1 exit /b 1\n')
    f.write('clang-cl -m32 /nologo /O2 -Wno-deprecated-declarations /I"%s" '
            '"%s\\memory.c" "%s\\gs.c" "%s\\gpu.c" "%s\\test_gpu.c" '
            '/Fe:"%s" /link d3d11.lib d3dcompiler.lib\n' % (OUT, gs, gs, gs, gs, gpu_exe))
    f.write('@if errorlevel 1 exit /b 1\n"%s"\n' % gpu_exe)

# The native VU0 microprograms are checked against the interpreter running the retail
# microcode, extracted locally (never tracked) to assets/vutext.bin.
vutext = os.path.join(ROOT, 'assets', 'vutext.bin')
if os.path.exists(vutext) and 'DHD_VUTEXT' not in os.environ:
    os.environ['DHD_VUTEXT'] = vutext
result = subprocess.run(['cmd', '/c', cmd]).returncode
if not result:
    result = subprocess.run([sys.executable, os.path.join(ROOT, 'port/tools/test_rider_time.py')]).returncode
if result == 0:
    result = subprocess.run([sys.executable, os.path.join(ROOT, 'port/tools/test_rider_interaction.py')]).returncode
if result == 0:
    result = subprocess.run([sys.executable, os.path.join(ROOT, 'port/tools/test_npc_animation.py')]).returncode
if result == 0:
    result = subprocess.run([sys.executable, os.path.join(ROOT, 'port/tools/test_rider_input.py')]).returncode
if result == 0:
    result = subprocess.run([sys.executable, os.path.join(ROOT, 'port/tools/test_npc_float_return.py')]).returncode
if not result:
    result = subprocess.run([sys.executable, os.path.join(ROOT, 'port/tools/test_float.py')]).returncode
if not result:
    result = subprocess.run([sys.executable, os.path.join(ROOT, 'port/tools/test_port_headers.py')]).returncode
if not result and os.environ.get('DHD_PCSX2_VU'):
    result = subprocess.run([sys.executable, os.path.join(ROOT, 'port/tools/test_vu1_pcsx2.py')]).returncode
if not result and os.environ.get('DHD_PCSX2_VU'):
    result = subprocess.run([sys.executable, os.path.join(ROOT, 'port/tools/test_vu0_pcsx2.py')]).returncode
sys.exit(result)
