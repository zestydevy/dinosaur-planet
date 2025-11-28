#include "common.h"

#include "dlls/objects/314_foodbag.h"

void putdown_func_80C(u8 arg0, u16 arg1, GplayStruct14* bagTimers, FoodbagItem* foodItems);
void putdown_update_food_quantity_gamebits(GplayStruct14* bagTimers, FoodbagItem* food);
u32 putdown_func_AFC(s32 arg0);

// offset: 0x0 | ctor
void putdown_ctor(void *dll) { }

// offset: 0xC | dtor
void putdown_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
int putdown_place_food(Object* arg0, s32 foodType, FoodbagPlaced* placed, FoodbagItem* foodDefs) {
    Food_Setup* foodSetup;
    Object* food;
    Object* player;
    s32 foodIndex;

    foodIndex = putdown_func_AFC(foodType);

    if (foodDefs[foodIndex].objectID != NO_FOOD_OBJECT_ID) {
        player = get_player();

        placed->nextIndex = (placed->nextIndex + 1) % 20;        
        food = placed->objects[placed->nextIndex];
        if (food) {
            obj_destroy_object(food);
        }
        
        foodSetup = obj_alloc_create_info(sizeof(Food_Setup),
                                           foodDefs[foodIndex].objectID);
        foodSetup->base.fadeDistance = 0xFF;
        foodSetup->base.loadFlags = 2;
        foodSetup->base.x = player->srt.transl.x;
        foodSetup->base.y = player->srt.transl.y;
        foodSetup->base.z = player->srt.transl.z;
        foodSetup->yaw = player->srt.yaw;
        foodSetup->unk19 = 0;
        foodSetup->unk18 = 0;

        STUBBED_PRINTF("about to place object %d\n");
        placed->objects[placed->nextIndex] = obj_create(
            (ObjSetup*)foodSetup, 
            1, -1, -1, arg0->parent);
        placed->foodType[placed->nextIndex] = foodType;

        return TRUE;
    }
    return FALSE;
}

// offset: 0x1B4 | func: 1 | export: 1
Object* putdown_get_nearest_placed_food_of_type(Object* arg0, Object* arg1, s32 typeMask, FoodbagPlaced* placedObjects) {
    f32 distance;
    f32 minDistance;
    s16 index;
    s16 closestFoodIndex;
    Object *placed_objects[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    Vec3f foodPosition;
    
    for (index = 0; index < MAX_PLACED_FOOD; index++){
        if (placedObjects->foodType[index] & typeMask){
            foodPosition.x = placedObjects->objects[index]->srt.transl.x;
            foodPosition.y = placedObjects->objects[index]->srt.transl.y;
            foodPosition.z = placedObjects->objects[index]->srt.transl.z;
            if (!func_80059C40(&arg1->srt.transl, &foodPosition, 0.1f, 0, 0, arg0, 8, -1, 0xFF, 0)){
                if (((DLL_314_Foodbag*) placedObjects->objects[index]->dll)->vtbl->Foodbag_func_590(placedObjects->objects[index]) == 0){
                    placed_objects[index] = placedObjects->objects[index];
                }
            }
        }
    }
    
    for (closestFoodIndex = -1, index = 0, minDistance = 10000.0f; index < MAX_PLACED_FOOD; index++){
        if (placed_objects[index]){
            distance = vec3_distance_squared(&arg1->positionMirror, &placed_objects[index]->positionMirror);
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
int putdown_destroy_placed_food(Object* foodObject, FoodbagPlaced* placedObjects) {
    s32 index;

    for (index = 0; index < MAX_PLACED_FOOD; index++){
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
u16 putdown_func_47C(GplayStruct14* bagTimers, FoodbagItem* foodDefs) {
    u16 pad;
    u16 sp34;
    f32 fExpiry;
    u16 index;
    u8 ID;
    u16 uExpiry;
    u16 foodType;
    
    for (index = 0; index < 12; index++){
        bagTimers->lifetime[index] += gUpdateRateF;
    }
    
    for (index = 0; index < 12; index++){
        foodType = bagTimers->foodType[index];
        if (foodType){
            ID = putdown_func_AFC(foodType);
            uExpiry = foodDefs[ID].expiry;
            fExpiry = uExpiry & 0xFFFFu;

            //Check if food expired
            if (bagTimers->lifetime[index] > fExpiry){
                ID = putdown_func_AFC(bagTimers->foodType[index]);
                sp34 = foodDefs[ID].unk4;
                putdown_func_80C(index, bagTimers->foodType[index], bagTimers, foodDefs);
                return sp34;
            }
        }
    }
    
    return 0;   
}

// offset: 0x5E8 | func: 4 | export: 4
int putdown_func_5E8(s32 arg0, s32 arg1, GplayStruct14* bagTimers, FoodbagItem* foodDefs) {
    s8 index1;
    s8 index2;
    u8 temp_a0;
    u32 ID;

    index1 = 0;
    
    ID = putdown_func_AFC(arg0);    
    
    while (bagTimers->foodType[index1]) {
        index1++; 
        if (index1 == arg1){
            return FALSE;
        }
    }   
    
    temp_a0 = foodDefs[ID].unk3;
    if ((s8) (arg1 - index1) < temp_a0) {
        return FALSE;
    }

    for (index2 = 0; index2 < foodDefs[ID].unk3; index2++){
        bagTimers->foodType[index1 + index2] = arg0;
    }

    
    bagTimers->lifetime[index1] = 0.0f;
    putdown_update_food_quantity_gamebits(bagTimers, foodDefs);
    return TRUE;
}

// offset: 0x730 | func: 5 | export: 5
void putdown_func_730(s16 foodGamebit, GplayStruct14* bagTimers, FoodbagItem* foodDefs) {
    s32 temp_t3;
    u8 index;
    u8 ID;
    u16 foodType;

    for (index = 0; index < 30; index++){
        foodType = bagTimers->foodType[index];
        ID = putdown_func_AFC(foodType);
        if (foodGamebit == foodDefs[ID].gamebitID){
            break;
        }
    }
    
    putdown_func_80C(index, foodType, bagTimers, foodDefs);
}

// offset: 0x80C | func: 6 | export: 6
void putdown_func_80C(u8 foodIndex, u16 arg1, GplayStruct14* bagTimers, FoodbagItem* foodDefs) {
    u8 offsetIndex;
    u32 ID;
    u8 index;
    
    index = foodIndex;

    ID = putdown_func_AFC(arg1);
    offsetIndex = foodIndex + foodDefs[ID].unk3;
    
    for (index = foodIndex; offsetIndex < 30; offsetIndex++, index++){
        bagTimers->lifetime[index] = bagTimers->lifetime[offsetIndex];
        bagTimers->foodType[index] = bagTimers->foodType[offsetIndex];
    }
    
    putdown_update_food_quantity_gamebits(bagTimers, foodDefs);
}

// offset: 0x900 | func: 7 | export: 7
/** 
  * Returns a foodbag's item capacity, based on whether small/medium/large bag gamebits are set 
  */
s32 putdown_get_capacity(FoodbagGamebits* gamebitIDs) {
    if (main_get_bits(gamebitIDs->large)) {
        main_set_bits(gamebitIDs->medium, 1);
        main_set_bits(gamebitIDs->small, 1);
        return 30;
    }

    if (main_get_bits(gamebitIDs->medium)) {
        main_set_bits(gamebitIDs->small, 1);
        return 20;
    }

    if (main_get_bits(gamebitIDs->small)) {
        return 10;
    }
    
    return 0;
}

// offset: 0x9E0 | func: 8 | export: 8
void putdown_update_food_quantity_gamebits(GplayStruct14* bagTimers, FoodbagItem* foodDefs) {
    u8 food_quantities[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    u8 index;

    index = 0;
    while (bagTimers->foodType[index]) { 
        food_quantities[putdown_func_AFC(bagTimers->foodType[index])]++;
        index++;
    }
    
    for (index = 1; index < 12; index++){
        if (foodDefs[index].gamebitID > 0){
            main_set_bits(foodDefs[index].gamebitID, food_quantities[index]);
        }
    }
}

// offset: 0xAFC | func: 9 | export: 9
u32 putdown_func_AFC(s32 arg0) {
    s32 i;

    for (i = 0; arg0; i++){
        arg0 >>= 1;
    }
    
    return i;
}
