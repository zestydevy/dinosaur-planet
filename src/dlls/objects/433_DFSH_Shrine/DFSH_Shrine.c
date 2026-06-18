#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "sys/dll.h"
#include "sys/gfx/animseq.h"
#include "sys/gfx/modgfx.h"
#include "sys/map_enums.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/segment_1460.h"
#include "sys/print.h"
#include "dll.h"

typedef struct {
/*00*/ s16 testStartRange;
/*02*/ s16 stateCooldown;
/*04*/ s16 whisperVolume;
/*06*/ s16 whisperVolumeRate;
/*08*/ s16 bgmVolume;
/*0A*/ s16 bgmVolumeRate;
/*0C*/ s16 unkC;
/*0E*/ s16 testTimer;
/*10*/ u8 sharpClawDefeated;
/*11*/ s8 state;
/*12*/ u8 unk12;
/*13*/ s8 _unk13;
/*14*/ u8 startedBgMusic;
} DFShrine_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 _unk18;
/*1A*/ s16 testStartRange;
} DFShrine_Setup;

/*0x0*/ static Texture* data_0 = NULL;

static int DFShrine_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3);
static void DFShrine_process_obj_messages(Object* self);

// offset: 0x0 | ctor
void DFShrine_ctor(void *dll) { }

// offset: 0xC | dtor
void DFShrine_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFShrine_setup(Object* self, DFShrine_Setup* setup, s32 arg2) {
    DFShrine_Data* objdata = self->data;
    DLL_IModgfx* modgfx;
    
    self->srt.yaw = 0;
    objdata->testStartRange = 10;
    if (setup->testStartRange > 0) {
        objdata->testStartRange = (s16) (setup->testStartRange >> 8);
    }
    objdata->state = 0;
    objdata->unk12 = 0;
    objdata->stateCooldown = 0;
    objdata->sharpClawDefeated = 0;
    self->animCallback = DFShrine_anim_callback;
    obj_init_mesg_queue(self, 4);
    main_set_bits(BIT_DB_Entered_Shrine_3, 1);
    main_set_bits(BIT_125, 0);
    main_set_bits(BIT_DB_Entered_Shrine_1, 1);
    main_set_bits(BIT_DB_Entered_Shrine_2, 1);
    main_set_bits(BIT_1E7, 0);
    objdata->whisperVolume = 0xC;
    objdata->bgmVolume = 0x1E;
    objdata->stateCooldown = 0xC8;
    objdata->whisperVolumeRate = 0;
    objdata->bgmVolumeRate = 0;
    objdata->startedBgMusic = 0;
    objdata->testTimer = 0;
    modgfx = dll_load_deferred(DLL_ID_122, 1);
    objdata->unkC = modgfx->vtbl->func0(self, 0, 0, 0x402, -1, 0);
    dll_unload(modgfx);
}

// offset: 0x1A8 | func: 1 | export: 1
void DFShrine_control(Object* self) {
    static u8 sFirstTick = TRUE;
    DFShrine_Data* objdata = self->data;
    Object* player = get_player();
    DLL_IModgfx* modgfx;
    f32 playerDoorZDist;
    f32 playerDoorDist;
    Object* door;
    s16 volume;
    s16 i;
    Object** baddieList;
    s32 baddieCount;

    baddieCount = 0;
    playerDoorDist = 1000.0f;
    if (sFirstTick) {
        self->globalPosition.x = self->srt.transl.x;
        self->globalPosition.y = self->srt.transl.y;
        self->globalPosition.z = self->srt.transl.z;
        main_set_bits(BIT_DB_Entered_Shrine_2, 1);
        // Start whispers
        gDLL_5_AMSEQ->vtbl->play_ex(2, 0x2B, 0x50, 1, 0);
    }
    if (sFirstTick) {
        ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 20);
        main_set_bits(BIT_Spell_Forcefield, 1);
        sFirstTick = FALSE;
    }
    // Process obj messages
    DFShrine_process_obj_messages(self);
    // Modulate background whispers
    if (objdata->whisperVolumeRate != 0) {
        objdata->whisperVolume += objdata->whisperVolumeRate;
        if (objdata->whisperVolume <= 12) {
            objdata->whisperVolume = 12;
            objdata->whisperVolumeRate = 0;
        }
        else if (objdata->whisperVolume >= 70) {
            objdata->whisperVolume = 70;

            objdata->whisperVolumeRate = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(2, objdata->whisperVolume);
    }
    // Modulate background music
    if (objdata->bgmVolumeRate != 0) {
        objdata->bgmVolume += objdata->bgmVolumeRate;
        if ((objdata->bgmVolume <= 1) && (objdata->bgmVolumeRate <= 0)) {
            objdata->bgmVolume = 1;
            objdata->bgmVolumeRate = 0;
        } 
        else if ((objdata->bgmVolume >= 70) && (objdata->bgmVolumeRate >= 0)) {
            objdata->bgmVolume = 70;
            objdata->bgmVolumeRate = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3, objdata->bgmVolume);
    }
    // Handle current state
    if (objdata->stateCooldown > 0) {
        objdata->stateCooldown -= gUpdateRate;
        if (objdata->stateCooldown <= 0) {
            objdata->stateCooldown = 0;
            // Start background music if not already started
            if (objdata->startedBgMusic == 0) {
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, objdata->bgmVolume, 0);
                objdata->startedBgMusic = 1;
            }
        }
    } else {
        // Modulate background music and whispers as player passes the shrine door
        door = obj_get_nearest_type_to(OBJTYPE_Door, player, &playerDoorDist);
        if ((door != NULL) && (playerDoorDist < 300.0f) && (playerDoorDist > 100.0f)) {
            playerDoorZDist = door->srt.transl.z - player->srt.transl.z;
            if (playerDoorZDist <= 0.0f) {
                if (playerDoorZDist < 0.0f) {
                    playerDoorZDist *= -1.0f;
                }
                if (objdata->bgmVolume != 0x1E) {
                    objdata->bgmVolume = 0x1E;
                }
                volume = (s16) ((f32) objdata->bgmVolume * ((playerDoorZDist - 100.0f) / 200.0f));
                if (volume <= 0) {
                    volume = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(3, volume);
                volume = (s16) ((f32) objdata->whisperVolume * ((200.0f - (playerDoorZDist - 100.0f)) / 200.0f));
                if (volume <= 0) {
                    volume = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(2, volume);
            }
        }
        switch (objdata->state) {
        case 0:
            if (vec3_distance(&self->globalPosition, &player->globalPosition) < (f32) objdata->testStartRange) {
                main_set_bits(BIT_589, 0);
                objdata->state = 1;
                main_set_bits(BIT_DB_Entered_Shrine_3, 0);
                gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                modgfx = dll_load_deferred(DLL_ID_147, 1);
                modgfx->vtbl->func0(self, 0, 0, 1, -1, 0);
                dll_unload(modgfx);
                modgfx = dll_load_deferred(DLL_ID_148, 1);
                modgfx->vtbl->func0(self, 0, 0, 1, -1, 0);
                dll_unload(modgfx);
                main_set_bits(BIT_DB_Entered_Shrine_1, 0);
                gDLL_14_Modgfx->vtbl->func7(&objdata->unkC);
            }
        default:
            break;
        case 1:
            if (objdata->unk12 == 1) {
                objdata->state = 2;
                objdata->stateCooldown = 160;
                objdata->testTimer = 6000;
            }
            break;
        case 2:
            // Test active
            objdata->testTimer -= gUpdateRate;
            diPrintf("TIMER %d\n", objdata->testTimer);
            if (objdata->sharpClawDefeated == 0) {
                if (main_get_bits(BIT_F6) == 0) {
                    main_set_bits(BIT_F6, 1);
                }
            } else if (objdata->sharpClawDefeated == 1) {
                if (main_get_bits(BIT_F7) == 0) {
                    main_set_bits(BIT_F7, 1);
                }
            } else if (objdata->sharpClawDefeated == 3) {
                if (main_get_bits(BIT_F8) == 0) {
                    main_set_bits(BIT_F8, 1);
                }
            } else if (objdata->sharpClawDefeated == 4) {
                if (main_get_bits(BIT_FA) == 0) {
                    main_set_bits(BIT_FA, 1);
                }
                if (main_get_bits(BIT_FB) == 0) {
                    main_set_bits(BIT_FB, 1);
                }
            }
            if (main_get_bits(BIT_1E7) != 0) {
                // A SharpClaw was defeated
                objdata->sharpClawDefeated++;
                objdata->stateCooldown = 100;
                if (objdata->sharpClawDefeated == 4) {
                    // Pass
                    objdata->state = 3;
                    objdata->stateCooldown = 200;
                    objdata->bgmVolumeRate = -3;
                    gDLL_5_AMSEQ->vtbl->play_ex(3, 0xE, 0x50, (u8)objdata->bgmVolume, 0);
                    objdata->bgmVolumeRate = 1;
                }
                main_set_bits(BIT_1E7, 0);
            } else if (objdata->testTimer <= 0) {
                // Fail
                objdata->state = 3;
            }
            break;
        case 3:
            if (objdata->sharpClawDefeated != 4) {
                // Test failed
                // Delete remaining SharpClaws
                baddieList = obj_get_all_of_type(OBJTYPE_Baddie, &baddieCount);
                for (i = 0; i < baddieCount; i++) {
                    obj_destroy_object(baddieList[i]);
                }
                objdata->stateCooldown = 0;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x35, 0x50, (u8) objdata->bgmVolume, 0);
                objdata->bgmVolumeRate = 1;
                gDLL_3_Animation->vtbl->start_obj_sequence(2, self, -1);
                objdata->stateCooldown = 10;
            } else if (((DLL_210_Player*)player->dll)->vtbl->get_spirit_bits(player, PLAYER_SPIRIT_1) != 0) {
                // Test passed, but the player is already carrying the spirit
                objdata->bgmVolume = 1;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, (u8) objdata->bgmVolume, 0);
                objdata->bgmVolumeRate = 1;
                main_set_bits(BIT_DB_Entered_Shrine_3, 1);
                objdata->state = 5;
            } else {
                // Test passed, play spirit cutscene
                objdata->stateCooldown = 0;
                main_set_bits(BIT_DB_Entered_Shrine_1, 0);
                gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
                objdata->stateCooldown = 10;
            }
            break;
        case 4:
            // Spirit granted
            main_set_bits(BIT_125, 0);
            objdata->state = 5;
            ((DLL_210_Player*)player->dll)->vtbl->set_spirit_bits(player, PLAYER_SPIRIT_1, TRUE);
            gDLL_29_Gplay->vtbl->set_act(MAP_WARLOCK_MOUNTAIN, 2);
            break;
        case 5:
            // Warp out
            if (main_get_bits(BIT_Shrine_Do_Exit_Warp) == 0) {
                main_set_bits(BIT_Shrine_Do_Exit_Warp, 1);
            }
            main_set_bits(BIT_DB_Entered_Shrine_2, 0);
            main_set_bits(BIT_DB_Entered_Shrine_3, 1);
            objdata->state = 7;
            break;
        case 8:
            // Reset
            objdata->state = 0;
            objdata->unk12 = 0;
            objdata->stateCooldown = 400;
            objdata->sharpClawDefeated = 0;
            main_set_bits(BIT_DB_Entered_Shrine_3, 1);
            main_set_bits(BIT_125, 0);
            main_set_bits(BIT_DB_Entered_Shrine_1, 1);
            main_set_bits(BIT_DB_Entered_Shrine_2, 1);
            main_set_bits(BIT_1E7, 0);
            main_set_bits(BIT_F6, 0);
            main_set_bits(BIT_F7, 0);
            main_set_bits(BIT_F8, 0);
            main_set_bits(BIT_F9, 0);
            main_set_bits(BIT_FA, 0);
            main_set_bits(BIT_FB, 0);
            main_set_bits(BIT_589, 1);
            objdata->testTimer = 0;
            modgfx = dll_load_deferred(DLL_ID_122, 1);
            objdata->unkC = modgfx->vtbl->func0(self, 0, 0, 0x402, -1, 0);
            dll_unload(modgfx);
            break;
        }
    }
}

// offset: 0xE04 | func: 2 | export: 2
void DFShrine_update(Object *self) { }

// offset: 0xE10 | func: 3 | export: 3
void DFShrine_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xE64 | func: 4 | export: 4
void DFShrine_free(Object* self, s32 a1) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_5_AMSEQ->vtbl->set_volume(3, 0);
    gDLL_5_AMSEQ->vtbl->set_volume(2, 0);
}

// offset: 0xEF4 | func: 5 | export: 5
u32 DFShrine_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xF04 | func: 6 | export: 6
u32 DFShrine_get_data_size(Object *self, u32 a1) {
    return sizeof(DFShrine_Data);
}

// offset: 0xF18 | func: 7
static int DFShrine_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    DFShrine_Data* objdata = self->data;
    s32 i;
    
    animObjData->unk62 = 0;
    if (objdata->bgmVolumeRate != 0) {
        objdata->bgmVolume += objdata->bgmVolumeRate;
        if ((objdata->bgmVolume < 2) && (objdata->bgmVolumeRate <= 0)) {
            objdata->bgmVolume = 1;
            objdata->bgmVolumeRate = 0;
        } else if ((objdata->bgmVolume >= 70) && (objdata->bgmVolumeRate >= 0)) {
            objdata->bgmVolume = 70;
            objdata->bgmVolumeRate = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3, objdata->bgmVolume);
    }

    for (i = 0; i < animObjData->messageCount; i++) {
        s32 message = animObjData->messages[i];
        if (message == 0) { // huh?
        } else {
            switch (message) {
                case 1:
                    func_80000860(self, self, 0xC3, 0);
                    break;
                case 2:
                    if (*D_80092A7C == -1) {
                        func_80000860(self, self, 0x14, 0);
                    } else {
                        func_80000860(self, self, *D_80092A7C, 0);
                    }
                    break;
                case 3:
                    objdata->unk12 = 1;
                    break;
                case 4:
                    objdata->state = 4;
                    objdata->unk12 = 2;
                    main_set_bits(BIT_DB_Entered_Shrine_3, 1);
                    main_set_bits(BIT_125, 0);
                    main_set_bits(BIT_DB_Entered_Shrine_1, 1);
                    objdata->bgmVolumeRate = -3;
                    break;
                case 5:
                    objdata->state = 8;
                    objdata->unk12 = 3;
                    objdata->bgmVolumeRate = -3;
                    main_set_bits(BIT_DB_Entered_Shrine_3, 1);
                    break;
                case 6:
                    main_set_bits(BIT_125, 1);
                    break;
                case 7:
                    main_set_bits(BIT_125, 0);
                    objdata->bgmVolumeRate = -3;
                    break;
                case 9:
                    main_set_bits(BIT_DB_Triggered_In_Shrine_Spirit_Cutscene, 1);
                    if (data_0 == NULL) {
                        data_0 = block_texanim_get_tex(1);
                    }
                    break;
                case 8:
                    main_set_bits(BIT_DB_Entered_Shrine_2, 1);
                    break;
                case 10:
                    objdata->bgmVolume = 100;
                    gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2D, 0x50, (u8) objdata->bgmVolume, 0);
                    break;
                case 11:
                    objdata->bgmVolume = 100;
                    gDLL_5_AMSEQ->vtbl->play_ex(3, 0x36, 0x50, (u8) objdata->bgmVolume, 0);
                    break;
            }
        }
        animObjData->messages[i] = 0;
    }

    return 0;
}

// offset: 0x12AC | func: 8
static void DFShrine_process_obj_messages(Object* self) {
    Object* sender;
    u32 mesgID;
    void* mesgArg;
    DFShrine_Data* objdata = self->data;
    
    mesgArg = NULL;
    while (obj_recv_mesg(self, &mesgID, &sender, &mesgArg) != 0) {
        switch (mesgID) {
        case 0x30005:
            objdata->whisperVolumeRate = -3;
            break;
        case 0x30006:
            objdata->whisperVolumeRate = 0x10;
            break;
        }
    }
}
