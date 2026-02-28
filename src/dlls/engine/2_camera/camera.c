#include "common.h"
#include "game/objects/object.h"
#include "sys/memory.h"
#include "sys/menu.h"
#include "game/objects/interaction_arrow.h"
#include "sys/voxmap.h"

typedef struct {
	s8 unk0; //disables first-person view when set
	s8 unk1; //transition duration
	s8 unk2; //fov
	s8 unk3; //player distance min
	s8 unk4; //player distance max
	s8 unk5; //height, or maybe angle - yaw/roll? (maybe s16?)
	s8 unk6;
	s8 unk7; //framebuffer/letterbox-related
	s8 unk8; //interpolation-related?
	s8 unk9; //interpolation-related?
	s8 unkA; //interpolation-related?
	s8 unkB; //max lateral speed?
	s8 unkC; //initial speed?
	s8 unkD; //pad?
	s8 unkE; //pad?
	s8 unkF; //pad?
} CameraAction;

typedef struct {
    SRT unk0;
    Vec3f unk18;
    s8 unk24[0xA0 - 0x24];
    Object* unkA0;
    Vec3f unkA4;
    f32 unkB0;
    f32 unkB4; //average of lateral speed samples
    f32 unkB8[5]; //lateral speed samples
    f32 unkCC;
    f32 unkD0;
    f32 unkD4;
    s8 unkD8[0xE4 - 0xD8];
    f32 unkE4; //Bezier tValue?
    f32 unkE8; //rate of change of tValue unkE4?
    f32 unkEC;
    s16 unkF0; //dYaw
    s16 unkF2; //dPitch
    s16 unkF4; //dRoll
    s16 unkF6; //yaw
    s16 unkF8; //pitch
    s16 unkFA; //roll
    f32 unkFC;
    f32 unk100;
    f32 unk104;
    f32 unk108;
    Object* unk10C;
    Object* unk110; //target object (when z-targetting)
    u8 unk114;
    s8 unk115; //letterbox goal height
    s8 unk116; //letterbox height rate of change?
    u8 unk117;
    s8 unk118;
    u8 unk119;
    u8 unk11A;
    u8 unk11B;
} UnkStructDLL2_bss_11C;

/** Related to gametext when holding R? */
static s16 _data_0[] = {
    0xffff, 0x0000, 0x00f9, 0x00fc, 0x0102, 0x0104, 0x0106, 0x0108, 0x00fa, 0x00fd, 0x0103, 0x0105, 0x0107, 0x0109, 0xff00, 0x0000
};

typedef struct {
    u16 unk0;
    u16 unk2;
    DLL_Unknown* unk4;
    u8 unk8;
} Unk_DLL2_BSS120;

/*0x0*/ static UnkStructDLL2_bss_11C _bss_0;
/*0x11C*/ static UnkStructDLL2_bss_11C* _bss_11C;   //ptr to _bss_0
/*0x120*/ static Unk_DLL2_BSS120* _bss_120[20];
/*0x170*/ static u8 _bss_170;
/*0x174*/ static Unk_DLL2_BSS120* _bss_174;
/*0x178*/ static s32 _bss_178;
/*0x17C*/ static s32 _bss_17C;
/*0x180*/ static s32 _bss_180;
/*0x184*/ static s32 _bss_184;
/*0x188*/ static s32 _bss_188;
/*0x18C*/ static void* _bss_18C;
/*0x190*/ static u8 _bss_190;
/*0x191*/ static s8 _bss_191;
/*0x192*/ static s8 _bss_192;
/*0x194*/ static s32 _bss_194;
/*0x198*/ static u8 _bss_198;
/*0x19C*/ static s32 _bss_19C;
/*0x1A0*/ static s32 _bss_1A0;
/*0x1A4*/ static s32 _bss_1A4;
/*0x1A8*/ static f32 _bss_1A8;
/*0x1AC*/ static f32 _bss_1AC;
/*0x1B0*/ static f32 _bss_1B0;
/*0x1B4*/ static f32 _bss_1B4;
/*0x1B8*/ static f32 _bss_1B8;
/*0x1BC*/ static f32 _bss_1BC;
/*0x1C0*/ static f32 _bss_1C0;
/*0x1C4*/ static s32 _bss_1C4;
/*0x1C8*/ static s8 _bss_1C8;
/*0x1C9*/ static s8 _bss_1C9;
/*0x1CA*/ static s8 _bss_1CA;
/*0x1CC*/ static s16 _bss_1CC; //rotate speed
/*0x1D0*/ static Object* _bss_1D0; //OBJ_LockIcon
/*0x1D4*/ static s16 _bss_1D4;  //letterbox height
// /*0x1D8*/ static u8 _bss_1D8[0x8]; //pad?

Unk_DLL2_BSS120* dll_2_func_5D0(void);
void dll_2_func_630(s32 arg0, s32 arg1, s32 arg2, s32 arg3, void* arg4, s32 arg5, u8 arg6);
CameraAction* dll_2_func_804(s32 index);
static s32 dll_2_func_15C4(u16 arg0);
static void dll_2_func_1630(s32 arg0);
static s32 dll_2_func_16C8(u16 arg0, s32 arg1);
static void dll_2_func_1770(UnkStructDLL2_bss_11C* arg0);
static void dll_2_func_1BD0(u16 arg0, s32 arg1);
static void dll_2_func_1D94(Object* arg0);
static void dll_2_func_1DF0(Object* obj);
static void dll_2_func_1E64(UnkStructDLL2_bss_11C* arg0, Object* arg1);
static Object* dll_2_func_1F8C(UnkStructDLL2_bss_11C* arg0, Object* arg1);
static void dll_2_func_236C(void);
static void dll_2_func_23D0(Object* arg0, s32 arg1, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols);

// offset: 0x0 | ctor
void dll_2_ctor(void* dll) {
    _bss_11C = &_bss_0;
    bzero(_bss_11C, sizeof(UnkStructDLL2_bss_11C));
    
    voxmap_init();
    _bss_178 = -1;
    _bss_17C = -1;
    _bss_180 = -1;
    _bss_1C4 = 0;
    _bss_1C8 = -1;
    
    dll_2_func_236C();
}

// offset: 0xA0 | dtor
void dll_2_dtor(void* dll) {
    voxmap_free();
    _bss_1C8 = -1;
}

// offset: 0xE4 | func: 0 | export: 0
void dll_2_func_E4(Object* arg0, f32 x, f32 y, f32 z) {
    bzero(_bss_11C, sizeof(UnkStructDLL2_bss_11C));
    _bss_11C->unk0.transl.x = x;
    _bss_11C->unk0.transl.y = y;
    _bss_11C->unk0.transl.z = z;
    _bss_11C->unk18.x = x;
    _bss_11C->unk18.y = y;
    _bss_11C->unk18.z = z;
    _bss_11C->unkA4.x = x;
    _bss_11C->unkA4.y = y;
    _bss_11C->unkA4.z = z;
    _bss_11C->unkA0 = arg0;
}

// offset: 0x1A4 | func: 1 | export: 1
void dll_2_func_1A4(void) {
    Object* obj;
    f32 var_fv0;
    u8 sp3F;

    if (menu_get_current() == 4) {
        sp3F = 1;
    } else {
        sp3F = 0;
    }
    
    obj = _bss_11C->unkA0;
    if (obj == NULL) {
        return;
    }
    
    func_80008D90(obj->parent);
    dll_2_func_1D94(obj);
    dll_2_func_1E64(_bss_11C, obj);
    
    if (_bss_11C->unk117 != 0) {
        obj->srt.transl.x = _bss_11C->unkCC;
        obj->srt.transl.y = _bss_11C->unkD0;
        obj->srt.transl.z = _bss_11C->unkD4;
        obj->positionMirror.x = _bss_11C->unkCC;
        obj->positionMirror.y = _bss_11C->unkD0;
        obj->positionMirror.z = _bss_11C->unkD4;
        _bss_11C->unk117 = 0;
    }
    
    if (obj->parent != NULL) {
        transform_point_by_object(
            obj->srt.transl.x, obj->srt.transl.y, obj->srt.transl.z, 
            &obj->srt.transl.x, &obj->srt.transl.y, &obj->srt.transl.z, 
            obj->parent
        );
        obj->srt.yaw += obj->parent->srt.yaw;
    }
    
    if (_bss_190 != 0) {
        if (_bss_194 >= 2) {
            var_fv0 = 1.0f / _bss_194;
            if ((var_fv0 <= 0.0f) || (var_fv0 > 1.0f)) {
                var_fv0 = 1.0f;
            }
            _bss_11C->unkE4 = 1.0f;
            _bss_11C->unkE8 = var_fv0;
            _bss_11C->unk119 = _bss_198;
        } else {
            _bss_11C->unkE4 = 0.0f;
            _bss_11C->unk119 = 0U;
        }
        
        if (_bss_11C->unkE4 == 1.0f) {
            _bss_11C->unkFC = _bss_11C->unk0.transl.x;
            _bss_11C->unk100 = _bss_11C->unk0.transl.y;
            _bss_11C->unk104 = _bss_11C->unk0.transl.z;
            _bss_11C->unkF6 = _bss_11C->unk0.yaw;
            _bss_11C->unkF8 = _bss_11C->unk0.pitch;
            _bss_11C->unkFA = _bss_11C->unk0.roll;
            _bss_11C->unk108 = _bss_11C->unkB0;
        }
        
        _bss_19C = _bss_178;
        _bss_1A0 = _bss_184;
        _bss_1A4 = _bss_188;
        dll_2_func_1BD0(_bss_180, _bss_18C);
        _bss_190 = 0;
        
        if (_bss_18C != 0) {
            mmFree(_bss_18C);
            _bss_18C = 0;
        }
    }

    if (_bss_174 != NULL) {
        _bss_174->unk4->vtbl->func[1].withOneArg(_bss_11C);
        dll_2_func_1770(_bss_11C);
    }

    if (sp3F == 0) {
        if (_bss_11C->unk10C == 0) {
            _bss_11C->unk110 = dll_2_func_1F8C(_bss_11C, obj);
        }
        
        if (_bss_178 != 0x55) {
            _bss_11C->unk11B &= ~1;
        }
    }
    
    _bss_11C->unkA4.x = _bss_11C->unk0.transl.x;
    _bss_11C->unkA4.y = _bss_11C->unk0.transl.y;
    _bss_11C->unkA4.z = _bss_11C->unk0.transl.z;
    _bss_11C->unk11A = 0;
    dll_2_func_1DF0(obj);
}

// offset: 0x57C | func: 2 | export: 2
UnkStructDLL2_bss_11C* dll_2_func_57C(void) {
    return _bss_11C;
}

// offset: 0x598 | func: 3 | export: 3
s32 dll_2_func_598(void) {
    return _bss_178;
}

// offset: 0x5B4 | func: 4 | export: 4
Unk_DLL2_BSS120* dll_2_func_5B4(void) {
    return _bss_174;
}

// offset: 0x5D0 | func: 5 | export: 5
Unk_DLL2_BSS120* dll_2_func_5D0(void) {
    s32 i;

    for (i = 0; i < _bss_170; i++){
        if (_bss_120[i]->unk0 == 0x54) {
            return _bss_120[i];
        }
    }
    
    return NULL;
}

// offset: 0x630 | func: 6 | export: 6
void dll_2_func_630(s32 arg0, s32 arg1, s32 arg2, s32 arg3, void* arg4, s32 arg5, u8 arg6) {
    void* sp24;

    sp24 = _bss_18C;
    if (sp24 != 0) {
        mmFree(sp24);
        _bss_18C = 0;
        _bss_190 = 0;
    }
    
    _bss_180 = arg0;
    _bss_194 = arg5;
    
    if (arg4 != 0) {
        _bss_18C = mmAlloc(arg3, ALLOC_TAG_CAM_COL, 0);
        bcopy(arg4, _bss_18C, arg3);
    } else {
        _bss_18C = 0;
    }
    
    _bss_192 = arg1;
    _bss_191 = arg2;
    _bss_190 = 1;
    _bss_198 = arg6;
}

// offset: 0x738 | func: 7 | export: 26
void dll_2_func_738(s32 arg0, u8 arg1) {
    if (_bss_19C != -1) {
        dll_2_func_630(_bss_19C, _bss_1A0, _bss_1A4, 0, 0, arg0, arg1);
    }
}

// offset: 0x7B4 | func: 8 | export: 22
void dll_2_func_7B4(void) {
    if (_bss_174 != NULL) {
        _bss_174->unk4->vtbl->func[3].asVoid();
    }
}

// offset: 0x804 | func: 9 | export: 7
CameraAction* dll_2_func_804(s32 index) {
    void* camAction;

    if (index == 0) {
        return 0;
    }
    index--;
    
    camAction = mmAlloc(sizeof(CameraAction), ALLOC_TAG_CAM_COL, 0);
    queue_load_file_region_to_ptr(
        camAction, 
        CAMACTIONS_BIN, 
        index * sizeof(CameraAction), 
        sizeof(CameraAction)
    );
    return camAction;
}

typedef struct {
    s32 unk0;
    s8 unk4;
} Unk_DLL2_Func888;

// offset: 0x888 | func: 10 | export: 8
void dll_2_func_888(u32 arg0, s32 arg1) {
    Unk_DLL2_Func888 sp50;
    Unk_DLL2_Func888 sp48;
    CameraAction* sp44;
    s32 pad[2];
    
    switch (arg0) {
    case 1:
        sp50.unk0 = arg1 & 0x7F;
        sp50.unk4 = arg1 & 0x80;
        dll_2_func_630(0x5A, 1, 0, 8, &sp50, 0x78, 0xFF);
        break;
    case 2:
        sp48.unk0 = arg1 & 0x7F;
        sp48.unk4 = arg1 & 0x80;
        dll_2_func_630(0x59, 1, 0, 8, &sp48, 0x78, 0xFF);
        break;
    case 3:
        dll_2_func_630(0x54, 0, 1, 0, NULL, 0x78, 0xFF);
        break;
    case 4:
        dll_2_func_630(arg1 + 0x54, 1, 0, 0, NULL, 0x78, 0xFF);
        break;
    case 0:
    default:
        if (arg1 != 0) {
            sp44 = dll_2_func_804(arg1);
        }
        if (sp44 != NULL) {
            gDLL_29_Gplay->vtbl->func_F60()->unk12 = arg1;
            if ((_bss_178 == 0x5B) || (_bss_178 == 0x5E)) {
                dll_2_func_5D0()->unk4->vtbl->func[3].withTwoArgs((s32)sp44, 0x10); //TO-DO: interface
            } else {
                if ((sp44->unk0 == 0) || (sp44->unk0 != 1)) {
                    dll_2_func_630(0x54, 0, 2, 0x10, sp44, 0, 0xFF);
                } else {
                    dll_2_func_630(0x5D, 1, 2, 0x10, sp44, 0, 0xFF);
                }
            }
            mmFree(sp44);
        }
        break;
    }
}

// offset: 0xB08 | func: 11 | export: 9
void dll_2_func_B08(Object* arg0, s32 arg1) {
    if (arg0 != _bss_11C->unkA0) {
        _bss_11C->unkA0 = arg0;
    }
}

// offset: 0xB38 | func: 12 | export: 10
void dll_2_func_B38(f32 x, f32 y, f32 z) {
    _bss_11C->unk117 = 1;
    _bss_11C->unkCC = x;
    _bss_11C->unkD0 = y;
    _bss_11C->unkD4 = z;
}

// offset: 0xB80 | func: 13 | export: 11
void dll_2_func_B80(f32 dx, f32 dy, f32 dz) {
    _bss_11C->unk0.transl.x += dx;
    _bss_11C->unk0.transl.y += dy;
    _bss_11C->unk0.transl.z += dz;
}

// offset: 0xBD4 | func: 14 | export: 12
void dll_2_func_BD4(f32 arg0, Vec4f* arg1, f32 arg2, f32 arg3, f32 y, f32 w) {
    f32 temp;

    temp = arg0 / arg2;
    if (temp < arg3) {
        temp = arg3;
    }
    
    arg1->x = temp;
    arg1->y = y;
    arg1->z = 0.0f;
    arg1->w = w;
}

// offset: 0xC28 | func: 15 | export: 13
void dll_2_func_C28(Object* arg0, f32* dx, f32* dy, f32* dz, f32* distance2D, f32 yOffset) {
    Object* temp_v0;

    temp_v0 = _bss_11C->unkA0;
    
    *dx = arg0->srt.transl.x - temp_v0->srt.transl.x;
    *dy = arg0->srt.transl.y - (temp_v0->srt.transl.y + yOffset);
    *dz = arg0->srt.transl.z - temp_v0->srt.transl.z;
    
    if (distance2D != NULL) {
        *distance2D = SQ(*dx) + SQ(*dz);
        if (*distance2D > 0.0f) {
            *distance2D = sqrtf(*distance2D);
        }

        //Clamp min value to 5
        if (*distance2D < 5.0f) {
            *distance2D = 5.0f;
        }
    }
}

// offset: 0xD10 | func: 16 | export: 14
Object* dll_2_func_D10(void) {
    return _bss_11C->unk10C;
}


// offset: 0xD30 | func: 17 | export: 15
Object* dll_2_func_D30(void) {
    return _bss_11C->unk110;
}

// offset: 0xD50 | func: 18 | export: 17
void dll_2_func_D50(Object* target) {
    _bss_11C->unk10C = target;
    _bss_11C->unk110 = target;
}

// offset: 0xD7C | func: 19 | export: 18
s32 dll_2_func_D7C(void) {
    return (_bss_11C->unkE4 > 0.0f);
}

// offset: 0xDB8 | func: 20 | export: 19
void dll_2_func_DB8(void) {
    Object* target;
    Object* arrow; //sp 38
    Object* player; //sp 34
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distSquared;
    s32 tempOpacity;
    u8 targetFlags;
    f32 temp;
    ObjectStruct74* targetCoords;

    arrow = _bss_1D0;
    target = _bss_11C->unk110;
    player = get_player();
    
    if (menu_get_current() == 4) {
        return;
    }
    
    _bss_11C->unk11B &= ~4;
    
    if ((player == NULL) || (player->unkB0 & 0x1000)) {
        _bss_1CA = 0;
        return;
    }
    
    if (target != NULL) {
        //Get distance between player and lock-on point?
        distSquared = target->unk78[target->unkD4].unk0 * 4;
        distSquared *= distSquared;

        targetCoords = &target->unk74[target->unkD4];
        dx = player->positionMirror.x - targetCoords->unkC.x;
        dz = player->positionMirror.z - targetCoords->unkC.z;
        
        if (target->unkAF & 0x80) {
            dy = 0.0f;
        } else {
            dy = player->positionMirror.y - targetCoords->unkC.y;
        }
        
        
        if ((SQ(dx) + SQ(dz) < distSquared) && ((dy > -100.0f) && (dy < 100.0f))) {
            target->unkAF = target->unkAF | 4;
            _bss_11C->unk11B |= 4;
        }
        
        if (target->unkAF & 4) {
            if (!(target->unkAF & 0x10)){
                if (joy_get_pressed(0) & A_BUTTON) {
                target->unkAF |= 1;
                }
            } else if (joy_get_pressed(0) & A_BUTTON) {
                gDLL_6_AMSFX->vtbl->play_sound(target, 0x6E6, MAX_VOLUME, 0, 0, 0, 0);
            }
        }
    }
    
    switch (_bss_1CA) {
    case 0:
    default:
        _bss_1CC = 0;
        arrow->opacity = 0xFF;
        if (target != NULL) {
            _bss_1CA = 2;
        }
        break;
    case 2:
        _bss_1CC = 0;
        arrow->opacity = 0xFF;
        _bss_1CA = 3;
        gDLL_6_AMSFX->vtbl->play_sound(target, 0x43C, MAX_VOLUME, 0, 0, 0, 0);
        break;
    case 3:
        if ((target == NULL) || (_bss_178 == 0x56) || (target->unkAF & 0x28)) {
            gDLL_6_AMSFX->vtbl->play_sound(target, 0x72E, MAX_VOLUME, 0, 0, 0, 0);
            _bss_1CA = 1;
        } else {
            if (_bss_178 == 0x5B) {
                gDLL_6_AMSFX->vtbl->play_sound(target, 0x72D, MAX_VOLUME, 0, 0, 0, 0);
                _bss_1CA = 4;
                _bss_1C9 = 0x3C;
            }
            
            if (_bss_1CC > 0) {
                _bss_1CC -= gUpdateRate * 8;
            }
            break;
        }
        break;
    case 4:
        if ((target == NULL) || (target->unkAF & 0x28)) {
            gDLL_6_AMSFX->vtbl->play_sound(target, 0x72E, MAX_VOLUME, 0, 0, 0, 0);
            _bss_1CA = 1;
            break;
        }
        
        if (_bss_178 != 0x5B) {
            _bss_1CA = 3;
        }
        
        target->unkAF |= 2;
        
        if (_bss_1C9 > 0) {
            _bss_1C9 -= gUpdateRate;
            _bss_1CC = 0x400;
        } else {
            if (_bss_1CC > 0x200) {
                _bss_1CC -= gUpdateRate * 8;
            }
            if (_bss_1CC < 0x200) {
                _bss_1CC = 0x200;
            }
        }
        break;
    case 1:
        tempOpacity = arrow->opacity - (gUpdateRate * 0xC);
        if (tempOpacity < 0) {
            tempOpacity = 0;
        }
        arrow->opacity = tempOpacity;
        
        if (arrow->opacity > 0) {
            _bss_1C9 -= gUpdateRate;
            arrow->srt.transl.y += 2.0f * gUpdateRateF;
            arrow->positionMirror.y += 2.0f * gUpdateRateF;
        } else {
            _bss_1CA = 0;
        }
        if (target != NULL) {
            _bss_1CA = 2;
        }
        break;
    }
    
    if (_bss_1CC < 0) {
        _bss_1CC = 0;
    }
    
    arrow->srt.yaw += _bss_1CC * gUpdateRate;
}

// offset: 0x1408 | func: 21 | export: 20
void dll_2_func_1408(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols) {
    if (menu_get_current() != 4) {
        _data_0[0] = -1;
        dll_2_func_23D0(_bss_11C->unk110, _bss_178 == 0x5B, gdl, mtxs, vtxs, pols);
    }
}

// offset: 0x14A8 | func: 22 | export: 21
void dll_2_func_14A8(s32 arg0, s32 arg1) {
    if (_bss_11C->unk115 < arg0) {
        _bss_11C->unk115 = arg0;
        _bss_11C->unk116 = 1;
        
        if (arg1 != 0) {
            camera_set_letterbox(arg0);
        }
    }
}

// offset: 0x1520 | func: 23 | export: 24
void dll_2_func_1520(s32 arg0) {
    _bss_11C->unk11A |= arg0;
}

// offset: 0x1548 | func: 24 | export: 16
void dll_2_func_1548(s32 arg0) {
    if (arg0 != 0) {
        _bss_11C->unk11B |= 2;
        return;
    }
    _bss_11C->unk11B &= ~2;
}

// offset: 0x1594 | func: 25 | export: 25
void dll_2_func_1594(s32 arg0) {
    _bss_11C->unk11B |= (arg0 * 8) & 0x18;
}

// offset: 0x15C4 | func: 26
s32 dll_2_func_15C4(u16 arg0) {
    s32 i;

    for (i = 0; i < _bss_170; i++){
        if (_bss_120[i]->unk0 == arg0) {
            return i;
        }
    }
    
    return -1;
}

// offset: 0x1630 | func: 27
void dll_2_func_1630(s32 arg0) {
    dll_unload(_bss_120[arg0]->unk4);
    mmFree(_bss_120[arg0]);
    _bss_120[arg0] = _bss_120[_bss_170 - 1];
    _bss_170--;
}

// offset: 0x16C8 | func: 28
s32 dll_2_func_16C8(u16 arg0, s32 arg1) {
    Unk_DLL2_BSS120 *new_var;
    Unk_DLL2_BSS120 *temp_v0;
    
    temp_v0 = mmAlloc(sizeof(Unk_DLL2_BSS120), ALLOC_TAG_CAM_COL, 0);
    
    _bss_120[_bss_170] = temp_v0;
    _bss_170++;
    
    new_var = temp_v0;
    new_var->unk0 = arg0;
    new_var->unk8 = arg1;
    new_var->unk4 = dll_load_deferred(arg0, 4);
    return _bss_170 - 1;
}

// offset: 0x1770 | func: 29
static void dll_2_func_1770(UnkStructDLL2_bss_11C* arg0) {
    Camera* camera;
    f32 tValue;
    Vec4f sp28;

    set_camera_selector(0);
    camera = get_main_camera();
    camera->srt.yaw = arg0->unk0.yaw;
    camera->srt.pitch = arg0->unk0.pitch;
    camera->srt.roll = arg0->unk0.roll;
    camera->srt.transl.x = arg0->unk0.transl.x;
    camera->srt.transl.y = arg0->unk0.transl.y;
    camera->srt.transl.z = arg0->unk0.transl.z;
    
    _bss_1C0 = arg0->unkB0;
    if (arg0->unkE4 > 0.0f) {
        arg0->unkE4 -= arg0->unkE8 * gUpdateRateF;

        //Get Bezier-eased tValue for interpolation
        sp28.w = 0.0f;
        sp28.z = 0.0f;
        sp28.x = 0.0f;
        sp28.y = 1.0f;
        tValue = 1.0f - func_80004C5C(&sp28, arg0->unkE4, 0);

        //Linear interpolation (position)
        if (arg0->unk119 & 8) {
            camera->srt.transl.x = arg0->unkFC + (camera->srt.transl.x - arg0->unkFC)*tValue;
        }
        if (arg0->unk119 & 0x10) {
            camera->srt.transl.y = arg0->unk100 + (camera->srt.transl.y - arg0->unk100)*tValue;
        }
        if (arg0->unk119 & 0x20) {
            camera->srt.transl.z = arg0->unk104 + (camera->srt.transl.z - arg0->unk104)*tValue;
        }
        
        //Linear interpolation (rotation)
        if (arg0->unk119 & 1) {
            arg0->unkF0 = (arg0->unkF6) - (camera->srt.yaw & 0xFFFF);
            CIRCLE_WRAP(arg0->unkF0)
            camera->srt.yaw = (arg0->unkF6) - ((s16)(arg0->unkF0 * tValue));
        }
        if (arg0->unk119 & 2) {
            arg0->unkF2 = (arg0->unkF8) - (camera->srt.pitch & 0xFFFF);
            CIRCLE_WRAP(arg0->unkF2)
            camera->srt.pitch = (arg0->unkF8) - ((s16)(arg0->unkF2 * tValue));
        }
        if (arg0->unk119 & 4) {
            arg0->unkF4 = (arg0->unkFA) - (camera->srt.roll & 0xFFFF);
            CIRCLE_WRAP(arg0->unkF4)
            camera->srt.roll = (arg0->unkFA) - ((s16)(arg0->unkF4 * tValue));
        }
    }
    
    if (camera_get_fov() != _bss_1C0) {
        camera_set_fov(_bss_1C0);
    }
    
    update_camera_for_object(camera);
    map_func_80046B58(camera->tx, camera->ty, camera->tz);

    //Update camera letterboxing
    _bss_1D4 = camera_get_letterbox();
    if (_bss_1D4 != arg0->unk115) {
        if (_bss_1D4 < arg0->unk115) {
            _bss_1D4 += arg0->unk116 * (s32) gUpdateRateF;
            if (arg0->unk115 < _bss_1D4) {
                _bss_1D4 = arg0->unk115;
            }
        } else {
            _bss_1D4 -= arg0->unk116 * (s32) gUpdateRateF;
            if (_bss_1D4 < arg0->unk115) {
                _bss_1D4 = arg0->unk115;
            }
        }
        camera_set_letterbox(_bss_1D4);
    }
    
    arg0->unk115 = 0;
}

// offset: 0x1BD0 | func: 30
void dll_2_func_1BD0(u16 arg0, s32 arg1) {   
    if ((_bss_174 != NULL) && (_bss_178 != arg0)){
        ((DLL_Unknown*)_bss_174->unk4)->vtbl->func[2].withOneArg((s32)_bss_11C); //TO-DO: interface
        if (_bss_174->unk8 == 1){
            dll_2_func_1630(_bss_17C);
            _bss_174 = NULL;
            _bss_178 = -1;
            _bss_17C = -1;
        }
    }
    
    _bss_17C = dll_2_func_15C4(arg0);
    if (_bss_17C == -1) {
        _bss_17C = dll_2_func_16C8(arg0, _bss_192);
    }
    
    if (_bss_17C != -1) {
        _bss_174 = _bss_120[_bss_17C];
        _bss_178 = _bss_174->unk0;
        _bss_174->unk4->vtbl->func[0].withThreeArgs((s32)_bss_11C, _bss_191, arg1); //TO-DO: interface
    } else {
        _bss_174 = NULL;
        _bss_178 = -1;
    }
    
    _bss_184 = _bss_192;
    _bss_188 = _bss_191;
}

// offset: 0x1D94 | func: 31
void dll_2_func_1D94(Object* arg0) {
    _bss_1A8 = arg0->srt.transl.x;
    _bss_1AC = arg0->srt.transl.y;
    _bss_1B0 = arg0->srt.transl.z;
    _bss_1B4 = arg0->positionMirror.x;
    _bss_1B8 = arg0->positionMirror.y;
    _bss_1BC = arg0->positionMirror.z;
}

// offset: 0x1DF0 | func: 32
void dll_2_func_1DF0(Object* obj) {
    Object* parent = obj->parent;
    
    obj->srt.transl.x = _bss_1A8;
    obj->srt.transl.y = _bss_1AC;
    obj->srt.transl.z = _bss_1B0;
    obj->positionMirror.x = _bss_1B4;
    obj->positionMirror.y = _bss_1B8;
    obj->positionMirror.z = _bss_1BC;
    
    if (parent != NULL) {
        obj->srt.yaw -= parent->srt.yaw;
    }
}

// offset: 0x1E64 | func: 33
void dll_2_func_1E64(UnkStructDLL2_bss_11C* arg0, Object* arg1) {
    f32 speedMagnitude;
    s32 i;

    //Copying speed samples up one slot
    for (i = 0; i < (s32)ARRAYCOUNT(arg0->unkB8) - 1; i++) {
        arg0->unkB8[i] = arg0->unkB8[i + 1];
    }

    //Put absolute lateral speed into last slot
    speedMagnitude = SQ(arg1->speed.x) + SQ(arg1->speed.z);
    if (speedMagnitude > 0.0f) {
        speedMagnitude = sqrtf(speedMagnitude);
    }
    arg0->unkB8[4] = speedMagnitude;

    //Average the 5 speed samples
    arg0->unkB4 = 0.0f;
    for (i = 0; i < (s32)ARRAYCOUNT(arg0->unkB8); i++) {
        arg0->unkB4 += arg0->unkB8[i];
    }
    arg0->unkB4 *= 0.2f;
    
    if (arg0->unkB4 < 0.0f) {
        arg0->unkB4 = -arg0->unkB4;
    }
}

// offset: 0x1F8C | func: 34
Object* dll_2_func_1F8C(UnkStructDLL2_bss_11C* arg0, Object* arg1) {
    Object* objects[8];
    s32 matchCount;
    s32 i;
    s32 temp;
    s32 lockIndex;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 dYaw;
    Object* obj;
    f32 distSquared;
    s32 count;
    Vec3s16 sp74;
    Vec3s16 sp6C;
    ObjectStruct74* targetCoords;
    ObjectStruct78* targetDef;
    s32 angularRange;

    if (arg0->unk11A & 2) {
        return NULL;
    }
    
    count = gDLL_1_UI->vtbl->func5(objects, 8, 0xFF, 1, 1000.0f);

    for (i = 0, matchCount = 0; i < count; i++){
        obj = objects[i];

        if ((obj->opacity != 0) && !(obj->srt.flags & 0x4000) && !(obj->unkB0 & 0x40)) {
            if (obj->unkAF & 0x28){
                continue;
            }

            targetCoords = obj->unk74;
            targetDef = obj->unk78;
                
            if (obj->unkD4 < obj->def->unk9b) {
                lockIndex = obj->unkD4;
            } else {
                lockIndex = 0;
            }
                
            if (!targetCoords) {
                continue;
            }
            
            dx = arg1->positionMirror.x - targetCoords[lockIndex].unk0.x;
            dz = arg1->positionMirror.z - targetCoords[lockIndex].unk0.z;
            if (obj->unkAF & 0x80) {
                dy = 0.0f;
            } else {
                dy = arg1->positionMirror.y - targetCoords[lockIndex].unk0.y;
            }

            distSquared = targetDef[lockIndex].unk2 * 4;
            distSquared *= distSquared;
            
            if (((SQ(dx) + SQ(dz)) < distSquared) && (dy > -100.0f) && (dy < 100.0f)) {
                if (_bss_178 == 0x5B) {
                    angularRange = 0x8000;
                } else {
                    angularRange = (s16)(targetDef[lockIndex].unk3 * 182.04f);
                }

                dYaw = arg1->srt.yaw - (arctan2s(dx, dz) & 0xFFFF);
                CIRCLE_WRAP(dYaw)
                
                if ((dYaw < angularRange) && (-angularRange < dYaw)) {
                    if (targetDef[lockIndex].colourIndex & 0x20) {
                        func_80007EE0((Vec3f*)&targetCoords[lockIndex], &sp74);
                        func_80007EE0(&arg1->srt.transl, &sp6C);
                        sp6C.y += 2;
                        if (func_80008048(&sp74, &sp6C, 0, 0, 0) != 0) {
                            objects[matchCount] = obj;
                            matchCount++;
                        }
                    } else {
                        objects[matchCount] = obj;
                        matchCount++;
                    }
                }
            }
        }
    }

    if (arg0->unk114 >= matchCount) {
        arg0->unk114 = 0;
    }
    
    if (matchCount) {
        return objects[arg0->unk114];
    }

    return NULL;  
}

// offset: 0x236C | func: 35
void dll_2_func_236C(void) {
    _bss_1D0 = obj_create(
        obj_alloc_create_info(sizeof(ObjSetup), OBJ_LockIcon),
        0,
        -1,
        -1,
        0
    );
}

// offset: 0x23D0 | func: 36
void dll_2_func_23D0(Object* arg0, s32 arg1, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols) {
    Object *lockIcon;
    s32 colourIndex;
    u8 temp_a0;
    u8 textIdx;
    ObjectStruct74 *lockCoords;
    
    lockIcon = _bss_1D0;
    
    if ((lockIcon->opacity == 0) || !((_bss_1CA == 3) || (_bss_1CA == 4) || (_bss_1CA == 1))) {
        return;
    }
    
    if (arg0) {
        if (!arg0->unk74) {
            return;
        }
        
        lockCoords = &arg0->unk74[temp_a0 = arg0->unkD4]; //fake?
        temp_a0 = arg0->unk78[arg0->unkD4].colourIndex;
        colourIndex = ((temp_a0 & 0xF) & 0xFFFF) - 1;
        
        if (colourIndex < 0) {
            colourIndex = 0;
        }
        
        if ((arg0->unkAF & 0x10) || (!(_bss_11C->unk11B & 4))) {
            colourIndex += 6;
        }
        
        textIdx = arg0->unkD8;
        
        if (textIdx >= 4) {
            textIdx = 0;
        }
        
        _data_0[0] = arg0->def->gametextIndex[textIdx];
        lockIcon->positionMirror.x = lockCoords->unk0.x;
        lockIcon->positionMirror.y = lockCoords->unk0.y;
        lockIcon->positionMirror.z = lockCoords->unk0.z;
        lockIcon->modelInstIdx = colourIndex;
        lockIcon->parent = arg0->parent;
        
        if (lockIcon->parent != 0){
            inverse_transform_point_by_object(
                lockIcon->positionMirror.x, lockIcon->positionMirror.y, lockIcon->positionMirror.z, 
                &lockIcon->srt.transl.x, &lockIcon->srt.transl.y, &lockIcon->srt.transl.z, 
                lockIcon->parent
            );
        } else {
            lockIcon->srt.transl.x = lockIcon->positionMirror.x;
            lockIcon->srt.transl.y = lockIcon->positionMirror.y;
            lockIcon->srt.transl.z = lockIcon->positionMirror.z;
        }
    }
    
    lockIcon->srt.pitch = 0;
    lockIcon->srt.roll = 0;
    lockIcon->srt.scale = 0.08f;
    lockIcon->opacityWithFade = lockIcon->opacity;

    draw_object(lockIcon, gdl, mtxs, vtxs, pols, 1.0f);
    
    lockIcon->modelInsts[lockIcon->modelInstIdx]->unk34 &= ~8;
}

// offset: 0x25D4 | func: 37 | export: 23
s16 dll_2_func_25D4(void) {
    return _data_0[0];
}
