"""Strict 32-bit GS metadata and ordered full-GS-state differential, including corruption control
(port/gs/test_native_frontend.c on the 64-bit host, where the GS front end runs)."""
import testprog


def main():
    gs = testprog.ROOT / 'port/gs'
    exe = testprog.host('native-frontend', [gs / 'test_native_frontend.c', gs / 'gs.c', gs / 'memory.c'], low=True)
    testprog.run(exe, timeout=60)
    testprog.run(exe, 'negative', expect=1, timeout=60, quiet=True)
    print('negative control failed as required')


if __name__ == '__main__':
    main()
