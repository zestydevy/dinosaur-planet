#include "common.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "sys/gfx/model.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "sys/newshadows.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/314_foodbag.h"
#include "types.h"

extern u16 func_80031BBC(f32 x, f32 y, f32 z);

#define COLLECT_OFF 1

typedef struct {
    ObjSetup base;
    s8 unused18;
    s8 messageArgBase;      //Base arg value used for sidekick-related collectables
    u8 unk1A;               //Stored to Collectable_Data, but seems to be unused 
    u8 yaw;
    s16 gamebitCollected;   //Set when collected: collectable vanishes when set
    s16 animMessage;
    s8 unused20;
    s8 unused21;    
    u8 pitch;
    u8 roll;
    s16 gamebitShow;        //(Optional) Only show collectable once this gamebit is set
    u8 modelIdx;
    u8 applyColourMultiplier;
    u8 multiplyR;           //Colour multiplier for model
    u8 multiplyG;           //Colour multiplier for model
    u8 multiplyB;           //Colour multiplier for model
    u8 unused2B;
    s16 gamebitCount;
} Collectable_Setup;

typedef struct {
    u32 soundHandle;            //Cleared on free, but not actaully used for any sound calls
    f32 distanceToPlayer;  
    f32 interactionRadius;
    f32 timerDestroy;           //Countdown after receiving collection message from sender
    s8 sidekickArgBase;         //Base arg value used for sidekick-related collectables
    u8 unk11;
    u8 unused12;
    u8 pause;                   //Control function ends early when this is set
    s16 gamebitCollected;       //Set when collected: collectable vanishes when set
    u8 unused16;
    u8 unused17;
    s16 gamebitShow;            //(Optional) Collectable only shows up once gamebit set
    u8 shadowOpacity;           //For magic
    u8 unused1B;
    s32 areaValue;              //Received from Area object, if the collectable is inside one
    s8 delayCollect;            //Timer, can only collect object once at 0
    u8 moving;
    u8 isHidden;
    u32 uID;
    Vec3f savedPosition;
    f32 pitchAnimate;           //Rotation for Dino Eggs' rattle animation
    s16 soundTimer;             //Timer for Dino Eggs' rattle sound/animation
    u8 useColourMultiplier;     //Toggles colour multiplier
    u8 interactFlags;           //Toggles ability to collect
    u8 multiplyR;               //Colour multiplier for model
    u8 multiplyG;               //Colour multiplier for model
    u8 multiplyB;               //Colour multiplier for model
    u8 unused3F;
    s16 rootTimer;              //Affects opacity of Alpine Root
    u16 unused42;
} Collectable_Data; //0x44

typedef struct {
    s8 unk0;
    s8 unk1;
    s16 category;           //see Collectable_Types
    s8 unk4;
    s8 unk5;
    s16 unk6;               // object ID of item in pickup sequence animation (the thing being held)
    s8 interactionRadius;   //radius for picking up
    s8 unk9;
    s8 unkA;
    s8 unkB;                //amount of magic restored?
} CollectableDef;

typedef enum {
    Collectable_Type_1 = 0x1,
    Collectable_Type_2 = 0x2,
    Collectable_Type_Food = 0x4,
    Collectable_Type_8_Sidekick = 0x8,
    Collectable_Type_10_Sidekick = 0x10,
    Collectable_Type_20 = 0x20,
    Collectable_Type_Magic = 0x40,
    Collectable_Type_80_Sidekick = 0x80
} Collectable_Types;

static int collectable_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData);
static void collectable_handle_animation_and_fx(Object* self);
static void collectable_handle_motion(Object* self);
static void collectable_collect(Object* self);
void collectable_set_speed(Object* self, f32 speedX, f32 speedY, f32 speedZ);

// offset: 0x0 | ctor
void collectable_ctor(void *dll) { }

// offset: 0xC | dtor
void collectable_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void collectable_setup(Object* self, Collectable_Setup* objSetup, s32 arg2) {
    s32 pad1;
    CollectableDef* collectableDef;
    LightAction lfxAction;
    s32 index;
    s16 id;
    Collectable_Data* objData;

    objData = self->data;
    obj_add_object_type(self, 5);
    obj_init_mesg_queue(self, 2);

    self->srt.yaw = objSetup->yaw << 8;
    self->srt.pitch = objSetup->pitch << 8;
    self->srt.roll = objSetup->roll << 8;
    self->srt.scale = self->def->scale;

    self->animCallback = (void*)&collectable_anim_callback;
    self->modelInstIdx = objSetup->modelIdx;
    self->unkB0 |= 0x2000;

    bzero(objData, sizeof(Collectable_Data));
    objData->sidekickArgBase = objSetup->messageArgBase;
    objData->unk11 = objSetup->unk1A;
    objData->pause = 0;
    objData->areaValue = -2;
    objData->moving = 0;
    objData->delayCollect = 60;
    objData->gamebitShow = objSetup->gamebitShow;
    objData->uID = objSetup->base.uID;
    objData->savedPosition.x = self->srt.transl.x;
    objData->savedPosition.y = self->srt.transl.y;
    objData->savedPosition.z = self->srt.transl.z;
    objData->useColourMultiplier = objSetup->applyColourMultiplier;

    //Check if hidden via gamebit
    if (objData->gamebitShow != NO_GAMEBIT) {
        objData->isHidden = main_get_bits(objData->gamebitShow) == 0;
    }

    //Check if already collected
    objData->gamebitCollected = objSetup->gamebitCollected;
    if (objData->gamebitCollected != NO_GAMEBIT) {
        self->unkDC = main_get_bits(objData->gamebitCollected);
    } else {
        self->unkDC = 0;
    }

    if (self->unkDC) {
        return;
    }

    collectableDef = (CollectableDef*)self->def->unk18;
    if (collectableDef && collectableDef->category == Collectable_Type_Magic) {
        if (arg2 == 0) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_8E_Magic_Chime, MAX_VOLUME, 0, 0, 0, 0);
        }

        for (index = 10; index > 0; index--){
            gDLL_17_partfx->vtbl->spawn(self, 0x42, 0, 4, -1, 0);
        }

        self->speed.y = -1.2f;
    }

    if (collectableDef) {
        objData->interactionRadius = collectableDef->interactionRadius;
    } else {
        objData->interactionRadius = 15.0f;
    }

    if (self->def->unk40) {
        objData->interactionRadius = self->def->unk40->unk0c * 4;
    }

    if (self->def->unk87 & 0x10) {
        if (1){ }
        lfxAction.unk12.asByte = 25;
        lfxAction.unke = 0;
        lfxAction.unk15 = 0;
        lfxAction.unk16 = 0;
        lfxAction.unk17 = 0;
        lfxAction.unk18 = *(u8*)((u32)self->def + 0x89); 
        lfxAction.unk19 = *(u8*)((u32)self->def + 0x8A);
        lfxAction.unk1a = *(u8*)((u32)self->def + 0x8B);
        lfxAction.unk10 = 0xFFFE;
        lfxAction.unk1b = 0;
        lfxAction.unk1c = 0;
        lfxAction.unk0 = 0;
        lfxAction.unk2 = 0;
        lfxAction.unk8 = 0;
        lfxAction.unk6 = 0;
        lfxAction.unk4 = 0;
        lfxAction.unka = 0x54;
        lfxAction.unkc = 0x25;
        lfxAction.unk1d = 0xFF;
        lfxAction.unk1e = 0;
        lfxAction.unk1f = self->def->lightIdx;
        lfxAction.unk20 = self->def->nLights;
        lfxAction.unk21 = 0;
        gDLL_11_Newlfx->vtbl->func0(self, self, &lfxAction, 0, 0, 0);
        self->unkD6 = lfxAction.unk10;
    }

    if (self->shadow){
        self->shadow->flags = OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR;
        id = self->id;
        if (id == OBJ_BoneDust || id == OBJ_WM_PureMagic){
            self->shadow->flags |= OBJ_SHADOW_FLAG_CUSTOM_OPACITY;
        }
    }

    if ((self->def->flags & 0x10000) && objData->useColourMultiplier) {
        objData->multiplyR = objSetup->multiplyR;
        objData->multiplyG = objSetup->multiplyG;
        objData->multiplyB = objSetup->multiplyB;
    }
}

// offset: 0x430 | func: 1 | export: 1
void collectable_control(Object* self) {
    Collectable_Data* objdata;
    Collectable_Setup* objsetup;
    Object* messageSender;
    Object* player;
    CollectableDef* collectableDef;
    f32 distance;
    u32 outMessage;
    s32 messageArg;
    s32 index;
    f32 newdayValue;

    objdata = self->data;
    objsetup = (Collectable_Setup*)self->setup;

    collectableDef = (CollectableDef*)self->def->unk18;

    newdayValue = 1.0f;

    //Countdown to destroy after receiving collection message
    if (objdata->timerDestroy != 0.0f) {
        objdata->timerDestroy -= gUpdateRateF;
        if (objdata->timerDestroy <= 0.0f) {
            obj_destroy_object(self);
            objdata->timerDestroy = 0.0f;
        }
        return;
    }

    gDLL_7_Newday->vtbl->func5(&newdayValue);
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    if (objdata->gamebitShow != NO_GAMEBIT) {
        objdata->isHidden = main_get_bits(objdata->gamebitShow) == 0;
    }

    //End early if hidden/paused
    if (objdata->isHidden || objdata->pause) {
        return;
    }

    //Destroy self when lifetime runs out
    if (self->unkE0) {
        self->unkE0 -= (s16)(gUpdateRateF * 3.0f);
        if (self->unkE0 <= 0) {
            obj_destroy_object(self);
            return;
        }
    }

    //Count down until can be collected
    objdata->delayCollect -= gUpdateRate;
    if (objdata->delayCollect < 0) {
        objdata->delayCollect = 0;
    }

    //Check for collection message
    while (obj_recv_mesg(self, &outMessage, &messageSender, 0)) {
        if (outMessage == 0x7000B) {
            objdata->timerDestroy = 180.0f;
            collectable_collect(self);
        }
    }

    //Timer for Alpine Roots
    if (self->id == OBJ_DIMAlpineRoot2) {
        if (objdata->rootTimer) {
            objdata->rootTimer -= gUpdateRate;
            if (objdata->rootTimer <= 0) {
                objdata->rootTimer = 0;
                self->opacity = 0xFF;
                self->unkDC = 0;
            }
        }
    }

    if (self->srt.flags & 0x4000) {
        return;
    }

    if (self->unkDC) {
        if (self->objhitInfo) {
            self->objhitInfo->unk58 |= 0x100;
        }
        if (objdata->gamebitCollected != NO_GAMEBIT && main_get_bits(objdata->gamebitCollected) == FALSE) {
            self->unkDC = 0;
        }
        return;
    }

    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
    collectable_handle_animation_and_fx(self);
    if (objdata->moving) {
        collectable_handle_motion(self);
    }

    player = get_player();
    if (!player || objdata->interactFlags & COLLECT_OFF || !self->def->unk18) {
        return;
    }

    //Handle collection when close enough 
    //(either automatically or via target arrow, depending on objectID)
    distance = vec3_distance_xz(&self->positionMirror, &player->positionMirror);
    if ((distance < objdata->interactionRadius) && (objdata->delayCollect == 0)) {
        switch (self->id) {
        case OBJ_meatPickup:
            objdata->interactFlags |= COLLECT_OFF;
            gDLL_13_Expgfx->vtbl->func5(self);

            for (index = 10; index > 0; index--){
                gDLL_17_partfx->vtbl->spawn(self, 0x549, 0, 1, -1, 0);
            }

            if (main_get_bits(BIT_90E) == 0) {
                gDLL_3_Animation->vtbl->func30(collectableDef->unk6, 0, 0);
                outMessage = 0;
                obj_send_mesg(
                    player, 
                    0x7000A, 
                    self, 
                    0
                );
                main_set_bits(BIT_90E, 1);
            } else {
                objdata->timerDestroy = 180.0f;
                collectable_collect(self);
            }
            break;
        case OBJ_BoneDust:
        case OBJ_WM_PureMagic:
        case OBJ_beanPickup:
        case OBJ_DIMAlpineRoot2:
        case OBJ_applePickup:
            objdata->interactFlags |= COLLECT_OFF;
            collectable_collect(self);
            break;
        default:
            //Check for A button press when highlighted with arrow
            messageArg = objsetup->animMessage;
            if (self->unkAF & ARROW_FLAG_1_Interacted) {
                gDLL_3_Animation->vtbl->func30(collectableDef->unk6, 0, 0);
                obj_send_mesg(
                    player,
                    0x7000A,
                    self,
                    (void*)messageArg
                );
            }
            break;
        }
    }
    objdata->distanceToPlayer = distance;
}

// offset: 0x994 | func: 2 | export: 2
void collectable_update(Object *self) { }

// offset: 0x9A0 | func: 3 | export: 3
void collectable_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    Collectable_Data* objdata = self->data;

    if (0) {}

    if (visibility && (objdata->timerDestroy == 0.0f) && (self->unkDC == 0) && !objdata->isHidden) {
        if (0) { }

        if ((self->def->flags & 0x10000) && objdata->useColourMultiplier) {
            func_80036F6C(objdata->multiplyR, objdata->multiplyG, objdata->multiplyB);
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xA88 | func: 4 | export: 4
void collectable_free(Object* self, s32 arg1) {
    Collectable_Data* objdata = self->data;
    obj_free_object_type(self, 5);
    if (objdata->soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
        objdata->soundHandle = 0;
    }
}

// offset: 0xB00 | func: 5 | export: 5
s32 collectable_get_model_flags(Object *self) {
    return MODFLAGS_10 | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0xB10 | func: 6 | export: 6
u32 collectable_get_data_size(Object *self, u32 a1) {
    return sizeof(Collectable_Data);
}

// offset: 0xB24 | func: 7
int collectable_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData) { //NOTE: no arg3?
    f32 cos;
    f32 sin;

    animObjData->unk62 = 0;
    if (animObjData->unk8D == 1) {
        sin = fsin16_precise(0x6900);
        cos = fcos16_precise(0x6900);
        collectable_set_speed(self, sin * 8.0f, 2, cos * 8.0f);
        collectable_set_speed(self, 4.0f, 2, 0.0f);
        animObjData->unk8D = 0;
    }
    return 0;
}

// offset: 0xBF8 | func: 8
/** Handles objectID-specific animation/particle/sound-effect behaviours */
void collectable_handle_animation_and_fx(Object* self) {
    s16 id;
    Collectable_Data* objdata;
    ObjectShadow* shadow;
    s32 opacity;
    CollectableDef* collectableDef;
    s32 temp;

    collectableDef = (CollectableDef*)self->def->unk18;
    if (collectableDef == NULL) {
        return;
    }

    objdata = self->data;

    //Handle magic (@framerate-dependent)
    if (collectableDef->category == Collectable_Type_Magic) {
        //Spin 
        self->srt.yaw += 50;
        self->srt.pitch += 50;
        self->srt.roll += 50;

        //Grow
        if (self->srt.scale < 0.008f) {
            self->srt.scale += 0.0002f;
        }

        //Vertical motion + shadow animation
        if (self->speed.y < 0.0f) {
            self->srt.transl.y += self->speed.y;
            self->speed.y += 0.03f;
            if (self->speed.y >= 0.0f) {
                shadows_func_8004D974(1);
                objdata->shadowOpacity = 0;
            }
        } else {
            shadow = self->shadow;
            if (shadow) {
                opacity = objdata->shadowOpacity + (gUpdateRate * 8);
                if (opacity > 0xFF) {
                    opacity = 0xFF;
                }
                objdata->shadowOpacity = opacity;
                temp = shadows_calc_opacity(self, shadow);
                shadow->opacity = (temp * (opacity + 1)) >> 8;
            }
        }

        //Sparkles
        if (rand_next(0, 80) == 0) {
            gDLL_17_partfx->vtbl->spawn(self, 0x47, 0, 4, -1, 0);
        }
    }

    id = self->id;
    switch (id) {
    case OBJ_meatPickup:
        //Play rattle sound when random timer runs out
        objdata->soundTimer -= gUpdateRate;
        if (objdata->soundTimer <= 0) {
            objdata->pitchAnimate = rand_next(600, 800);
            objdata->soundTimer = rand_next(180, 240);
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_8FC_Egg_Rattle, MAX_VOLUME, 0, 0, 0, 0);
        }

        //Rapidly oscillate rotational pitch for a basic rattle animation
        self->srt.pitch = objdata->pitchAnimate;
        objdata->pitchAnimate *= -0.8f; //@framerate-dependent
        if ((10 > self->srt.pitch) && (self->srt.pitch > -10)) {
            self->srt.pitch = 0;
            return;
        }
        break;
    case OBJ_DIM2PuzzleKey:
    case OBJ_DIM2GoldKey:
    case OBJ_DIM2SilverKey:
    case OBJ_DIMTruthHorn:
    case OBJ_DIMBridgeCogCol:
        self->srt.yaw += gUpdateRate * 200;
        return;
    case OBJ_SC_golden_nugge:
        if (objdata->distanceToPlayer < 200.0f) {
            if (rand_next(0, 10) == 0) {
                gDLL_17_partfx->vtbl->spawn(self, 0x423, 0, 2, -1, 0);
            }
            self->srt.yaw += (s16) (182.0f * gUpdateRateF);
            return;
        }
        break;
    case OBJ_WCTrexTooth:
        if (objdata->distanceToPlayer < 200.0f) {
            if (rand_next(0, 10) == 0) {
                if (self->modelInstIdx == 0) {
                    gDLL_17_partfx->vtbl->spawn(self, 0x73D, 0, 2, -1, 0);
                } else {
                    gDLL_17_partfx->vtbl->spawn(self, 0x73E, 0, 2, -1, 0);
                }
            }
            self->srt.yaw += (s16) (182.0f * gUpdateRateF);
        }
        break;
    }

}

// offset: 0x10C8 | func: 9
void collectable_handle_motion(Object* self) {
    Collectable_Data* objdata;
    Func_80057F1C_Struct** sp50;
    f32 dt;
    f32 maxFound;
    f32 sampleValue;
    s32 count;
    s32 i;

    objdata = self->data;

    count = func_80057F1C(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &sp50, 0, 0);

    maxFound = -10000.0f;
    for (i = 0; i < count; i++){
        sampleValue = sp50[i]->unk0[0];
        if ((sampleValue < (self->srt.transl.y + 30.0f)) && (maxFound < sampleValue)) {
            maxFound = sampleValue;
        }
    }   

    if (self->srt.transl.y < (maxFound + 10.0f)) {
        self->srt.transl.y = maxFound + 10.0f;
        self->speed.y = 0.0f - self->speed.y;
        self->speed.y *= 0.99f;
        self->speed.x *= 0.7f;
        self->speed.z *= 0.7f;
        if (sqrtf(SQ(self->speed.x) + SQ(self->speed.z)) < 0.03f) {
            objdata->moving--;
            self->speed.y *= 0.5f;
            if (objdata->moving <= 0) {
                objdata->moving = 0;
                self->speed.y = 0.0f;
            }
        }
    } else {
        self->speed.y *= 0.99f;
        self->speed.y -= 0.07f;
    }

    dt = (f32) gUpdateRate;
    obj_integrate_speed(self, self->speed.x * dt, self->speed.y * dt, self->speed.z * dt);
}

// offset: 0x1304 | func: 10
/** Runs when collectable is obtained */
void collectable_collect(Object* self) {
    Collectable_Data* objdata;
    Collectable_Setup* objsetup;
    CollectableDef* collectableDef;
    Object* player;
    Object* sidekick;
    s16 id;
    LightAction lfxAction;
    Object* foodbag;
    ObjDef* objdef;

    objdata = self->data;
    objsetup = (Collectable_Setup*)self->setup;
    player = get_player();
    sidekick = get_sidekick();
    collectableDef = (CollectableDef*)self->def->unk18;

    if (collectableDef == NULL) {
        return;
    }

    //Set the collection gamebit (usually for making sure the object doesn't reappear)
    if (objdata->gamebitCollected != NO_GAMEBIT) {
        main_set_bits(objdata->gamebitCollected, TRUE);
    }

    //Increment the counter gamebit (if one is being used)
    if (objsetup->gamebitCount > 0) {
        main_increment_bits(objsetup->gamebitCount);
    }

    switch (collectableDef->category) {
    case Collectable_Type_1:
        id = self->id;
        switch (id) {
        case OBJ_Duster:   
        default:
            break;
        case OBJ_DIMAlpineRoot2: 
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_506_Chomping_Food, MAX_VOLUME, 0, 0, 0, 0);
            main_set_bits(BIT_3E9, 1);
            self->unkDC = 1;
            objdata->rootTimer = 1200;
            return;
        case OBJ_foodbagSmall:
        case OBJ_foodbagMedium:
        case OBJ_foodbagLarge:
            foodbag = (Object*)((DLL_210_Player*)player->dll)->vtbl->func66(player, 15);
            (0, ((DLL_314_Foodbag*)foodbag->dll)->vtbl->set_capacity)(foodbag);
            break;
        }
        break;
    case Collectable_Type_Food:
        foodbag = (Object*)((DLL_210_Player*)player->dll)->vtbl->func66(player, 15);
        id = self->id;
        switch (id) {  
        case OBJ_meatPickup:      
            ((DLL_314_Foodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Dino_Egg);
            obj_free_object_type(self, 5);
            return;
        case OBJ_applePickup:
            ((DLL_314_Foodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Red_Apple);
            obj_free_object_type(self, 5);
            obj_destroy_object(self);
            return;
        case OBJ_beanPickup:
            ((DLL_314_Foodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Blue_Bean);
            break;
        }
        break;
    case Collectable_Type_8_Sidekick:
        obj_send_mesg(sidekick, 0x70004, self, (void*)(collectableDef->unkB + objdata->sidekickArgBase));
        break;
    case Collectable_Type_10_Sidekick:
        obj_send_mesg(sidekick, 0x70005, self, (void*)(collectableDef->unkB + objdata->sidekickArgBase));
        break;
    case Collectable_Type_Magic:
        ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, collectableDef->unkB);
        gDLL_13_Expgfx->vtbl->func5(self);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_8E_Magic_Chime, MAX_VOLUME, 0, 0, 0, 0);
        break;
    case Collectable_Type_80_Sidekick:
        obj_send_mesg(sidekick, 0x70008, self, (void*)(collectableDef->unkB + objdata->sidekickArgBase));
        break;
    }

    objdef = self->def;
    self->srt.scale = objdef->scale;
    self->unkDC = 1;
    if (objdef->unk87 & 0x10) {
        lfxAction.unk12.asByte = 2;
        lfxAction.unke = 0;
        lfxAction.unk1b = 0;
        lfxAction.unk10 = self->unkD6;
        gDLL_11_Newlfx->vtbl->func0(self, self, &lfxAction, 0, 0, 0);
    }
}

// offset: 0x1780 | func: 11 | export: 7
/** Queries whether collectable is collected */
int collectable_is_collected(Object* self) {
    return self->unkDC;
}

// offset: 0x178C | func: 12 | export: 8
void collectable_set_pause_state(Object* self, s32 state) {
    Collectable_Data *objdata = self->data;
    objdata->pause = state;
}

// offset: 0x179C | func: 13 | export: 9
/** Checks if the collectable is inside the bounds of any "Area" object, 
  * and if so takes a value from the object's Area_Setup */
s32 collectable_get_area_value(Object* self) {
    Collectable_Data *objdata;

    objdata = self->data;
    if (objdata->areaValue == -2) {
        objdata->areaValue = func_80031BBC(
            self->positionMirror.x, 
            self->positionMirror.y, 
            self->positionMirror.z
        );
    }
    return objdata->areaValue;
}

// offset: 0x1804 | func: 14 | export: 11
void collectable_set_visibility(Object* self, s32 visibility) {
    Collectable_Data *objdata = self->data;
    objdata->isHidden = visibility;
}

// offset: 0x1814 | func: 15 | export: 12
u8 collectable_get_visibility(Object* self) {
    Collectable_Data *objdata = self->data;
    return objdata->isHidden;
}

// offset: 0x1824 | func: 16 | export: 10
void collectable_set_speed(Object* self, f32 speedX, f32 speedY, f32 speedZ) {
    Collectable_Data *objdata = self->data;
    objdata->moving = 8;
    self->speed.x = speedX;
    self->speed.y = speedY;
    self->speed.z = speedZ;
}

// offset: 0x1854 | func: 17 | export: 13
void collectable_save_position(Object* self, f32 x, f32 y, f32 z) {
    Collectable_Data* objdata = self->data;
    if (self->setup) {
        self->srt.transl.x = x;
        objdata->savedPosition.x = x;
        self->srt.transl.y = y;
        objdata->savedPosition.y = y;
        self->srt.transl.z = z;
        objdata->savedPosition.z = z;
        map_save_object(
            self->setup, self->mapID, 
            self->srt.transl.x,
            self->srt.transl.y,
            self->srt.transl.z
        );
    }
}
