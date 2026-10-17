"""Compile the real sound request producers against a synthetic RPC receiver."""
from pathlib import Path
import sys
import transform


def prepare(directory):
    root = Path(__file__).resolve().parents[2]
    sys.path.insert(0, str(root / 'port'))
    import patches
    out = Path(directory) / 'integer-abi'
    out.mkdir(exist_ok=True)
    sources = {name: (root / 'src/unsorted' / name).read_text()
               for name in ('00201B68.c', '00201BD0.c', '0018EB50.c', '002211F0.c', '00206E48.c', '00167A00.c', '001304F8.c', '001B3028.c', '001B2E50.c')}
    for name in ('0018EB50.c', '002211F0.c', '00167A00.c', '001304F8.c', '001B3028.c'):
        for kind, old, new in patches.PATCHES.get('unsorted/' + name, []):
            assert kind == 'text' and sources[name].count(old) == 1
            sources[name] = sources[name].replace(old, new)
    name = '00179338.cpp'
    sources[name] = (root / 'src/unsorted' / name).read_text()
    sources[name] += r'''
Players D_002AF2C0;
State D_004B25A0;
G32270 D_003B2270;
static int handle_failures, handle_calls, handle_mode;
static Sound expected_handle;
int func_0023DED0(void) { return handle_mode; }
void func_00203B68(const Sound *input, int apply, float value) {
    const Sound &sound = *input;
    handle_calls++;
    handle_failures += sound.x0 != expected_handle.x0 || sound.x4 != expected_handle.x4 ||
                       sound.x8 != expected_handle.x8 || sound.xC != expected_handle.xC ||
                       sound.xE != expected_handle.xE ||
                       apply != 1 || value != 0.5f;
}
extern "C" int test_rider_handles(void) {
    Obj obj = {}; List list = {}; Body body = {};
    obj.xF8C = &list; body.x18 = 6;
    Rider *rider = &D_002AF2C0.riders[0];
    rider->x792C = &body; D_002AF2C0.count = 1;
    D_003B2270.x4F50 = 10; D_003B2270.x4F54 = 2;
    Sound *slots[] = {&obj.x1124, &obj.x1134, &obj.x1054[0], &obj.x10E4, &obj.x10F4, &obj.xFC4[0]};
    for (int i = 0; i < 6; i++) {
        expected_handle.x0 = (AnimationOwner *)(i + 1); expected_handle.x4 = (AnimationTrack *)22;
        expected_handle.x8 = 33; expected_handle.xC = 44; expected_handle.xE = 55;
        *slots[i] = expected_handle;
        obj.x1164 = i % 3 == 0 ? rider : 0;
        rider->x7A5C = i % 3 == 2 ? 2 : 1;
        handle_mode = i < 3 ? 2 : 0;
        func_00179338(&obj);
        handle_failures += rider->x7A40 != 0.5f;
    }
    return handle_failures || handle_calls != 6;
}
'''
    for name, handle, timer in (('0015A7C0.cpp', 'x58', 'x1E0'), ('0015B018.cpp', 'x28', 'x1E8')):
        source = (root / 'src/unsorted' / name).read_text()
        source += r'''
static int query_failures, status_calls, transition_calls, update_calls;
static float progress;
static int active;
static Value wanted;
static void check_handle(const Value &value) {
    query_failures += __builtin_memcmp(&value, &wanted, sizeof wanted) != 0;
}
float func_00203C28(const Value *value) { check_handle(*value); return progress; }
int func_00204230(const Value *value) { check_handle(*value); status_calls++; return active; }
void func_00159CE0(Obj *, int mode) { query_failures += mode != 2; transition_calls++; }
float func_0015A968(NpcMotionActor *) { update_calls++; return 0; }
void func_00202E38(Value *value, int a, int b, float time) {
    check_handle(*value); query_failures += a != 1 || b != 1 || time != 110.0f; transition_calls++;
}
void func_0015AE10(Obj *) { update_calls++; }
extern "C" int TEST_NAME(void) {
    Obj obj = {};
    wanted.x0 = (AnimationOwner *)0x11223344; wanted.x4 = (AnimationTrack *)0x55667788;
    wanted.x8 = 0x12345678; wanted.xC = 0x5678; wanted.xE = 0x1234;
    for (int i = 0; i < 3; i++) {
        obj.HANDLE = wanted; obj.TIMER = 20;
        progress = i == 0 ? 1.0f : 0.5f; active = i == 1;
        CALLER(&obj);
        check_handle(obj.HANDLE);
    }
    return query_failures || status_calls != 2 || transition_calls != 2 || update_calls != UPDATES;
}
'''.replace('TEST_NAME', 'test_handle_' + name[:8]).replace('HANDLE', handle).replace('TIMER', timer).replace('CALLER', 'func_' + name[:8]).replace('UPDATES', '3' if name[:8] == '0015A7C0' else '0')
        # Keep these independent receiver fixtures separate from the real game symbols.
        source = '#include "npc_motion.h"\n' + source
        for header in ('npc_motion.h', 'animation.h', 'animation_select.h', 'animation_query.h', 'motion_sample.h', 'scene.h', 'vector.h'):
            source = source.replace('#include "' + header + '"', (root / 'include' / header).read_text())
        source = source.replace('func_', 'fixture_' + name[:8] + '_')
        sources[name] = source
    sources['caller.c'] = r'''
typedef unsigned int Quad __attribute__((mode(TI)));
extern void func_00201B68_alias(int, int, Quad, int, int, int, int) asm("func_00201B68");
extern void func_00201BD0(int, int, Quad, int, int, int, int, int, long);
int received;
static int failures;
extern int puts(const char *);
extern int test_rider_handles(void);
extern int test_handle_0015A7C0(void), test_handle_0015B018(void);
extern void func_001B3028(void *, int, float);
typedef struct { float f[4]; } Vec __attribute__((aligned(16)));
extern int func_001304F8(Vec *, Vec *, void *, const Vec *, const Vec *, const Vec *, Vec *);
static int transformed, oriented;
void func_00221888(void *matrix, Vec *in, Vec *out) {
    failures += matrix != (void *)123 || in->f[0] != 3 || in->f[1] != 4 || in->f[2] != 0;
    *out = *in; transformed++;
}
void func_002226C8(Vec *matrix, Vec *direction) {
    failures += direction->f[0] != 3.0f * 0.2f || direction->f[1] != 4.0f * 0.2f;
    oriented++;
}
extern int func_0018EB50(void *);
extern int func_00167A00(char *, int, float *, float *, float *, float);
int func_001678E0(char *base, int track, float *fraction, float time) {
    *fraction = 0.0f; return 1;
}
void func_00167800(char *base, float *work, char *prev, char *next, float t) {
    int k;
    failures += ((unsigned int)work & 15) != 0 || prev != base || next != base + 48;
    for (k = 0; k < 12; k++) work[k] = (float)k;
}
void func_0020ED30(int track, int a, int b, int c, float d, float e, float f, float g, float h) {
    failures += track != 2 || a != 4 || b != 5 || c != 6 || d != 1 || e != 0 || f != 1 || g != 7 || h != 2;
}
extern void func_002211F0(void *, float *);
static void *light_position;
static float *light_color;
static int light_found;
typedef float (*Animation)(unsigned short *, int, float);
Animation D_004D0278[6][3];
extern float func_00206E48(unsigned short *, int, float);
static unsigned short *animation_node;
static float animation(unsigned short *node, int values, float time) {
    failures += node != animation_node || values != 0x12345678;
    return time * 0.5f + *node;
}
int func_00220DA0(void *position, float *color) {
    failures += position != light_position || color != light_color;
    if (color == light_color && light_found) {
        color[0] = 1.0f; color[1] = 0.0f; color[2] = 0.0f; color[3] = 0.0f;
    }
    return light_found;
}
int func_0018EAC8(void *obj, int index) { return index == 7 ? 0x12345678 : 0; }
static unsigned int word(const unsigned char *p) {
    return p[0] | (unsigned int)p[1]<<8 | (unsigned int)p[2]<<16 | (unsigned int)p[3]<<24;
}
void func_00202048(int command, int length, unsigned char *p, int cb, long extra) {
    received++;
    failures += command != 0x5c || length != 48;
    failures += word(p) != (received <= 2 ? 123 : 0x44332211) || word(p+8) != 1024 || word(p+12) != 90;
    if (received > 2) failures += word(p+4) != 0x88776655;
    failures += word(p+16) != 0x44434241 || word(p+20) != 0x48474645;
    failures += word(p+24) != 0x4c4b4a49 || word(p+28) != 0x504f4e4d;
    failures += p[32] != 7 || p[34] != 9 || word(p+40) != (received <= 2 ? 0x3d : 0x3c);
    failures += received & 1 ? (cb != 0 || extra != 0) : (cb != 456 || extra != 0x123456789L);
}
int main(void) {
    long long wide = ((long long)0x504f4e4d4c4b4a49L << 64) | 0x4847464544434241L;
    unsigned long long unsigned_wide = ~0ULL;
    struct Layout { int a; long long b; } layout;
    unsigned char object[0x540] __attribute__((aligned(16)));
    int j;
    failures += test_rider_handles();
    failures += test_handle_0015A7C0() + test_handle_0015B018();
    unsigned char indexed[0x120 + 13 * 0x50] = {0};
    for (j = 0; j < 13; j++) {
        float *value = (float *)(indexed + 0x120 + j * 0x50 + 0x48);
        int *enabled = (int *)(value + 1);
        func_001B3028(indexed, j, 1.0f + j);
        failures += *value != 0;
        *enabled = 1;
        func_001B3028(indexed, j, 1.0f + j);
        failures += *value != 1.0f + j;
        func_001B3028(indexed, j, 0.0f);
        failures += *value != 1.0f + j;
    }
    Vec input_a = {{4, 6, 3, 1}}, input_b = {{1, 2, 3, 1}};
    Vec input_c = {{0, 0, 1, 0}}, direction = {{0}};
    Vec matrix[4] = {{{1, 0, 0, 0}}, {{0, 1, 0, 0}}, {{0, 0, 1, 0}}, {{2, 3, 4, 1}}};
    Vec output[4];
    failures += func_001304F8(output, matrix, (void *)123, &input_a, &input_b, &input_c, &direction) != 1;
    failures += transformed != 1 || oriented != 1 || direction.f[2] != 0;
    failures += output[0].f[0] != 1 || output[3].f[2] != 4;
    failures += input_a.f[0] != 4 || input_b.f[1] != 2 || input_c.f[2] != 1;
    input_c.f[0] = 1;
    failures += func_001304F8(output, matrix, (void *)123, &input_a, &input_b, &input_c, &direction) != 0;
    failures += transformed != 1 || oriented != 1;
    float fraction, value, extra;
    failures += func_00167A00((char *)object, 2, &fraction, &value, &extra, 1.0f) != 1;
    failures += fraction != 0 || value != 3 || extra != 10;
    unsigned short node;
    animation_node = &node;
    for (j = 0; j < 18; j++) {
        D_004D0278[j / 3][j % 3] = animation;
        node = (j / 3) | ((j % 3) << 3);
        failures += func_00206E48(&node, 0x12345678, 3.0f) != 1.5f + node;
    }
    float color[4] = {2, 3, 4, 5};
    light_position = object + 16; light_color = color;
    func_002211F0(object, color);
    failures += color[0] != 2 || color[1] != 3 || color[2] != 4 || color[3] != 5;
    light_found = 1;
    func_002211F0(object, color);
    failures += color[0] != 46.08f || color[1] != 0 || color[2] != 0 ||
                color[3] != 0.3f * 1.2f;
    for (j = 0; j < sizeof object; j++) object[j] = 0xab;
    *(int *)object = 7; object[0x530] = 1;
    failures += func_0018EB50(object) != 0x12345678;
    object[0x530] = 0;
    failures += func_0018EB50(object) != 0;
    for (j = 0x490; j < 0x4a0; j++) failures += object[j] != 0;
    failures += object[0x48f] != 0xab || object[0x4a0] != 0xab;
    failures += sizeof(long) != 8 || sizeof(long long) != 16 || sizeof(long double) != 8;
    failures += sizeof(1L) != 8 || sizeof(1LL) != 16 || sizeof(1ULL) != 16;
    failures += sizeof(layout) != 32 || (char *)&layout.b - (char *)&layout != 16;
    failures += (unsigned_wide >> 127) != 1 || (wide >> 96) != 0x504f4e4d;
    func_00201B68_alias(123, 0, wide, 1024, 90, 7, 9);
    func_00201BD0(123, 0, wide, 1024, 90, 7, 9, 456, 0x123456789L);
    func_00201B68_alias(0, 0x8877665544332211UL, wide, 1024, 90, 7, 9);
    func_00201BD0(0, 0x8877665544332211UL, wide, 1024, 90, 7, 9, 456, 0x123456789L);
    if (!failures && received == 4) puts("EE integer widths and sound RPC layout tests passed");
    return failures || received != 4;
}
'''
    paths = []
    for name, source in sources.items():
        for header in ("sound.h", "camera.h", "npc_motion.h", "animation.h", "animation_select.h", "animation_query.h", "motion_sample.h", "scene.h", "vector.h"):
            directive = '#include "' + header + '"'
            if directive in source:
                source = source.replace(directive, (root / "include" / header).read_text())
        path = out / name
        path.write_text('#include "port.h"\n' + transform.transform(source, label_prefix='_'))
        paths.append(str(path))
    return paths
