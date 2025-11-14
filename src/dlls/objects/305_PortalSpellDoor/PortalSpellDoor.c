#include "common.h"
#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "macros.h"

int func_80058F50(void);

#define ONE_OVER_SIXTY_FOUR 0.015625f

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 hitsAnimatorID;      //animatorID of the HITS line(s) that should be removed when door is open
    s16 scale;
    s16 pitch;
    s16 gamebitActivated;   //gamebitID set when door is open
} PortalSpellDoor_Setup;

typedef struct {
    Object* portalDoorAnim; //secondary object used during door transformation sequence
    f32 scale;              //the object's scale multiplied by 32.0
    s32 timer;              //countdown to door transformation after player uses Portal Spell
    u32 sequencePlayed : 1; //set at beginning of door transformation sequence
} PortalSpellDoor_Data;

static Object* PortalSpellDoor_create_anim_obj(Object* self);

// offset: 0x0 | ctor
void PortalSpellDoor_ctor(void *dll) { }

// offset: 0xC | dtor
void PortalSpellDoor_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void PortalSpellDoor_setup(Object* self, PortalSpellDoor_Setup* objSetup, s32 arg2) {
    PortalSpellDoor_Data* objData;
    ObjDef* objDef;

    objData = self->data;

    self->srt.yaw = objSetup->yaw << 8;
    self->srt.pitch = objSetup->pitch << 8;

    //Set scale
    if (objSetup->scale) {
        if (arg2 == 0) {
            objDef = self->def;
            self->srt.scale = objSetup->scale * objDef->scale * ONE_OVER_SIXTY_FOUR; //@bug: unnecessary line?
            self->srt.scale = objSetup->scale * 0.35f * ONE_OVER_SIXTY_FOUR;
            objDef->scale = self->srt.scale;
            self->unkA8 = self->def->scale * 64.0f;
        } else {
            self->srt.scale = objSetup->scale * 0.35f * ONE_OVER_SIXTY_FOUR;
        }
    }
    objData->scale = self->unkA8 * 0.5f;

    //Check if door is already open
    if (main_get_bits(objSetup->gamebitActivated)) {
        //Remove HITS line
        if (objSetup->hitsAnimatorID && !func_80058F50()) {
            func_80059038(objSetup->hitsAnimatorID, self->parent, 0);
        }
        self->srt.flags |= 0x4000;
        self->unkB0 |= 0xE000;
    }

    objData->timer = -1;
}

// offset: 0x1A8 | func: 1 | export: 1
void PortalSpellDoor_control(Object* self) {
    Object* player;
    PortalSpellDoor_Data* objData;
    PortalSpellDoor_Setup* objSetup;

    player = get_player();
    objData = self->data;
    objSetup = (PortalSpellDoor_Setup*)self->setup;

    //Check if the door transformation sequence has played, otherwise wait for player to use spell
    if (objData->sequencePlayed) {
        self->srt.flags |= 0x4000;

        //Destroy secondary door object
        if (objData->portalDoorAnim != NULL) {
            obj_destroy_object(objData->portalDoorAnim);
            objData->portalDoorAnim = NULL;
        }

        ((DLL_210_Player*)player->dll)->vtbl->func51(player, -1);

        main_set_bits(objSetup->gamebitActivated, TRUE);

        //Remove HITS line
        if (objSetup->hitsAnimatorID && !func_80058F50()) {
            func_80059038(objSetup->hitsAnimatorID, self->parent, 0);
        }
    } else if ((((DLL_210_Player*)player->dll)->vtbl->func50(player) == BIT_Spell_Portal) && (objData->timer == -1)) {
        //Start countdown to door transformation
        objData->timer = 157;
    }

    //Handle countdown timer
    if (objData->timer != -1) {
        objData->timer -= gUpdateRate;
        if (DEBUG){
            STUBBED_PRINTF(" t %i ");
        }
        //Play door transformation sequence
        if (objData->timer < 0) {
            self->unkAF |= 8;
            self->def->scale *= 0.5f;
            objData->portalDoorAnim = PortalSpellDoor_create_anim_obj(self);
            gDLL_3_Animation->vtbl->func31(fsin16_precise(self->srt.yaw) * objData->scale, 0.0f, fcos16_precise(self->srt.yaw) * objData->scale);
            gDLL_3_Animation->vtbl->func17(0, self, -1);
            objData->sequencePlayed = TRUE;
            objData->timer = -1;
        }
    }
}

// offset: 0x3FC | func: 2 | export: 2
void PortalSpellDoor_update(Object *self) { }

// offset: 0x408 | func: 3 | export: 3
void PortalSpellDoor_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x45C | func: 4 | export: 4
void PortalSpellDoor_free(Object* self, s32 arg1) {
    PortalSpellDoor_Data* objData;

    objData = self->data;
    if (objData->portalDoorAnim && (arg1 == 0)) {
        obj_destroy_object(objData->portalDoorAnim);
    }
}

// offset: 0x4B0 | func: 5 | export: 5
u32 PortalSpellDoor_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x4C0 | func: 6 | export: 6
u32 PortalSpellDoor_get_data_size(Object *self, u32 a1) {
    return sizeof(PortalSpellDoor_Data);
}

// offset: 0x4D4 | func: 7
Object* PortalSpellDoor_create_anim_obj(Object* self) {
    ObjSetup* animObjSetup;
    Object* portalDoorAnim;

    animObjSetup = obj_alloc_create_info(sizeof(ObjSetup), OBJ_PortalDoorAnim);
    animObjSetup->loadFlags = 2;
    animObjSetup->byte5 = 1;
    animObjSetup->x = self->srt.transl.x;
    animObjSetup->y = self->srt.transl.y;
    animObjSetup->z = self->srt.transl.z;
    portalDoorAnim = obj_create(animObjSetup, 5, -1, -1, self->parent);
    portalDoorAnim->def->scale = self->def->scale;
    portalDoorAnim->srt.scale = 1.0f;
    return portalDoorAnim;
}
