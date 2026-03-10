//"screenoverlay.c" seems to have been Rare's official filename

#include "common.h"
#include "sys/camera.h"

typedef struct {
    s8 fadeInSpeed;
    s8 fadeOutSpeed;
    s16 soundIDIn;      //soundID to play when overlay appears
    s16 soundIDOut;     //soundID to play when overlay disappears
    s16 soundIDLoop;    //soundID to loop while overlay is visibleS
} SpellOverlay_Config;

typedef struct {
    ObjSetup base;
} SpellOverlay_Setup;

typedef struct {
    SpellOverlay_Config* config;
    s16 fadeSpeed;
    u32 soundHandle;
} SpellOverlay_Data;

typedef enum {
    SpellOverlay_ANIMFLAG_None = 0,
    SpellOverlay_ANIMFLAG_Scroll_U = 1,
    SpellOverlay_ANIMFLAG_Scroll_V = 2,
    SpellOverlay_ANIMFLAG_FrameBlend = 4
} SpellOverlay_Anim_Flags;

/*0x0*/ static SpellOverlay_Config dSpellOverlayConfig = {
    2, 
    1, 
    SOUND_19A_Magic_Reverse_Cymbal,     
    NO_SOUND,                          
    NO_SOUND                          
};

/*0x8*/ static f32 dCameraNearPlane = 4.0f;

static void SpellOverlay_animate_texture(Object* self, s32 arg1, u8 arg2);

// offset: 0x0 | ctor
void SpellOverlay_ctor(void *dll) {
    camera_set_near_plane(dCameraNearPlane);
}

// offset: 0x40 | dtor
void SpellOverlay_dtor(void *dll) {
    camera_reset_near_plane();
}

// offset: 0x7C | func: 0 | export: 0
void SpellOverlay_setup(Object* self, SpellOverlay_Setup* objSetup, s32 arg2) {
    SpellOverlay_Data* objData;

    objData = self->data;
    
    self->unkDC = 0;
    objData->config = &dSpellOverlayConfig;
    objData->fadeSpeed = objData->config->fadeInSpeed;
    
    if (arg2 == 0) {
        STUBBED_PRINTF(" Playing Snd");

        //Play a sound as the effect appears
        if (objData->config->soundIDIn != NO_SOUND) {
            gDLL_6_AMSFX->vtbl->play_sound(self, objData->config->soundIDIn, MAX_VOLUME, 0, 0, 0, 0);
        }
        
        //Start a looping sound (none assigned in config, however)
        if (objData->config->soundIDLoop != NO_SOUND) {
            gDLL_6_AMSFX->vtbl->play_sound(self, objData->config->soundIDLoop, MAX_VOLUME, &objData->soundHandle, 0, 0, 0);
        }
    }
    
    self->srt.transl.x = objSetup->base.x;
    self->srt.transl.y = objSetup->base.y;
    self->srt.transl.z = objSetup->base.z;
    self->opacity = 0;
}

// offset: 0x190 | func: 1 | export: 1
void SpellOverlay_control(Object *self) {
    SpellOverlay_animate_texture(self, 1, SpellOverlay_ANIMFLAG_Scroll_U | SpellOverlay_ANIMFLAG_Scroll_V);
}

// offset: 0x1D4 | func: 2 | export: 2
void SpellOverlay_update(Object *self) { }

// offset: 0x1E0 | func: 3 | export: 3
void SpellOverlay_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    u32 current_opacity;
    f32 fov;
    s16 new_opacity;
    SpellOverlay_Data *objData;
    Object *player;
    Camera *camera;
    
    objData = self->data;
    player = get_player();
    
    //Animate opacity
    if (objData->fadeSpeed != 0){
        current_opacity = self->opacity;
        new_opacity = self->opacity;
        
        if ((current_opacity == 0) && (objData->fadeSpeed < 0)){
            objData->fadeSpeed = 0;
            obj_destroy_object(self); 
        }

        if ((self->opacity == OBJECT_OPACITY_MAX) && (objData->fadeSpeed > 0)){
            objData->fadeSpeed = 0;
        }
        
        new_opacity += objData->fadeSpeed * gUpdateRate;
        if (new_opacity < 0){
            new_opacity = 0;
        } else {
            if (new_opacity > OBJECT_OPACITY_MAX) {
                current_opacity = OBJECT_OPACITY_MAX;
            } else {
                current_opacity = new_opacity;
            }
            new_opacity = current_opacity;
        }
        self->opacity = new_opacity;
    }
    
    //Draw while player isn't in a sequence
    if (!(player->unkB0 & 0x1000)){
        diPrintf(" DRAWING THE THING ");

        get_main_camera(); //@bug?: not stored/used
        fov = camera_get_fov();
        set_camera_selector(2);

        camera = get_main_camera();
        camera->srt.transl.x = gWorldX;
        camera->srt.transl.y = 0;
        camera->srt.transl.z = gWorldZ;
        camera->srt.roll = 0;
        camera->srt.pitch = 0;
        camera->srt.yaw = 0;
        camera->dty = 0.0f;
        camera_set_fov(40.0f);

        self->srt.scale = self->def->scale;
        self->srt.roll = 0;
        self->srt.yaw = -0x8000;
        self->srt.pitch = 0;
        self->srt.transl.x = gWorldX;
        self->srt.transl.y = -1.5f;
        self->srt.transl.z = gWorldZ;
        self->positionMirror.x = gWorldX;
        self->positionMirror.y = -1.5f;
        self->positionMirror.z = gWorldZ;

        camera_setup_viewport_and_matrices(gdl, mtxs);
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        set_camera_selector(0);
        camera_set_fov(fov);
        camera_setup_viewport_and_matrices(gdl, mtxs);
    }
}

// offset: 0x494 | func: 4 | export: 4
void SpellOverlay_free(Object* self, s32 arg1) {
    SpellOverlay_Data* objData;
    u32 soundHandle;

    objData = self->data;
    soundHandle = objData->soundHandle;
    if (soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(soundHandle);
    }
}

// offset: 0x4F0 | func: 5 | export: 5
u32 SpellOverlay_get_model_flags(Object *self){
    return MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x500 | func: 6 | export: 6
u32 SpellOverlay_get_data_size(Object *self, u32 a1) {
    return sizeof(SpellOverlay_Data);
}

// offset: 0x514 | func: 7 | export: 7
/** Starts fading out the overlay */
int SpellOverlay_fade_out(Object* self) {
    SpellOverlay_Data* objData;

    objData = self->data;
    if (objData->fadeSpeed < 0) {
        return 0;
    }

    objData->fadeSpeed = -objData->config->fadeOutSpeed;

    //Play a fade out sound (none assigned in the config, however)
    if (objData->config->soundIDOut != NO_SOUND) {
        gDLL_6_AMSFX->vtbl->play_sound(self, objData->config->soundIDOut, MAX_VOLUME, 0, 0, 0, 0);
    }

    return 1;
}

// offset: 0x5AC | func: 8
void SpellOverlay_animate_texture(Object* self, s32 materialID, u8 animFlags) {
    TextureAnimator* texAnim;
    s32 new_frame;

    texAnim = func_800348A0(self, materialID - 1, 0);
    if (!texAnim) {
        return;
    }
        
    diPrintf(" Anim %i ", materialID);
    
    if (animFlags & SpellOverlay_ANIMFLAG_Scroll_U) {
        texAnim->positionU += 0x20;
    }

    if (animFlags & SpellOverlay_ANIMFLAG_Scroll_V) {
        texAnim->positionV += 0x20;
    }

    if (animFlags & SpellOverlay_ANIMFLAG_FrameBlend) {
        new_frame = texAnim->frame + gUpdateRate * 4;
        texAnim->frame = new_frame; //@bug?: doesn't wrap the value
    }
}
