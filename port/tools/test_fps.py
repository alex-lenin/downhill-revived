"""Compile/run production clock invariance checks: port/hle/fps.c in the core (wasm32)."""
import testprog

exe = testprog.guest('fps', [testprog.ROOT / 'port/hle/fps.c', testprog.ROOT / 'port/hle/test_fps.c'],
                     memory=0x10010000)
for rate in (60, 61, 75, 90, 100, 120, 144, 165, 240):
    testprog.run(exe, rate)
    if rate > 60:
        testprog.run(exe, rate, 'wall')
