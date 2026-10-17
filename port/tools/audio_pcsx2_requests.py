"""Observe delivered 989snd SIF RPCs in an ignored PCSX2 shadow unit.

Uses the ps2sdk SIF packet layout as a protocol reference. Does not change DMA,
IOP memory or driver execution. BIOS, game payloads and upstream sources stay
outside the tracked implementation. The generated unit retains PCSX2's GPL.
"""
from pathlib import Path


HOOK = r'''
// Audio-only observation, enabled by DHD_AUDIO_REFERENCE.
extern unsigned DhdAudioSamplesForRequests();
static u32 dhd_audio_tag_address, dhd_audio_tag_bytes;
static unsigned dhd_audio_event;
static u32 dhd_audio_driver_base;
struct DhdAudioPending { u32 address, bytes, first, count; bool loading; };
static DhdAudioPending dhd_audio_pending[32];
static u32 DhdAudioWord(const u8* p)
{
    return p[0] | (u32(p[1]) << 8) | (u32(p[2]) << 16) | (u32(p[3]) << 24);
}
void DhdAudioDriverFrame(unsigned sample)
{
    // Retail driver offsets documented in finding 022. Observe header state;
    // do not invoke a query, change IOP memory, or wait for a host thread.
    if (!dhd_audio_driver_base || sample % 16) return;
    static FILE* state;
    static u32 handles[8], flags[8], last_rng=~0u;
    if (!state) {
        const char* dir=std::getenv("DHD_AUDIO_REFERENCE"); if (!dir) return;
        state=std::fopen((std::string(dir)+"\\driver-state.csv").c_str(),"wb");
        if (!state) std::exit(34);
        std::fprintf(state,"sample,kind,slot,handle,value,aux\n");
    }
    const u8* globals=iopPhysMem(dhd_audio_driver_base);
    static FILE* grains;
    static u32 grain_handles[64], grain_pcs[64], grain_masks[64][2];
    if (!grains) {
        const char* dir=std::getenv("DHD_AUDIO_REFERENCE");
        if (dir) grains=std::fopen((std::string(dir)+"\\driver-grains.csv").c_str(),"wb");
        if (grains) std::fprintf(grains,"sample,slot,handle,pc,delay,mask0,mask1,soundptr,bankptr,sounds\n");
    }
    for (u32 i=0;grains && i<64;i++) {
        const u8* h=globals+0x1ee68+i*284;
        u32 handle=DhdAudioWord(h);
        if ((handle&0x9f000000)!=0x85000000) continue;
        u32 pc=h[66]|(u32(h[67])<<8), mask0=DhdAudioWord(h+24), mask1=DhdAudioWord(h+28);
        if (handle==grain_handles[i] && pc==grain_pcs[i] && mask0==grain_masks[i][0] && mask1==grain_masks[i][1]) continue;
        u32 bank=DhdAudioWord(h+80)&0x1fffffff;
        std::fprintf(grains,"%u,%u,0x%08X,%u,%d,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X\n",
            sample,i,handle,pc,s16(h[64]|(u32(h[65])<<8)),mask0,mask1,DhdAudioWord(h+4),bank,
            bank<0x200000-32?DhdAudioWord(iopPhysMem(bank)+28):0);
        grain_handles[i]=handle;grain_pcs[i]=pc;grain_masks[i][0]=mask0;grain_masks[i][1]=mask1;
    }
    if (grains) std::fflush(grains);
    u32 rng=globals[0x1d964]|(u32(globals[0x1d965])<<8);
    if (rng!=last_rng && rng<250) {
        u32 prev=(rng+249)%250, at=0x1d968+prev*2;
        std::fprintf(state,"%u,rng,%u,0,%u,%u\n",sample,rng,
            globals[at]|(u32(globals[at+1])<<8),dhd_audio_driver_base);
        last_rng=rng;
    }
    u32 count=DhdAudioWord(globals+0x1e244), array=DhdAudioWord(globals+0x1e240)&0x1fffffff;
    if (count>8 || array>0x200000-count*80) return;
    for (u32 i=0;i<count;i++) {
        const u8* header=iopPhysMem(array+i*80);
        u32 handle=DhdAudioWord(header), flag=header[22];
        if (handle!=handles[i] || flag!=flags[i]) {
            std::fprintf(state,"%u,stream,%u,0x%08X,%u,0x%08X\n",sample,i,handle,flag,DhdAudioWord(header+16));
            handles[i]=handle; flags[i]=flag;
        }
    }
    std::fflush(state);
}
static void DhdAudioRequest(unsigned command, const u8* data, unsigned bytes)
{
    static FILE* events;
    if (!events) {
        const char* directory = std::getenv("DHD_AUDIO_REFERENCE");
        if (!directory) return;
        const std::string path = std::string(directory) + "\\rpc-events.csv";
        events = std::fopen(path.c_str(), "wb");
        if (!events) { std::fprintf(stderr, "DHD_AUDIO_REQUEST_OPEN_ERROR\n"); std::exit(31); }
        std::fprintf(events, "event,sample,field,command,length,payload_words_hex\n");
    }
    std::fprintf(events, "%u,%u,%u,0x%04X,%u", dhd_audio_event++, DhdAudioSamplesForRequests(), g_FrameCount, command, bytes);
    for (unsigned i = 0; i + 4 <= bytes; i += 4)
        std::fprintf(events, ",0x%08X", DhdAudioWord(data + i));
    std::fprintf(events, "\n");
    std::fflush(events);
}
void DhdAudioRpcResponse(u32 address, const u8* data, unsigned bytes)
{
    address &= 0x1fffffff;
    for (auto& pending : dhd_audio_pending) {
        if (!pending.count || address != pending.address || bytes < pending.bytes) continue;
        static FILE* results;
        if (!results) {
            const char* directory = std::getenv("DHD_AUDIO_REFERENCE");
            if (!directory) return;
            const std::string path = std::string(directory) + "\\rpc-results.csv";
            results = std::fopen(path.c_str(), "wb");
            if (!results) std::exit(32);
            std::fprintf(results, "event,sample,result\n");
        }
        for (unsigned i=0; i<pending.count; i++) {
            const unsigned at = pending.loading ? 0 : (i+1)*4;
            if (at+4 > pending.bytes) break;
            std::fprintf(results, "%u,%u,0x%08X\n",pending.first+i,DhdAudioSamplesForRequests(),DhdAudioWord(data+at));
        }
        std::fflush(results); pending.count=0;
    }
}
static void DhdAudioRpcDelivered()
{
    // Only complete command packets, never partial DMA chunks or payload tags.
    if (dhd_audio_tag_bytes < 56 || dhd_audio_tag_address > 0x200000 - dhd_audio_tag_bytes) return;
    const u8* packet = iopPhysMem(dhd_audio_tag_address);
    if (DhdAudioWord(packet + 8) != 0x8000000a || (packet[0] < 56 || packet[0] > 112)) return;
    const u32 server_address = DhdAudioWord(packet + 52) & 0x1fffffff;
    if (server_address > 0x200000 - 12) return;
    const u8* server = iopPhysMem(server_address);
    const u32 sid = DhdAudioWord(server);
    if (sid != 0x123456 && sid != 0x123457) return;
    if (sid==0x123456) {
        u32 callback=DhdAudioWord(server+4)&0x1fffffff;
        if (callback>=0x177c && callback-0x177c<0x200000-0x1e248)
            dhd_audio_driver_base=callback-0x177c;
    }
    const u32 command = DhdAudioWord(packet + 32), bytes = DhdAudioWord(packet + 36);
    const u32 address = DhdAudioWord(server + 8) & 0x1fffffff;
    if (bytes > 4096 || address > 0x200000 - bytes) return;
    const u8* data = iopPhysMem(address);
    DhdAudioPending* pending=nullptr;
    for (auto& item : dhd_audio_pending) if (!item.count) { pending=&item; break; }
    if (!pending) std::exit(33);
    pending->address=DhdAudioWord(packet+40)&0x1fffffff;
    pending->bytes=DhdAudioWord(packet+44); pending->first=dhd_audio_event;
    pending->loading=sid==0x123457; pending->count=1;
    if (pending->loading) { DhdAudioRequest(0x10000|command,data,bytes); return; }
    if (command != 0x4d) { DhdAudioRequest(command, data, bytes); return; }
    if (bytes < 4) return;
    const unsigned count = DhdAudioWord(data);
    pending->count=count;
    unsigned at = 4;
    for (unsigned i = 0; i < count && at + 4 <= bytes; i++) {
        const unsigned cmd = data[at] | (unsigned(data[at + 1]) << 8);
        const unsigned size = data[at + 2] | (unsigned(data[at + 3]) << 8);
        if (size > bytes - at - 4) return;
        DhdAudioRequest(cmd, data + at + 4, size);
        at += (size + 7) & ~3u;
    }
}
'''


def instrument(source: Path, private: Path, spu_unit: str):
    unit = (source / 'pcsx2/Sif1.cpp').read_text(encoding='utf-8')
    anchor = '_sif sif1;'
    assert unit.count(anchor) == 1
    unit = unit.replace(anchor, '#include "Counters.h"\n#include <cstdlib>\n'
                        '#include <cstdio>\n' + HOOK + '\n' + anchor)
    anchor = '\tsif1.iop.counter -= readSize;'
    assert unit.count(anchor) == 1
    unit = unit.replace(anchor, anchor + '\n\tif (!sif1.iop.counter) DhdAudioRpcDelivered();')
    anchor = '\tsif1.iop.counter = sif1words & 0xFFFFC;'
    assert unit.count(anchor) == 1
    unit = unit.replace(anchor, anchor + '\n\tdhd_audio_tag_address = hw_dma10.madr;'
                        '\n\tdhd_audio_tag_bytes = sif1.iop.counter * 4;')
    target = private / 'Sif1-audio.cpp'
    target.write_text(unit, encoding='utf-8', newline='\n')
    # The same captured sample counter supplies both waveform and event clocks.
    spu_unit += '\nunsigned DhdAudioSamplesForRequests() { return dhd_audio_samples; }\n'
    spu_unit = spu_unit.replace('static void DhdAudioCapture(',
        'extern void DhdAudioDriverFrame(unsigned);\nstatic void DhdAudioCapture(')
    spu_unit = spu_unit.replace('    dhd_audio_samples++;',
        '    DhdAudioDriverFrame(dhd_audio_samples);\n    dhd_audio_samples++;')
    reply = (source / 'pcsx2/Sif0.cpp').read_text(encoding='utf-8')
    anchor = '_sif sif0;'
    reply = reply.replace(anchor, 'extern void DhdAudioRpcResponse(u32, const u8*, unsigned);\n' + anchor)
    anchor = '\tsif0.fifo.read((u32*)ptag, readSize << 2);'
    assert reply.count(anchor) == 1
    # Replies are small (<= one FIFO transfer in the exercised menu). The probe
    # refuses partial fragments rather than inventing a result from junk words.
    reply = reply.replace(anchor, anchor + '\n\tDhdAudioRpcResponse(sif0ch.madr, '
                          'reinterpret_cast<const u8*>(ptag), readSize << 4);')
    reply_target = private / 'Sif0-audio.cpp'
    reply_target.write_text(reply, encoding='utf-8', newline='\n')
    items = f'''<ClCompile Remove="Sif1.cpp" />
<ClCompile Include="{target}"><ObjectFileName>$(IntDir)Sif1-audio.obj</ObjectFileName><PrecompiledHeader>NotUsing</PrecompiledHeader></ClCompile>'''
    items += f'''<ClCompile Remove="Sif0.cpp" />
<ClCompile Include="{reply_target}"><ObjectFileName>$(IntDir)Sif0-audio.obj</ObjectFileName><PrecompiledHeader>NotUsing</PrecompiledHeader></ClCompile>'''
    return spu_unit, items
