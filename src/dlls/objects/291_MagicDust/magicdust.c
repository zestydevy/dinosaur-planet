#include "common.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/math.h"
#include "sys/objmsg.h"
#include "dlls/engine/17_partfx.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/291_magicdust.h"

#define DURATION_DELAY_FALL 60.0f
#define DURATION_LIFETIME 1800.0f
#define DURATION_VANISH 180.0f

/*0x0*/ static Vec3f dCollisionPoint = VEC3F(0.0f, 0.0f, 0.0f);

static void MagicDust_collect(Object* self, Object* player, MagicDust_Data* objData);

// offset: 0x0 | ctor
void MagicDust_ctor(void *dll) { }

// offset: 0xC | dtor
void MagicDust_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void MagicDust_setup(Object* self, MagicDust_Setup* objSetup, s32 arg2) {
    s32 angle;
    f32 speed;
    MagicDust_Data* objData = self->data;
    f32 attractDuration;
    u8 colliderArg;
    Object* player;

    colliderArg = 3;

    angle = rand_next(0, 0xFFFF);
    speed = (rand_next(39, 44) / 100.0f);
    self->velocity.x = fsin16_precise(angle) * speed;
    self->velocity.z = fcos16_precise(angle) * speed;
    self->velocity.y = rand_next(40, 50) / 50.0f;

    //Set up special modes
    if (objSetup->mode == MagicDust_MODE_Delay_Fall) {
        objData->flags |= MagicDust_FLAG_Delay_Fall;
        self->opacity = 1;
    } else if (objSetup->mode == MagicDust_MODE_Towards_Player) {
        objData->flags |= MagicDust_FLAG_Delay_Fall;
        self->opacity = 1;

        func_800267A4(self);

        //Set initial speed so gem moves towards the player (arriving in 2 seconds)
        attractDuration = 120.0f;
        if (attractDuration != 0.0f) {
            player = get_player(); //@bug?: missing NULL check
            self->velocity.x = (player->srt.transl.x - self->srt.transl.x) / attractDuration;
            self->velocity.y = (player->srt.transl.y - self->srt.transl.y) / attractDuration;
            self->velocity.z = (player->srt.transl.z - self->srt.transl.z) / attractDuration;
        }
    }

    //Set up model index (even though MagicDustSmall/Mid/Large/Huge only have 1 model)
    self->modelInstIdx = objSetup->modelIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }

    //Set up shadow
    if (self->shadow != NULL) {
        self->shadow->distFadeMaxOpacity = 100;
        self->shadow->distFadeMinOpacity = 150;
        self->shadow->maxDistScale *= 2.0f;
    }

    //Set up the crystal's various IDs
    switch (self->id) {
    case OBJ_MagicDustSmall:
        objData->fxIDGlow = PARTICLE_54D;
        objData->fxIDSparkle = PARTICLE_549;
        objData->soundID = SOUND_B2A_Magic_Gliss_Small;
        objData->animObjectID = OBJ_MagicDustSmallA;
        break;
    case OBJ_MagicDustMid:
        objData->fxIDGlow = PARTICLE_54E;
        objData->fxIDSparkle = PARTICLE_54A;
        objData->soundID = SOUND_B6B_Magic_Gliss_Mid;
        objData->animObjectID = OBJ_MagicDustMidAni;
        break;
    case OBJ_MagicDustLarge:
        objData->fxIDGlow = PARTICLE_54F;
        objData->fxIDSparkle = PARTICLE_54B;
        objData->soundID = SOUND_B6C_Magic_Gliss_Large;
        objData->animObjectID = OBJ_MagicDustLargeA;
        break;
    default:
    case OBJ_MagicDustHuge:
        objData->fxIDGlow = PARTICLE_550;
        objData->fxIDSparkle = PARTICLE_54C;
        objData->soundID = SOUND_B6D_Magic_Gliss_Huge;
        objData->animObjectID = OBJ_MagicDustHugeAn;
        break;
    }

    //Set up collision
    objData->collisionRadius = self->objhitInfo->unk52;
    if (self->srt.flags & OBJFLAG_OWNS_SETUP) {
        gDLL_27->vtbl->init(&objData->collision, 
            DLL27FLAG_NONE, 
            DLL27FLAG_1 | DLL27FLAG_2 | DLL27FLAG_4 | DLL27FLAG_40000, 
            DLL27MODE_DISABLED
        );
        gDLL_27->vtbl->setup_terrain_collider(
            &objData->collision, 
            1, 
            &dCollisionPoint, 
            &objData->collisionRadius, 
            &colliderArg
        );
        gDLL_27->vtbl->reset(self, &objData->collision);
    }

    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;

    //Set up timer
    if (objData->flags & MagicDust_FLAG_Delay_Fall) {
        objData->timer = DURATION_DELAY_FALL;
    } else {
        objData->flags |= MagicDust_FLAG_Fell;
        objData->timer = DURATION_LIFETIME;
    }
}

// offset: 0x3F0 | func: 1 | export: 1
void MagicDust_control(Object* self) {
    Object *player;
    MagicDust_Data *objData;
    f32 playerDistance;
    DLL27_Data *collision;
    Vec3f negativeS;
    Vec3f vReflect;
    Vec3f nSurface;
    f32 absSpeed;
    f32 pad[2];
    f32 volumeMultiplier;
    f32 distance;
    f32 coefficient;
    u8 i;
    u8 fxParam;
    u32 outMesgID;
    
    player = get_player();
    objData = self->data;
    collision = &objData->collision;
    
    //Wait for the "You have picked up a… MAGIC GEM!" sequence to end (if it's active)
    if (objData->flags & MagicDust_FLAG_Tutorial_Sequence) {
        while (obj_recv_mesg(self, &outMesgID, NULL, NULL)) {
            if (outMesgID == 0x7000B) {
                MagicDust_collect(self, player, objData);
                objData->flags &= ~MagicDust_FLAG_Tutorial_Sequence;
            }
        }
        
        if (objData->flags & MagicDust_FLAG_Tutorial_Sequence) {
            return;
        }
    }
    
    //Create glow effects
    if (self->unkDC == 0) {
        fxParam = 0;
        gDLL_17_partfx->vtbl->spawn(self, objData->fxIDGlow, NULL, 0x10002, -1, &fxParam);
        fxParam = 1;
        gDLL_17_partfx->vtbl->spawn(self, objData->fxIDGlow, NULL, 0x10002, -1, &fxParam);
        fxParam = 2;
        gDLL_17_partfx->vtbl->spawn(self, objData->fxIDGlow, NULL, 0x10002, -1, &fxParam);
        self->unkDC = 1;
    }
    
    //Handle behaviour when at rest
    if (objData->flags & MagicDust_FLAG_On_Ground) {
        //Spin
        self->srt.yaw += gUpdateRate << 8;

        //Play a ringing sound at intervals
        objData->soundTimer -= gUpdateRate;
        if (objData->soundTimer < 0) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_BA1_MagicDust_Twinkle, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->soundTimer = rand_next(240, 300);
        }
    }
    
    //Handle motion
    if (self->srt.flags & OBJFLAG_OWNS_SETUP) {
        //Do nothing when parented to MagicPlant
        if (self->unkC4 != NULL) {
            self->shadow->flags |= OBJ_SHADOW_FLAG_FADE_OUT;
            gDLL_27->vtbl->reset(self, collision);
            return;
        }

        self->shadow->flags &= ~OBJ_SHADOW_FLAG_FADE_OUT;
        collision->mode = 1;

        //Apply drag and gravity when falling
        if (!(objData->flags & (MagicDust_FLAG_Delay_Fall | MagicDust_FLAG_On_Ground))) {
            self->velocity.x *= 0.99f;
            self->velocity.z *= 0.99f;
            self->velocity.y -= (0.1f * gUpdateRateF);
        }
        
        //Decrement timer
        objData->timer -= gUpdateRateF;

        //FALL DELAY
        if (objData->flags & MagicDust_FLAG_Delay_Fall) {
            //Set "fell" flag when fall delay timer's up
            if (objData->timer <= 0.0f) {
                objData->flags &= ~MagicDust_FLAG_Delay_Fall;
                objData->flags |= MagicDust_FLAG_Fell;
                objData->timer = DURATION_LIFETIME;
                self->opacity = OBJECT_OPACITY_MAX;
            }
            
            //Create sparkles (if not parented)
            if (self->parent == NULL) {
                gDLL_17_partfx->vtbl->spawn(self, objData->fxIDSparkle, NULL, 1, -1, NULL);
                gDLL_17_partfx->vtbl->spawn(self, objData->fxIDSparkle, NULL, 1, -1, NULL);
            }

        //FALLING/ON GROUND
        } else if (objData->flags & MagicDust_FLAG_Fell) {
            //Vanish when the gem's lifetime is up
            if (objData->timer <= 0.0f) {
                objData->flags &= ~MagicDust_FLAG_Fell;
                objData->flags |= MagicDust_FLAG_Vanish;
                objData->timer = DURATION_VANISH;
                gDLL_13_Expgfx->vtbl->func5(self);

                //Create lots of sparkles
                if (self->parent == NULL) {
                    for (i = 30; i != 0; i--) {
                        gDLL_17_partfx->vtbl->spawn(self, objData->fxIDSparkle, NULL, 1, -1, &i);
                    }
                }

                self->opacity = 1;
                gDLL_6_AMSFX->vtbl->play(self, SOUND_B2B_Magic_Chime, MAX_VOLUME, NULL, NULL, 0, NULL);
            }

            //Move
            obj_move(self, 
                self->velocity.x * gUpdateRateF, 
                self->velocity.y * gUpdateRateF, 
                self->velocity.z * gUpdateRateF
            );
        } else {
            //Destroy self when vanish timer's up
            if (objData->timer <= 0.0f) {
                if (self->srt.flags & OBJFLAG_OWNS_SETUP) {
                    obj_destroy_object(self);
                } else {
                    obj_free_tick(self);
                    self->srt.flags |= OBJFLAG_INVISIBLE;
                }
            }
            return;
        }
        
        //Handle falling
        if (!(objData->flags & (MagicDust_FLAG_Delay_Fall | MagicDust_FLAG_On_Ground))) {
            gDLL_27->vtbl->func_1E8(self, collision, gUpdateRateF);
            gDLL_27->vtbl->func_5A8(self, collision);
            gDLL_27->vtbl->func_624(self, collision, gUpdateRateF);

            //Bounce when colliding with ground
            if (collision->unk25D != 0) {
                negativeS.x = -self->velocity.x;
                negativeS.y = -self->velocity.y;
                negativeS.z = -self->velocity.z;
                absSpeed = VECTOR_MAGNITUDE(negativeS);

                //Play a "ting!" sound scaling volume by speed
                volumeMultiplier = absSpeed;
                if (absSpeed > 0.5f) {
                    if (absSpeed > 2.0f) {
                        volumeMultiplier = 2.0f;
                    }
                    gDLL_6_AMSFX->vtbl->play(self, SOUND_66E_Ting, volumeMultiplier * 32.0f, NULL, NULL, 0, NULL);
                }
                
                //Reflect speed vector off surface normal (@bug: result unused?)
                {
                    //R = V - 2*(V ⋅ N)*N

                    //Get negative velocity unit vector
                    if (absSpeed != 0.0f) {
                        coefficient = 1.0f / absSpeed;
                        negativeS.f[0] *= coefficient;
                        negativeS.f[1] *= coefficient;
                        negativeS.f[2] *= coefficient;
                    }

                    //Get dot product between surface normal and negative velocity unit vector
                    nSurface.f[0] = collision->unk68.unk0[0].f[0];
                    nSurface.f[1] = collision->unk68.unk0[0].f[1];
                    nSurface.f[2] = collision->unk68.unk0[0].f[2];
                    coefficient = 2.0f * DOT_PRODUCT(negativeS, nSurface);

                    //Get reflected bounce vector
                    vReflect.f[0] = nSurface.f[0] * coefficient;
                    vReflect.f[1] = nSurface.f[1] * coefficient;
                    vReflect.f[2] = nSurface.f[2] * coefficient;
                    vReflect.f[0] -= negativeS.f[0];
                    vReflect.f[1] -= negativeS.f[1];
                    vReflect.f[2] -= negativeS.f[2];
                }
                
                //Bounce upwards if the ground is mostly level
                if (nSurface.f[1] >= 0.707f) {
                    self->velocity.y = -self->velocity.y;
                    self->velocity.y *= 0.85f;

                    //Stop after 6 bounces
                    objData->bounces++;
                    if (objData->bounces >= 6) {
                        objData->flags |= MagicDust_FLAG_On_Ground;
                        self->velocity.x = 0;
                        self->velocity.y = 0;
                        self->velocity.z = 0;
                    }
                //Otherwise bounce laterally and lose vertical momentum
                } else {
                    self->velocity.x = -self->velocity.x;
                    self->velocity.z = -self->velocity.z;
                    self->velocity.x *= 0.85f;
                    self->velocity.z *= 0.85f;
                }
            }
        }
    }
    
    //Collect the gem when close
    distance = self->srt.transl.y - player->srt.transl.y;
    if (distance < 0.0f) {
        distance = -distance;
    } 
    if (distance < 20.0f) {
        playerDistance = vec3_distance_xz_squared(&self->globalPosition, &player->globalPosition);
        distance = objData->collisionRadius + 8.0f;
        if (playerDistance < SQ(distance)) {
            //Display a tutorial box when the player first collects a Magic Crystal
            if (main_get_bits(BIT_Tutorial_Magic_Crystal) == 0) {
                gDLL_3_Animation->vtbl->func30(objData->animObjectID, NULL, 0);
                outMesgID = 0;
                obj_send_mesg(player, 0x7000A, self, NULL);
                main_set_bits(BIT_Tutorial_Magic_Crystal, 1);
                objData->flags |= MagicDust_FLAG_Tutorial_Sequence;
            } else {
                MagicDust_collect(self, player, objData);
            }
            
            self->opacity = 1;
            gDLL_13_Expgfx->vtbl->func5(self);
            for (i = 10; i != 0; i--) {
                gDLL_17_partfx->vtbl->spawn(self, objData->fxIDSparkle, NULL, 1, -1, NULL);
            }
            
            gDLL_6_AMSFX->vtbl->play(self, objData->soundID, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    }
}

// offset: 0xEFC | func: 2 | export: 2
void MagicDust_update(Object *self) { }

// offset: 0xF08 | func: 3 | export: 3
void MagicDust_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    //When the MagicDust gem is attached to a MagicPlant, the plant handles drawing the gem instead
    if (visibility && (self->unkC4 == NULL)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xF68 | func: 4 | export: 4
void MagicDust_free(Object *self, s32 a1) { }

// offset: 0xF78 | func: 5 | export: 5
u32 MagicDust_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xF88 | func: 6 | export: 6
u32 MagicDust_get_data_size(Object *self, u32 a1) {
    return sizeof(MagicDust_Data);
}

// offset: 0xF9C | func: 7
void MagicDust_collect(Object* self, Object* player, MagicDust_Data* objData) {
    CollectableDef* collectDef;
    s8 magicRestored;

    collectDef = self->def->collectableDef;
    magicRestored = collectDef->amountRestored;
    ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, magicRestored);
    
    objData->timer = DURATION_VANISH;
    objData->flags &= ~(MagicDust_FLAG_Fell | MagicDust_FLAG_Delay_Fall);
    objData->flags |= MagicDust_FLAG_Vanish;
}
