#include "common.h"
#include "macros.h"

s32 func_80049B84(s32 uSpeedA, s32 vSpeedA, s32 widthA, s32 heightA, s32 uSpeedB, s32 vSpeedB, s32 widthB, s32 heightB);
void func_80049CE4(u32 scrollerID, s32 uSpeedA, s32 vSpeedA, s32 widthA, s32 heightA, s32 uSpeedB, s32 vSpeedB, s32 widthB, s32 heightB);

typedef struct {
    ObjSetup base;
    s16 textureIndex;       //Primary material: index in TABLES.bin subfile #14 ("scroll table")
    s16 gamebitActivate;    //Unused, but instances of this object store a gamebitID here (e.g. 0x95)
    s8 uSpeedB;             //U scroll speed for secondary blend material 
    s8 vSpeedB;             //V scroll speed for secondary blend material 
    s8 uSpeedA;             //U scroll speed for primary material              
    s8 vSpeedA;             //V scroll speed for primary material              
    s16 blendTextureIndex;  //Secondary blend material: index in TABLES.bin subfile #14 (optional, -1 if unused)
                            //Blend material is used for multitextured scrolling water, etc.
} TexScroll2_Setup;

typedef struct {
    s32 materialIndex;      //Index of the main material being animated (in the local Block's materials)
    s32 blendMaterialIndex; //Index of the secondary material being animated (optional, -1 if unused)
    u8 scrollSetupNeeded;
    s8 uSpeedA;             //U scroll speed for primary material
    s8 vSpeedA;             //V scroll speed for primary material
    s8 uSpeedB;             //U scroll speed for secondary blend material
    s8 vSpeedB;             //V scroll speed for secondary blend material
} TexScroll2_Data;

static void TexScroll2_setup_texture_scrolling(Object* self, TexScroll2_Data* objData);

// offset: 0x0 | ctor
void TexScroll2_ctor(void *dll) { }

// offset: 0xC | dtor
void TexScroll2_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void TexScroll2_setup(Object* self, TexScroll2_Setup* objSetup, s32 arg2) {
    TexScroll2_Data* objData;

    objData = self->data;
    objData->uSpeedA = objSetup->uSpeedA;
    objData->vSpeedA = objSetup->vSpeedA;
    objData->uSpeedB = objSetup->uSpeedB;
    objData->vSpeedB = objSetup->vSpeedB;
    if (arg2 == 0) {
        TexScroll2_setup_texture_scrolling(self, objData);
    }
}

// offset: 0x80 | func: 1 | export: 1
void TexScroll2_control(Object* self) {
    TexScroll2_Data* objData;

    objData = self->data;

    //Get object's local Block model
    if (func_80044BB0(func_8004454C(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z)) == NULL) {
        objData->scrollSetupNeeded = TRUE;
        return;
    }

    //Set up texture scrolling handler
    if (objData->scrollSetupNeeded) {
        TexScroll2_setup_texture_scrolling(self, objData);
        objData->scrollSetupNeeded = FALSE;
    }
}

// offset: 0x128 | func: 2 | export: 2
void TexScroll2_update(Object *self) { }

// offset: 0x134 | func: 3 | export: 3
void TexScroll2_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x188 | func: 4 | export: 4
void TexScroll2_free(Object *self, s32 a1) { }

// offset: 0x198 | func: 5 | export: 5
u32 TexScroll2_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1A8 | func: 6 | export: 6
u32 TexScroll2_get_data_size(Object *self, u32 a1) {
    return sizeof(TexScroll2_Data);
}

// offset: 0x1BC | func: 7
void TexScroll2_setup_texture_scrolling(Object* self, TexScroll2_Data* objData) {
    Block* block;
    TexScroll2_Setup* objSetup;
    Texture* texture;
    Texture* textureBlended;
    s32 textureCount;
    s32 materialIndex;
    s32 shapeIndex;
    s32 widthA;
    s32 heightA;
    s32 widthB;
    s32 heightB;
    s32* scrollTable;

    objSetup = (TexScroll2_Setup*)self->setup;

    //Get object's local Block
    block = func_80044BB0(func_8004454C(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if (block == NULL) {
        objData->scrollSetupNeeded = TRUE;
        return;
    }

    //Get TABLES.bin subfile #14 ("scroll table")
    scrollTable = func_800213A0(14);
    if (scrollTable == NULL) {
        STUBBED_PRINTF("TEXSCROLL: no scroll table\n");
        return;
    }

    //Get texture from table
    texture = func_8003E960(-scrollTable[objSetup->textureIndex]);
    if (texture == NULL) {
        STUBBED_PRINTF("TEXSCROLL: cannot find texture '%d' %f %f %f %d\n");
        return;
    }

    //Iterate over Block's materials until finding one that uses the texture
    for (materialIndex = 0; materialIndex < block->textureCount; materialIndex++){
        if (texture == block->tiles[materialIndex].texture){
            break;
        }
    }
    //Bail if material not found
    if (materialIndex == block->textureCount) {
        return;
    }

    widthA = texture->width << 6;
    heightA = texture->height << 6;
    objData->materialIndex = materialIndex;
    objData->blendMaterialIndex = -1U;
    widthB = widthA;
    heightB = heightA;

    //Iterate over Block's materials until finding one that uses the blended texture (if it's in use)
    if (objSetup->blendTextureIndex != -1) {
        textureBlended = func_8003E960(-scrollTable[objSetup->blendTextureIndex]);
        if (textureBlended != NULL) {
            for (materialIndex = 0; materialIndex < block->textureCount; materialIndex++) {
                if (textureBlended == block->tiles[materialIndex].texture) {
                    break;
                }
            }
            if (materialIndex == block->textureCount) {
                return;
            }
            widthB = textureBlended->width << 6;
            heightB = textureBlended->height << 6;
            objData->blendMaterialIndex = materialIndex;
        }
    }

    //Iterate over Block's shapes and enable UV scrolling on any that use the relevant materials
    for (shapeIndex = 0; shapeIndex < block->shapeCount; shapeIndex++){
        if (block->shapes[shapeIndex].tileIdx0 == objData->materialIndex) {
            //If blend material is specified, only apply scrolling if the shape uses it as its secondary multitexture material
            if (objData->blendMaterialIndex == -1 || 
                objData->blendMaterialIndex == block->shapes[shapeIndex].tileIdx1) {
                if (block->shapes[shapeIndex].unk16 != 0xFF) {
                    func_80049CE4(block->shapes[shapeIndex].unk16, 
                        objData->uSpeedA, objData->vSpeedA, widthA, heightA, 
                        objData->uSpeedB, objData->vSpeedB, widthB, heightB);
                } else {
                    block->shapes[shapeIndex].unk16 = func_80049B84(
                        objData->uSpeedA, objData->vSpeedA, widthA, heightA,
                        objData->uSpeedB, objData->vSpeedB, widthB, heightB);
                }
            }
        }
    }
}

// offset: 0x468 | func: 8 | export: 7
void TexScroll2_change_scroll_speed(Object* self, s8 vSpeedA) {
    TexScroll2_Data* objData;

    objData = self->data;
    if (vSpeedA != objData->vSpeedA) {
        objData->vSpeedA = vSpeedA;
        objData->scrollSetupNeeded = TRUE;
    }
}
