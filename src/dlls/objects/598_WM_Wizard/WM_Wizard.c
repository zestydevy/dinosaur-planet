#include "PR/ultratypes.h"
#include "common.h"
#include "dll.h"
#include "sys/gfx/model.h"
#include "sys/objanim.h"
#include "game/objects/interaction_arrow.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/common/foodbag.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/607_WL_LevelControl.h"

#include "dlls/objects/598_WM_Wizard.h"

static int WMWizard_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3);
static int WMWizard_anim_visit_1_first_meeting(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3);
static int WMWizard_anim_visit_2_spirit_df(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3);
static int WMWizard_anim_visit_3_spirit_mmp(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3);
static int WMWizard_anim_visit_3_spirit_cc(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3);
static void WMWizard_handle_visit_1_first_meeting(Object* self);
static void WMWizard_handle_visit_2_spirit_df(Object* self);
static void WMWizard_handle_visit_3_spirit_mmp(Object* self);
static void WMWizard_handle_visit_4_spirit_cc(Object* self);

// offset: 0x0 | ctor
void WMWizard_ctor(void *dll) { }

// offset: 0xC | dtor
void WMWizard_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WMWizard_setup(Object* self, WMWizard_Setup* objSetup, s32 arg2) {
    WMWizard_Data* objData;

    self->unkDC = 0;
    objData = self->data;
    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = WMWizard_anim_callback;
    
    objData->objectID = objSetup->base.objId;
    objData->unk1C = 0;
    objData->unk18 = 0;
    
    objData->home.x = objSetup->base.x;
    objData->home.y = objSetup->base.y;
    objData->home.z = objSetup->base.z;

    objData->hasMetKrystal = main_get_bits(BIT_WM_Played_Randorn_First_Meeting);
    objData->timesFed = 0;
    objData->walkIndexFlags = 1; //In setup2, always walk towards pillar first
    objData->prevWalkIndex = WALK_STOPPING;
    objData->walkWaitTimer = 300;
    objData->animSpeed = 0.0f;
    objData->unk14 = 1.0f;
}

// offset: 0xE4 | func: 1 | export: 1
void WMWizard_control(Object* self) {
    WMWizard_Data* objdata = self->data;
    
    //Objhits
    if (func_80026DF4(
        self, 
        dObjHitsData, 
        11,
        (objdata->walkIndexFlags & WMWizard_FLAG_80) ? TRUE : FALSE,
        &objdata->objHitsValue
    )) {
        objdata->walkIndexFlags |= WMWizard_FLAG_80;
        return;
    }
    objdata->walkIndexFlags &= ~WMWizard_FLAG_80;
    
    //Handle setup-specific behaviour
    switch (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID)) {
    case 0:
    case WM_Setup3_Spirit2_Sabre_DB:
    case WM_Setup5_Spirit4_Sabre_WC:
        break;
    case WM_Setup1_Krystal_Meeting_Randorn:
        WMWizard_handle_visit_1_first_meeting(self);
        break;
    case WM_Setup2_Spirit1_Krystal_DF:
        WMWizard_handle_visit_2_spirit_df(self);
        break;
    case WM_Setup4_Spirit3_Krystal_MMP:
        WMWizard_handle_visit_3_spirit_mmp(self);
        break;
    case WM_Setup6_Spirit5_6_Krystal_CC_Sabre_WG:
        WMWizard_handle_visit_4_spirit_cc(self);
        break;
    }
}

// offset: 0x238 | func: 2 | export: 2
void WMWizard_update(Object *self) { }

// offset: 0x244 | func: 3 | export: 3
void WMWizard_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        if (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID) == WM_Setup4_Spirit3_Krystal_MMP) {
            if (main_get_bits(BIT_WM_Setup4_Show_Randorn)) {
                draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
            }
        } else {
            draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0x330 | func: 4 | export: 4
void WMWizard_free(Object *self, s32 a1) { }

// offset: 0x340 | func: 5 | export: 5
u32 WMWizard_get_model_flags(Object* self) {
    return MODFLAGS_1;
}

// offset: 0x350 | func: 6 | export: 6
u32 WMWizard_get_data_size(Object *self, u32 a1) {
    return sizeof(WMWizard_Data);
}

// offset: 0x364 | func: 7
int WMWizard_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3) {
    //Setup-specific anim callback behaviours
    switch (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID)) {
    case 0:
    case WM_Setup3_Spirit2_Sabre_DB:
    case WM_Setup5_Spirit4_Sabre_WC:
        break;
    case WM_Setup1_Krystal_Meeting_Randorn:
        WMWizard_anim_visit_1_first_meeting(self, overrideObj, animData, arg3);
        break;
    case WM_Setup2_Spirit1_Krystal_DF:
        WMWizard_anim_visit_2_spirit_df(self, overrideObj, animData, arg3);
        break;
    case WM_Setup4_Spirit3_Krystal_MMP:
        WMWizard_anim_visit_3_spirit_mmp(self, overrideObj, animData, arg3);
        break;
    case WM_Setup6_Spirit5_6_Krystal_CC_Sabre_WG:
        WMWizard_anim_visit_3_spirit_cc(self, overrideObj, animData, arg3);
        break;
    } 
    return 0;
}

// offset: 0x470 | func: 8
int WMWizard_anim_visit_1_first_meeting(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3) {
    WMWizard_Data* objData;
    Object* player;
    s32 i;
    int new_var;

    player = get_player();
    objData = self->data;
    self->unkAF |= ARROW_FLAG_8_No_Targetting;

    for (i = 0; i < animData->unk98; i++) {

        if (objData->activeSeqIndex == 1) {
            if (animData->unk8E[i] == 4) {
                ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 5);
            }
            continue;
        }

        if (objData->activeSeqIndex == 2) {
            continue;
        }

        if (animData->unk8E[i] == 1) {
            main_set_bits(BIT_WM_Played_Randorn_First_Meeting, 1);
            objData->hasMetKrystal = TRUE;
        } else if (animData->unk8E[i] == 2) {
            // very fake
            if (player) {}
            if (player) {}
            new_var = 1;
            if (i == 1) {}
            if (i == 2) {}
            if (i == 3) {}
            ((DLL_210_Player*)player->dll)->vtbl->func41(player, 0, new_var);
            ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 5);
        }
    }
    
    return 0;
}

// offset: 0x610 | func: 9
int WMWizard_anim_visit_2_spirit_df(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3) {
    return 0;
}

// offset: 0x62C | func: 10
int WMWizard_anim_visit_3_spirit_mmp(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3) {
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    return 0;
}

// offset: 0x650 | func: 11
int WMWizard_anim_visit_3_spirit_cc(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3) {
    WMWizard_Data* objData;
    s32 i;

    objData = self->data;
    self->unkAF |= ARROW_FLAG_8_No_Targetting;

    for (i = 0; i < animData->unk98; i++) {
        switch (animData->unk8E[i]) {
        case 0:
            break;
        case 1:
            //Set a gamebit if Randorn has been fed at least 2 times before Krystal's last visit
            if (objData->timesFed >= 2) {
                main_set_bits(BIT_Randorn_Saved, 1);
            }
            break;
        }
    }

    return 0;
}

// offset: 0x72C | func: 12
void WMWizard_handle_visit_1_first_meeting(Object* self) {
    WMWizard_Data* objData;
    Object* player;

    objData = self->data;

    //Handle animation
    if (self->curModAnimId != Randorn_MODANIM_2_Sitting_Hands_on_Knees_LOOP) {
        func_80023D30(self, Randorn_MODANIM_2_Sitting_Hands_on_Knees_LOOP, 0.0f, 0);
    }
    func_80024108(self, 0.005f, gUpdateRate, NULL);

    //Before meeting Randorn
    if (objData->hasMetKrystal == FALSE) {
        //Play Krystal and Randorn's first conversation sequence when talked to
        if (self->unkAF & ARROW_FLAG_1_Interacted) {
            main_set_bits(BIT_WM_Played_Randorn_First_Meeting, 1);
            objData->hasMetKrystal = TRUE;
            joy_set_button_mask(0, A_BUTTON);
        }

        //Call out to Krystal randomly
        objData->talkTimer -= (s16)gUpdateRateF;
        if ((objData->talkTimer <= 0) && (objData->hasMetKrystal == 0)) {
            objData->talkTimer = rand_next(0, 600) + 800;
            gDLL_6_AMSFX->vtbl->play(
                NULL,
                SOUND_BA8_Randorn_Calling_Krystal_1 + rand_next(0, 2), 
                0x57 + rand_next(0, 0x28),
                NULL,
                NULL,
                0,
                NULL
            );
        }

        return;
    }

    //After meeting Randorn
    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        player = get_player();

        //Check if Krystal has magic
        if (((DLL_210_Player*)player->dll)->vtbl->get_magic(player) > 0) {
            //Remind about visiting Discovery Falls
            objData->activeSeqIndex = WMWizard_OBJSEQ_2_Discovery_Falls_Reminder;
            gDLL_3_Animation->vtbl->func17(WMWizard_OBJSEQ_2_Discovery_Falls_Reminder, self, -1);
            joy_set_button_mask(0, A_BUTTON);

        //Otherwise, restore Krystal's magic if she hasn't deactivated the lasers yet
        } else if (
            main_get_bits(BIT_WM_Force_Field_1_Disabled) == 0 || 
            main_get_bits(BIT_WM_Force_Field_2_Disabled) == 0 || 
            main_get_bits(BIT_WM_Force_Field_3_Disabled) == 0
        ) {
            objData->activeSeqIndex = WMWizard_OBJSEQ_1_Offering_Magic_Refill;
            gDLL_3_Animation->vtbl->func17(WMWizard_OBJSEQ_1_Offering_Magic_Refill, self, -1);
            joy_set_button_mask(0, A_BUTTON);
        }
    }
}

// offset: 0xA20 | func: 13
void WMWizard_handle_visit_2_spirit_df(Object* self) {
    WMWizard_Data* objData = self->data;
    Object* player = get_player();
    Object* foodbag;
    s32 foodGamebit;
    s32 dAcceptedFoodsVisit2[3] = {
        BIT_Green_Apple_Count,
        BIT_Red_Apple_Count,
        BIT_Brown_Apple_Count
    };
    f32 dx;
    f32 dz;
    f32 distance;
    s16 dYaw;
    u8 temp;
    UnkFunc_80024108Struct sp30;
    RandomWalkData* walkData;

    self->srt.transl.y = objData->home.y;

    //After Quan Ata Lachu cutscene (sitting at edge of podium)
    if (main_get_bits(BIT_Play_Seq_0170_WM_Return_to_Randorn_Quan_Ata_Lachu_Speaks)) {
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;

        if ((self->unkAF & ARROW_FLAG_1_Interacted) && (gDLL_1_cmdmenu->vtbl->get_subpage_gamebit() == BIT_Foodbag_Give)) {
            foodGamebit = gDLL_1_cmdmenu->vtbl->was_used_item_in_gamebit_array(
                            dAcceptedFoodsVisit2, ARRAYCOUNT(dAcceptedFoodsVisit2)
                        );
            
            if (foodGamebit >= 0) {
                main_set_bits(BIT_4D1, 1);
                objData->timesFed++;
                main_set_bits(BIT_310, 1);

                foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 0xF);
                ((DLL_IFoodbag*)foodbag->dll)->vtbl->delete_food_by_gamebit(foodbag, foodGamebit);
                joy_set_button_mask(0, A_BUTTON);
            }
        }
        return;
    }
    
    //Before Quan Ata Lachu cutscene (random walk behaviour)
    self->unkAF |= ARROW_FLAG_8_No_Targetting;

    //Pick next walk destination
    if (objData->walkWaitTimer <= 0) {
        switch (rand_next(1, 4)) {
        case 1:
            objData->prevWalkIndex = objData->walkIndexFlags;
            objData->walkIndexFlags = 1;
            objData->walkWaitTimer = 400;
            break;
        case 2:
            objData->prevWalkIndex = objData->walkIndexFlags;
            objData->walkIndexFlags = 2;
            objData->walkWaitTimer = 400;
            break;
        case 3:
            objData->prevWalkIndex = objData->walkIndexFlags;
            objData->walkIndexFlags = 3;
            objData->walkWaitTimer = 400;
            break;
        case 4:
            objData->prevWalkIndex = objData->walkIndexFlags;
            objData->walkIndexFlags = 4;
            objData->walkWaitTimer = 400;
            break;
        case 5: //unreachable?
            objData->prevWalkIndex = objData->walkIndexFlags;
            objData->walkIndexFlags = 5;
            objData->walkWaitTimer = 400;
            break;
        }
        return;
    }

    //Handle random walk movement
    if (objData->walkIndexFlags == WALK_STOPPING) {
        //Stopping: Rotate until facing away from centre of mural
        walkData = (RandomWalkData*)&dRandomWalkData[objData->prevWalkIndex * 5];
        dYaw = ((u16)arctan2_f(walkData->x, walkData->z) & 0xFFFF) - self->srt.yaw;
        diPrintf("diff %d\n", dYaw);

        if ((dYaw < -1000) || (dYaw > 1000)) {
            if (dYaw > 0) {
                self->srt.yaw += gUpdateRate * 100;
            } else {
                self->srt.yaw -= gUpdateRate * 100;
            }
        } else {
            //Done rotating outwards: start the walk point's intro idle animation and go to stopped state
            func_80023D30(self, dRandomWalkData[(objData->prevWalkIndex * 5) + 2], 0.0f, 0);
            objData->animSpeed = dRandomWalkData[(objData->prevWalkIndex * 5) + 4];
            objData->walkIndexFlags = WALK_STOPPED;
        }
    } else if (objData->walkIndexFlags == WALK_STOPPED) {
        //Stopped: wait for the intro idle animation to finish
        if (func_80024108(self, objData->animSpeed, gUpdateRateF, &sp30)) {
            //Start the walk point's secondary idle loop animation
            walkData = (RandomWalkData*)&dRandomWalkData[objData->prevWalkIndex * 5];
            if (walkData->modAnimIDIdleIntro == self->curModAnimId) {
                func_80023D30(self, walkData->modAnimIDIdleLoop, 0.0f, 0);
                objData->animSpeed = dRandomWalkData[(objData->prevWalkIndex * 5) + 4];
            }
        }
        
        //Wait here until timer runs out
        objData->walkWaitTimer -= gUpdateRate;
        if (objData->walkWaitTimer <= 0) {
            objData->walkWaitTimer = 0;
        }
    } else {
        //Go to next walk point
        walkData = (RandomWalkData*)&dRandomWalkData[objData->walkIndexFlags * 5];
        dx = walkData->x - (self->srt.transl.x - objData->home.x);
        dz = walkData->z - (self->srt.transl.z - objData->home.z);
        distance = sqrtf(SQ(dx) + SQ(dz));
        dYaw = ((u16)arctan2_f(dx, dz) & 0xFFFF) - (self->srt.yaw);
        
        if ((dYaw >= -1000) && (dYaw <= 1000)) {
            //Walk towards destination
            if (self->curModAnimId != Randorn_MODANIM_59_Walk_LOOP) {
                func_80023D30(self, Randorn_MODANIM_59_Walk_LOOP, 0.0f, 0);
                objData->animSpeed = 0.04f;
            }

            self->velocity.x = (dx / distance) * 0.25f;
            self->velocity.z = (dz / distance) * 0.25f;
            func_8002493C(self, 0.25f, &objData->animSpeed);
        } else {
            //Limping on the spot, turn to face the next walk point
            if (self->curModAnimId != Randorn_MODANIM_12_Limp_Walk_LOOP) {
                func_80023D30(self, Randorn_MODANIM_12_Limp_Walk_LOOP, 0.0f, 0);
                objData->animSpeed = 0.01f;
            }
            
            if (dYaw > 0) {
                self->srt.yaw += gUpdateRate * 300;
            } else {
                self->srt.yaw -= gUpdateRate * 300;
            }
        }
        
        //Arrive at destination
        if (distance < 4.0f) {
            objData->prevWalkIndex = objData->walkIndexFlags;
            objData->walkIndexFlags = WALK_STOPPING;
            self->velocity.x = 0.0f;
            self->velocity.z = 0.0f;
        }
        
        //Move and advance animation playback
        self->srt.transl.x += (self->velocity.x * gUpdateRateF);
        self->srt.transl.z += (self->velocity.z * gUpdateRateF);
        func_80024108(self, objData->animSpeed, gUpdateRateF, &sp30);
    }
}

// offset: 0x1124 | func: 14
void WMWizard_handle_visit_3_spirit_mmp(Object* self) {
    self->unkAF |= ARROW_FLAG_8_No_Targetting;

    //Handle animation
    if (self->curModAnimId != Randorn_MODANIM_2_Sitting_Hands_on_Knees_LOOP) {
        func_80023D30(self, Randorn_MODANIM_2_Sitting_Hands_on_Knees_LOOP, 0.0f, 0);
    }
    func_80024108(self, 0.005f, gUpdateRate, NULL);
}

// offset: 0x11C8 | func: 15
void WMWizard_handle_visit_4_spirit_cc(Object* self) {
    WMWizard_Data* objdata = self->data;
    Object* player = get_player();
    s32 foodGamebit;
    Object* foodbag; 
    s32 dAcceptedFoodsVisit4[3] = {
        BIT_Green_Apple_Count,
        BIT_Red_Apple_Count,
        BIT_Brown_Apple_Count
    };

    if (self->unkAF & ARROW_FLAG_8_No_Targetting) {
        self->unkAF ^= ARROW_FLAG_8_No_Targetting;
    }

    //Handle animations 
    if (main_get_bits(BIT_WM_Setup6_Randorn_Sitting_Up) == FALSE) {
        //Randorn initially slumped to the side
        if (self->curModAnimId != Randorn_MODANIM_7_Sitting_Attempt_to_Stand_Collapse) {
            func_80023D30(self, Randorn_MODANIM_7_Sitting_Attempt_to_Stand_Collapse, 0.0f, 0);
        }
        
        func_80024108(self, 0.005f, gUpdateRate, NULL);
    } else {
        //He sits up after the player talks with him
        if (self->curModAnimId != Randorn_MODANIM_2_Sitting_Hands_on_Knees_LOOP) {
            func_80023D30(self, Randorn_MODANIM_2_Sitting_Hands_on_Knees_LOOP, 0.0f, 0); 
        }
    
        func_80024108(self, 0.005f, gUpdateRate, NULL);
    }

    //Check if player talks to Randorn while he's collapsed
    if ((self->unkAF & ARROW_FLAG_1_Interacted) && (main_get_bits(BIT_WM_Setup6_Randorn_Sitting_Up) == FALSE)) {
        main_set_bits(BIT_WM_Setup6_Randorn_Sitting_Up, 1);
        objdata->timesFed = 0;
        joy_set_button_mask(0, A_BUTTON);
    
    //Check if player offers food to Randorn while he's sitting up
    } else if (
        (self->unkAF & ARROW_FLAG_1_Interacted) && 
        (gDLL_1_cmdmenu->vtbl->get_subpage_gamebit() == BIT_Foodbag_Give)
    ) {
        foodGamebit = gDLL_1_cmdmenu->vtbl->was_used_item_in_gamebit_array(
                            dAcceptedFoodsVisit4, 
                            ARRAYCOUNT(dAcceptedFoodsVisit4)
                        );

        // @fake
        if (objdata) {}
        if (foodGamebit >= 0) {
            main_set_bits(BIT_310, 1);
            objdata->timesFed++;
            foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 15);
            ((DLL_IFoodbag*)foodbag->dll)->vtbl->delete_food_by_gamebit(foodbag, foodGamebit);
            joy_set_button_mask(0, A_BUTTON);
        }
    }
}
