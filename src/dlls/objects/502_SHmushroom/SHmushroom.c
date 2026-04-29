#include "common.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "macros.h"
#include "sys/gfx/model.h"
#include "sys/math.h"
#include "sys/objanim.h"
#include "sys/objmsg.h"
#include "sys/curves.h"
#include "sys/objtype.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/315_sidefoodbag.h"
#include "dlls/objects/713_DRearthwalk.h"

#include "dlls/objects/502_SHmushroom.h"

/*0x0*/ static s16 dStateModAnimIDs[] = {
	SHmushroom_MODANIM_0_Idle_LOOP,         //SHmushroom_STATE_0_Idle
	SHmushroom_MODANIM_1_Jump,              //SHmushroom_STATE_1_Jump,
	SHmushroom_MODANIM_6_Dancing_LOOP,      //SHmushroom_STATE_2_Collected
	SHmushroom_MODANIM_2_Look_Intro,        //SHmushroom_STATE_3_Alert_Intro
	SHmushroom_MODANIM_3_Look_Idle_LOOP,    //SHmushroom_STATE_4_Alert
	SHmushroom_MODANIM_4_Look_Hop_LOOP,     //SHmushroom_STATE_5_Surprised
	SHmushroom_MODANIM_0_Idle_LOOP,         //SHmushroom_STATE_6_Trapped
	SHmushroom_MODANIM_5_Sigh,              //SHmushroom_STATE_7_Relieved_Sigh
	SHmushroom_MODANIM_6_Dancing_LOOP,      //SHmushroom_STATE_8_Hidden
	SHmushroom_MODANIM_7_Stunned_LOOP       //SHmushroom_STATE_9_Stunned
};
/*0x14*/ static f32 dStateAnimSpeeds[] = {
	0.005f,   //SHmushroom_STATE_0_Idle
	0.01f,    //SHmushroom_STATE_1_Jump,
	0.005f,   //SHmushroom_STATE_2_Collected
	0.01f,    //SHmushroom_STATE_3_Alert_Intro
	0.01f,    //SHmushroom_STATE_4_Alert
	0.015f,   //SHmushroom_STATE_5_Surprised
	0.005f,   //SHmushroom_STATE_6_Trapped
	0.01f,    //SHmushroom_STATE_7_Relieved_Sigh
	0.005f,   //SHmushroom_STATE_8_Hidden
	0.012f,   //SHmushroom_STATE_9_Stunned
};

static s16 SHmushroom_flee_from_player(Object* self, Object* fleeingFrom, SHmushroom_Data* objData, f32 distance);
static s16 SHmushroom_flee_along_curve(Object* self, Object* fleeingFrom, SHmushroom_Data* objData, f32 distance);
static void SHmushroom_tick_state_machine(Object* self, SHmushroom_Data* objData, SHmushroom_Setup* objSetup);

// offset: 0x0 | ctor
void SHmushroom_ctor(void *dll) { }

// offset: 0xC | dtor
void SHmushroom_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHmushroom_setup(Object* self, SHmushroom_Setup* setup, s32 arg2) {
	SHmushroom_Data* objData;
	UnkFunc_80024108Struct animInfo;
	s32 curveEndpoint;
	Object* player;
	ObjectShadow* shadow;
	f32 distanceToPlayer;

	objData = self->data;
	curveEndpoint = 25; //Matches unk18 on the initial curve node for SwapStone Hollow's lily pond mushroom (uID 0x3081c)
	player = get_player();
	self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED);

	if (main_get_bits(setup->gamebitCollected)) {
		objData->state = SHmushroom_STATE_8_Hidden;
		self->objhitInfo->unk58 &= ~1;
		self->srt.flags |= OBJFLAG_INVISIBLE;
	}

	//Choose model index
	self->modelInstIdx = setup->modelIdx;
	if (self->modelInstIdx >= self->def->numModels) {
		self->modelInstIdx = SHmushroom_MODEL_0_Blue_Mushroom;
	}

	//Set up shadow
	shadow = self->shadow;
	shadow->maxDistScale = 2.0f * shadow->scale;
	shadow = self->shadow;
	shadow->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR;

	objData->jumpSpeedMultiplier = setup->jumpSpeed / 255.0f;
	objData->hopSpeedMultiplier = (setup->hopSpeed / 255.0f) * 0.2f;

	self->srt.scale = self->def->scale;

	//Get root motion speed from jump animation?
	func_80023D30(self, SHmushroom_MODANIM_1_Jump, 0.0f, 0);
	func_80024108(self, 1.0f, 1.0f, &animInfo);
	objData->jumpSpeed = animInfo.unk0[0];
	if (objData->jumpSpeed < 0.0f) {
		objData->jumpSpeed = -objData->jumpSpeed;
	}
	objData->jumpSpeed *= objData->jumpSpeedMultiplier;
	objData->jumpSpeed += 20.0f;

	//Get root motion speed from hop animation?
	func_80023D30(self, SHmushroom_MODANIM_4_Look_Hop_LOOP, 0.0f, 0);
	func_80024108(self, 1.0f, 1.0f, &animInfo);
	objData->hopSpeed = animInfo.unk0[2];
	if (objData->hopSpeed < 0.0f) {
		objData->hopSpeed = -objData->hopSpeed;
	}
	objData->hopSpeed += 20.0f;

	//Set up message queue
	obj_init_mesg_queue(self, 1);

	//Optionally set the mushroom to follow curves (only affects specific mushrooms)
	if ((setup->index == 4) || //White Mushroom around lily pond in SwapStone Hollow well
		(setup->index == 5)    //Unknown
	) {
		objData->flags |= SHmushroom_FLAG_Follow_Curve;
		gDLL_26_Curves->vtbl->func_4288(&objData->curves, self, 1000.0f, &curveEndpoint, -1);
		self->srt.transl.x = objData->curves.unk68.x;
		self->srt.transl.z = objData->curves.unk68.z;
	}

	objData->curvesDelta = 5.0f;

	//Set initial pursuer distance
	if (player != NULL) {
		distanceToPlayer = vec3_distance(&player->globalPosition, &self->globalPosition);
		objData->prevPursuerDistance = distanceToPlayer;
		objData->pursuerDistance = distanceToPlayer;
	} else {
		objData->pursuerDistance = 200.0f;
		objData->prevPursuerDistance = 200.0f;
	}

	obj_add_object_type(self, OBJTYPE_51);

	//Set up inventory gamebit (value incremented when collected)
	if (self->modelInstIdx == SHmushroom_MODEL_0_Blue_Mushroom) {
		objData->gamebitInventory = BIT_Inventory_Blue_Mushrooms;
	} else {
		objData->gamebitInventory = BIT_Inventory_White_Mushrooms;
	}

	func_80023D08(self, self->modelInstIdx);
}

// offset: 0x3A8 | func: 1 | export: 1
void SHmushroom_control(Object* self) {
	Object* dinoFoodBag;
	SHmushroom_Setup* objSetup;
	s32 i;
	Object* sidekick;
	Object* hitBy;
	s32 pad;
	f32 playerDistanceSquared;
	s32 count;
	Func_80057F1C_Struct** spBC;
	SHmushroom_Data* objData;
	f32 sidekickDistanceSquared;
	Object* player;
	u32 outMesgID;
	Func_80059C40_Struct sp58;
	s32 temp;

	objData = self->data;
	objSetup = (SHmushroom_Setup*)self->setup;
	player = get_player();
	sidekick = get_sidekick();

	if (objData->state == SHmushroom_STATE_8_Hidden) {

		// Increment the player's inventory count when a message is received
		/* @bug:
		   If the mushroom unloads before the message is received, the mushroom will be missed permanently!
		   This can cause a softlock while saving Queen EarthWalker in SwapStone Hollow.
		*/
		while (obj_recv_mesg(self, &outMesgID, NULL, NULL)) {
			if (outMesgID == 0x7000B) {
				if (self->modelInstIdx == SHmushroom_MODEL_0_Blue_Mushroom) {
					dinoFoodBag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 16);

					//If the player has a sidekick foodbag, store Blue Mushrooms there
					if (((DLL_315_SideFoodbag*)dinoFoodBag->dll)->vtbl->is_obtained(dinoFoodBag)) {
						((DLL_315_SideFoodbag*)dinoFoodBag->dll)->vtbl->collect_food(dinoFoodBag, SIDEFOOD_Blue_Mushrooms);

					//Otherwise store Blue Mushrooms directly in the inventory
					} else {
						main_increment_bits(objData->gamebitInventory);
					}
				} else {
					//Other mushroom types (White Mushrooms) are always stored directly in the inventory
					main_increment_bits(objData->gamebitInventory);
				}
			}
		}
		return;
	}

	//Handle being chased by player/sidekick (get distance to whoever's nearest)
	objData->prevPursuerDistance = objData->pursuerDistance;

	playerDistanceSquared = vec3_distance_squared(&player->globalPosition, &self->globalPosition);
	if (sidekick == NULL) {
		objData->pursuerDistance = sqrtf(playerDistanceSquared);
	} else {
		sidekickDistanceSquared = vec3_distance_squared(&sidekick->globalPosition, &self->globalPosition);
		if (playerDistanceSquared < sidekickDistanceSquared) {
			objData->pursuerDistance = sqrtf(playerDistanceSquared);
		} else {
			objData->pursuerDistance = sqrtf(sidekickDistanceSquared);
		}

		//Show Find command option when sidekick nearby (@bug: can still show up while mushroom's invisible)
		if (objData->pursuerDistance < objSetup->alertRange) {
			((DLL_ISidekick*)sidekick->dll)->vtbl->func14(sidekick, Sidekick_Command_INDEX_1_Find);
		}
	}

	//React to attacks
	if (func_80025F40(self, &hitBy, NULL, NULL) != 0) {
		gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_744_Mushroom_Hit, MAX_VOLUME, NULL, NULL, 0, NULL);

		//Get eaten when attacked by an EarthWalker
		if (hitBy->id == OBJ_DR_EarthWarrior) {
			objData->state = SHmushroom_STATE_8_Hidden;
			((DLL_713_DR_EarthWarrior*)hitBy->dll)->vtbl->func20(hitBy, 1, self);
			self->srt.flags |= OBJFLAG_INVISIBLE;
			func_800267A4(self);
		} else {
			objData->flags |= SHmushroom_FLAG_Hurt;
		}
	}

	switch (objSetup->index) {
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		break;
	}

	SHmushroom_tick_state_machine(self, objData, objSetup);

	if (objData->flags & SHmushroom_FLAG_Moving) {
		//Snap Y to ground
		count = func_80057F1C(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &spBC, 0, 0);
		for (i = 0; i < count; i++) {
			if (spBC[i]->unk0[0] < (self->srt.transl.y + 10.0f)) {
				self->srt.transl.y = spBC[i]->unk0[0];
				break;
			}
		}

		/* Stop fleeing under some circumstances? (TODO: investigate!)

		   Should only affect curve-following mushroom around lily pond in SwapStone Hollow - Well.

		   Considering the "MUSHROOM: trapped" string, maybe the mushroom's intended to stop fleeing
		   if its path around the pond is blocked by dropping the cave stalactites?
		*/
		temp = func_80059C40(&self->prevLocalPosition, &self->srt.transl, 6.0f, 2, &sp58, self, 8, -1, 0xFF, 0x14);
		if ((objSetup->index == 4) && (temp) && (sp58.unk50 == 0xD)) {
			objData->flags |= SHmushroom_FLAG_No_Fleeing;
			STUBBED_PRINTF("MUSHROOM: trapped!!!!\n");
		}
	}
}

// offset: 0x848 | func: 2 | export: 2
void SHmushroom_update(Object *self) { }

// offset: 0x854 | func: 3 | export: 3
void SHmushroom_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x86C | func: 4 | export: 4
void SHmushroom_free(Object* self, s32 a1) {
	SHmushroom_Data* objData = self->data;

	obj_free_object_type(self, OBJTYPE_51);

	//Stop sound loop
	if (objData->soundHandleStun != 0) {
		gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandleStun);
		objData->soundHandleStun = 0;
	}
}

// offset: 0x8E4 | func: 5 | export: 5
u32 SHmushroom_get_model_flags(Object* self) {
	SHmushroom_Setup* objSetup = (SHmushroom_Setup*)self->setup;
	return MODFLAGS_MODEL_INDEX(objSetup->modelIdx) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x8FC | func: 6 | export: 6
u32 SHmushroom_get_data_size(Object *self, u32 a1) {
	return sizeof(SHmushroom_Data);
}

// offset: 0x910 | func: 7
/**
  * Figures out what direction the mushroom should flee in when escaping from player/sidekick (returns an angle).
  *
  * Seems to flee away along the line between the mushroom and the threat, or in a different direction if a wall/obstacle blocks that path.
  */
static s16 SHmushroom_flee_from_player(Object* self, Object* fleeingFrom, SHmushroom_Data* objData, f32 distance) {
	s16 angle;
	s32 _pad1;
	s16 angle1;
	s16 angle2;
	s32 i;
	f32 dz;
	f32 dx;
	f32 sp90[4];
	f32 sp80[4];
	Vec3f v;
	s32 _pad2[3];

	dx = self->srt.transl.x - fleeingFrom->srt.transl.x;
	dz = self->srt.transl.z - fleeingFrom->srt.transl.z;

	angle = arctan2_f(-dx, -dz);
	dz = fsin16_precise(angle);
	dx = fcos16_precise(angle);

	v.x = self->srt.transl.x - (distance * dz);
	v.y = self->srt.transl.y;
	v.z = self->srt.transl.z - (distance * dx);

	if (func_80059C40(&self->srt.transl, &v, 0.1f, 3, NULL, self, 8, -1, 0xFF, 0)) {
		angle1 = angle;
		angle2 = angle;

		sp90[2] = sp90[3] = dz;
		sp80[2] = fsin16_precise( M_20_DEGREES);
		sp80[3] = fsin16_precise(-M_20_DEGREES);
		sp90[0] = sp90[1] = dx;
		sp80[0] = fcos16_precise( M_20_DEGREES);
		sp80[1] = fcos16_precise(-M_20_DEGREES);

		for (i = 0; i < 8; i++) {
			dummy:

			dz = (sp90[2] * sp80[0]) + (sp90[0] * sp80[2]);
			sp90[0] = (sp90[0] * sp80[0]) - (sp90[2] * sp80[2]);
			sp90[2] = dz;
			v.x = self->srt.transl.x - (sp90[2] * distance);
			v.z = self->srt.transl.z - (sp90[0] * distance);

			angle1 += M_20_DEGREES;
			if (func_80059C40(&self->srt.transl, &v, 0.1f, 1, NULL, self, 8, -1, 0xFF, 0) == 0) {
				return angle1;
			}

			dz = (sp90[3] * sp80[1]) + (sp90[1] * sp80[3]);
			sp90[1] = (sp90[1] * sp80[1]) - (sp90[3] * sp80[3]);
			sp90[3] = dz;
			v.x = self->srt.transl.x - (sp90[3] * distance);
			v.z = self->srt.transl.z - (sp90[1] * distance);

			angle2 -= M_20_DEGREES;
			if (func_80059C40(&self->srt.transl, &v, 0.1f, 1, NULL, self, 8, -1, 0xFF, 0) == 0) {
				return angle2;
			}
		}
	}

	return angle;
}

// offset: 0xC70 | func: 8
/**
  * Figures out what direction the mushroom should flee in when escaping from player/sidekick (returns an angle).
  *
  * Flees along a curve network's path (e.g. the mushroom by the lily pond in SwapStone Hollow's well).
  */
s16 SHmushroom_flee_along_curve(Object* self, Object* fleeingFrom, SHmushroom_Data* objData, f32 distance) {
	f32 dx;
	f32 dz;

	while (TRUE) {
		dx = objData->curves.unk68.x - self->srt.transl.x;
		dz = objData->curves.unk68.z - self->srt.transl.z;

		if ((SQ(dx) + SQ(dz)) < SQ(distance)) {
			if ((func_800053B0(&objData->curves, objData->curvesDelta) || objData->curves.unk10)) {
				if (gDLL_26_Curves->vtbl->func_4704(&objData->curves) != 0) {
					STUBBED_PRINTF("MUSHROOM ERROR: no node found\n");
				}
			}
		} else {
			break;
		}
	}

	return (s32)arctan2_f(-dx, -dz);
}

// offset: 0xD74 | func: 9
static void SHmushroom_tick_state_machine(Object* self, SHmushroom_Data* objData, SHmushroom_Setup* objSetup) {
	f32 speed;
	f32 dx;
	f32 dz;
	UnkFunc_80024108Struct animInfo;
	SRT fxTransform;
	Object* player;

	player = get_player();

	objData->flags &= ~SHmushroom_FLAG_Moving;

	if (objData->flags & SHmushroom_FLAG_No_Fleeing) {
		objData->state = SHmushroom_STATE_6_Trapped;
	}

	//Get player/sidekick's approach speed
	speed = (objData->prevPursuerDistance - objData->pursuerDistance) / gUpdateRateF;

	self->unkAF |= ARROW_FLAG_8_No_Targetting;

	switch (objData->state) {
	case SHmushroom_STATE_0_Idle:
		if (objData->flags & SHmushroom_FLAG_Hurt) {
			objData->state = SHmushroom_STATE_9_Stunned;
		} else {
			//Flee if the player/sidekick are very close
			if (objData->pursuerDistance < objSetup->fleeRange) {
				if (objData->flags & SHmushroom_FLAG_Follow_Curve) {
					objData->fleeAngle = SHmushroom_flee_along_curve(self, player, objData, objData->jumpSpeed);
				} else {
					objData->fleeAngle = SHmushroom_flee_from_player(self, player, objData, objData->jumpSpeed);
				}

				objData->state = SHmushroom_STATE_1_Jump;
				gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_53C_Mushroom_Bounce, MAX_VOLUME, NULL, NULL, 0, NULL);
				self->srt.yaw = objData->fleeAngle - M_90_DEGREES;

			//Enter alert state if the player/sidekick are close
			} else {
				if (objData->pursuerDistance < objSetup->alertRange) {
					objData->state = SHmushroom_STATE_3_Alert_Intro;
				}
			}
		}
		break;
	case SHmushroom_STATE_1_Jump:
		if (objData->flags & SHmushroom_FLAG_Hurt) {
			objData->state = SHmushroom_STATE_9_Stunned;
		} else {
			//Fleeing
			objData->flags |= SHmushroom_FLAG_Moving;
			if (objData->flags & SHmushroom_FLAG_Animation_Finished) {
				objData->state = SHmushroom_STATE_0_Idle;
			}
		}
		break;
	case SHmushroom_STATE_3_Alert_Intro:
	case SHmushroom_STATE_7_Alert_Outro:
		if (objData->flags & SHmushroom_FLAG_Hurt) {
			objData->state = SHmushroom_STATE_9_Stunned;
			break;
		} else if (objData->flags & SHmushroom_FLAG_Animation_Finished) {
			//Alert: animate head tilting back, then advance state and stay in a poised loop
			if (objData->state == SHmushroom_STATE_3_Alert_Intro) {
				objData->state = SHmushroom_STATE_4_Alert;

			//Relieved: go back to idle when finished sighing
			} else {
				objData->state = SHmushroom_STATE_0_Idle;
			}
			break;
		}
	case SHmushroom_STATE_4_Alert:
		if (objData->flags & SHmushroom_FLAG_Hurt) {
			objData->state = SHmushroom_STATE_9_Stunned;
		} else {
			//Alert state: staying still in poised pose, facing towards threat
			dx = self->srt.transl.x - player->srt.transl.x;
			dz = self->srt.transl.z - player->srt.transl.z;
			self->srt.yaw = arctan2_f(-dx,-dz);

			//If pursuer backs off, play a little "phew!" animation
			if ((objSetup->alertRange + 10.0f) < objData->pursuerDistance) {
				objData->state = SHmushroom_STATE_7_Alert_Outro;
			} else {
				//Otherwise: try to flee
				if (objData->pursuerDistance < objSetup->fleeRange) {
					//Flee by jumping (if the player/sidekick are approaching quickly)
					if (speed >= 0.54f) {
						if (objData->flags & SHmushroom_FLAG_Follow_Curve) {
							objData->fleeAngle = SHmushroom_flee_along_curve(self, player, objData, objData->jumpSpeed);
						} else {
							objData->fleeAngle = SHmushroom_flee_from_player(self, player, objData, objData->jumpSpeed);
						}

						objData->state = SHmushroom_STATE_1_Jump;
						gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_53C_Mushroom_Bounce, MAX_VOLUME, NULL, NULL, 0, NULL);
						self->srt.yaw = objData->fleeAngle - M_90_DEGREES;

					//Hop in surprise (if the player/sidekick are sneaking up)
					} else {
						if (objData->flags & SHmushroom_FLAG_Follow_Curve) {
							objData->fleeAngle = SHmushroom_flee_along_curve(self, player, objData, objData->hopSpeed);
						} else {
							objData->fleeAngle = SHmushroom_flee_from_player(self, player, objData, objData->hopSpeed);
						}

						objData->state = SHmushroom_STATE_5_Surprised_Hop;
						gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_53C_Mushroom_Bounce, MAX_VOLUME, NULL, NULL, 0, NULL);
						self->srt.yaw = objData->fleeAngle;
					}
				}
			}
		}

		break;
	case SHmushroom_STATE_5_Surprised_Hop: //(NOTE: harder to enter this state at smoother framerates)
		//When hurt, finish playing the hop animation before advancing to stunned state
		if ((objData->flags & (SHmushroom_FLAG_Hurt | SHmushroom_FLAG_Animation_Finished)) == (SHmushroom_FLAG_Hurt | SHmushroom_FLAG_Animation_Finished)) {
			objData->state = SHmushroom_STATE_9_Stunned;
		}

		objData->flags |= SHmushroom_FLAG_Moving;

		//If threat backs off and hop finished, return to alert state
		if (((objSetup->fleeRange + 10.0f) < objData->pursuerDistance) && (objData->flags & SHmushroom_FLAG_Animation_Finished)) {
			objData->state = SHmushroom_STATE_4_Alert;

		//Otherwise, flee if the player/sidekick are no longer sneaking up
		} else if (speed >= 0.54f) {
			if (objData->flags & SHmushroom_FLAG_Follow_Curve) {
				objData->fleeAngle = SHmushroom_flee_along_curve(self, player, objData, objData->jumpSpeed);
			} else {
				objData->fleeAngle = SHmushroom_flee_from_player(self, player, objData, objData->jumpSpeed);
			}

			objData->state = SHmushroom_STATE_1_Jump;
			gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_53C_Mushroom_Bounce, MAX_VOLUME, NULL, NULL, 0, NULL);
			self->srt.yaw = objData->fleeAngle - M_90_DEGREES;
		}
		break;

	case SHmushroom_STATE_2_Collected:
		//NOTE: plays an unseen dancing animation, but the mushroom immediately becomes invisible
		if (objData->flags & SHmushroom_FLAG_Animation_Finished) {
			objData->state = SHmushroom_STATE_8_Hidden;
			self->srt.flags |= OBJFLAG_INVISIBLE;
			func_800267A4(self);
		}
		break;

	case SHmushroom_STATE_9_Stunned:
		//Start stunned sound loop
		if (objData->stunnedTimer <= 0) {
			gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_745_Mushroom_Stunned_Loop, MAX_VOLUME, &objData->soundHandleStun, NULL, 0, NULL);
			objData->stunnedTimer = rand_next(240, 300);
		}

		//Run down stun timer
		objData->stunnedTimer -= gUpdateRateF;

		//Return to idle once stun wears off
		if (objData->stunnedTimer <= 0) {
			gDLL_13_Expgfx->vtbl->func4(self);
			objData->state = SHmushroom_STATE_0_Idle;
			objData->flags &= ~SHmushroom_FLAG_Hurt;

			//Stop sound loop
			if (objData->soundHandleStun != 0) {
				gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandleStun);
				objData->soundHandleStun = 0;
			}

		//If still stunned, emit particles periodically
		} else {
			objData->stunFxTimer -= gUpdateRateF;
			if (objData->stunFxTimer <= 0) {
				fxTransform.transl.x = 10.0f;
				fxTransform.transl.y = 12.0f;
				gDLL_17_partfx->vtbl->spawn(self, PARTICLE_51D, &fxTransform, PARTFXFLAG_2, -1, NULL);
				objData->stunFxTimer = 20.0f;
			}

			self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
		}
		break;

	case SHmushroom_STATE_6_Trapped:
		//Won't flee, and can be collected immediately without stunning
		if (objData->flags & SHmushroom_FLAG_Hurt) {
			objData->state = SHmushroom_STATE_9_Stunned;
		}

		self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
		break;
	}

	//Handle player collecting mushroom
	if (self->unkAF & ARROW_FLAG_1_Interacted) {
		//Set the objectID for the item collection sequence, and set a gamebit
		if ((self->modelInstIdx == SHmushroom_MODEL_0_Blue_Mushroom) &&
			(main_get_bits(BIT_Tutorial_Collected_Blue_Mushroom_Assigned_AnimObj) == FALSE)
		) {
			gDLL_3_Animation->vtbl->func30(OBJ_SHmushroomanim, NULL, 0);
			main_set_bits(BIT_Tutorial_Collected_Blue_Mushroom_Assigned_AnimObj, TRUE);
		}

		/* Send message to player, displaying Blue Mushroom's tutorial box
		   NOTE: uses different gamebitID to the above one, which sets the tutorial sequence's animObj

		   @bug: can appear when collecting White Mushrooms before Blue Mushrooms (with no anim object set)
		*/
		obj_send_mesg(player,
			0x7000A,
			self,
			(void*)BIT_Tutorial_Collected_Blue_Mushroom
		);

		//Set this mushroom's collection gamebit, so it won't reappear
		if (objSetup->gamebitCollected != NO_GAMEBIT) {
			main_set_bits(objSetup->gamebitCollected, TRUE);
		}

		self->unkAF |= ARROW_FLAG_8_No_Targetting;

		objData->state = SHmushroom_STATE_2_Collected;

		//Stop sound loop
		if (objData->soundHandleStun != 0) {
			gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandleStun);
			objData->soundHandleStun = 0;
		}
	}

	//Change animation when needed
	if (self->curModAnimId != dStateModAnimIDs[objData->state]) {
		func_80023D30(self, dStateModAnimIDs[objData->state], 0.25f, 0);
	}

	//Advance animation
	if (func_80024108(self, dStateAnimSpeeds[objData->state], gUpdateRateF, &animInfo)) {
		objData->flags |= SHmushroom_FLAG_Animation_Finished;
	} else {
		objData->flags &= ~SHmushroom_FLAG_Animation_Finished;
	}

	//Set speed based on current state
	if (objData->state == SHmushroom_STATE_1_Jump) {
		speed = (animInfo.unk0[0] / gUpdateRateF) * objData->jumpSpeedMultiplier;
	} else if (objData->state == SHmushroom_STATE_5_Surprised_Hop) {
		speed = animInfo.unk0[2] / gUpdateRateF;
	} else {
		speed = 0.0f;
	}

	//Move
	self->velocity.x = fsin16_precise(objData->fleeAngle) * speed;
	self->velocity.z = fcos16_precise(objData->fleeAngle) * speed;
	obj_move(self, self->velocity.x * gUpdateRateF, 0.0f, self->velocity.z * gUpdateRateF);
}
