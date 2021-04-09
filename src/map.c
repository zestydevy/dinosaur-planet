#include "common.h"

#define RGBA8(r, g, b, a) (((u32)(r) << 24) | ((u32)(g) << 16) | ((u32)(b) << 8) | (u32)(a))

typedef struct
{
/*0000*/    Gfx gfx0;
/*0008*/    Gfx gfx1;
/*0010*/    u32 unk_0x10;
/*0014*/    u32 primColor;
/*0018*/    u32 envColor;
/*001C*/    u32 unk_0x1c;
/*0020*/    u32 fillColor;
/*0024*/    u32 unk_0x24;
/*0028*/    u8 needsPipeSync;
/*0029*/    u8 dirtyFlags;
} DLBuilder;

extern u32 UINT_80092a98;
extern DLBuilder *gDLBuilder;

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80040FD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80040FF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041028.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041040.s")
#else
void _func_80041040(Gfx **gdl)
{
    Gfx *currGfx = &gDLBuilder->gfx0;
    u8 dirty;

    if (gDLBuilder->dirtyFlags & 0x1)
    {
        gDLBuilder->dirtyFlags &= ~0x1;
        dirty = TRUE;
    }
    else
    {
        dirty = gDLBuilder->gfx0.words.w0 != (**gdl).words.w0 || gDLBuilder->gfx0.words.w1 != (**gdl).words.w1;
    }

    if (dirty)
    {
        *currGfx = **gdl;

        if (gDLBuilder->needsPipeSync)
        {
            gDLBuilder->needsPipeSync = FALSE;

            gDPPipeSync((*gdl)++);

            **gdl = *currGfx;
        }

        (*gdl)++;
    }
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041118.s")
#else
void _func_80041118(Gfx **gdl)
{
    Gfx *currGfx = &gDLBuilder->gfx1;
    u8 dirty;

    if (UINT_80092a98 & 0x2000) {
        (**gdl).words.w1 &= ~0x30;
    }

    if (gDLBuilder->dirtyFlags & 0x2)
    {
        gDLBuilder->dirtyFlags &= ~0x2;
        dirty = TRUE;
    }
    else
    {
        dirty = gDLBuilder->gfx1.words.w0 != (**gdl).words.w0 || gDLBuilder->gfx1.words.w1 != (**gdl).words.w1;
    }

    if (dirty)
    {
        *currGfx = **gdl;

        if (gDLBuilder->needsPipeSync)
        {
            gDLBuilder->needsPipeSync = FALSE;

            gDPPipeSync((*gdl)++);

            **gdl = *currGfx;
        }

        (*gdl)++;
    }
}
#endif

void func_80041210(Gfx **gdl)
{
    u8 dirty;

    if (UINT_80092a98 & 0x2000) {
        (**gdl).words.w1 &= ~0x1;
    }

    if (gDLBuilder->dirtyFlags & 0x40) {
        gDLBuilder->dirtyFlags &= ~0x40;
        dirty = TRUE;
    } else {
        dirty = gDLBuilder->unk_0x10 != (**gdl).words.w1;
    }

    if (dirty) {
        gDLBuilder->unk_0x10 = (**gdl).words.w1;
        (*gdl)++;
    }
}

void dl_set_geometry_mode(Gfx **gdl, u32 mode)
{
    mode |= gDLBuilder->unk_0x10;

    // FIXME: This macro requires the use of #define F3DEX_GBI_2x, but gbi.h fails if it is defined.
    // gSPSetGeometryMode(*gdl, mode);
    {
        Gfx *_g = (Gfx *)((*gdl));
                                        \
        _g->words.w0 = _SHIFTL(G_GEOMETRYMODE, 24, 8);
        _g->words.w1 = (unsigned int)(mode);
    }

    func_80041210(gdl);
}

void dl_clear_geometry_mode(Gfx **gdl, u32 mode)
{
    mode = gDLBuilder->unk_0x10 & ~mode;

    // FIXME: This macro requires the use of #define F3DEX_GBI_2x, but gbi.h fails if it is defined.
    // gSPSetGeometryMode(*gdl, mode);
    {
        Gfx *_g = (Gfx *)((*gdl));
                   
        // FIXME: does this actually clear geometry mode bits?
        // then why does it call gSPSetGeometryMode?
        // gSPClearGeometryMode puts bits to clear in w0.
        _g->words.w0 = _SHIFTL(G_GEOMETRYMODE, 24, 8);
        _g->words.w1 = (unsigned int)(mode);
    }

    func_80041210(gdl);
}

void dl_set_prim_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a)
{
    u32 rgba = RGBA8(r, g, b, a);
    u8 dirty;

    if (gDLBuilder->dirtyFlags & 0x4)
    {
        gDLBuilder->dirtyFlags &= ~0x4;
        dirty = TRUE;
    }
    else
    {
        dirty = rgba != gDLBuilder->primColor;
    }

    if (dirty)
    {
        gDLBuilder->primColor = rgba;
        gDPSetPrimColor((*gdl)++, 0, 0, r, g, b, a);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041418.s")

void dl_set_env_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a)
{
    u32 rgba = RGBA8(r, g, b, a);
    u8 dirty;

    if (gDLBuilder->dirtyFlags & 0x8)
    {
        gDLBuilder->dirtyFlags &= ~0x8;
        dirty = TRUE;
    }
    else
    {
        dirty = rgba != gDLBuilder->envColor;
    }

    if (dirty)
    {
        if (gDLBuilder->needsPipeSync)
        {
            gDLBuilder->needsPipeSync = FALSE;
            gDPPipeSync((*gdl)++);
        }

        gDLBuilder->envColor = rgba;
        gDPSetEnvColor((*gdl)++, r, g, b, a);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800415A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041608.s")

void dl_set_fill_color(Gfx **gdl, u32 color)
{
    u8 dirty;

    if (gDLBuilder->dirtyFlags & 0x10)
    {
        gDLBuilder->dirtyFlags &= ~0x10;
        dirty = TRUE;
    }
    else
    {
        dirty = color != gDLBuilder->fillColor;
    }

    if (dirty)
    {
        if (gDLBuilder->needsPipeSync)
        {
            gDLBuilder->needsPipeSync = FALSE;
            gDPPipeSync((*gdl)++);
        }

        gDLBuilder->fillColor = color;
        gDPSetFillColor((*gdl)++, color);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800417C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800418E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041C30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041C6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041CA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041CE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041D20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041D5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041D74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041D8C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041DA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041DBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041DD4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041DEC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041E08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041E24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041E68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/init_maps.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80042174.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004225C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/track_c_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80042DA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800436DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80043950.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80043E00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80043FD8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800441F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044320.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044448.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004454C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004478C.s")


#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/map_get_map_id_from_xz_ws.s")
#else
s32 floor_f(f32);
extern s32 gMapCurrentStreamCoords;
extern s32 D_80092A6C;
extern u32 *gDecodedGlobalMap;
s16 _map_get_map_id_from_xz_ws(f32 arg0, f32 arg1) {
    s32 temp_a1;
    s32 temp_v1;

    temp_a1 = floor_f(arg0 / 640.0f) - gMapCurrentStreamCoords;
    temp_v1 = floor_f(arg1 / 640.0f) - D_80092A6C;
    if ((temp_a1 < 0) || (temp_a1 >= 0x10)) {
        return -1;
    }
    if ((temp_v1 < 0) || (temp_v1 >= 0x10)) {
        return -1;
    }
    return *(gDecodedGlobalMap + (temp_v1 * 0x30 * 4) + (temp_a1 * 0xC));
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800448D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044A10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044A20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044A7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044B18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044B98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044BB0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044BEC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800451A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004530C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/some_cell_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045600.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800456AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045864.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_load_streammap.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_load_streammap_add_to_table.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045D58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_find_streammap_index.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045DC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045F48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045FC4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800462B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_convert_objpositions_to_ws.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046320.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046428.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046688.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046698.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800466C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046718.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046728.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/init_global_map.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_read_layout.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046B58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_update_streaming.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80047374.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800473BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80047404.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80047710.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80047724.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004773C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048024.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048034.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048054.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800483BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048478.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048498.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800484A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800485FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048724.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048B14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048C24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048D58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048E04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048F58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049024.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049110.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800493B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800496E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004984C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049870.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800499B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800499BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049B84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049CE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049D38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049D68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049D88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049E30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049FA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A058.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A164.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A1E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A284.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A2CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A2E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A528.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A5D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A67C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_update_objects_streaming.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004AEFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_should_stream_load_object.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B190.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B4A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B548.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B710.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_check_some_mapobj_flag.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B7D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B85C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B914.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B948.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B984.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004BC20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004BD40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004C040.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004C1A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D1D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D328.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D470.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D698.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D844.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D880.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D8A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D974.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D984.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D99C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D9AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D9B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004DABC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004DBAC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004E540.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004E64C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004E7A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004EEC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004F378.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004FA3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004FA4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004FA58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800502AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80050B88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800511E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800516BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80051944.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80051C54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80051CFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80051D68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80051F64.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80052148.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80052230.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80052300.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80052644.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800528AC.s")
