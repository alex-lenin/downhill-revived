"""Build bounded 32-bit tests for the split VIF/VU and GS queues."""
import os
from pathlib import Path
import subprocess
ROOT=Path(__file__).resolve().parents[2]
def main():
    out=ROOT/'build/port/split-queue-tests';out.mkdir(parents=True,exist_ok=True)
    common=['clang-cl','-m32','/nologo','/O2','-Wno-deprecated-declarations',
            '-msse2','-mfpmath=sse','/clang:-ffp-contract=off',
            '/DGRAPHICS_QUEUE_TEST','/DGS_QUEUE_TEST','/I'+str(ROOT/'port/runtime')]
    tests={
        'queue':['hle/graphics_queue.c','hle/test_gs_queue.c','gs/memory.c','gs/gs.c'],
        'vif':['runtime/vu0_interp.c','hle/vif.c','hle/graphics.c','hle/graphics_queue.c',
               'hle/test_vif.c','gs/memory.c','gs/gs.c']}
    for name,sources in tests.items():
        exe=out/(name+'.exe')
        subprocess.run(common+[str(ROOT/'port'/s) for s in sources]+['/Fe:'+str(exe)],
                       cwd=out,check=True,timeout=120)
        for adaptive in ('0', '1'):
            env=dict(os.environ,DHD_GS_WORKER='1',DHD_FIFO_ADAPTIVE=adaptive)
            subprocess.run([str(exe)],env=env,check=True,timeout=60)
            if name == 'queue':
                result=subprocess.run([str(exe),'negative'],env=env,timeout=60)
                if result.returncode != 1:
                    raise RuntimeError('full-state negative control did not fail')
if __name__=='__main__':main()
