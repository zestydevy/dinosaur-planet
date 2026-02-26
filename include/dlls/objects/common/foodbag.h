#ifndef _DLLS_COMMON_FOODBAG_H
#define _DLLS_COMMON_FOODBAG_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "dlls/engine/56_putdown.h"

#define MAX_FOOD_DEFINITIONS 12 //Number of food types defined in "foodbag_items", "dino_foodbag_items"

#define MAX_FOOD_TIMERS 30      //Same as largest bag capacity

#define BAG_CAPACITY_LARGE 30
#define BAG_CAPACITY_MEDIUM 20
#define BAG_CAPACITY_SMALL 10

#define FOOD_TYPE(position) 1 << (position - 1)
#define FOOD_GONE 0

typedef enum {
    FOOD_Green_Apple = FOOD_TYPE(1),
    FOOD_Red_Apple   = FOOD_TYPE(2),
    FOOD_Brown_Apple = FOOD_TYPE(3),
    FOOD_Fish        = FOOD_TYPE(4),
    FOOD_Smoked_Fish = FOOD_TYPE(5),
    FOOD_Dino_Egg    = FOOD_TYPE(6),
    FOOD_Moldy_Meat  = FOOD_TYPE(7),
    FOOD_Green_Bean  = FOOD_TYPE(8),
    FOOD_Red_Bean    = FOOD_TYPE(9),
    FOOD_Brown_Bean  = FOOD_TYPE(10),
    FOOD_Blue_Bean   = FOOD_TYPE(11)
} FoodTypes;

typedef enum {
    SIDEFOOD_Blue_Mushrooms = FOOD_TYPE(1), //1
    SIDEFOOD_Red_Mushrooms  = FOOD_TYPE(2), //2
    SIDEFOOD_Old_Mushrooms  = FOOD_TYPE(3), //4
    SIDEFOOD_Blue_Grubs     = FOOD_TYPE(4), //8
    SIDEFOOD_Red_Grubs      = FOOD_TYPE(5), //0x10
    SIDEFOOD_Old_Grubs      = FOOD_TYPE(6), //0x20
} SideFoodTypes;

/** ObjSetup struct for food Objects placed in the world
  * 
  * (e.g. OBJ_foodbagRedApple, OBJ_foodbagNewFish, OBJ_foodbagBlueMush)
  */
typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s8 yaw;
    s16 unk1C;
    s16 unk1E;
} PlacedFood_Setup;


typedef struct {
    ObjSetup base;
	u8 pad[0x24-0x18];
} Foodbag_ObjSetup;

typedef struct {
    struct DLL_56_Putdown *dllPutdown;  //Handles removing/updating food bag items
    FoodbagGamebits capacityGamebits;   //Bag size gamebits
    u16 capacity;                       //Current capacity
    FoodbagContents* bagSlots;           //Expiry timers for bag items
    s8 *playerHealth;                   //Pointer to current health
    s8 *playerHealthMax;                //Pointer to current max health (for Eat First behaviour)
    u8 eatFirst;                        //Player food bag config (prioritise healing vs. storing)
    FoodbagPlaced placedObjects;        //Food placed in world
    f32 playerEatTimer;                 /*Mostly unused, but set when health recovered via eating 
                                         (Possibly intended as timer: too full to eat for a little while?)
                                        */
} Foodbag_Data;

#define BLANK_FOODBAG_ITEM {0, 0, 0, 0, 0, 0}
#define NO_FOOD_OBJECT_ID -1

DLL_INTERFACE(DLL_IFoodbag) {
	/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ int (*is_obtained)(Object* self);
    /*8*/ Object* (*get_nearest_placed_food_of_type)(Object* self, Object* target, s32 foodType);
    /*9*/ int (*destroy_placed_food)(Object* self, Object* foodObject);
    /*10*/ void (*set_capacity)(Object* self);
    /*11*/ void (*collect_food)(Object* self, s32 foodType);
    /*12*/ void (*delete_food_by_gamebit)(Object* self, s16 gamebit);
    /*13*/ int (*set_eat_config)(Object* self, s32 eatFirst);
    /*14*/ u16 (*get_capacity)(Object* self);
    /*15*/ s32 (*count_slots_occupied)(Object* self);
    /*16*/ s16 (*get_anim_objectID_from_food_gamebit)(s16 foodGamebit);
};

#endif //_DLLS_COMMON_FOODBAG_H
