#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "sys/rand.h"
#include "sys/controller.h"
#include "game/objects/object.h"

void func_8002674C(Object* obj);

static s32 dll_227_func_1FA0(Object* self);

/*0x0*/ static f32 _data_0[] = {
    0.0, 0.0, 0.0
};
/*0xC*/ static f32 _data_C = {
    25.0
};

typedef struct {
/*00*/ ObjCreateInfo base;
/*18*/ s8 roll;
/*19*/ s8 pitch;
/*1A*/ s8 yaw;
/*1B*/ u8 unk1B;
/*1C*/ f32 unk1C;
} TumbleweedCreateInfo;

typedef struct {
/*00*/ ObjCreateInfo base;
/*18*/ s32 unk18;
/*1c*/ s32 unk1c;
/*20*/ s32 unk20;
/*24*/ s16 unk24;
} GoldenNuggetCreateInfo;

typedef struct {
/*000*/ s8 unk0[0x260 - 0];
/*260*/ u16 unk260;
/*262*/ u16 unk262;
/*264*/ f32 scaleCopy;
/*268*/ f32 unk268;
/*26C*/ f32 unk26C;
/*270*/ u8 unk270;
/*271*/ u8 unk271;
/*272*/ u8 flags;
/*273*/ s8 unk273;
/*274*/ s16 unk274; //rollSpeed
/*276*/ s16 unk276; //pitchSpeed
/*278*/ s16 unk278; //yawSpeed
/*27A*/ s16 unk27A;
/*27C*/ Object* goldenNugget;
/*280*/ s32 unk280;
/*284*/ s16 goldenNuggetFlag;
/*286*/ s16 unk286;
/*288*/ f32 xCopy;
/*28C*/ f32 yCopy;
/*290*/ Vec3f* unk290;
/*294*/ s16 unk294;
/*296*/ s16 unk296;
/*298*/ u8 unk298;
/*299*/ u8 unk299;
} TumbleweedState;

// offset: 0x0 | ctor
void dll_227_ctor(void* dll){
}

// offset: 0xC | dtor
void dll_227_dtor(void* dll){
}

// offset: 0x18 | func: 0 | export: 0
void dll_227_func_18(Object* self, TumbleweedCreateInfo* createInfo, GoldenNuggetCreateInfo* arg2) {
    Object* object;
    Object** objects;
    TumbleweedState* state;
    s32 index;
    s32 count;

    state = self->state;
    state->xCopy = self->srt.transl.x;
    state->yCopy = self->srt.transl.z;
    state->unk262 = 2.0f * createInfo->unk1C;
    state->unk271 = createInfo->unk1B;
    state->flags = 0;
    state->scaleCopy = self->srt.scale;
    state->unk268 = (state->scaleCopy / rand_next(0xC8, 0x1F4));
    state->unk280 = 0;
    
    self->speed.x = 0.0f;
    self->speed.y = 0.0f;
    self->speed.z = 0.0f;
     
    if (self->ptr0x64) {
        self->ptr0x64->flags |= 0x810;
    }
    
    state->unk296 = 0x32;
    if (self->id == OBJ_Tumbleweed1twig || self->id == OBJ_Tumbleweed2twig || self->id == OBJ_Tumbleweed3twig) {
        self->unk0xaf &= 0xFFF7;
        state->unk270 = 3;
        state->unk268 = 480.0f;
        self->srt.scale = state->scaleCopy;
    } else {
        self->srt.scale = 0.001f;
        gDLL_27_HeadTurn->vtbl->head_turn_func_18((void*)state, 0, 0x40000, 1);
        gDLL_27_HeadTurn->vtbl->head_turn_func_84.withFiveArgs((s32)state, 1, (s32)&_data_0[0], (s32)&_data_C, 4);
        gDLL_27_HeadTurn->vtbl->head_turn_func_fb8(self, state);
        state->unk270 = 0;
        self->unk0xaf |= 8;
        
        if (state->unk271 & 1) {
            objects = get_world_objects(&index, &count);

            while (index < count){
                object = objects[index];
                if (object->id == OBJ_SC_golden_nugge) {

                    // @fake
                    if (1) {}
                    state->goldenNugget = object;
                    arg2 = (GoldenNuggetCreateInfo*)object->createInfo;
                    state->goldenNuggetFlag = arg2->unk24 & 0xFFFF;
                    // @fake
                    if (arg2) {}

                    if (main_get_bits(state->goldenNuggetFlag)) {
                        state->goldenNugget = NULL;
                    }
                    index = count;
                }
                index++;
            }
            
        }
    }
    
    obj_add_object_type(self, 4);
    obj_add_object_type(self, 0x33);
    func_800267A4(self);
}

// offset: 0x354 | func: 1 | export: 1
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/227_tumbleweed/dll_227_func_354.s")
#else

void dll_227_func_420(Object*);
void dll_227_func_658(Object*);
void dll_227_func_C04(Object*);

void dll_227_func_354(Object* self) {
    TumbleweedState* state;
    Object* shiny;

    state = self->state;
    if (self->id == OBJ_Tumbleweed1 || self->id == OBJ_Tumbleweed1twig) {
        dll_227_func_658(self);
    } else {
        dll_227_func_C04(self);
    }
    dll_227_func_420(self);
    
    shiny = state->goldenNugget;
    if (shiny) {
        //TODO: update with SCcollectables DLL interface
        ((DLL_Unknown*)shiny->dll)->vtbl->func[8].withTwoArgs((s32)shiny, (s32)self); 
    }
}
#endif

// offset: 0x420 | func: 2
void dll_227_func_420(Object* self) {
    TumbleweedState *new_var;
    s16 id;
    s32 index;
    TumbleweedState *state;
    
    new_var = self->state;
    state = new_var;
    
    //When being struck by weapon
    if (state->flags & 1){
        id = self->id;
        if (id == OBJ_Tumbleweed1 || id == OBJ_Tumbleweed1twig || id == OBJ_Tumbleweed3 || id == OBJ_Tumbleweed3twig){
            for (index = 0x14; index > 0; index--){
                //Creates leaf particles
                gDLL_17->vtbl->func1(self, 845, NULL, 2, -1, NULL);
            }
        } else {
            for (index = 0x14; index > 0; index--){
                //Creates frosty leaf particles
                gDLL_17->vtbl->func1(self, 814, NULL, 2, -1, NULL);
            }
        }
        //Play disintegration sound
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_5F7, MAX_VOLUME, 0, 0, 0, 0);
    }
    
    if (state->flags & 2){
        id = self->id;
        if (id == OBJ_Tumbleweed1 || id == OBJ_Tumbleweed1twig || id == OBJ_Tumbleweed3 || id == OBJ_Tumbleweed3twig){
            //Creates cloud of dust
            gDLL_17->vtbl->func1(self, 844, NULL, 2, -1, NULL);
            if (1) { } if (1) { } if (1) { } if (1) { } if (1) { } //nice, definitely how Rare wrote it
        } else {
            //Creates cloud of frost
            gDLL_17->vtbl->func1(self, 813, NULL, 2, -1, NULL);
        }
    }
    
    if (state->flags & 4){
        self->unk_0x36 = 0;
        self->unk0xaf |= 8;
        state->unk270 = 4;
        state->unk268 = 120.0f;
    }
    
    state->flags = 0;
}

// offset: 0x658 | func: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/227_tumbleweed/dll_227_func_658.s")

// offset: 0xC04 | func: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/227_tumbleweed/dll_227_func_C04.s")

// offset: 0x1518 | func: 5 | export: 2
void dll_227_func_1518(Object* self){
}

// offset: 0x1524 | func: 6 | export: 3
void dll_227_func_1524(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility > 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1578 | func: 7 | export: 4
void dll_227_func_1578(Object* self, s32 arg1) {
    TumbleweedState* state;
    Object* object;
    s32 sp3C;
    s32 count;
    s32 id;
    Object** objects;

    state = self->state;
      
    switch (self->id) {
        case OBJ_Tumbleweed1:
        case OBJ_Tumbleweed1twig:
            id = OBJ_TumbleWeedBush1;
            break;
        case OBJ_Tumbleweed2:
        case OBJ_Tumbleweed2twig:
            id = OBJ_TumbleWeedBush2;
            break;
        case OBJ_Tumbleweed3:
        case OBJ_Tumbleweed3twig:
            id = OBJ_TumbleWeedBush3;
            break;
    }
    
    objects = get_world_objects(&sp3C, &count);
    while (sp3C < count) {
        object = objects[sp3C];
        if (id == object->id) {
            ((DLL_Unknown*)object->dll)->vtbl->func[7].withTwoArgs((s32)object, (s32)self);
        }
        sp3C++;
    }
    
    if (state->goldenNugget) {
        main_set_bits(state->goldenNuggetFlag, 1);
        state->goldenNugget = 0;
    }
    obj_free_object_type(self, 4);
    obj_free_object_type(self, 0x33);
}

// offset: 0x16F8 | func: 8 | export: 5
s32 dll_227_func_16F8(Object* self) {
    return 0;
}

// offset: 0x1708 | func: 9 | export: 6
s32 dll_227_func_1708(Object* self, s32 arg1) {
    return 0x29C;
}

// offset: 0x171C | func: 10 | export: 7
u8 dll_227_func_171C(Object* self) {
    TumbleweedState* state = self->state;
    return state->unk270;
}

// offset: 0x172C | func: 11 | export: 8
void dll_227_func_172C(Object* self, f32 arg1, f32 arg2) {
    TumbleweedState *state = self->state;
    state->xCopy = arg1;
    state->yCopy = arg2;
}

// offset: 0x174C | func: 12 | export: 9
void dll_227_func_174C(Object* self) {
    TumbleweedState *state = self->state;
    if (state->unk270 == 1) {
        func_8002674C(self);
        state->unk270 = 2;
        self->unk0xaf &= ~8;
    }
}

// offset: 0x17C0 | func: 13 | export: 10
/** When being eaten */
void dll_227_func_17C0(Object* self, Vec3f* arg1) {
    TumbleweedState *state = self->state;
    f32 dx;
    f32 dy;
    f32 dz;
    
    dx = arg1->x - self->srt.transl.x;
    dy = arg1->y - self->srt.transl.y;
    dz = arg1->z - self->srt.transl.z;
    
    //@bug: not using delayByte (delta-time)?
    self->speed.x = dx * 0.02f;
    self->speed.y = dy * 0.02f;
    self->speed.z = dz * 0.02f;
    
    state->unk270 = 5;
    state->unk290 = arg1;
}

// offset: 0x1828 | func: 14 | export: 11
s32 dll_227_func_1828(Object* self) {
    TumbleweedState *state = self->state;
    return state->unk270 == 5;
}

// offset: 0x1840 | func: 15 | export: 12
void dll_227_func_1840(Object* self, s32 arg1) {
    TumbleweedState *state = self->state;
    state->unk280 = arg1;
}

// offset: 0x1850 | func: 16
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/227_tumbleweed/dll_227_func_1850.s")
#else

s32 func_80057F1C(Object*, f32, f32, f32, f32***, s32, s32);

void dll_227_func_1850(Object* self, TumbleweedState* state) {
    f32 var_fv0;
    s32 sampleCount;
    s32 minimumIndex;
    f32 **samples;
    f32 minimum;
    s32 volume;
    s32 i;

    samples = NULL;
    minimum = 10000.0f;
    sampleCount = func_80057F1C(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &samples, 0, 0);

    for (i = 0, minimumIndex = 0; i < sampleCount; i++){
        var_fv0 = self->srt.transl.y - *samples[i];
        if (var_fv0 < 0.0f) {
            var_fv0 = (var_fv0 * -1.0f) + 10.0f;
        }
        if (var_fv0 < minimum) {
            minimumIndex = i;
            minimum = var_fv0;
        }
    }

    if (self->speed.x > 1.0f) {
        self->speed.x = 1.0f;
    } else if (self->speed.x < -1.0f) {
        self->speed.x = -1.0f;
    }
    
    if (self->id == 0x39D || self->id == 0x40A) {
        if (self->speed.y > 1.0f) {
            self->speed.y = 1.0f;
        } else if (self->speed.y < -1.0f) {
            self->speed.y = -1.0f;
        }
    } else {
        if (self->speed.y > 1.5f) {
            self->speed.y = 1.5f;
        }
        else if (self->speed.y < -1.5f) {
            self->speed.y = -1.5f;
        }
    }
    
    if (self->speed.z > 1.0f) {
        self->speed.z = 1.0f;
    } else if (self->speed.z < -1.0f) {
        self->speed.z = -1.0f;
    }
    
    self->srt.transl.x += self->speed.x * delayFloat;
    self->srt.transl.y += self->speed.y * delayFloat;
    self->srt.transl.z += self->speed.z * delayFloat;
    self->srt.roll += state->unk274 * delayFloat;
    self->srt.pitch += state->unk276 * delayFloat;
    self->srt.yaw += state->unk278 * delayFloat;
    
    if (samples){        
        minimum = *samples[minimumIndex] + 7.0f;
        if (minimum < self->srt.transl.y) {
            self->speed.y = self->speed.y - 0.17f;
            return;
        }
        
        self->srt.transl.y = minimum;
        if (self->id == OBJ_Tumbleweed2 || self->id == OBJ_Tumbleweed2twig) {
            self->speed.y = 0.0f - (((f32)state->unk260 / rand_next(0x8C, 0xB4)) * (self->speed.y * 0.8f));
        } else {
            self->speed.y = 0.0f - (((f32) state->unk260 / rand_next(0x14, 0x28)) * (self->speed.y * 0.8f));
        }
        
        volume = self->speed.y * 32.0f;
        if (volume > MAX_VOLUME) {
            volume = MAX_VOLUME;
        }
        
        //Noises when bouncing
        if (volume > 0x10) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_5F6, volume, 0, 0, 0, 0);
            if (rand_next(0, 5) == 0) {
                gDLL_6_AMSFX->vtbl->play_sound(self, rand_next(SOUND_614, SOUND_615), volume, 0, 0, 0, 0);
            }
        }
    }
}
#endif

// offset: 0x1D64 | func: 17
s32 dll_227_func_1D64(Object* self) {
    u8 pad[4];
    Object* player;
    u8 pad2[4];
    u8 random2;
    u32 temp_v0;
    TumbleweedState* state;

    state = self->state;
    player = get_player();
    if (state->unk298 == 0) {
        temp_v0 = dll_227_func_1FA0(self);
        state->unk298 = temp_v0;
        if (temp_v0 & 0xFF) {
            state->unk299 = 1;
        }
        self->unk0xaf &= 0xFFF7;
        return 1;
    }
    
    state->unk26C = state->unk26C - delayFloat;
    if (state->unk26C < 0.0f) {
        //Squeaking (and growing in size temporarily)
        state->unk26C = rand_next(0x78, 0xF0);
        temp_v0 = rand_next(SOUND_614, SOUND_615);
        random2 = rand_next(90, 100);
        gDLL_6_AMSFX->vtbl->play_sound(self, temp_v0, random2, 0, 0, 0, 0);
        self->srt.scale = 0.2f;
    } else {
        self->srt.scale = 0.15f;
    }
    
    self->unk0xaf |= 8;
    state->unk268 = 0.0f;
    if (get_masked_button_presses(0) & 0x8000) {
        set_button_mask(0, 0x8000);
        state->unk299 = 0;
    }
    if (self->unk_0xe0 == 1) {
        state->unk298 = 2;
    }
    if ((state->unk298 == 2) && (self->unk_0xe0 == 0)) {
        state->unk298 = 0;
        state->unk299 = 0;
    }
    if (state->unk299 != 0) {
        obj_send_mesg(player, 0x100008, self, (void*)((state->unk296 << 0x10) | (state->unk294 & 0xFFFF)));
    }
    return 0;
}

// offset: 0x1FA0 | func: 18
s32 dll_227_func_1FA0(Object* self) {
    TumbleweedState* state = self->state;
    s32 returnVal;

    state = self->state;
    returnVal = 0;
    if (self->unk0xaf & 1 && self->unk_0xe0 == 0) {
        state->unk294 = 0;
        set_button_mask(0, 0x8000);
        state->unk26C = rand_next(0x78, 0xF0);
        returnVal = 1;
    }
    return returnVal;
}

// offset: 0x2038 | func: 19
void dll_227_func_2038(Object* self) {
    TumbleweedCreateInfo* createInfo;

    switch (self->id) {
        case OBJ_Tumbleweed1:
            createInfo = obj_alloc_create_info(0x20, OBJ_Tumbleweed1twig);
            break;
        case OBJ_Tumbleweed2:
            createInfo = obj_alloc_create_info(0x20, OBJ_Tumbleweed2twig);
            break;
        case OBJ_Tumbleweed3:
            createInfo = obj_alloc_create_info(0x20, OBJ_Tumbleweed3twig);
            break;
    }
    
    createInfo->base.x = self->srt.transl.x;
    createInfo->base.y = self->srt.transl.y;
    createInfo->base.z = self->srt.transl.z;
    createInfo->base.fadeDistance = 0xFF;
    createInfo->base.loadParamA = 0x20;
    createInfo->base.loadParamB = 2;
    createInfo->roll = self->srt.roll;
    createInfo->pitch = self->srt.pitch;
    createInfo->yaw = self->srt.yaw;
    createInfo->unk1B = 0;
    createInfo->unk1C = 64.0f;
    obj_create((ObjCreateInfo*)createInfo, 5, self->mapID, -1, self->parent);
}
