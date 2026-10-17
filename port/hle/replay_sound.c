/* Replay observed retail audio RPCs through the native mixer, at sample times.
   This is an audio-subsystem comparison, not a replay of game/renderer state.
   Requires the user's ISO in place; all logs and WAVs belong under build/port/audio-*.
   Build sound.c + this file with SND_NO_HOST, 32-bit/O2/SSE2 like the port. */
#define cdvd_read fixture_unused_disc_read
#define cdvd_find fixture_unused_disc_find
#define main sound_fixture_main
#include "test_sound.c"
#undef main
#undef cdvd_find
#undef cdvd_read
#include <inttypes.h>

static FILE *iso;
static FILE *wav;
int cdvd_read(uint32_t lsn, uint32_t sectors, void *destination)
{
    return !_fseeki64(iso,(int64_t)lsn * 2048,SEEK_SET) &&
           fread(destination,2048,sectors,iso) == sectors;
}

/* The mixer reads its reverb presets from the disc's libsd module: an ISO 9660 path lookup
   ("/DIR/FILE;1", plain names, as port/hle/cdvd.c answers it in the game). */
static uint32_t iso32(const uint8_t *p) { return p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24; }
int cdvd_find(const char *path, uint32_t *lsn, uint32_t *size)
{
    uint8_t sector[2048];
    uint32_t at, extent, length;
    if (!cdvd_read(16,1,sector)) return 0;
    extent=iso32(sector+158); length=iso32(sector+166);
    while (*path=='/' || *path=='\\') path++;
    while (*path) {
        size_t n=strcspn(path,"/\\;");
        int found=0;
        for (at=0; at<length && !found; at+=2048) {
            uint32_t i;
            if (!cdvd_read(extent+at/2048,1,sector)) return 0;
            for (i=0; i<2048 && sector[i] && !found; i+=sector[i]) {
                const uint8_t *r=sector+i;
                size_t id=r[32];
                if (id>=n && !_strnicmp((const char *)r+33,path,n) && (id==n || r[33+n]==';')) {
                    extent=iso32(r+2); length=iso32(r+10); found=1;
                }
            }
        }
        if (!found) return 0;
        path+=n;
        if (*path==';') break;
        while (*path=='/' || *path=='\\') path++;
    }
    *lsn=extent; *size=length;
    return 1;
}

static uint32_t retail_results[65536];
static uint8_t result_known[65536];
static struct { uint32_t retail, native; } handles[8192];
static int nhandles;

static uint32_t mapped(uint32_t value)
{
    int i;
    for (i=nhandles-1; i>=0; i--) if (handles[i].retail == value) return handles[i].native;
    return value;
}

static void remember(uint32_t retail, uint32_t native)
{
    if (!retail || !native) return;
    if (nhandles >= 8192) exit(6);
    handles[nhandles].retail=retail; handles[nhandles++].native=native;
}

static void advance(uint64_t *position, uint64_t end)
{
    static int16_t pcm[4800 * 2];
    while (*position < end) {
        int count = end-*position > 4800 ? 4800 : (int)(end-*position);
        snd_test_render(pcm,count); *position += count;
        if (fwrite(pcm,4,count,wav) != (size_t)count) exit(7);
    }
}

int main(int argc, char **argv)
{
    char line[16384];
    FILE *events, *results, *report;
    unsigned char header[44] = {0};
    uint64_t position=0, end;
    unsigned requests=0, mismatches=0;
    const char *iso_path=getenv("DHD_ISO");
    if (argc != 6 || !iso_path || !(iso=fopen(iso_path,"rb")) ||
        !(events=fopen(argv[1],"r")) || !(results=fopen(argv[2],"r"))) return 2;
    end=_strtoui64(argv[5],NULL,0);
    snprintf(line,sizeof line,"%s.results.csv",argv[3]); report=fopen(line,"w");
    if (!report) return 2;
    wav=fopen(argv[3],"wb"); if (!wav) return 2;
    fwrite(header,1,sizeof header,wav);
    fputs("event,command,retail_result,native_result,equal_after_handle_mapping\n",report);
    fgets(line,sizeof line,results);
    while (fgets(line,sizeof line,results)) {
        char *p=strtok(line,","); unsigned id=(unsigned)strtoul(p,NULL,0);
        strtok(NULL,","); p=strtok(NULL,",");
        if (!p || id>=65536) return 3;
        retail_results[id]=(uint32_t)strtoul(p,NULL,0); result_known[id]=1;
    }
    fclose(results);
    debug_env("DHD_SND_WAV",""); debug_env("DHD_SND_REPORT",argv[4]); snd_test_reset();
    snd_test_report();
    fgets(line,sizeof line,events);
    while (fgets(line,sizeof line,events)) {
        uint32_t payload[1024]={0}, reply[1024]={0}, actual, expected;
        unsigned id, cmd, bytes, i=0; uint64_t when; char *p;
        p=strtok(line,","); id=(unsigned)strtoul(p,NULL,0);
        p=strtok(NULL,","); if (!p) return 3; when=_strtoui64(p,NULL,0);
        strtok(NULL,","); p=strtok(NULL,","); if (!p) return 3; cmd=(unsigned)strtoul(p,NULL,0);
        p=strtok(NULL,","); if (!p) return 3; bytes=(unsigned)strtoul(p,NULL,0);
        if (id>=65536 || bytes>sizeof payload || when<position) return 3;
        while ((p=strtok(NULL,",")) && i<1024) payload[i++]=(uint32_t)strtoul(p,NULL,0);
        if (i*4 != bytes) return 3;
        if (when>end) break;
        if (cmd==0x38) { fputs("disc-to-EE writes are outside audio replay\n",stderr); return 8; }
        advance(&position,when);
        switch(cmd) {
        case 0x06: case 0x13: case 0x14: case 0x15: case 0x19: case 0x1B: case 0x1C:
        case 0x1F: case 0x20: case 0x21: case 0x2D: case 0x2E: case 0x2F:
        case 0x30: case 0x31: case 0x32: case 0x4F: case 0x5C:
            payload[0]=mapped(payload[0]); break;
        case 0x2C: payload[5]=mapped(payload[5]); break;
        }
        if (cmd>=0x10000) {
            sound_rpc2(cmd&0xffff,payload,bytes,reply,sizeof reply); actual=reply[0];
        } else {
            sound_rpc(cmd,payload,bytes,reply,12); actual=reply[1];
        }
        requests++;
        /* Calls without observed replies still play (e.g. NOWAIT). Unknown
           results never supply guessed handle maps or parity assertions. */
        if (!result_known[id]) continue;
        expected=retail_results[id];
        if (cmd==0x10003 || cmd==0x5C || (cmd==0x2C && !payload[5])) {
            remember(expected,actual); expected=mapped(expected);
        } else if (cmd==0x19) expected=mapped(expected);
        /* Void commands return driver scratch state; only semantic results
           are compared. Handle bits legitimately differ in the HLE allocator. */
        if (cmd==0x10003 || cmd==0x5C || cmd==0x2C || cmd==0x19 || cmd==0x4F || cmd==0x0A ||
            cmd==0x30 || cmd==0x31 || cmd==0x32) {
            int equal=expected==actual; mismatches+=!equal;
            fprintf(report,"%u,0x%04X,0x%08X,0x%08X,%d\n",id,cmd,retail_results[id],actual,equal);
        }
    }
    advance(&position,end); snd_test_report(); snd_test_reset(); fclose(events); fclose(report); fclose(iso);
    memcpy(header,"RIFF",4); put32(header+4,(uint32_t)position*4+36); memcpy(header+8,"WAVEfmt ",8);
    put32(header+16,16); header[20]=1; header[22]=2; put32(header+24,48000);
    put32(header+28,192000); header[32]=4; header[34]=16; memcpy(header+36,"data",4);
    put32(header+40,(uint32_t)position*4); fseek(wav,0,SEEK_SET); fwrite(header,1,44,wav); fclose(wav);
    printf("requests=%u; compared_result_mismatches=%u; output_samples=%" PRIu64 "\n",requests,mismatches,position);
    return 0;
}
