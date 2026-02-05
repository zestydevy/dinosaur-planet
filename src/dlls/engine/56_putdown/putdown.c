#include "common.h"

#include "dlls/engine/56_putdown.h"
#include "dlls/objects/314_foodbag.h"

void putdown_delete_food_from_bag(u8 arg0, u16 arg1, FoodbagContents* bagSlots, FoodbagItem* foodItems);
void putdown_update_food_quantity_gamebits(FoodbagContents* bagSlots, FoodbagItem* food);
u32 putdown_get_foodID_from_foodType(s32 arg0);

// offset: 0x0 | ctor
void putdown_ctor(void *dll) { }

// offset: 0xC | dtor
void putdown_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
/**
  * Places a food Object in the world (replacing the oldest-created placed food Object if needed)
  */
int putdown_place_food(Object* foodbag, s32 foodType, FoodbagPlaced* placed, FoodbagItem* foodDefs) {
    PlacedFood_Setup* foodSetup;
    Object* food;
    Object* player;
    s32 foodIndex;

    foodIndex = putdown_get_foodID_from_foodType(foodType);
    STUBBED_PRINTF("about to place object %d\n", foodDefs[foodIndex].objectID);

    if (foodDefs[foodIndex].objectID != NO_FOOD_OBJECT_ID) {
        player = get_player();

        placed->nextIndex = (placed->nextIndex + 1) % MAX_FOOD_PLACED;        
        food = placed->objects[placed->nextIndex];
        if (food) {
            obj_destroy_object(food);
        }
        
        foodSetup = obj_alloc_create_info(sizeof(PlacedFood_Setup),
                                           foodDefs[foodIndex].objectID);
        foodSetup->base.fadeDistance = 0xFF;
        foodSetup->base.loadFlags = 2;
        foodSetup->base.x = player->srt.transl.x;
        foodSetup->base.y = player->srt.transl.y;
        foodSetup->base.z = player->srt.transl.z;
        foodSetup->yaw = player->srt.yaw;
        foodSetup->unk19 = 0;
        foodSetup->unk18 = 0;

        placed->objects[placed->nextIndex] = obj_create(
            (ObjSetup*)foodSetup, 
            1, -1, -1, foodbag->parent);
        placed->foodType[placed->nextIndex] = foodType;

        return TRUE;
    }
    return FALSE;
}

// offset: 0x1B4 | func: 1 | export: 1
/** 
  * Searches around a target Object, and returns the nearest placed food Object of a particular type.
  *
  * Returns NULL if no matching food Object found within search radius (10000).
  */
Object* putdown_get_nearest_placed_food_of_type(Object* foodbag, Object* target, s32 foodType, FoodbagPlaced* placedObjects) {
    f32 distance;
    f32 minDistance;
    s16 index;
    s16 closestFoodIndex;
    Object *placed_objects[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    Vec3f foodPosition;
    
    //Get the relevant subset of the foodbag's placed objects
    for (index = 0; index < MAX_FOOD_PLACED; index++){
        if (placedObjects->foodType[index] & foodType){
            foodPosition.x = placedObjects->objects[index]->srt.transl.x;
            foodPosition.y = placedObjects->objects[index]->srt.transl.y;
            foodPosition.z = placedObjects->objects[index]->srt.transl.z;
            if (!func_80059C40(&target->srt.transl, &foodPosition, 0.1f, 0, 0, foodbag, 8, -1, 0xFF, 0)){
                //TODO: verify DLL interface used by placed food
                if (((DLL_314_Foodbag*) placedObjects->objects[index]->dll)->vtbl->is_obtained(placedObjects->objects[index]) == 0){
                    placed_objects[index] = placedObjects->objects[index];
                }
            }
        }
    }
    
    //Find the nearest object in the placed food's subset
    for (closestFoodIndex = -1, index = 0, minDistance = 10000.0f; index < MAX_FOOD_PLACED; index++){
        if (placed_objects[index]){
            distance = vec3_distance_squared(&target->positionMirror, &placed_objects[index]->positionMirror);
            if (closestFoodIndex == -1 || distance <= minDistance) {
                minDistance = distance;
                closestFoodIndex = index;
            }
        }
    }
    
    if (closestFoodIndex == -1){
        return NULL;
    }
    return placed_objects[closestFoodIndex];
}

// offset: 0x3F0 | func: 2 | export: 2
/**
  * Destroys an Object placed in the world from the foodbag, 
  * clearing the foodbag's references to that Object.
  */
int putdown_destroy_placed_food(Object* foodObject, FoodbagPlaced* placedObjects) {
    s32 index;

    for (index = 0; index < MAX_FOOD_PLACED; index++){
        if (foodObject == placedObjects->objects[index]) {
            obj_destroy_object(foodObject);
            placedObjects->objects[index] = NULL;
            placedObjects->foodType[index] = 0;
            return TRUE;
        }
    }

    return FALSE;
}

// offset: 0x47C | func: 3 | export: 3
/**
  * Advances the lifetime counters of each item in the bag,
  * and if a food expires returns the foodID it should transform into.
  */
u16 putdown_tick_food_lifetimes(FoodbagContents* bagSlots, FoodbagItem* foodDefs) {
    u16 pad;
    u16 expiryID;
    f32 fExpiry;
    u16 index;
    u8 ID;
    u16 uExpiry;
    u16 foodType;
    
    for (index = 0; index < MAX_FOOD_DEFINITIONS; index++){
        bagSlots->lifetime[index] += gUpdateRateF;
    }
    
    for (index = 0; index < MAX_FOOD_DEFINITIONS; index++){
        foodType = bagSlots->foodType[index];
        if (foodType){
            ID = putdown_get_foodID_from_foodType(foodType);
            uExpiry = foodDefs[ID].expiry;
            fExpiry = uExpiry & 0xFFFFu;

            //Check if food expired
            if (bagSlots->lifetime[index] > fExpiry){
                ID = putdown_get_foodID_from_foodType(bagSlots->foodType[index]);
                expiryID = foodDefs[ID].expiryID;
                putdown_delete_food_from_bag(index, bagSlots->foodType[index], bagSlots, foodDefs);
                return expiryID;
            }
        }
    }
    
    return 0;   
}

// offset: 0x5E8 | func: 4 | export: 4
/** 
  * Attempts to add food of a particular type to the bag.
  * Returns TRUE if successful, or FALSE if bag doesn't have enough room.
  */
int putdown_add_food(s32 foodType, s32 capacity, FoodbagContents* bagSlots, FoodbagItem* foodDefs) {
    s8 emptySlotIndex;
    s8 placingIndex;
    u8 slotsNeeded;
    u32 ID;

    emptySlotIndex = 0;
    
    ID = putdown_get_foodID_from_foodType(foodType);    
    
    //Iterate over bag's slots and return FALSE if all slots occupied
    while (bagSlots->foodType[emptySlotIndex]) {
        emptySlotIndex++; 
        if (emptySlotIndex == capacity){
            return FALSE;
        }
    }   
    
    //Get the number of slots occupied by the food (usually 1, but 2 for fish)
    slotsNeeded = foodDefs[ID].slotsUsed;
    if (slotsNeeded > (s8) (capacity - emptySlotIndex)) {
        return FALSE;
    }

    //Insert food into slot(s)
    for (placingIndex = 0; placingIndex < foodDefs[ID].slotsUsed; placingIndex++){
        bagSlots->foodType[emptySlotIndex + placingIndex] = foodType;
    }
    
    //Set initial lifetime and update gamebits
    bagSlots->lifetime[emptySlotIndex] = 0.0f;
    putdown_update_food_quantity_gamebits(bagSlots, foodDefs);
    return TRUE;
}

// offset: 0x730 | func: 5 | export: 5
/** 
  * Decrements a particular food quantity gamebit while deleting 
  * the first food item of that type stored in the bag.
  */
void putdown_delete_food_by_gamebit(s16 foodGamebit, FoodbagContents* bagSlots, FoodbagItem* foodDefs) {
    u8 index;
    u8 ID;
    u16 foodType;

    //Search the bag for the first food item with a matching gamebit
    for (index = 0; index < MAX_FOOD_TIMERS; index++){
        foodType = bagSlots->foodType[index];
        ID = putdown_get_foodID_from_foodType(foodType);
        if (foodGamebit == foodDefs[ID].gamebitID){
            break;
        }
    }
    
    //Delete the item from the matching slot (also updates food quantity gamebits)
    putdown_delete_food_from_bag(index, foodType, bagSlots, foodDefs);
}

// offset: 0x80C | func: 6 | export: 6
/** 
  * Deletes a food item stored at a particular index in the bag
  * If the food uses multiple slots (e.g. fish) those are cleared too.
  */
void putdown_delete_food_from_bag(u8 indexInBag, u16 foodType, FoodbagContents* bagSlots, FoodbagItem* foodDefs) {
    u8 offsetIndex;
    u32 ID;
    u8 index;
    
    index = indexInBag;
    ID = putdown_get_foodID_from_foodType(foodType);

    //Get the offset index to start shifting data up from
    //(usually 1 away, but larger for food like fish which occupy multiple slots)
    offsetIndex = indexInBag + foodDefs[ID].slotsUsed;
    
    //Clear the food's slots by shifting up subsequent slots' lifetime/foodType data
    for (index = indexInBag; offsetIndex < MAX_FOOD_TIMERS; offsetIndex++, index++){
        bagSlots->lifetime[index] = bagSlots->lifetime[offsetIndex];
        bagSlots->foodType[index] = bagSlots->foodType[offsetIndex];
    }
    
    putdown_update_food_quantity_gamebits(bagSlots, foodDefs);
}

// offset: 0x900 | func: 7 | export: 7
/** 
  * Returns a foodbag's item capacity, based on whether small/medium/large bag gamebits are set 
  */
s32 putdown_get_capacity(FoodbagGamebits* gamebitIDs) {
    if (main_get_bits(gamebitIDs->large)) {
        main_set_bits(gamebitIDs->medium, TRUE);
        main_set_bits(gamebitIDs->small, TRUE);
        return BAG_CAPACITY_LARGE;
    }

    if (main_get_bits(gamebitIDs->medium)) {
        main_set_bits(gamebitIDs->small, TRUE);
        return BAG_CAPACITY_MEDIUM;
    }

    if (main_get_bits(gamebitIDs->small)) {
        return BAG_CAPACITY_SMALL;
    }

    return 0;
}

// offset: 0x9E0 | func: 8 | export: 8
void putdown_update_food_quantity_gamebits(FoodbagContents* bagSlots, FoodbagItem* foodDefs) {
    u8 food_quantities[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    u8 index;

    index = 0;
    while (bagSlots->foodType[index]) { 
        food_quantities[putdown_get_foodID_from_foodType(bagSlots->foodType[index])]++;
        index++;
    }
    
    //Iterate over food definitions (skip empty 1st entry)
    for (index = 1; index < MAX_FOOD_DEFINITIONS; index++){
        if (foodDefs[index].gamebitID > 0){
            main_set_bits(foodDefs[index].gamebitID, food_quantities[index]);
        }
    }
}

// offset: 0xAFC | func: 9 | export: 9
/**
  * Returns a foodID from its bitfield encoding
  */
u32 putdown_get_foodID_from_foodType(s32 foodTypeBitfield) {
    s32 foodID;

    for (foodID = 0; foodTypeBitfield; foodID++){
        foodTypeBitfield >>= 1;
    }
    
    return foodID;
}
