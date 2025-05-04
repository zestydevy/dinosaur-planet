/** Filesystem and ROM access
 */
#ifndef _SYS_FS_H
#define _SYS_FS_H

#include "PR/ultratypes.h"

typedef enum
{
/*00*/    AUDIO_TAB,
/*01*/    AUDIO_BIN,
/*02*/    SFX_TAB,
/*03*/    SFX_BIN,
/*04*/    AMBIENT_TAB,
/*05*/    AMBIENT_BIN,
/*06*/    MUSIC_TAB,
/*07*/    MUSIC_BIN,
/*08*/    MPEG_TAB,
/*09*/    MPEG_BIN,
/*0A*/    MUSICACTIONS_BIN,
/*0B*/    CAMACTIONS_BIN,
/*0C*/    LACTIONS_BIN,
/*0D*/    ANIMCURVES_BIN,
/*0E*/    ANIMCURVES_TAB,
/*0F*/    OBJSEQ2CURVE_TAB,
/*10*/    FONTS_BIN,
/*11*/    CACHEFON_BIN,
/*12*/    CACHEFON2_BIN,
/*13*/    GAMETEXT_BIN,
/*14*/    GAMETEXT_TAB,
/*15*/    GLOBALMAP_BIN,
/*16*/    TABLES_BIN,
/*17*/    TABLES_TAB,
/*18*/    SCREENS_BIN,
/*19*/    SCREENS_TAB,
/*1A*/    VOXMAP_BIN,
/*1B*/    VOXMAP_TAB,
/*1C*/    TEXPRE_TAB,
/*1D*/    TEXPRE_BIN,
/*1E*/    WARPTAB_BIN,
/*1F*/    MAPS_BIN,
/*20*/    MAPS_TAB,
/*21*/    MAPINFO_BIN,
/*22*/    MAPSETUP_IND,
/*23*/    MAPSETUP_TAB,
/*24*/    TEX1_BIN,
/*25*/    TEX1_TAB,
/*26*/    TEXTABLE_BIN,
/*27*/    TEX0_BIN,
/*28*/    TEX0_TAB,
/*29*/    BLOCKS_BIN,
/*2A*/    BLOCKS_TAB,
/*2B*/    TRKBLK_BIN,
/*2C*/    HITS_BIN,
/*2D*/    HITS_TAB,
/*2E*/    MODELS_TAB,
/*2F*/    MODELS_BIN,
/*30*/    MODELIND_BIN,
/*31*/    MODANIM_TAB,
/*32*/    MODANIM_BIN,
/*33*/    ANIM_TAB,
/*34*/    ANIM_BIN,
/*35*/    AMAP_TAB,
/*36*/    AMAP_BIN,
/*37*/    BITTABLE_BIN,
/*38*/    WEAPONDATA_BIN,
/*39*/    VOXOBJ_TAB,
/*3A*/    VOXOBJ_BIN,
/*3B*/    MODLINES_BIN,
/*3C*/    MODLINES_TAB,
/*3D*/    SAVEGAME_BIN,
/*3E*/    SAVEGAME_TAB,
/*3F*/    OBJSEQ_BIN,
/*40*/    OBJSEQ_TAB,
/*41*/    OBJECTS_TAB,
/*42*/    OBJECTS_BIN,
/*43*/    OBJINDEX_BIN,
/*44*/    OBJEVENT_BIN,
/*45*/    OBJHITS_BIN,
/*46*/    DLLS_BIN,
/*47*/    DLLS_TAB,
/*48*/    DLLSIMPORTTAB_BIN,
/*49*/    ENVFXACT_BIN,

/*4A*/    NUM_FILES
} EFile;

typedef struct
{ 
    s32 fileCount;
    u32 offsets[NUM_FILES];
} Fs;

void init_filesystem(void);
void *read_alloc_file(u32 id, u32 a1);
s32 read_file(u32 id, void *dest);
s32 read_file_region(u32 id, void *dst, u32 offset, s32 size);
s32 file_get_romaddr(u32 id, s32 offset);
s32 get_file_size(u32 id);

#endif //_SYS_FS_H
