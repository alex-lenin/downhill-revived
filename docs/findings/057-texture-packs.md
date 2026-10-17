# Round 40: texture replacement packs (upscale mods) in PCSX2's format

Status: committed. The native renderer (`DHD_RENDER=native`) loads PCSX2 texture replacement
packs unchanged: a folder of PNGs named by PCSX2's texture hash (in PCSX2,
`textures\SCUS-97177\replacements`). Code: `port/gs/texpack.c` (names, index, loaders, budget),
`port/gs/gpu_native.inc` (`native_replace`, the sampler states), `port/gs/raster.hlsl`
(`native_replacement`, flag 2048). Packs are their authors' work: players supply them, nothing
of a pack is in the repository or the release.

Sources: PCSX2 v2.8.2 (GPL-3.0-or-later), `pcsx2/GS/Renderers/HW/GSTextureReplacements.cpp`,
`GSTextureReplacementLoaders.cpp`, `GSTextureCache.cpp` (`HashCacheKey::Create`,
`HashTextureLevel`, `SourceRegion::Create`, `LookupHashCache`), `GSClut.cpp` (`Read32`,
`Expand16`), `GSLocalMemory.cpp` (block sizes, `fmsk`), `GSState.cpp` (`GetTex0Layer`, MTBA), in
the local tree `build/port/compare-pcsx2-src` (finding 025). The scheme is reimplemented in C;
no PCSX2 code is copied. XXH3-64 is xxHash 0.8.3 itself (BSD-2-Clause, `port/gs/xxhash.h`, the
version PCSX2 ships). PRs read for the history: PCSX2 #5547 (the first texture dumping and
replacement, whose name had TCC in bit 14) and #5574 (replacement fixes, no hash change).

## The name

    <TEX0 hash>-<CLUT hash>-<bits>.png    paletted textures (PSMT8, T4, T8H, T4HL, T4HH)
    <TEX0 hash>-<bits>.png                the others

Hashes are lower-case hex without padding (`%llx`), bits are `%08x`.

- **TEX0 hash**: XXH3-64 (seed 0) of the texture's GS memory, as `HashTextureLevel` reads it.
  - Formats whose bits fill each 32-bit word (`fmsk` all ones: PSMCT32, PSMT8, PSMT4, PSMZ32),
    when the texture is at least one block (8x8, T8 16x16, T4 32x16): the raw 256-byte blocks it
    covers, row of blocks by row of blocks, left to right, each block from the format's block
    layout (`gs_pixel_address` at the block's corner, block number masked to 0x3FFF).
  - Otherwise (PSMCT24, CT16, the 8H/4HL/4HH formats, textures smaller than a block): the
    texels expanded, row by row: one index byte each for the paletted formats, a 32-bit colour
    (TEXA applied, as `gs_color`) for the direct ones.
  - A mipmapped draw (MXL > 0, MMIN a mipmapped filter, PCSX2's "full" mipmapping) continues the
    same hash over levels 1..MXL (at most 6), from MIPTBP1/2, each level's TW/TH reduced by its
    number as `GetTex0Layer` does.
- **CLUT hash**: XXH3-64 of the palette as `GSClut::Read32` leaves it, 16 or 256 32-bit
  entries: CT32/CT24 entries as stored (CSA applied; T8 clamps the group to 240), CT16 expanded
  with TEXA (`Expand16`: alpha TA0 or TA1 by bit 15, 0 for a zero colour when AEM).
- **bits**: `PSM | TW << 6 | TH << 10`, and for the 16- and 24-bit direct formats (and Z24/Z16)
  `TA0 << 15 | AEM << 23 | TA1 << 24`. Bit 14 was TCC in the first PCSX2 versions (PR #5547):
  every name in the user's pack has it set; PCSX2 now ignores it and so do we.

What we don't compute: PCSX2's region names (`-rWxH-`, region clamp narrower than the texture,
or its "recent upload" estimate) and the base-level shift of distant mipmapped draws (lod.x > 0
hashes from a smaller level). The user's pack has no region names. `-mipN` files are dumps of
mip levels; PCSX2 never loads them by name (they don't parse) and neither do we.

Lookup order: the name with mip levels (if the draw is mipmapped), the base level alone, then
both again without TEXA in the bits (packs dumped by versions that had no TEXA there).

## Results on the user's pack

The pack (someone else's mod, read in place, never copied): 6,181 base PNGs and 3,383 `-mipN`
dumps in one folder; no region names; T8 and T4 nearly all of it (plus 44 T4HH, 36 T4HL, 27 T8H,
1 CT24); every replacement 4x the GS size (512-2048 pixels a side); bit 14 set in every name
(dumped by an early PCSX2).

The scripted race (`race_compare.py`'s NAV, race pad and seed; menus, rider and track choice,
loading flythrough, race to field 7000) at 2x, `DHD_TEXTURE_PACK_LOG`:

| part | textures drawn | in the pack |
| --- | --- | --- |
| menus (fields < 1500) | 161 | 161 |
| track flythrough and race | 110 | 110 |
| all, by PSM | T8 173, T4 95, T4HL 2, CT24 1 | 271 (100%), 40 found with mip levels |

So the hash is right for block-hashed T8/T4, expanded T4HL (non-full `fmsk`), CT24 (expanded with
TEXA, the name's TEXA bits) and the mip-level variant. Draw states using a replacement: 749,363
over the run. What the lookup doesn't see: render targets used as textures (`from_surface`) and
the native renderer's unsupported textures (region repeat and the like) keep the game's own
pixels, as PCSX2 never replaces render targets either.

Loading and memory: 271 PNGs loaded, 24-62 ms each on a loader thread (WIC decode + mips + upload),
all during the menus and the track's flythrough. The race peaks at 738 MB of replacement textures
(R8G8B8A8 with mips) within the default budget, 1,536 MB on this PC (half the dedicated memory DXGI reports). Process
memory: peak private 356 MB with the pack against 346 MB off (the textures are in graphics memory).
With `texture_cache_mb = 200`: 228 replacements released and 339 loaded over the run, peak 243 MB
(textures drawn in the last 8 fields are never released), the race picture identical.

Pictures (build/port/r40tex, not tracked): `shots-off/`, `shots-aniso/` (pack on, final build),
`shots-on/` (trilinear without anisotropy, the first build), `cmp-*.png` side by side,
`crop-race.png`, `crop-menu.png`, `cmp3-f001600.png` (off / trilinear / anisotropic). Trilinear
alone softened the ground at low angles against the original (sampled without mips, sharp but
aliased); 16x anisotropic filtering brought most of the detail back.

Gate (on the first builds, based on 076de351; main later carries two other changed words, finding
055): `gate_native_renderer.py` passes with `DHD_TEXTURES=off`, and with the pack on (EE dumps
and 1,154 flare Z readbacks match: replacement alpha changes no Z the game reads back in this
race). With no pack or textures off the pictures are byte-identical to the base build 076de351 at
eight fields from the menus to the race.

Speed, measured on a busy PC (the user working, other workers' games and builds running; the
planned interleaved benchmarks were stopped part way, so these are a sanity check, not a
benchmark):
- Paced at 60 (`DHD_FRAME_CLOCK=0`), pack on, one run, fields 1000-4000 (rider and track menus,
  the flythrough where most replacements stream in, the race start): 59.56 fields/s, p50
  16.7 ms, p99 21.6 ms, worst 1% 30.4 ms, max 69.4 ms. 17 of 3,000 frames took over 25 ms. The
  big ones fall on menu screen changes where 41-55 new textures appear at once (fields 1120,
  1464, 1756-1767), with the GS worker busy (`DHD_TIMELINE`: the EE waits for it); two (1402,
  3170) have nothing loading. No comparable run with the pack off was made, so how much of
  those hitches is the pack (the game uploads its own textures at the same moments) is open.
- Unpaced race, fields 6000-7000, everything already loaded, three interleaved runs before the
  stop: pack on 110-119 fields/s, off 106-132 (median 116 vs 132); GS worker 7.0-7.7 ms per
  field on against 6.4-7.3 off. 60 holds with room.
- The partial paced race run with the pack off: 59.94 fields/s, max 21.2 ms.

## Loading

- At start the pack folder is indexed recursively, file names only.
- A texture's replacement is looked up once per decode (`NativeTexture.pack`), on the GS
  worker, when the native renderer decodes a new texture (the decode key changes with its
  pages or palette, so the PCSX2 hash is computed only then).
- The first draw that uses a found replacement queues it. Two loader threads (below normal
  priority) decode the PNG with WIC (`IWICFormatConverter` to 32bppRGBA), build the mip chain
  (2x2 box), and create an immutable D3D11 texture with every level. Until it is ready the
  original texture draws: nothing waits on the render thread or the GS worker.
- Uploaded replacements count against a budget (`texture_cache_mb`; default half the card's
  dedicated memory, between 512 and 2048 MB, the process being 32-bit). Over it, the least
  recently drawn replacements that no field drew in the last 8 fields are released (the same
  margin the native texture cache uses for queued fields), and load again when needed.
- Alpha: PNG alpha is GS alpha, as in PCSX2 (its PNG loader copies RGBA bytes and makes a PNG
  without alpha 0x80, opaque): 128 is opaque. The shader reads it exactly as a decoded texture.
- Sampling (`native_replacement`): the GS texture coordinate divided by the GS texture size,
  so the replacement covers the same normalised space whatever its size; `SampleGrad` with the
  coordinate's derivatives. 16x anisotropic when the GS filters (TEX1 MMAG), else point
  magnification with linear minification between mip levels. PCSX2 (default trilinear
  "Automatic") samples a replacement with mips only for draws the GS mipmaps, else clamps to the
  base level (`GSRendererHW::EmulateTextureSampler`, `sampler.lodclamp`); its anisotropy is off
  by default. We always use the mips, trading the shimmer of a 4x texture minified from its base
  level for some softness. Wrap/clamp from CLAMP; region clamp is applied
  in the shader, kept half a replacement texel inside the region; region repeat (an address mask)
  keeps the original texture. TFX/TCC, fog and alpha test are unchanged.

## Config

- `texture_pack =` the pack's folder (or a folder holding `replacements\` or
  `SCUS-97177\replacements\`, PCSX2's layout). Empty: `textures\SCUS-97177\replacements`
  beside dhd.exe, if it exists.
- `textures = on|off` (default on; PC SETTINGS > Texture Pack, applied at the next start).
- `texture_cache_mb = N` the budget.
- Environment: `DHD_TEXTURE_PACK`, `DHD_TEXTURES`, `DHD_TEXTURE_CACHE_MB`; diagnostics
  `DHD_TEXTURE_PACK_LOG=file` (every distinct texture drawn: field, PSM, size, name, found).
- Without a pack nothing changes: `texpack_open` returns 0, no replacement is ever looked up,
  flag 2048 is never set.

## Left

- A paced pack-off/pack-on comparison on an idle PC, to tell whether the screen-change hitches
  are the pack's. If they are: spread loads (fewer per field) or move the texture creation off
  the loader threads' burst.
- Texture dumping in PCSX2's layout (to make packs): not done.
- PCSX2's region names (`-rWxH-`) and the base-level shift of distant mipmapped draws
  (lod.x > 0): not computed; this pack needs neither.
- Render targets used as textures and the native renderer's unsupported textures are never
  replaced (as in PCSX2 for render targets).
