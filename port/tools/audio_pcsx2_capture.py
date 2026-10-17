"""Capture retail SPU2 PCM through the comparison worker's existing boot/input adapter.

BIOS/ISO stay in place. The optional build replaces one upstream translation
unit through MSBuild metadata; it does not edit the comparison worker's source.
The capture hook is linked only into the ignored GPL emulator executable.
"""
from pathlib import Path
import argparse
import configparser
import json
import os
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/tools'))
import compare_pcsx2 as compare
import pcsx2_retail as retail


def build(trace_requests=False):
    source = ROOT / 'build/port/compare-pcsx2-src'
    private = ROOT / 'build/port/audio-pcsx2-adapter'
    private.mkdir(exist_ok=True)
    unit = (source / 'pcsx2/SPU2/spu2.cpp').read_text(encoding='utf-8')
    unit = unit.replace('#include "common/Error.h"', '#include "common/Error.h"\n'
                        '#include "common/WAVWriter.h"\n#include "Counters.h"\n'
                        '#include <cstdlib>\n#include <cstdio>')
    hook = r''' 
static FILE *dhd_raw, *dhd_output, *dhd_audio_clock, *dhd_spu_state;
static u32 dhd_audio_samples;
static void DhdWaveHeader(FILE* file)
{
    if (!file) return;
    unsigned char h[44] = {'R','I','F','F',0,0,0,0,'W','A','V','E','f','m','t',' '};
    auto put = [&](int at, u32 value, int n) { for (int i=0;i<n;i++) h[at+i] = static_cast<unsigned char>(value >> (8*i)); };
    put(4, dhd_audio_samples*4+36,4); put(16,16,4); put(20,1,2); put(22,2,2);
    put(24,48000,4); put(28,192000,4); put(32,4,2); put(34,16,2);
    h[36]='d';h[37]='a';h[38]='t';h[39]='a';put(40,dhd_audio_samples*4,4);
    std::fseek(file,0,SEEK_SET); std::fwrite(h,1,44,file); std::fseek(file,0,SEEK_END); std::fflush(file);
}
static void DhdAudioClose()
{
    DhdWaveHeader(dhd_raw); DhdWaveHeader(dhd_output);
    if (dhd_raw) std::fclose(dhd_raw); if (dhd_output) std::fclose(dhd_output);
    if (dhd_audio_clock) std::fclose(dhd_audio_clock);
    if (dhd_spu_state) std::fclose(dhd_spu_state);
    dhd_raw=dhd_output=dhd_audio_clock=nullptr; dhd_audio_samples=0;
    dhd_spu_state=nullptr;
}
static void DhdAudioCapture(StereoOut32 out, const float* conv)
{
    if (!dhd_raw) {
        if (const char* directory = std::getenv("DHD_AUDIO_REFERENCE")) {
            const std::string prefix = std::string(directory) + "\\";
            dhd_raw = std::fopen((prefix + "spu2-mix.wav").c_str(),"wb+");
            dhd_output = std::fopen((prefix + "spu2-output.wav").c_str(),"wb+");
            dhd_audio_clock = std::fopen((prefix + "sample-clock.csv").c_str(),"wb");
            dhd_spu_state = std::fopen((prefix + "spu2-state.csv").c_str(),"wb");
            if (!dhd_raw || !dhd_output || !dhd_audio_clock) { std::fprintf(stderr,"DHD_AUDIO_OPEN_ERROR\n"); std::exit(30); }
            DhdWaveHeader(dhd_raw); DhdWaveHeader(dhd_output);
            std::fprintf(dhd_audio_clock,"sample,boot_field,spu_cycle\n");
            if (dhd_spu_state) std::fprintf(dhd_spu_state,"sample,field,core,wet_left_mask,wet_right_mask,effect_left,effect_right,wet_input_left,wet_input_right,wet_output_left,wet_output_right\n");
        }
    }
    if (!dhd_raw) return;
    const s16 raw[2]={static_cast<s16>(clamp_mix(out.Left)),static_cast<s16>(clamp_mix(out.Right))};
    const s16 filtered[2]={static_cast<s16>(std::clamp(conv[0]*32767.0f,-32768.0f,32767.0f)),static_cast<s16>(std::clamp(conv[1]*32767.0f,-32768.0f,32767.0f))};
    std::fwrite(raw,4,1,dhd_raw); std::fwrite(filtered,4,1,dhd_output);
    if (dhd_audio_samples%4800==0) std::fprintf(dhd_audio_clock,"%u,%u,%u\n",dhd_audio_samples,g_FrameCount,Cycles);
    if (dhd_spu_state && dhd_audio_samples%4800==0) for (int core=0;core<2;core++) {
        const auto& c=Cores[core];const u32 pos=(c.RevbSampleBufPos-1)&63;
        std::fprintf(dhd_spu_state,"%u,%u,%d,%u,%u,%d,%d,%d,%d,%d,%d\n",dhd_audio_samples,g_FrameCount,core,
            c.Regs.VMIXEL,c.Regs.VMIXER,c.FxVol.Left,c.FxVol.Right,
            c.RevbDownBuf[0][pos],c.RevbDownBuf[1][pos],c.RevbUpBuf[0][pos],c.RevbUpBuf[1][pos]);
    }
    dhd_audio_samples++;
    if (dhd_audio_samples%48000==0) { DhdWaveHeader(dhd_raw); DhdWaveHeader(dhd_output); std::fflush(dhd_audio_clock); if (dhd_spu_state) std::fflush(dhd_spu_state); }
}
'''
    unit = unit.replace('u64 lClocks = 0;', hook + '\nu64 lClocks = 0;')
    unit = unit.replace('\tDCFilter(conv);', '\tDCFilter(conv);\n\tDhdAudioCapture(out, conv);')
    unit = unit.replace('void SPU2::Close()\n{', 'void SPU2::Close()\n{\n\tDhdAudioClose();')
    extra_items = ''
    if trace_requests:
        from audio_pcsx2_requests import instrument
        unit, extra_items = instrument(source, private, unit)
    target = private / 'spu2-audio.cpp'
    target.write_text(unit, encoding='utf-8', newline='\n')
    props = private / 'audio.targets'
    props.write_text(f'''<Project xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
<ItemDefinitionGroup Condition="'$(MSBuildProjectName)'=='pcsx2'">
<ClCompile><ProgramDataBaseFileName>{private / 'audio.pdb'}</ProgramDataBaseFileName>
<PrecompiledHeaderOutputFile>{private / 'audio.pch'}</PrecompiledHeaderOutputFile>
<AdditionalOptions>/FS %(AdditionalOptions)</AdditionalOptions></ClCompile>
</ItemDefinitionGroup>
<Target Name="DhdAudioSource" BeforeTargets="ClCompile" Condition="'$(MSBuildProjectName)'=='pcsx2'">
<ItemGroup><ClCompile Remove="SPU2\\spu2.cpp" />
<ClCompile Include="{target}"><ObjectFileName>$(IntDir)spu2-audio.obj</ObjectFileName><PrecompiledHeader>NotUsing</PrecompiledHeader></ClCompile></ItemGroup>
<ItemGroup>{extra_items}</ItemGroup>
</Target></Project>''', encoding='utf-8')
    msbuild = Path(r'C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe')
    subprocess.run([str(msbuild), str(source / 'PCSX2_qt.sln'), '/t:pcsx2-qt', '/m:4',
                    '/p:Configuration=Release AVX2', '/p:Platform=x64', '/p:EXEString=pcsx2-dhd-audio',
                    '/p:PlatformToolset=v145', '/p:DefaultPlatformToolset=v145',
                    '/p:ForceImportBeforeCppTargets='+str(props), '/v:minimal', '/nologo'], check=True)
    return source / 'bin/pcsx2-dhd-audio.exe'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('label')
    parser.add_argument('--build', action='store_true')
    parser.add_argument('--requests', action='store_true', help='build a read-only delivered-RPC event probe')
    parser.add_argument('--until', type=float, default=380)
    parser.add_argument('--timeout', type=float, default=1200)
    parser.add_argument('--pad', default=compare.STANDARD_PAD)
    args = parser.parse_args()
    emulator = build(args.requests) if args.build else ROOT / 'build/port/compare-pcsx2-src/bin/pcsx2-dhd-audio.exe'
    directory = ROOT / 'build/port' / ('audio-' + args.label)
    directory.mkdir(exist_ok=False)
    events = compare.parse_pad(args.pad)
    # VM shutdown is asynchronous; leave neutral input while it drains.
    compare.write_movie(directory / 'input.p2m2', events, compare.field_at(args.until)+120)
    original_config, original_popen = retail.config, subprocess.Popen
    def config(*params):
        data = original_config(*params)
        path = data / 'PCSX2/inis/PCSX2.ini'
        ini = configparser.ConfigParser(interpolation=None)
        ini.optionxform = str
        ini.read(path)
        ini['SPU2/Output'].update(Backend='Null', SyncMode='None', StandardVolume='100', FastForwardVolume='100')
        with path.open('w', encoding='utf-8') as file:
            ini.write(file)
        return data
    def popen(*params, **kwargs):
        if 'env' in kwargs:
            kwargs['env']['DHD_AUDIO_REFERENCE'] = str(directory)
        return original_popen(*params, **kwargs)
    retail.config, subprocess.Popen = config, popen
    args.bios = None
    args.retail_pcsx2, args.retail_renderer = emulator, 13
    args.reference_offset, args.match_window = 0, 0
    manifest = dict(pad=args.pad, until=args.until, capture='SPU2 mix before host volume/time stretch; output also has PCSX2 DC filter')
    try:
        times = [t for t in (80, 190, 250) if t < args.until] + [args.until]
        retail.run_reference(args, directory, [compare.field_at(t) for t in times], manifest, compare)
        manifest['status'] = 'complete'
    except BaseException as error:
        manifest.update(status='failed', error=str(error))
        raise
    finally:
        (directory / 'manifest.json').write_text(json.dumps(manifest, indent=2), encoding='utf-8')
        retail.config, subprocess.Popen = original_config, original_popen


if __name__ == '__main__':
    main()
