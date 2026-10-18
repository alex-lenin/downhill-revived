"""stop_verified must refuse a live PID whose executable is not the expected one."""
from pathlib import Path
import subprocess
import sys
import unittest
from owned_process import stop_verified

class OwnedProcess(unittest.TestCase):
    def test_wrong_path_is_left_running(self):
        process=subprocess.Popen([sys.executable,'-c','import time; time.sleep(60)'])
        try:
            with self.assertRaises(subprocess.CalledProcessError):
                stop_verified(process,Path(sys.executable).with_name('not-the-owned-image.exe'))
            self.assertIsNone(process.poll())
        finally:
            stop_verified(process,sys.executable)
        self.assertIsNotNone(process.poll())

if __name__=='__main__':unittest.main()
