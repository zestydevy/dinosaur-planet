#include "macros.h"
#include "game/gamebits.h"
#include "sys/gfx/animseq.h"
#include "sys/main.h"
#include "sys/menu.h"
#include "sys/objtype.h"
#include "sys/objprint.h"
#include "sys/segment_1050.h"
#include "sys/segment_1460.h"
#include "dll.h"

#define PUZZLE_UNIT 48

#define SUN_PUZZLE_ORIGIN_X 32
#define SUN_PUZZLE_ORIGIN_Z 129

#define MOON_PUZZLE_ORIGIN_X 224
#define MOON_PUZZLE_ORIGIN_Z 128

typedef struct {
    f32 timer;
    u8 state;
    u8 flags;
    u8 previousState;
} WCLevelControl_Data;

typedef enum {
    FLAG_0 = 0x0,
    FLAG_1 = 0x1,
    FLAG_2 = 0x2,
    FLAG_4 = 0x4,
    FLAG_8 = 0x8,
    FLAG_10 = 0x10,
    FLAG_20 = 0x20,
    FLAG_40 = 0x40,
    FLAG_80 = 0x80
} WCLevelControl_Flags;

typedef enum {
    STATE_0,
    STATE_1,
    STATE_2,
    STATE_3,
    STATE_4,
    STATE_5,
    STATE_6
} WCLevelControl_States;

/*0x0*/ static u8 dSunPuzzleHard[8][8] = {
    {0, 0, 0, 0, 0, 0, 8, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 4, 0, 0, 0}, 
    {0, 2, 0, 0, 0, 0, 3, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 7}, 
    {0, 0, 0, 0, 6, 0, 0, 0}, 
    {0, 5, 0, 0, 1, 0, 0, 0}
};
/*0x40*/ static u8 dSunPuzzleEasy[8][8] = {
    {0, 0, 0, 0, 0, 0, 4, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 3}, 
    {0, 0, 0, 0, 2, 0, 0, 0}, 
    {0, 1, 0, 0, 0, 0, 0, 0}
};
/*0x80*/ static u8 dMoonPuzzleHard[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 1, 5, 0, 0, 2, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 3}, 
    {0, 0, 0, 0, 0, 0, 6, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 7}, 
    {0, 8, 0, 0, 4, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}
};
/*0xC0*/ static u8 dMoonPuzzleEasy[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 1, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 2, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 3}, 
    {0, 4, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}
};

/*0x0*/ static u8 sSunPuzzleCells[8][8];
/*0x40*/ static u8 sMoonPuzzleCells[8][8];

static int WCLevelControl_anim_callback(Object *self, Object *overrideObj, AnimObj_Data *animData, s8 a3);
static void WCLevelControl_handle_act1(Object *self, WCLevelControl_Data *objdata);
static void WCLevelControl_handle_act2(Object *self, WCLevelControl_Data *objdata);
static void WCLevelControl_sun_puzzle_init_hard(void);
static void WCLevelControl_moon_puzzle_init_hard(void);

// offset: 0x0 | ctor
void WCLevelControl_ctor(void *dll) { }

// offset: 0xC | dtor
void WCLevelControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCLevelControl_setup(Object *self, ObjSetup *setup, s32 arg2) {
    WCLevelControl_Data *objdata;

    objdata = self->data;
    self->animCallback = WCLevelControl_anim_callback;
    WCLevelControl_sun_puzzle_init_hard();
    WCLevelControl_moon_puzzle_init_hard();

    if (main_get_bits(BIT_7FA)) {
        objdata->flags |= FLAG_8;
    }
    if (main_get_bits(BIT_7F9)) {
        objdata->flags |= FLAG_4;
    }
    if (main_get_bits(BIT_813)) {
        objdata->flags |= FLAG_20;
    }
    if (main_get_bits(BIT_812)) {
        objdata->flags |= FLAG_10;
    }
    if (main_get_bits(BIT_2A5)) {
        objdata->flags |= FLAG_40;
    }
    obj_add_object_type(self, OBJTYPE_LevelControl);
    main_set_bits(BIT_226, 1);
    main_set_bits(BIT_2A6, 1);
    main_set_bits(BIT_206, 1);
    main_set_bits(BIT_25F, 1);
}

// offset: 0x1B4 | func: 1 | export: 1
void WCLevelControl_control(Object *self) {
    WCLevelControl_Data *objdata = self->data;
    f32 time;
    u8 act;

    if (self->unkDC == 0) {
        func_80000860(self, self, 0x1FB, 0);
        func_80000860(self, self, 0x1FC, 0);
        func_80000860(self, self, 0x149, 0);
        func_80000450(self, self, 0x97, 0, 0, 0);
        func_80000450(self, self, 0x24F, 0, 0, 0);
        self->unkDC = 1;
    }
    act = gDLL_29_Gplay->vtbl->get_act(self->mapID);
    if ((act == 1) || (act != 2)) {
        WCLevelControl_handle_act1(self, objdata);
    } else {
        WCLevelControl_handle_act2(self, objdata);
    }

    //Check if night-time
    if (gDLL_7_Newday->vtbl->func8(&time)) {
        main_set_bits(BIT_7F3, 1);
        main_set_bits(BIT_7F1, 0);
    } else {
        main_set_bits(BIT_7F3, 0);
        main_set_bits(BIT_7F1, 1);
    }
}

// offset: 0x3A0 | func: 2 | export: 2
void WCLevelControl_update(Object *self) { }

// offset: 0x3AC | func: 3 | export: 3
void WCLevelControl_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x400 | func: 4 | export: 4
void dll_779_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_LevelControl);
}

// offset: 0x440 | func: 5 | export: 5
u32 WCLevelControl_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x450 | func: 6 | export: 6
u32 WCLevelControl_get_data_size(Object *self, u32 a1) {
    return sizeof(WCLevelControl_Data);
}

// offset: 0x464 | func: 7 | export: 7
void WCLevelControl_sun_puzzle_set_coords_from_grid_position(SRT *arg0, s16 puzzleGridX, s16 puzzleGridZ, f32 *x, f32 *z) {
    f32 blockX;
    f32 blockZ;

    map_world_to_block_world_coords(arg0->transl.x, arg0->transl.y, arg0->transl.z, &blockX, &blockZ);
    *x = blockX + SUN_PUZZLE_ORIGIN_X + (puzzleGridX * PUZZLE_UNIT) + PUZZLE_UNIT/2;
    *z = blockZ + SUN_PUZZLE_ORIGIN_Z + (puzzleGridZ * PUZZLE_UNIT) + PUZZLE_UNIT/2;
}

// offset: 0x534 | func: 8 | export: 8
void WCLevelControl_sun_puzzle_set_grid_position_from_coords(SRT *arg0, f32 x, f32 z, s16 *puzzleGridX, s16 *puzzleGridZ) {
    f32 blockX;
    f32 blockZ;

    map_world_to_block_world_coords(arg0->transl.x, arg0->transl.y, arg0->transl.z, &blockX, &blockZ);
    *puzzleGridX = (s16) ((x - blockX) - SUN_PUZZLE_ORIGIN_X) / PUZZLE_UNIT;
    *puzzleGridZ = (s16) ((z - blockZ) - SUN_PUZZLE_ORIGIN_Z) / PUZZLE_UNIT;
}

// offset: 0x64C | func: 9 | export: 9
void WCLevelControl_sun_puzzle_set_cell(s16 puzzleBlockID, s16 x, s16 z) {
    if ((x >= 0) && (x < 8) && (z >= 0) && (z < 8)) {
        sSunPuzzleCells[x][z] = puzzleBlockID;
    } else {
        STUBBED_PRINTF("SUNBLOCK: invalid grid position\n");
    }
}

// offset: 0x6C0 | func: 10 | export: 10
u8 WCLevelControl_sun_puzzle_get_cell(s16 x, s16 z) {
    if ((x < 0) || (x >= 8) || (z < 0) || (z >= 8)) {
        STUBBED_PRINTF("SUNBLOCK: invalid grid position\n");
        return 0;
    }
    return sSunPuzzleCells[x][z];
}

// offset: 0x730 | func: 11 | export: 11
void WCLevelControl_sun_puzzle_setup_position_hard(s16 puzzleBlockID, s16 *outX, s16 *outZ) {
    s32 x;
    s32 z;

    for (x = 0; x < 8; x++) {
        for (z = 0; z < 8; z++) {
            if (puzzleBlockID == dSunPuzzleHard[x][z]) {
                *outX = x;
                *outZ = z;
                return;
            }
        }
    }

    STUBBED_PRINTF("SUN GRID PUZZLE: Error cannot find given id\n");
}

// offset: 0x7FC | func: 12 | export: 12
void WCLevelControl_sun_puzzle_setup_position_easy(s16 puzzleBlockID, s16 *outX, s16 *outZ) {
    s32 x;
    s32 z;

    for (x = 0; x < 8; x++) {
        for (z = 0; z < 8; z++) {
            if (puzzleBlockID == dSunPuzzleEasy[x][z]) {
                *outX = x;
                *outZ = z;
                return;
            }
        }
    }

    STUBBED_PRINTF("SUN GRID PUZZLE: Error cannot find given id\n");
}

// offset: 0x8C8 | func: 13 | export: 13
s32 WCLevelControl_sun_puzzle_func_8C8(SRT *arg0, s16 arg1, s16 arg2, f32 *arg3, f32 *arg4, s32 arg5, s32 arg6) {
    s32 var_v0;
    s32 var_a1;
    s32 var_a1_2;
    f32 sp38;

    if (arg5 != 0) {
        if (arg5 == -1) {
            WCLevelControl_sun_puzzle_set_coords_from_grid_position(arg0, 7, arg2, arg3, arg4);
            arg1 += 1;
            var_a1 = 8;
        } else {
            WCLevelControl_sun_puzzle_set_coords_from_grid_position(arg0, 0, arg2, arg3, arg4);
            arg1 -= 1;
            var_a1 = -1;
        }

        var_v0 = arg1;
        while (var_v0 != var_a1) {
            if (sSunPuzzleCells[var_v0][arg2] != 0) {
                if (sSunPuzzleCells[var_v0][arg2] < 5) {
                    var_v0 += arg5;
                    WCLevelControl_sun_puzzle_set_coords_from_grid_position(arg0, var_v0, arg2, arg3, &sp38);
                    return 1;
                } else {
                    WCLevelControl_sun_puzzle_set_coords_from_grid_position(arg0, var_v0, arg2, arg3, &sp38);
                    return 2;
                }
            }
            var_v0 -= arg5;
        }
    } else {
        if (arg6 == -1) {
            WCLevelControl_sun_puzzle_set_coords_from_grid_position(arg0, arg1, 7, arg3, arg4);
            arg2 += 1;
            var_a1_2 = 8;
        } else {
            WCLevelControl_sun_puzzle_set_coords_from_grid_position(arg0, arg1, 0, arg3, arg4);
            arg2 -= 1;
            var_a1_2 = -1;
        }

        var_v0 = arg2;
        while (var_v0 != var_a1_2) {
            if (sSunPuzzleCells[arg1][var_v0] != 0) {
                if (sSunPuzzleCells[arg1][var_v0] < 5) {
                    var_v0 += arg6;
                    WCLevelControl_sun_puzzle_set_coords_from_grid_position(arg0, arg1, var_v0, &sp38, arg4);
                    return 1;
                } else {
                    WCLevelControl_sun_puzzle_set_coords_from_grid_position(arg0, arg1, var_v0, &sp38, arg4);
                    return 2;
                }
            }
            var_v0 -= arg6;
        }
    }
    
    return 4;
}

// offset: 0xBA8 | func: 14 | export: 14
void WCLevelControl_moon_puzzle_set_coords_from_grid_position(SRT *arg0, s16 puzzleGridX, s16 puzzleGridZ, f32* x, f32 *z) {
    f32 blockX;
    f32 blockZ;

    map_world_to_block_world_coords(arg0->transl.x, arg0->transl.y, arg0->transl.z, &blockX, &blockZ);
    *x = blockX + MOON_PUZZLE_ORIGIN_X + (puzzleGridX * PUZZLE_UNIT) + PUZZLE_UNIT/2;
    *z = blockZ + MOON_PUZZLE_ORIGIN_Z + (puzzleGridZ * PUZZLE_UNIT) + PUZZLE_UNIT/2;
}

// offset: 0xC78 | func: 15 | export: 15
void WCLevelControl_moon_puzzle_set_grid_position_from_coords(SRT *arg0, f32 x, f32 z, s16 *puzzleGridX, s16 *puzzleGridZ) {
    f32 blockX;
    f32 blockZ;

    map_world_to_block_world_coords(arg0->transl.x, arg0->transl.y, arg0->transl.z, &blockX, &blockZ);
    *puzzleGridX = (s16)((x - blockX) - MOON_PUZZLE_ORIGIN_X) / PUZZLE_UNIT;
    *puzzleGridZ = (s16)((z - blockZ) - MOON_PUZZLE_ORIGIN_Z) / PUZZLE_UNIT;
}

// offset: 0xD90 | func: 16 | export: 16
void WCLevelControl_moon_puzzle_set_cell(s16 puzzleBlockID, s16 x, s16 z) {
    if ((x >= 0) && (x < 8) && (z >= 0) && (z < 8)) {
        sMoonPuzzleCells[x][z] = puzzleBlockID;
    } else {
        STUBBED_PRINTF("MOONBLOCK: invalid grid position\n");
    }
}

// offset: 0xE04 | func: 17 | export: 17
u8 WCLevelControl_moon_puzzle_get_cell(s16 x, s16 z) {
    if ((x < 0) || (x >= 8) || (z < 0) || (z >= 8)) {
        STUBBED_PRINTF("MOONBLOCK: invalid grid position\n");
        return 0;
    }
    return sMoonPuzzleCells[x][z];
}

// offset: 0xE74 | func: 18 | export: 18
void WCLevelControl_moon_puzzle_setup_position_hard(s16 puzzleBlockID, s16 *outX, s16 *outZ) {
    s32 x;
    s32 z;

    for (x = 0; x < 8; x++) {
        for (z = 0; z < 8; z++) {
            if (puzzleBlockID == dMoonPuzzleHard[x][z]) {
                *outX = x;
                *outZ = z;
                return;
            }
        }
    }

    STUBBED_PRINTF("MOON GRID PUZZLE: Error cannot find given id\n");
}

// offset: 0xF40 | func: 19 | export: 19
void WCLevelControl_moon_puzzle_setup_position_easy(s16 puzzleBlockID, s16 *outX, s16 *outZ) {
    s32 x;
    s32 z;

    for (x = 0; x < 8; x++) {
        for (z = 0; z < 8; z++) {
            if (puzzleBlockID == dMoonPuzzleEasy[x][z]) {
                *outX = x;
                *outZ = z;
                return;
            }
        }
    }

    STUBBED_PRINTF("MOON GRID PUZZLE: Error cannot find given id\n");
}

// offset: 0x100C | func: 20 | export: 20
s32 WCLevelControl_moon_puzzle_func_100C(SRT *arg0, s16 puzzleGridX, s16 puzzleGridZ, f32 *x, f32 *z, s32 arg5, s32 arg6) {
    s32 temp;
    s32 var_a1;
    s32 var_a1_2;
    f32 sp38;

    if (arg5 != 0) {
        if (arg5 == -1) {
            WCLevelControl_moon_puzzle_set_coords_from_grid_position(arg0, 7, puzzleGridZ, x, z);
            puzzleGridX++;
            var_a1 = 8;
        } else {
            WCLevelControl_moon_puzzle_set_coords_from_grid_position(arg0, 0, puzzleGridZ, x, z);
            puzzleGridX--;
            var_a1 = -1;
        }

        temp = puzzleGridX;
        while (temp != var_a1) {
            if (sMoonPuzzleCells[temp][puzzleGridZ] != 0) {
                if (sMoonPuzzleCells[temp][puzzleGridZ] < 5) {
                    temp += arg5;
                    WCLevelControl_moon_puzzle_set_coords_from_grid_position(arg0, temp, puzzleGridZ, x, &sp38);
                    return 1;
                } else {
                    WCLevelControl_moon_puzzle_set_coords_from_grid_position(arg0, temp, puzzleGridZ, x, &sp38);
                    return 2;
                }
            }
            temp -= arg5;
        }
    } else {
        if (arg6 == -1) {
            WCLevelControl_moon_puzzle_set_coords_from_grid_position(arg0, puzzleGridX, 7, x, z);
            puzzleGridZ++;
            var_a1_2 = 8;
        } else {
            WCLevelControl_moon_puzzle_set_coords_from_grid_position(arg0, puzzleGridX, 0, x, z);
            puzzleGridZ--;
            var_a1_2 = -1;
        }

        temp = puzzleGridZ;
        while (temp != var_a1_2) {
            if (sMoonPuzzleCells[puzzleGridX][temp] != 0) {
                if (sMoonPuzzleCells[puzzleGridX][temp] < 5) {
                    temp += arg6;
                    WCLevelControl_moon_puzzle_set_coords_from_grid_position(arg0, puzzleGridX, temp, &sp38, z);
                    return 1;
                } else {
                    WCLevelControl_moon_puzzle_set_coords_from_grid_position(arg0, puzzleGridX, temp, &sp38, z);
                    return 2;
                }
            }
            temp -= arg6;
        }
    }

    return 4;
}

// offset: 0x12EC | func: 21
static int WCLevelControl_anim_callback(Object *self, Object *overrideObj, AnimObj_Data *animData, s8 a3) {
    WCLevelControl_Data* objdata = self->data;
    s32 i;
    
    objdata->flags |= FLAG_1;
    objdata->flags &= ~FLAG_2;

    if (objdata->previousState == STATE_1) {
        objdata->timer -= gUpdateRateF;
        if (objdata->timer <= 0.0f) {
            main_set_bits(BIT_7F7, 1);
        }
    } else if (objdata->previousState == STATE_2) {
        objdata->timer -= gUpdateRateF;
        if (objdata->timer <= 0.0f) {
            main_set_bits(BIT_802, 1);
        }
    }
    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
            case 1:
                objdata->state = STATE_6;
                break;
            default:
                break;
        }
    }

    return 0;
}

// offset: 0x1444 | func: 22
/**
  * Also used by any acts after Act 2.
  */
static void WCLevelControl_handle_act1(Object *self, WCLevelControl_Data *objdata) {
    if (objdata->flags & FLAG_2) {
        return;
    }

    objdata->previousState = objdata->state;

    switch (objdata->state) {
    case STATE_1:
        if (objdata->flags & FLAG_1) {
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x106, 0, 0, 0);
            func_8000F64C(0x11, 60);
            func_8000F6CC();
        } else if (main_get_bits(BIT_7F9)) {
            objdata->flags |= FLAG_4;
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x104, 0, 0, 0);
            func_8000FAC8();
            if (main_get_bits(BIT_7FA) != 0) {
                gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                objdata->state = STATE_3;
            } else {
                gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
                objdata->state = STATE_0;
            }
        } else if (func_8000FB1C()) {
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x104, 0, 0, 0);
            main_set_bits(BIT_7EF, 0);
            main_set_bits(BIT_7ED, 0);
            objdata->state = STATE_0;
        }
        break;
    case STATE_2:
        if (objdata->flags & FLAG_1) {
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x106, 0, 0, 0);
            func_8000F64C(0x11, 60);
            func_8000F6CC();
        } else if (main_get_bits(BIT_7FA)) {
            objdata->flags |= FLAG_8;
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x104, 0, 0, 0);
            func_8000FAC8();
            if (main_get_bits(BIT_7F9) != 0) {
                gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                objdata->state = STATE_3;
            } else {
                gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
                objdata->state = STATE_0;
            }
        } else if (func_8000FB1C()) {
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x104, 0, 0, 0);
            main_set_bits(BIT_7F0, 0);
            main_set_bits(BIT_7EE, 0);
            objdata->state = STATE_0;
        }
        break;
    case STATE_3:
        break;
    default:
        if (!(objdata->flags & FLAG_4) && (main_get_bits(BIT_7ED))) {
            main_set_bits(BIT_7EF, 1);
            objdata->state = STATE_1;
            objdata->flags |= FLAG_2;
            objdata->timer = 70.0f;
        } else if (!(objdata->flags & FLAG_8) && (main_get_bits(BIT_7EE))) {
            main_set_bits(BIT_7F0, 1);
            objdata->state = STATE_2;
            objdata->flags |= FLAG_2;
            objdata->timer = 70.0f;
        }
        break;
    }

    objdata->flags &= ~FLAG_1;
}

// offset: 0x1928 | func: 23
static void WCLevelControl_handle_act2(Object *self, WCLevelControl_Data *objdata) {
    u8 isNightTime;
    u8 temp;
    f32 time;

    isNightTime = gDLL_7_Newday->vtbl->func8(&time);

    switch(objdata->state) {
        case STATE_6:
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x106, 0, 0, 0);
            func_8000F64C(0x11, 60);
            func_8000F6CC();
            objdata->state = STATE_4;
            break;
        case STATE_4:
            if (func_8000FB1C()) {
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x104, 0, 0, 0);
                if (main_get_bits(BIT_2A5)) {
                    objdata->flags |= FLAG_40;
                } else {
                    main_set_bits(BIT_274, 0);
                    main_set_bits(BIT_2B1, 0);
                    main_set_bits(BIT_226, 1);
                    main_set_bits(BIT_2A6, 1);
                    main_set_bits(BIT_206, 1);
                    main_set_bits(BIT_25F, 1);
                }
                objdata->state = STATE_0;
            }
            break;
        default:
            if (!(objdata->flags & FLAG_40)) {
                if (main_get_bits(BIT_2B1) && main_get_bits(BIT_204)) {
                    main_set_bits(BIT_226, 0);
                    main_set_bits(BIT_2A6, 0);
                    main_set_bits(BIT_206, 0);
                    main_set_bits(BIT_25F, 0);
                    main_set_bits(BIT_274, 1);
                    objdata->state = STATE_6;
                }
            }
            break;
    }

    if (!(objdata->flags & FLAG_10)) {
        temp = main_get_bits(BIT_810); // get sun block puzzle pieces in correct place
        if (temp == 4) {
            main_set_bits(BIT_812, 1);
            objdata->flags |= FLAG_10;
        } else if (isNightTime || main_get_bits(BIT_808)) {
            WCLevelControl_sun_puzzle_init_hard();
        }
    }
    if (!(objdata->flags & FLAG_20)) {
        temp = main_get_bits(BIT_811); // get moon block puzzle pieces in correct place
        if (temp == 4) {
            main_set_bits(BIT_813, 1);
            objdata->flags |= FLAG_20;
        } else if (!isNightTime || main_get_bits(BIT_809)) {
            WCLevelControl_moon_puzzle_init_hard();
        }
    }
    objdata->flags &= ~FLAG_1;
}

// offset: 0x1CF4 | func: 24
static void WCLevelControl_sun_puzzle_init_hard(void) {
    main_set_bits(BIT_810, 0);
    bcopy(dSunPuzzleHard, sSunPuzzleCells, sizeof(sSunPuzzleCells));
}

// offset: 0x1D54 | func: 25
static void WCLevelControl_moon_puzzle_init_hard(void) {
    main_set_bits(BIT_811, 0);
    bcopy(dMoonPuzzleHard, sMoonPuzzleCells, sizeof(sMoonPuzzleCells));
}
