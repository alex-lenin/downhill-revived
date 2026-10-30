"""Cross-platform lobby identity regression; no retail files or WASI SDK needed.

    python port/tools/test_online_identity.py

Checks equivalent source trees in different paths/line endings, local source edits, ignored
outputs, and the actual C formatter in two distinct native executables (finding 115).
"""
import hashlib
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

from online_identity import ROOT, SOURCE_ROOTS, source_digest, write_header
import testprog


class OnlineIdentityTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name) / 'checkout with spaces'
        for directory in SOURCE_ROOTS:
            (self.root / directory).mkdir(parents=True)
        self.source = self.root / 'src' / 'race.c'
        self.source.write_bytes(b'int race(void) { return 1; }\n')
        (self.root / 'port' / 'net.h').write_bytes(b'#define PROTOCOL 1\n')

    def test_checkout_path_line_endings_and_metadata_do_not_matter(self):
        other = self.root.parent / 'other checkout'
        shutil.copytree(self.root, other)
        for path in other.rglob('*'):
            if path.is_file():
                path.write_bytes(path.read_bytes().replace(b'\n', b'\r\n'))
                path.touch()
        self.assertEqual(source_digest(self.root), source_digest(other))

    def test_uncommitted_edit_changes_identity(self):
        before = source_digest(self.root)
        self.source.write_bytes(b'int race(void) { return 2; }\n')
        self.assertNotEqual(before, source_digest(self.root))

    def test_added_removed_and_renamed_sources_change_identity(self):
        before = source_digest(self.root)
        added = self.root / 'port' / 'new.c'
        added.write_bytes(b'int state;\n')
        self.assertNotEqual(before, source_digest(self.root))
        added.unlink()
        self.assertEqual(before, source_digest(self.root))
        self.source.rename(self.source.with_name('other.c'))
        self.assertNotEqual(before, source_digest(self.root))

    def test_builds_dependencies_and_documents_do_not_matter(self):
        before = source_digest(self.root)
        for relative in ('build/native.c', 'port/render/target/generated.rs',
                         'port/launcher/node_modules/dependency.js', 'port/launcher/src-tauri/gen/schema.json',
                         'port/tools/__pycache__/cache.py',
                         'docs/note.md', 'port/dhd.ini'):
            path = self.root / relative
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text('unrelated output\n')
        self.assertEqual(before, source_digest(self.root))

    def test_release_and_protocol_changes_separate_builds(self):
        before = source_digest(self.root)
        (self.root / 'port' / 'net.h').write_bytes(b'#define PROTOCOL 2\n')
        self.assertNotEqual(before, source_digest(self.root))
        before = source_digest(self.root)
        (self.root / 'port' / 'version.h').write_bytes(b'#define DHD_VERSION "0.8.0"\n')
        self.assertNotEqual(before, source_digest(self.root))

    def test_missing_source_tree_fails_closed(self):
        (self.root / 'config').rmdir()
        with self.assertRaises(ValueError):
            source_digest(self.root)

    def test_online_module_keeps_high_rate_declarations(self):
        # The rebase must retain fps.h: without it C can silently assume int returns for
        # the new race-time helpers. Compile the actual unit with implicit calls fatal.
        subprocess.run([testprog.CLANG, *testprog.HOST_TARGET, '-std=gnu11', '-fsyntax-only',
                        '-Werror=implicit-function-declaration', '-Wno-int-to-pointer-cast',
                        '-Wno-pointer-to-int-cast', '-I', str(ROOT / 'port/runtime'),
                        '-I', str(ROOT / 'port/include'), '-I', str(ROOT / 'include'),
                        str(ROOT / 'port/hle/online.c')], check=True)

    def test_real_formatter_ignores_native_binary_and_fits_protocol(self):
        out = ROOT / 'build' / 'tests' / 'online-identity'
        identity = write_header(out / 'online_build.h', self.root)
        harness = out / 'main.c'
        harness.write_text('#include <stdio.h>\n#include <stddef.h>\n'
                           'void hle_online_version(char *, size_t);\n'
                           'int main(int argc, char **argv) {\n'
                           '  char version[64];\n'
                           '  if (argc > 1) { puts(PLATFORM); return 0; }\n'
                           '  hle_online_version(version, sizeof version);\n'
                           '  puts(version); return 0;\n}\n')
        results, hashes = [], []
        for platform in ('Windows', 'macOS'):
            exe = testprog.host('identity-' + platform, [ROOT / 'port/hle/online_version.c', harness],
                                out=out / platform, includes=[out], defines=['PLATFORM="%s"' % platform])
            results.append(testprog.run(exe, quiet=True).stdout.strip())
            hashes.append(hashlib.sha256(exe.read_bytes()).digest())
            self.assertEqual(testprog.run(exe, 'platform', quiet=True).stdout.strip(), platform)
        self.assertNotEqual(hashes[0], hashes[1])
        self.assertEqual(results[0], results[1])
        self.assertTrue(results[0].endswith(' src1-' + identity))
        self.assertLessEqual(len(results[0]), 63)  # hello.version and NetClient.version[64]


if __name__ == '__main__':
    unittest.main(verbosity=2)
