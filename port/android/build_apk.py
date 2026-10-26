"""Build Downhill Revived's debug APK for Android arm64 (finding 091).

    python port/android/build_apk.py [--wasm build/wasm/dhd_core.wasm] [--jobs 2] [--offline]

1. The native libraries: build/android/lib/arm64-v8a/libmain.so and libSDL3.so, built by
   port/tools/build_host64_android.py from the module (--wasm, default build/wasm/dhd_core.wasm,
   when given or when the libraries are missing).
2. The Gradle project in build/android/project: this folder's files (the Gradle scripts, the
   manifest, DhdActivity.java, the strings and theme), SDL's Java sources of the same release
   (org.libsdl.app, from the SDL source sdl3.py unpacked), the icon (port/res/icon.png, scaled per
   density), the two libraries (src/main/jniLibs/arm64-v8a) and the Gradle wrapper (SDL's
   gradle-wrapper.jar, Gradle WRAPPER_GRADLE: the version already in GRADLE_USER_HOME).
3. gradlew assembleDebug (no daemon), signed with the debug key ANDROID_USER_HOME holds
   (DHD_DEVCACHES\\AndroidUser\\debug.keystore, made on first use). Output:
   build/android/downhill-revived-<version>-debug.apk.

Needs: the Android SDK (ANDROID_HOME; platform android-35, build-tools), a JDK 17+ (JAVA_HOME),
GRADLE_USER_HOME on F: (Gradle, the Android Gradle Plugin and its libraries are fetched there on
the first build unless already cached; --offline uses only the cache). No Android Studio. Nothing
here goes into git: build/ is ignored.
"""
import argparse
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
sys.path.insert(0, str(ROOT / 'port' / 'tools'))
import sdl3  # noqa: E402

OUT = ROOT / 'build' / 'android'
PROJECT = OUT / 'project'
WRAPPER_GRADLE = '8.10.2'   # AGP 8.7.0 (build.gradle) needs Gradle 8.9 or later
ICON_SIZES = {'mdpi': 48, 'hdpi': 72, 'xhdpi': 96, 'xxhdpi': 144, 'xxxhdpi': 192}
TEMPLATE = ('settings.gradle', 'build.gradle', 'gradle.properties', 'app/build.gradle', 'app/src/main')


def version():
    text = (ROOT / 'port' / 'include' / 'version.h').read_text()
    name = re.search(r'#define DHD_VERSION "([0-9.]+)"', text).group(1)
    major, minor, patch = (int(x) for x in name.split('.'))
    return name, major * 10000 + minor * 100 + patch


def icons(res):
    source = ROOT / 'port' / 'res' / 'icon.png'
    try:
        from PIL import Image
    except ImportError:  # one icon, scaled by Android
        (res / 'mipmap-xxxhdpi').mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, res / 'mipmap-xxxhdpi' / 'ic_launcher.png')
        return
    image = Image.open(source).convert('RGBA')
    for density, size in ICON_SIZES.items():
        (res / ('mipmap-' + density)).mkdir(parents=True, exist_ok=True)
        image.resize((size, size), Image.LANCZOS).save(res / ('mipmap-' + density) / 'ic_launcher.png')


def project(libs):
    if PROJECT.exists():
        shutil.rmtree(PROJECT)
    for rel in TEMPLATE:
        src, dst = HERE / rel, PROJECT / rel
        if src.is_dir():
            shutil.copytree(src, dst)
        else:
            dst.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(src, dst)
    _lib, _include, java = sdl3.android_build()
    shutil.copytree(Path(java) / 'org', PROJECT / 'app' / 'src' / 'main' / 'java' / 'org')
    icons(PROJECT / 'app' / 'src' / 'main' / 'res')
    jni = PROJECT / 'app' / 'src' / 'main' / 'jniLibs' / 'arm64-v8a'
    jni.mkdir(parents=True)
    for lib in libs:
        shutil.copyfile(lib, jni / lib.name)
    template = Path(java).parents[3]  # SDL's android-project
    for rel in ('gradlew', 'gradlew.bat', 'gradle/wrapper/gradle-wrapper.jar'):
        (PROJECT / rel).parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(template / rel, PROJECT / rel)
    (PROJECT / 'gradle' / 'wrapper' / 'gradle-wrapper.properties').write_text(
        'distributionBase=GRADLE_USER_HOME\ndistributionPath=wrapper/dists\n'
        'distributionUrl=https\\://services.gradle.org/distributions/gradle-%s-all.zip\n'
        'zipStoreBase=GRADLE_USER_HOME\nzipStorePath=wrapper/dists\n' % WRAPPER_GRADLE)
    sdk = os.environ.get('ANDROID_HOME') or os.environ.get('ANDROID_SDK_ROOT') or str(Path.home() / 'Android/Sdk')
    (PROJECT / 'local.properties').write_text('sdk.dir=%s\n' % sdk.replace('\\', '/'))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--wasm', type=Path, help='rebuild the libraries from this module first')
    ap.add_argument('--jobs', type=int, default=2)
    ap.add_argument('--offline', action='store_true', help="Gradle from GRADLE_USER_HOME's cache only")
    ap.add_argument('--extra-lib', type=Path, action='append', default=[],
                    help='another arm64 .so to pack (e.g. libVkLayer_khronos_validation.so for DHD_WGPU_DEBUG)')
    a = ap.parse_args()
    for var in ('GRADLE_USER_HOME', 'ANDROID_USER_HOME'):
        value = os.environ.get(var, '')
        if not value or os.path.splitdrive(os.path.abspath(value))[0].upper() == 'C:':
            sys.exit('build_apk: %s must be set, off C: (%r)' % (var, value))
    libs = [OUT / 'lib' / 'arm64-v8a' / n for n in ('libmain.so', 'libSDL3.so')]
    wasm = a.wasm or (ROOT / 'build' / 'wasm' / 'dhd_core.wasm')
    if a.wasm or not all(p.exists() for p in libs):
        r = subprocess.run([sys.executable, str(ROOT / 'port' / 'tools' / 'build_host64_android.py'), str(wasm),
                            '--jobs', str(a.jobs)])
        if r.returncode:
            sys.exit('build_apk: build_host64_android.py failed')
    project(libs + a.extra_lib)
    name, code = version()
    gradlew = PROJECT / ('gradlew.bat' if os.name == 'nt' else 'gradlew')
    cmd = [str(gradlew), '--no-daemon', '--console=plain', '-PdhdVersionName=' + name, '-PdhdVersionCode=%d' % code,
           'assembleDebug'] + (['--offline'] if a.offline else [])
    print('build_apk: ' + ' '.join(cmd), flush=True)
    r = subprocess.run(cmd, cwd=str(PROJECT))
    if r.returncode:
        sys.exit('build_apk: Gradle failed (exit %d)' % r.returncode)
    built = PROJECT / 'app' / 'build' / 'outputs' / 'apk' / 'debug' / 'app-debug.apk'
    apk = OUT / ('downhill-revived-%s-debug.apk' % name)
    shutil.copyfile(built, apk)
    print('PASS: %s (%.1f MB), app.downhillrevived %s (%d), arm64-v8a, debug-signed' % (
        apk, apk.stat().st_size / 1048576, name, code), flush=True)


if __name__ == '__main__':
    main()
