/* PCSX2-format texture replacement packs for the native renderer (texpack.c, finding 057). */
#ifndef PORT_GS_TEXPACK_H
#define PORT_GS_TEXPACK_H
#include "gs.h"
#include <d3d11.h>

typedef struct TexPackEntry TexPackEntry;

/* Reads DHD_TEXTURES and DHD_TEXTURE_PACK (dhd.ini "textures", "texture_pack"), indexes the
   pack's file names and starts the loaders. 0: no pack, nothing else here may be called. */
int texpack_open(ID3D11Device *device);

/* Whether a pack folder is named or beside dhd.exe (whatever DHD_TEXTURES says). */
int texpack_available(void);

/* A texture just decoded from GS memory: its replacement in the pack, or NULL. The name is
   PCSX2's, from the texture's GS memory, its CLUT and its context's TEX0/TEX1/MIPTBP registers
   (GS worker thread). */
TexPackEntry *texpack_find(const Gs *g, unsigned ctxt);

/* The replacement's view, if it is loaded; else NULL and its loading starts (once). The view
   stays valid for at least 8 fields after the last call that returned it. */
ID3D11ShaderResourceView *texpack_view(TexPackEntry *e, uint64_t field);

/* Once a field (GS worker): replacements over the memory budget that no recent field drew are
   released. */
void texpack_field(uint64_t field);

/* Counts at exit (stderr). */
void texpack_report(void);

#endif
