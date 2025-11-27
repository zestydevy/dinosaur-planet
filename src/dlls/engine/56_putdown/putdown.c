#include "common.h"

#include "dlls/objects/314_foodbag.h"
#include "macros.h"

/*0x0*/ static u32 _data_0[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
// /*0x50*/ static u32 _data_50[] = { 0, 0, 0, 0 };
// /*0x0*/ static const char str_0[] = "about to place object %d\n";

void dll_56_func_80C(u8 arg0, u16 arg1, GplayStruct14* bagTimers, FoodbagItem* foodItems);
void dll_56_func_9E0(GplayStruct14* bagTimers, FoodbagItem* food);
u32 dll_56_func_AFC(s32 arg0);

// offset: 0x0 | ctor
void dll_56_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_56_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
int dll_56_func_18(Object* arg0, s32 foodType, FoodbagStructUnk* placedFood, FoodbagItem* foodItems) {
    Food_Setup* foodSetup;
    Object* food;
    Object* player;
    s32 foodIndex;

    foodIndex = dll_56_func_AFC(foodType);

    if (foodItems[foodIndex].objectID != NO_FOOD_OBJECT_ID) {
        player = get_player();
        placedFood->nextIndex = (placedFood->nextIndex + 1) % 20;
        
        food = placedFood->placedObjects[placedFood->nextIndex];
        if (food) {
            obj_destroy_object(food);
        }
        
        foodSetup = obj_alloc_create_info(sizeof(Food_Setup),
                                           foodItems[foodIndex].objectID);
        foodSetup->base.fadeDistance = 0xFF;
        foodSetup->base.loadFlags = 2;
        foodSetup->base.x = player->srt.transl.x;
        foodSetup->base.y = player->srt.transl.y;
        foodSetup->base.z = player->srt.transl.z;
        foodSetup->unk1A = player->srt.yaw;
        foodSetup->unk19 = 0;
        foodSetup->unk18 = 0;

        STUBBED_PRINTF("about to place object %d\n");
        placedFood->placedObjects[placedFood->nextIndex] = obj_create(
            (ObjSetup*)foodSetup, 
            1, -1, -1, arg0->parent);
        placedFood->foodType[placedFood->nextIndex] = foodType;

        return TRUE;
    }
    return FALSE;
}

// offset: 0x1B4 | func: 1 | export: 1
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/56_putdown/dll_56_func_1B4.s")
#else
Object* dll_56_func_1B4(Object* arg0, Object* arg1, s32 arg2, FoodbagStructUnk* arg3) {
    f32 distance;
    f32 minDistance;
    s16 index;
    // Object *new_var;
    s16 closestFoodIndex;
    Object *_data_0[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    Vec3f position;
    
    for (index = 0; index < 20; index++){
        if (arg3->foodType[index] & arg2){
            position.x = arg3->placedObjects[index]->srt.transl.x;
            position.y = arg3->placedObjects[index]->srt.transl.y;
            position.z = arg3->placedObjects[index]->srt.transl.z;
            if (!func_80059C40(&arg1->srt.transl, &position, 0.1f, 0, 0, arg0, 8, -1, 0xFF, 0)){
                // new_var = arg3->placedObjects[index];
                if (((DLL_314_Foodbag*) arg3->placedObjects[index]->dll)->vtbl->Foodbag_func_590(arg3->placedObjects[index]) == 0){
                    _data_0[index] = arg3->placedObjects[index];
                }
            }
        }
    }
    
    for (closestFoodIndex = -1, index = 0, minDistance = 100000.0f; index < 20; index++){
        if (_data_0[index]){
            distance = vec3_distance_squared(&arg1->positionMirror, &_data_0[index]->positionMirror);
            if (closestFoodIndex == -1 || distance <= minDistance) {
                minDistance = distance;
                closestFoodIndex = index;
            }
        }
    }
    
    if (closestFoodIndex == -1){
        return NULL;
    }
    return _data_0[closestFoodIndex];
}
#endif

// offset: 0x3F0 | func: 2 | export: 2
s32 dll_56_func_3F0(Object* foodObject, FoodbagStructUnk* arg1) {
    s32 index;

    for (index = 0; index < 20; index++){
        if (foodObject == arg1->placedObjects[index]) {
            obj_destroy_object(foodObject);
            arg1->placedObjects[index] = NULL;
            arg1->foodType[index] = 0;
            return 1;
        }
    }

    return 0;
}

// offset: 0x47C | func: 3 | export: 3
u16 dll_56_func_47C(GplayStruct14* arg0, FoodbagItem* arg1) {
    u16 pad;
    u16 sp34;
    f32 var_ft4;
    u16 index;
    u8 index2;
    u16 temp_t2;
    u16 temp_v0_2;
    
    for (index = 0; index < 12; index++){
        arg0->lifetime[index] += gUpdateRateF;
    }
    
    for (index = 0; index < 12; index++){
        temp_v0_2 = arg0->unk78[index];
        if (temp_v0_2){
            index2 = dll_56_func_AFC(temp_v0_2);
            temp_t2 = arg1[index2].expiry;
            var_ft4 = temp_t2 & 0xFFFFu;
            if (var_ft4 < arg0->lifetime[index]){
                index2 = dll_56_func_AFC(arg0->unk78[index]);
                sp34 = arg1[index2].unk4;
                dll_56_func_80C(index, arg0->unk78[index], arg0, arg1);
                return sp34;
            }
        }
    }
    
    return 0;   
}

// offset: 0x5E8 | func: 4 | export: 4
s32 dll_56_func_5E8(s32 arg0, s32 arg1, GplayStruct14* arg2, FoodbagItem* arg3) {
    s8 var_t0;
    s8 var_v1;
    u8 temp_a0;
    u32 temp_v0;

    var_t0 = 0;
    
    temp_v0 = dll_56_func_AFC(arg0);    
    
    while (arg2->unk78[var_t0]) {
        var_t0++; 
        if (var_t0 == arg1){
            return 0;
        }
    }   
    
    temp_a0 = arg3[temp_v0].unk3;
    if ((s8) (arg1 - var_t0) < temp_a0) {
        return 0;
    }

    for (var_v1 = 0; var_v1 < arg3[temp_v0].unk3; var_v1++){
        arg2->unk78[var_t0 + var_v1] = arg0;
    }

    
    arg2->lifetime[var_t0] = 0.0f;
    dll_56_func_9E0(arg2, arg3);
    return 1;
}

// offset: 0x730 | func: 5 | export: 5
void dll_56_func_730(s16 arg0, GplayStruct14* arg1, FoodbagItem* arg2) {
    s32 temp_t3;
    u8 index;
    u8 index2;
    u16 temp_s1;

    for (index = 0; index < 30; index++){
        temp_s1 = arg1->unk78[index];
        index2 = dll_56_func_AFC(temp_s1);
        if (arg0 == arg2[index2].gamebitID){
            break;
        }
    }
    
    dll_56_func_80C(index, temp_s1, arg1, arg2);
}

// offset: 0x80C | func: 6 | export: 6
void dll_56_func_80C(u8 arg0, u16 arg1, GplayStruct14* bagTimers, FoodbagItem* foodItems) {
    u8 offset;
    u32 new_var;
    u8 index;
    
    index = arg0;

    new_var = dll_56_func_AFC(arg1);
    offset = arg0 + foodItems[new_var].unk3;
    
    for (index = arg0; offset < 30; offset++, index++){
        bagTimers->lifetime[index] = bagTimers->lifetime[offset];
        bagTimers->unk78[index] = bagTimers->unk78[offset];
    }
    
    dll_56_func_9E0(bagTimers, foodItems);
}

// offset: 0x900 | func: 7 | export: 7
//putdown_get_capacity?
s32 dll_56_func_900(FoodbagGamebits* gamebitIDs) {
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
void dll_56_func_9E0(GplayStruct14* bagTimers, FoodbagItem* food) {
    u8 sp3C[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    u8 index;

    index = 0;
    while (bagTimers->unk78[index] != 0) { 
        sp3C[dll_56_func_AFC(bagTimers->unk78[index])]++;
        index++;
    }
    
    for (index = 1; index < 12; index++){
        if (food[index].gamebitID > 0){
            main_set_bits(food[index].gamebitID, sp3C[index]);
        }
    }
}

// offset: 0xAFC | func: 9 | export: 9
u32 dll_56_func_AFC(s32 arg0) {
    s32 i;

    for (i = 0; arg0; i++){
        arg0 >>= 1;
    }
    
    return i;
}
