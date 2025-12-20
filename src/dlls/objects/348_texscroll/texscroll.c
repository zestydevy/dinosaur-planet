#include "common.h"
#include "game/gamebits.h"
#include "macros.h"
#include "sys/debug.h"

typedef struct {
    ObjSetup base;
    s16 textureIndex;       //in TABLES.bin subfile #14 ("scroll table")
    s16 gamebitActivate;    //gamebitID that activates animation (or -1 to skip bit check)
    s16 unused1C;
    s8 speedU;              //scroll speed (horizontal)
    s8 speedV;              //scroll speed (vertical)
} TexScroll_Setup;

typedef struct {
    s16 materialIndex;      //index of material being animated (in local Block's materials)
    s16 adjustDeltaTime;    //applies speed correction factor on 1st tick after Block reloads
    s16 speedU;
    s16 speedV;
    s16 prevDeltaU;
    s16 prevDeltaV;
    s32 scrollProgress;
    s32 unused10;
    s16 gamebitActivate;
    s16 unused16;
    s8 unused18;            //initialised as 0, but otherwise unused
    u8 searchForMaterial;
} TexScroll_Data;

static void TexScroll_find_animated_material_index(Object* self, TexScroll_Data* objData);

// offset: 0x0 | ctor
void TexScroll_ctor(void *dll) { }

// offset: 0xC | dtor
void TexScroll_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void TexScroll_setup(Object* self, TexScroll_Setup* objSetup, s32 arg2) {
    TexScroll_Data* objData;

    objData = self->data;
    if (objData == NULL) {
        return;
    }

    objData->adjustDeltaTime = TRUE;
    objData->speedU = objSetup->speedU;
    objData->speedV = objSetup->speedV;
    objData->scrollProgress = 0;
    objData->unused18 = 0;
    objData->gamebitActivate = objSetup->gamebitActivate;
    if (arg2 == 0) {
        objData->prevDeltaU = 0;
        objData->prevDeltaV = 0;
    }
    TexScroll_find_animated_material_index(self, objData);
    objData->adjustDeltaTime = FALSE;

}

// offset: 0xA4 | func: 1 | export: 1
void TexScroll_control(Object* self) {
    Block* block;
    TexScroll_Data* objData;
    BlockShape* shapes;
    Texture* texture;
    Vtx_t* verts;
    Vtx_t* vertsAnimated;
    Vtx_t* vertexAnimated;
    s32 shapeIndex;
    s32 vertexIndex;
    s32 width;
    s32 height;
    s32 vertexBit;
    s32 wrapVertexBitfieldU;
    s32 wrapVertexBitfieldV;
    s32 wrapAllVertsBitfield;
    s32 wrapNeededU;
    s32 wrapNeededV;
    s32 speedU;
    s32 speedV;
    s32 deltaU;
    s32 deltaV;
    u32 deltaTimeAdjustedU;
    u32 deltaTimeAdjustedV;

    objData = self->data;

    //Only animate if activation gamebit is set (or if no gamebit is specified)
    if (!(objData->gamebitActivate == NO_GAMEBIT || main_get_bits(objData->gamebitActivate))) {
        return;
    }

    objData->scrollProgress += gUpdateRate;

    //Get object's local Block
    block = func_80044BB0(func_8004454C(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if (block == NULL) {
        objData->adjustDeltaTime = TRUE;
        objData->searchForMaterial = TRUE;
        return;
    }

    //Get index of Block material being animated
    if (objData->searchForMaterial != FALSE) {
        TexScroll_find_animated_material_index(self, objData);
        objData->searchForMaterial = FALSE;
    }

    if (objData->materialIndex >= block->textureCount) {
        objData->materialIndex = 0;
    }

    texture = block->tiles[objData->materialIndex].texture;
    width = texture->width << 6;
    height = texture->height << 6;
    speedU = objData->speedU;
    speedV = objData->speedV;
    deltaTimeAdjustedU = gUpdateRate;
    deltaTimeAdjustedV = gUpdateRate;

    //Apply correction factor on first tick after object's local Block reloads
    if (objData->adjustDeltaTime != FALSE) {
        if (speedU) {
            deltaTimeAdjustedU = gUpdateRate + (objData->scrollProgress % ((width * 0x10) / speedU));
        }
        if (speedV) {
            deltaTimeAdjustedV = gUpdateRate + (objData->scrollProgress % ((height * 0x10) / speedV));
        }
        objData->adjustDeltaTime = FALSE;
    }

    deltaU = objData->prevDeltaU;
    speedU *= deltaTimeAdjustedU;
    deltaU += speedU;

    deltaV = objData->prevDeltaV;
    speedV *= deltaTimeAdjustedV;
    deltaV += speedV;

    objData->prevDeltaU = deltaU & 3;
    objData->prevDeltaV = deltaV & 3;

    verts = (void*)block->shapes;
    shapes = (BlockShape*)verts;
    verts = block->vertices;
    vertsAnimated = block->vertices2[block->vtxFlags & 1];
    if (1) { }
    for (shapeIndex = 0; shapeIndex < block->shapeCount; shapeIndex++){
        if (objData->materialIndex == shapes[shapeIndex].tileIdx0) {
            wrapAllVertsBitfield = 0;
            wrapVertexBitfieldU = 0;
            wrapVertexBitfieldV = 0;
            vertexBit = 1;
            wrapNeededU = FALSE;
            wrapNeededV = FALSE;

            //Check if all the shape's vertices have scrolled out of range (along U and V axes)
            for (vertexIndex = shapes[shapeIndex].vtxBase; vertexIndex < shapes[shapeIndex + 1].vtxBase; vertexIndex++) {
                wrapAllVertsBitfield |= vertexBit;
                verts[vertexIndex].tc[0] += deltaU >> 2;
                verts[vertexIndex].tc[1] += deltaV >> 2;

                if (width < verts[vertexIndex].tc[0]) {
                    wrapVertexBitfieldU |= vertexBit;
                }
                if (verts[vertexIndex].tc[0] < -width) {
                    wrapVertexBitfieldU |= vertexBit;
                }

                if (height < verts[vertexIndex].tc[1]) {
                    wrapVertexBitfieldV |= vertexBit;
                }
                if (verts[vertexIndex].tc[1] < -height) {
                    wrapVertexBitfieldV |= vertexBit;
                }
                vertexBit <<= 1;
            }

            if (wrapVertexBitfieldU == wrapAllVertsBitfield) {
                wrapNeededU = TRUE;
            }
            if (wrapVertexBitfieldV == wrapAllVertsBitfield) {
                wrapNeededV = TRUE;
            }

            //Wrap all the shape's vertices back into range
            for (vertexIndex = shapes[shapeIndex].vtxBase; vertexIndex < shapes[shapeIndex + 1].vtxBase; vertexIndex++) {
                if (wrapNeededU) {
                    if (width < verts[vertexIndex].tc[0]) {
                        verts[vertexIndex].tc[0] -= width;
                    } else {
                        verts[vertexIndex].tc[0] += width;
                    }
                }
                if (wrapNeededV) {
                    if (height < verts[vertexIndex].tc[1]) {
                        verts[vertexIndex].tc[1] -= height;
                    } else {
                        verts[vertexIndex].tc[1] += height;
                    }
                }
                vertexAnimated = &vertsAnimated[vertexIndex];
                vertexAnimated->tc[0] = verts[vertexIndex].tc[0];
                vertexAnimated->tc[1] = verts[vertexIndex].tc[1];
            }
        }
    }
}

// offset: 0x4F4 | func: 2 | export: 2
void TexScroll_update(Object *self) { }

// offset: 0x500 | func: 3 | export: 3
void TexScroll_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x554 | func: 4 | export: 4
void TexScroll_free(Object *self, s32 a1) { }

// offset: 0x564 | func: 5 | export: 5
u32 TexScroll_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x574 | func: 6 | export: 6
u32 TexScroll_get_data_size(Object *self, u32 a1) {
    return sizeof(TexScroll_Data);
}

// offset: 0x588 | func: 7
/** 
  * Searches through the local Block model's materials until finding the material that will be scrolled.
  * The relevant Block material's index is then stored in TexScroll_Data->materialIndex
*/
void TexScroll_find_animated_material_index(Object* self, TexScroll_Data* objData) {
    Block* block;
    Block_0x0Struct* material;
    Texture* texture;
    s32 index;
    TexScroll_Setup* objSetup;
    s32* scroll_table;

    objSetup = (TexScroll_Setup*)self->setup;

    //Find object's local Block
    block = func_80044BB0(func_8004454C(self->srt.transl.x, self->srt.transl.y,self->srt.transl.z));
    if (block == NULL) {
        objData->searchForMaterial = TRUE;
        return;
    }

    //Get TABLES.bin subfile #14 ("scroll table")
    scroll_table = func_800213A0(14);
    if (scroll_table == NULL){
        STUBBED_PRINTF("TEXSCROLL: no scroll table\n");
        return;
    }

    //Get texture from table
    texture = func_8003E960(-scroll_table[objSetup->textureIndex]);
    if (texture == NULL){
        STUBBED_PRINTF("TEXSCROLL: cannot find texture '%d'\n");
        return;
    }

    //Iterate over Block's materials until finding one that uses the texture
    for (index = 0; index < block->textureCount; index++){
        if (texture == block->tiles[index].texture) {
            break;
        }
    }           

    //Store material index of UV-scrolled texture
    if (index != block->textureCount) {
        objData->materialIndex = index;
    }
}

/*0x44*/ static const char str_44[] = "";
/*0x48*/ static const char str_48[] = "";
/*0x4C*/ static const char str_4C[] = "";
