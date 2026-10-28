"""Run the actual ported camera envelope units against synthetic timing keys."""
import testprog

out = testprog.out_dir('fps-envelopes')
units = [testprog.game_unit(rel, out) for rel in ('unsorted/00239778.c', 'unsorted/002398C0.c')]
testprog.run(testprog.guest('fps-envelopes', [testprog.ROOT / 'port/hle/test_fps_envelopes.c', *units], out=out))
