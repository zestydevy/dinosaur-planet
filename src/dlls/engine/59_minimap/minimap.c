#include "common.h"
#include "game/gamebits.h"
#include "macros.h"
#include "sys/map.h"
#include "sys/map_enums.h"
#include "sys/gfx/textable.h"

#define MINIMAP_SCREEN_X 50
#define MINIMAP_SCREEN_Y 200
#define NO_MAP_ID -1

/*0x0*/ static s32 sLoadedTexTableID = 0;           //Current map tile's index in TEXTABLE.bin (maps a texture in TEX0.bin)
/*0x4*/ static u8 sMinimapVisible = TRUE;
/*0x8*/ static s16 sOpacity = 0;
/*0xC*/ static Texture* sMapTile = NULL;            //current map texture
/*0x10*/ static Texture* sMarkerSidekick = NULL;    //green dot texture
/*0x14*/ static Texture* sMarkerPlayer = NULL;      //blue diamond texture
/*0x18*/ static s8 sOffsetX = 0;
/*0x1C*/ static s8 sOffsetY = 0;
/*0x20*/ static u8 sGridX = 0;
/*0x24*/ static u8 sGridZ = 0;
/*0x28*/ static s16 sLevelMaxX = 0;
/*0x2C*/ static s16 sLevelMaxZ = 0;
/*0x30*/ static s16 sLevelMinX = 0x7fff;
/*0x34*/ static s16 sLevelMinZ = 0x7fff;

typedef struct {
    s16 minX; //Section's bounds in worldSpace
    s16 maxX; //Section's bounds in worldSpace
    s16 minZ; //Section's bounds in worldSpace
    s16 maxZ; //Section's bounds in worldSpace
    s16 minY; //Section's bounds in worldSpace
    s16 maxY; //Section's bounds in worldSpace
    u16 gamebitSection; //For toggling a specific section of a map (e.g. deciding when to switch between the exterior/interior maps of LightFoot Village's ring fort)
    s8 screenOffsetX; //For repositioning map tiles' framing on screen
    s8 screenOffsetY; //For repositioning map tiles' framing on screen
    u16 texTableID; //Index into TEXTABLE.bin, used to find the texture's index in TEX0.bin
} MinimapSection;

typedef struct {
    MinimapSection* tiles;
    u16 gamebitLevel; //Overall gamebitID for having obtained the level's map (not checked)
    u8 mapID;
    u8 tileCount;
} MinimapLevel;

/*0x38*/ static MinimapSection sTilesVFPT[] = {
    {0xce00, 0xd080, 0xf300, 0xfd80, 0x8000, 0xfb1e, BIT_ALWAYS_1, 0, 0, TEXTABLE_3E0_MINIMAP_VFPT_Entrance_Area_Old,}, //Depicts an older version of the level design, related to Diamond Bay's numerous layout changes.
    {0xcb80, 0xce00, 0xf380, 0xfd80, 0x8000, 0xfb1e, BIT_ALWAYS_1, 0, 0, TEXTABLE_3E0_MINIMAP_VFPT_Entrance_Area_Old,}, //Placed one block east. May suggest all of VFPT was later moved one block west as part of Diamond Bay's changes.
    {0xc180, 0xcb80, 0xf600, 0x0000, 0x8000, 0xfb1e, BIT_ALWAYS_1, 0, 0, TEXTABLE_415_MINIMAP_VFPT_Lower_Floor}, //NOTE: shows that the scrapped lava path connected into the south side of the entrance hall.
    {0xc680, 0xd080, 0xf100, 0xf600, 0x8000, 0xfb1e, BIT_ALWAYS_1, 0, 0, TEXTABLE_416_MINIMAP_VFPT_Winding_Lava_Path}, //A scrapped section leading to/from the south of Diamond Bay. Its HITS still exist, and a single polygon from each of its BLOCKS.
    {0xc900, 0xd080, 0xf600, 0x0000, 0xfb1e, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_417_MINIMAP_VFPT_Upper_Floor_West},
    {0xc400, 0xc900, 0xf880, 0xfd80, 0xfb1e, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_418_MINIMAP_VFPT_Upper_Caldera},
    {0xbf00, 0xc400, 0xf600, 0x0000, 0xfb1e, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_419_MINIMAP_VFPT_Upper_Floor_East}
};
/*0xB8*/ static MinimapSection sTilesDIMExterior[] = {
    {0xe420, 0xe700, 0x0c80, 0x1180, 0x8000, 0xfb1e, BIT_ALWAYS_1, 0,   0, TEXTABLE_41E_MINIMAP_DIM_Snow_House_GuardClaw},
    {0xdf80, 0xe240, 0x0f00, 0x1180, 0xfa83, 0xfb14, BIT_ALWAYS_1, 0,   0, TEXTABLE_41F_MINIMAP_DIM_Snow_House_Bonfires_Upper}, //Slightly misplaced in-game, it's shown further east than it should be.
    {0xdf80, 0xe480, 0x0c80, 0x1180, 0x8000, 0xfa88, BIT_ALWAYS_1, 0,   0, TEXTABLE_420_MINIMAP_DIM_Snow_House_Bonfires_Lower},
    {0xdf80, 0xe3c0, 0x0780, 0x1400, 0x8000, 0x7fff, BIT_ALWAYS_1, 0,   0, TEXTABLE_41A_MINIMAP_DIM_Snow_Houses_Area}, //Positioned slightly too far south in-game, causing a small gap in the combined minimap beyond the river, and misalignment of the house entrances.
    {0xe3c0, 0xe700, 0x0780, 0x1338, 0x8000, 0x7fff, BIT_ALWAYS_1, 0,   0, TEXTABLE_41A_MINIMAP_DIM_Snow_Houses_Area},
    {0xda80, 0xe200, 0x1ac0, 0x1b80, 0x8000, 0x7fff, BIT_ALWAYS_1, -16, 0, TEXTABLE_41C_MINIMAP_DIM_Jetbike_Cave}, //Slightly misplaced in-game, positioned too far south of where it should be.
    {0xde40, 0xe0c0, 0x1a40, 0x1b80, 0x8000, 0x7fff, BIT_ALWAYS_1, -16, 0, TEXTABLE_41C_MINIMAP_DIM_Jetbike_Cave},
    {0xda80, 0xdf80, 0x1900, 0x1b80, 0x8000, 0x7fff, BIT_ALWAYS_1, -16, 0, TEXTABLE_41C_MINIMAP_DIM_Jetbike_Cave},
    {0xda80, 0xdd00, 0x1900, 0x1d80, 0x8000, 0x7fff, BIT_ALWAYS_1, -16, 0, TEXTABLE_41C_MINIMAP_DIM_Jetbike_Cave},
    {0xda80, 0xe200, 0x1900, 0x1a40, 0x8000, 0xfb6e, BIT_ALWAYS_1, -16, 0, TEXTABLE_41C_MINIMAP_DIM_Jetbike_Cave},
    {0xe080, 0xe200, 0x1900, 0x1b00, 0x8000, 0xfb96, BIT_ALWAYS_1, -16, 0, TEXTABLE_41C_MINIMAP_DIM_Jetbike_Cave},
    {0xe200, 0xe700, 0x1b13, 0x1e00, 0x8000, 0x7fff, BIT_ALWAYS_1, 0,   0, TEXTABLE_41D_MINIMAP_DIM_Blizzard_Plains_Entrance},
    {0xdf80, 0xe980, 0x1180, 0x1e00, 0x8000, 0x7fff, BIT_ALWAYS_1, 0,   5, TEXTABLE_41B_MINIMAP_DIM_Main_Area_and_Leap_of_Faith}
};
/*0x1A4*/ static MinimapSection sTilesDIMInterior[] = {
    {0xda80, 0xdd40, 0x1680, 0x1940, 0xf736, 0x7fff, BIT_ALWAYS_1, 0,  0, TEXTABLE_442_MINIMAP_DIM_Pressure_Switch_Room_Upper}, //Minimap tile is placed way to the south-east, and takes priority over the lower tile of the pressure switch room.
    {0xda80, 0xdf80, 0x1400, 0x1bc0, 0xf736, 0x7fff, BIT_ALWAYS_1, -1, 9, TEXTABLE_443_MINIMAP_DIM_Waterfall_Cave},
    {0xdf80, 0xe700, 0x1400, 0x2080, 0xf736, 0x7fff, BIT_ALWAYS_1, 0,  0, TEXTABLE_444_MINIMAP_DIM_Upper_Cavern},
    {0xdf08, 0xe688, 0x1400, 0x1b80, 0x8000, 0xf736, BIT_ALWAYS_1, 3,  0, TEXTABLE_447_MINIMAP_DIM_Lower_Cavern},
    {0xda80, 0xe200, 0x1180, 0x1b80, 0x8000, 0xf736, BIT_ALWAYS_1, 0,  0, TEXTABLE_445_MINIMAP_DIM_Pressure_Switch_Room_and_Dark_Passage}, //Despite its section being drawn, the minimap doesn't show up in the dark tunnel (probably intentionally!)
    {0xe200, 0xe980, 0x1180, 0x1b80, 0x8000, 0xf736, BIT_ALWAYS_1, 0,  0, TEXTABLE_446_MINIMAP_DIM_Chasm_and_Magma_Pit_and_Spiral_Descent}
};
/*0x210*/ static MinimapSection sTilesGaladon[] = {
    {0xdf80, 0xe480, 0x1b80, 0x2080, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_441_MINIMAP_DIM_Boss_Room}
};
/*0x224*/ static MinimapSection sTilesSW[] = {
    {0xe200, 0xec00, 0xfd80, 0x0780, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_448_MINIMAP_SW_Garunda_Te_Waterfall_Area},
    {0xee80, 0xf380, 0xfd80, 0x0280, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_44A_MINIMAP_SW_Geyser_and_Tricky_Tutorial},
    {0xec00, 0xf100, 0xfd80, 0x0780, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_449_MINIMAP_SW_Meandering_Path_Icy_Platforms_and_Tree_Bridge},
    {0xf380, 0xf880, 0xfd80, 0x0780, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_44B_MINIMAP_IM_Hot_Spring},
    {0xe700, 0xee80, 0x0780, 0x1180, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_44C_MINIMAP_SW_Ice_Cave_Krazoa_Shrine_Entrance} //Leads to the Test of Sacrifice. The tile shows some kind of snaking route which isn't present in the cave model - possibly an older layout.
};
/*0x280*/ static MinimapSection sTilesIMHotSpringA[] = {
    {0xf380, 0xf880, 0xfd80, 0x0780, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_44B_MINIMAP_IM_Hot_Spring}
};
/*0x294*/ static MinimapSection sTilesIMHotSpringB[] = {
    {0xf380, 0xf880, 0xfd80, 0x0780, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_44B_MINIMAP_IM_Hot_Spring} //has no mapID (unused duplicate?)
};
/*0x2A8*/ static MinimapSection sTilesCRF[] = {
    {0xc900, 0xd1f4, 0x4380, 0x4d80, 0x07d0, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_44F_MINIMAP_CRF_Skydock}, //Slightly misplaced in-game, east of where it really is.
    {0xd080, 0xd800, 0x4380, 0x4d80, 0x0708, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_450_MINIMAP_CRF_Courtyard},
    {0xd580, 0xdd00, 0x4380, 0x4d80, 0x8000, 0x0708, BIT_ALWAYS_1, 0, 0, TEXTABLE_451_MINIMAP_CRF_Entrance_Canyon},
    {0xcb80, 0xd080, 0x4600, 0x4d80, 0x8000, 0x07d0, BIT_ALWAYS_1, 0, 0, TEXTABLE_44D_MINIMAP_CRF_Treasure_Room_Tunnels},
    {0xd080, 0xd580, 0x4b00, 0x5000, 0x8000, 0x0708, BIT_ALWAYS_1, 0, 0, TEXTABLE_44E_MINIMAP_CRF_Dungeon}
};
/*0x304*/ static MinimapSection sTilesSC[] = {
    {0xf880, 0x0000, 0x3c00, 0x4880, 0x8000, 0x7fff, BIT_ALWAYS_1,                               0, 0, TEXTABLE_452_MINIMAP_SC_SwapStone_Area},
    {0x0000, 0x0500, 0x4380, 0x4880, 0x8000, 0x7fff, BIT_MINIMAP_LightFoot_Village_Inside_Mound, 0, 0, TEXTABLE_454_MINIMAP_SC_LightFoot_Village_Inside_Mound},
    {0x0000, 0x0500, 0x4100, 0x4880, 0x8000, 0x7fff, BIT_ALWAYS_1,                               0, 0, TEXTABLE_453_MINIMAP_SC_LightFoot_Village}
};
/*0x33C*/ static MinimapSection sTilesSH[] = {
    {0xdd00, 0xe480, 0xf100, 0xf880, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_455_MINIMAP_SH_River_Crossing_and_Walled_City_Gateway},
    {0xdd00, 0xe480, 0xf880, 0x0000, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_457_MINIMAP_SH_Tree_Hollow_Exterior},
    {0xe480, 0xee80, 0xfa80, 0xfd80, 0x8000, 0xfd94, BIT_ALWAYS_1, 0, 0, TEXTABLE_458_MINIMAP_SH_Burrows}, //Shows that the burrows once exited onto Willow Grove! (Corroborated by leftover HITS lines.)
    {0xe840, 0xee80, 0xf380, 0xfd80, 0x8000, 0xfd76, BIT_ALWAYS_1, 0, 0, TEXTABLE_458_MINIMAP_SH_Burrows},
    {0xe480, 0xec00, 0xf100, 0xfd80, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_456_MINIMAP_SH_SwapStone_Pond}
};
/*0x398*/ static MinimapSection sTilesSHWell[] = {
    {0xdf80, 0xe200, 0xf880, 0xfb00, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_459_MINIMAP_SH_Well_Upper_Floor},
    {0xdf80, 0xe480, 0xf600, 0xfb00, 0xfc36, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_459_MINIMAP_SH_Well_Upper_Floor},
    {0xdf80, 0xe480, 0xf600, 0x0000, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_45A_MINIMAP_SH_Well_Lower_Floor}
};
/*0x3D0*/ static MinimapSection sTilesDF[] = {
    {0xf600, 0xf880, 0x4100, 0x4380, 0x00a0, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_552_MINIMAP_DF_Cave_Burrows_Upper},
    {0xf600, 0xf880, 0x4100, 0x4380, 0x8000, 0x00a0, BIT_ALWAYS_1, 0, 0, TEXTABLE_45F_MINIMAP_DF_Cave_Burrows_Lower},
    {0xee80, 0xf600, 0x4380, 0x4600, 0x8000, 0x01e0, BIT_ALWAYS_1, 0, 0, TEXTABLE_45D_MINIMAP_DF_Lower_Falls},
    {0xef27, 0xf600, 0x4380, 0x4600, 0x01e0, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_45D_MINIMAP_DF_Lower_Falls},
    {0xf100, 0xf600, 0x4100, 0x4380, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_45D_MINIMAP_DF_Lower_Falls},
    {0xee80, 0xf600, 0x4600, 0x4d80, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_45D_MINIMAP_DF_Lower_Falls},
    {0xe980, 0xf380, 0x3e80, 0x4600, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_45C_MINIMAP_DF_Upper_Falls_Shrine_and_Whirlpool_Cave},
    {0xf600, 0xf880, 0x4600, 0x4d80, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_45E_MINIMAP_DF_Cascading_Approach},
    {0xf600, 0xf880, 0x4380, 0x4600, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_464_MINIMAP_DF_Toxic_Cave}
};
/*0x474*/ static MinimapSection sTilesCRFTrack[] = {
    {0xc900, 0xd080, 0x4380, 0x5000, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_460_MINIMAP_CRF_Track_Entrance},
    {0xc180, 0xc900, 0x4380, 0x5000, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_461_MINIMAP_CRF_Track_Loops}
};
/*0x498*/ static MinimapSection sTilesDRTop[] = {
    {0xee80, 0xf600, 0xdd00, 0xe200, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_501_MINIMAP_DR_CloudRunner_Prison},
    {0xee80, 0xfb00, 0xe200, 0xe700, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_502_MINIMAP_DR_Plains_South_East},
    {0xfb00, 0x0500, 0xe200, 0xe700, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_503_MINIMAP_DR_Plains_South_West},
    {0xee80, 0xfb00, 0xe700, 0xf100, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_504_MINIMAP_DR_Plains_North_East_and_HighTop_Prison},
    {0xfb00, 0x0500, 0xe700, 0xee80, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_505_MINIMAP_DR_Plains_North_West}
};
/*0x4F4*/ static MinimapSection sTilesDRBottom[] = {
    {0xf880, 0xfd80, 0xe480, 0xe688, 0xf9ca, 0x7fff, BIT_ALWAYS_1, 0, -3, TEXTABLE_508_MINIMAP_DR_Bottom_Central_Mine_Mezzanine},
    {0xf600, 0xfd80, 0xdf80, 0xec00, 0x8000, 0x7fff, BIT_ALWAYS_1, 0,  0, TEXTABLE_506_MINIMAP_DR_Bottom_Central_Mine},
    {0xfd80, 0x0a00, 0xe480, 0xec00, 0x8000, 0x7fff, BIT_ALWAYS_1, 0,  0, TEXTABLE_507_MINIMAP_DR_Bottom_Quarry_and_Skeetla_Cave}
};
/*0x52C*/ static MinimapSection sTilesGP[] = {
    {0xfb00, 0x0000, 0x3200, 0x3700, 0x8000, 0xfeb6, BIT_ALWAYS_1, 0, 0, TEXTABLE_50C_MINIMAP_GP_Krazoa_Shrine},
    {0xfb00, 0x0000, 0x3200, 0x3500, 0x8000, 0x014a, BIT_ALWAYS_1, 0, 0, TEXTABLE_50C_MINIMAP_GP_Krazoa_Shrine},
    {0xf880, 0xfd80, 0x2f80, 0x3c00, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_509_MINIMAP_GP_East_Oasis_and_Sand_Arches},
    {0xfd80, 0x0500, 0x2f80, 0x3c00, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_50A_MINIMAP_GP_West_Dunes_and_Bonfires},
    {0x0280, 0x0780, 0x2a80, 0x3200, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_50B_MINIMAP_GP_DFPT_Entrance}
};
/*0x588*/ static MinimapSection sTilesBWC[] = {
    {0x0000, 0x0500, 0x4880, 0x4d80, 0x000a, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_50D_MINIMAP_BWC_Entrance_LightFoot_Village},
    {0x0000, 0x0280, 0x4880, 0x4d80, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_50D_MINIMAP_BWC_Entrance_LightFoot_Village},
    {0xfd80, 0x0500, 0x4880, 0x5000, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_50E_MINIMAP_BWC_Entrance_Rib_Cage_River},
    {0xec00, 0xf380, 0x4d80, 0x5280, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_514_MINIMAP_BWC_River_of_the_Dead_Start},
    {0xec00, 0xf880, 0x4d80, 0x5280, 0x8000, 0xff9c, BIT_ALWAYS_1, 0, 0, TEXTABLE_514_MINIMAP_BWC_River_of_the_Dead_Start},
    {0xf380, 0xfd80, 0x4880, 0x5280, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_50F_MINIMAP_BWC_Tree_of_Life_and_Bone_Serpent},
    {0xfd80, 0x0500, 0x5000, 0x5a00, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_510_MINIMAP_BWC_Tomb_and_Boss_Room},
    {0xf880, 0xfd80, 0x5280, 0x5780, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_511_MINIMAP_BWC_River_of_the_Dead_Rapids},
    {0xf380, 0xfd80, 0x5280, 0x5500, 0x8000, 0x008c, BIT_ALWAYS_1, 0, 0, TEXTABLE_511_MINIMAP_BWC_River_of_the_Dead_Rapids},
    {0xf100, 0xfd80, 0x5500, 0x5780, 0x8000, 0x0000, BIT_ALWAYS_1, 0, 0, TEXTABLE_511_MINIMAP_BWC_River_of_the_Dead_Rapids},
    {0xf100, 0xf880, 0x5280, 0x5a00, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_512_MINIMAP_BWC_River_of_the_Dead_Loop},
    {0xec00, 0xf100, 0x5280, 0x5780, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_513_MINIMAP_BWC_Tree_Trunk_Spiral}
};
/*0x660*/ static MinimapSection sTilesKP[] = {
    {0x1180, 0x1e00, 0x5780, 0x5f00, 0x8000, 0x0384, BIT_ALWAYS_1, 0, 0, TEXTABLE_515_MINIMAP_KP_Central_Atrium_and_Lift_Corridor},
    {0x1180, 0x1b80, 0x5000, 0x5780, 0x8000, 0x0190, BIT_ALWAYS_1, 0, 0, TEXTABLE_516_MINIMAP_KP_Elemental_Reactor_SE_Magic}, 
    {0x1e00, 0x2300, 0x5000, 0x5a00, 0x8000, 0x0190, BIT_ALWAYS_1, 0, 0, TEXTABLE_517_MINIMAP_KP_Elemental_Reactor_SW_Fire},
    {0x1e00, 0x2580, 0x5000, 0x5780, 0x8000, 0x0190, BIT_ALWAYS_1, 0, 0, TEXTABLE_517_MINIMAP_KP_Elemental_Reactor_SW_Fire}, //duplicate, positioned slightly more south-west
    {0x1180, 0x1b80, 0x5f00, 0x6680, 0x8000, 0x0190, BIT_ALWAYS_1, 0, 0, TEXTABLE_518_MINIMAP_KP_Elemental_Reactor_NE_Water}, 
    {0x1e00, 0x2580, 0x5c80, 0x6680, 0x8000, 0x0190, BIT_ALWAYS_1, 0, 0, TEXTABLE_519_MINIMAP_KP_Elemental_Reactor_NW_Earth}, 
    {0x2300, 0x2a80, 0x5780, 0x5c80, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_51A_MINIMAP_KP_Exterior_Deep_Sea},
    {0x0f00, 0x1400, 0x5780, 0x5f00, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_51B_MINIMAP_KP_Grand_Hall_Above_Sea},
};
/*0x6F0*/ static MinimapSection sTilesWG[] = {
    {0xec00, 0xf100, 0xee80, 0xf380, 0xfd1c, 0x7fff, BIT_ALWAYS_1, 0,   0, TEXTABLE_51C_MINIMAP_WG_Uppermost_Tier}, //A scrapped section? Shows up if you use noclipping to ascend to upper area!
    {0xe980, 0xf100, 0xee80, 0xf380, 0xfc7c, 0x7fff, BIT_ALWAYS_1, 0,   0, TEXTABLE_51D_MINIMAP_WG_Upper_Tier},
    {0xe980, 0xf100, 0xee80, 0xf380, 0xfbb4, 0x7fff, BIT_ALWAYS_1, 0,   0, TEXTABLE_51E_MINIMAP_WG_Middle_Tier},
    {0xec00, 0xf100, 0xee80, 0xf1c8, 0x8000, 0xface, BIT_ALWAYS_1, 0, -11, TEXTABLE_520_MINIMAP_WG_Bottom_Tier_Path_to_Dragon_Rock},
    {0xec00, 0xf100, 0xf600, 0xf880, 0x8000, 0xface, BIT_ALWAYS_1, 0,   0, TEXTABLE_521_MINIMAP_WG_Shabunga_Home_Interior}, //An older layout for Shabunga's home, facing south instead of east. Doesn't line up with Dec 2000 design.
    {0xe980, 0xf100, 0xee80, 0xf880, 0x8000, 0x7fff, BIT_ALWAYS_1, 0,   0, TEXTABLE_51F_MINIMAP_WG_Lower_Tier_and_Shabunga_Home} //An older, wider layout for Shabunga's area, with its entrance facing south instead of east.
};
/*0x75C*/ static MinimapSection sTilesDFPTTop[] = {
    {0x0280, 0x0780, 0x2300, 0x2a80, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_522_MINIMAP_DFPT_Top}
};
/*0x770*/ static MinimapSection sTilesDFTPBottom[] = {
    {0xfb00, 0x0a00, 0x2080, 0x2800, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_523_MINIMAP_DFPT_Bottom},
    {0xfd80, 0x0780, 0x2800, 0x2a80, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_524_MINIMAP_DFPT_Bottom_3rd_SpellStone_Holder}
};
/*0x794*/ static MinimapSection sTilesWC[] = {
    {0xdf9a, 0xe1e7, 0xe200, 0xe480, 0xfc3b, 0xfd8a, BIT_ALWAYS_1, 0, 0, TEXTABLE_52F_MINIMAP_WC_Central_Ziggurat_Tunnels}, 
    {0xdeed, 0xe294, 0xe200, 0xe480, 0xfc3b, 0xfcf4, BIT_ALWAYS_1, 0, 0, TEXTABLE_52F_MINIMAP_WC_Central_Ziggurat_Tunnels},
    {0xdd00, 0xe480, 0xe200, 0xe480, 0xfc3b, 0xfc90, BIT_ALWAYS_1, 0, 0, TEXTABLE_52F_MINIMAP_WC_Central_Ziggurat_Tunnels}, //Shown in-game, but it's misplaced slightly to the south of the ziggurat.
    {0xdd00, 0xdd80, 0xe480, 0xe594, 0xfc3b, 0xfd0d, BIT_ALWAYS_1, 0, 0, TEXTABLE_52F_MINIMAP_WC_Central_Ziggurat_Tunnels},
    {0xe400, 0xe480, 0xe0ed, 0xe200, 0xfc3b, 0xfd0d, BIT_ALWAYS_1, 0, 0, TEXTABLE_52F_MINIMAP_WC_Central_Ziggurat_Tunnels},
    {0xd800, 0xd940, 0xdec0, 0xe040, 0x8000, 0xfc72, BIT_ALWAYS_1, 0, 0, TEXTABLE_52C_MINIMAP_WC_Sun_Temple_Interior},
    {0xd580, 0xda80, 0xdd00, 0xe480, 0x8000, 0xfc04, BIT_ALWAYS_1, 0, 0, TEXTABLE_52C_MINIMAP_WC_Sun_Temple_Interior},
    {0xe840, 0xe900, 0xe640, 0xe7c0, 0x8000, 0xfc72, BIT_ALWAYS_1, 0, 0, TEXTABLE_52E_MINIMAP_WC_Moon_Temple_Interior},
    {0xe700, 0xec00, 0xe200, 0xe980, 0x8000, 0xfc04, BIT_ALWAYS_1, 0, 0, TEXTABLE_52E_MINIMAP_WC_Moon_Temple_Interior},
    {0xdd00, 0xe480, 0xe700, 0xf100, 0xfc90, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_533_MINIMAP_WC_Jungle_Door_and_Approach},
    {0xdd00, 0xe480, 0xdf80, 0xe700, 0xfc90, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_534_MINIMAP_WC_Central_Ziggurat},
    {0xdf80, 0xe200, 0xdd00, 0xdf80, 0xfc90, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_534_MINIMAP_WC_Central_Ziggurat},
    {0xd580, 0xda80, 0xdd00, 0xe480, 0xfc04, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_535_MINIMAP_WC_Sun_Temple_Exterior},
    {0xda80, 0xdd00, 0xe2c0, 0xe3c0, 0xfc86, 0xfd12, BIT_ALWAYS_1, 0, 0, TEXTABLE_535_MINIMAP_WC_Sun_Temple_Exterior},
    {0xe700, 0xec00, 0xe200, 0xe980, 0xfc04, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_536_MINIMAP_WC_Moon_Temple_Exterior},
    {0xe480, 0xe700, 0xe200, 0xe480, 0xfc04, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_536_MINIMAP_WC_Moon_Temple_Exterior},
    {0xdf80, 0xe200, 0xdf80, 0xe480, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_52D_MINIMAP_WC_Central_Ziggurat_RedEye_Chamber},
    {0xe480, 0xe980, 0xdd00, 0xe980, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_530_MINIMAP_WC_Outskirts_Start},
    {0xda80, 0xe480, 0xda80, 0xdf80, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_531_MINIMAP_WC_Outskirts_Middle},
    {0xd800, 0xdd00, 0xdf80, 0xe980, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_532_MINIMAP_WC_Outskirts_End}
};
/*0x8FC*/ static MinimapSection sTilesDB[] = {
    {0xd080, 0xd2e0, 0xf100, 0xf380, 0x8000, 0xfa4c, BIT_ALWAYS_1, 0, 0, TEXTABLE_558_MINIMAP_DB_Spike_Trap_Corridor},
    {0xd080, 0xd300, 0xf380, 0xf880, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_558_MINIMAP_DB_Spike_Trap_Corridor},
    {0xd800, 0xdf80, 0xf600, 0xfd80, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_555_MINIMAP_DB_River_Rapids_Upper},
    {0xd080, 0xd800, 0xf600, 0xfd80, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_556_MINIMAP_DB_River_Rapids_Lower_and_Light_Cave},
    {0xd080, 0xda80, 0xec00, 0xf600, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_557_MINIMAP_DB_Main_Bay}
};
/*0x958*/ static MinimapSection sTilesCC[] = {
    {0x0a00, 0x0c80, 0x4100, 0x4380, 0x8000, 0xff60, BIT_ALWAYS_1, 0, 0, TEXTABLE_539_MINIMAP_CC_Krazoa_Sculpture_Transporter},
    {0x1400, 0x1680, 0x4100, 0x4600, 0x8000, 0xff88, BIT_MINIMAP_Cape_Claw_Inside_Tablet_Chamber,      0, 0, TEXTABLE_53A_MINIMAP_CC_Ruins_Secret_Room_Krazoa_Tablet},
    {0x0f00, 0x1400, 0x4380, 0x4600, 0x8000, 0xffb0, BIT_ALWAYS_1, 0, 0, TEXTABLE_53B_MINIMAP_CC_Courtyard_Well_Shrine_Entrance},
    {0x0d00, 0x0f00, 0x46c0, 0x4880, 0x8000, 0xff88, BIT_ALWAYS_1, 0, 0, TEXTABLE_53C_MINIMAP_CC_Shiny_Nugget_Cave_and_Cliffside_Beacon},
    {0x0c80, 0x0f00, 0x4880, 0x4b00, 0xff38, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_53C_MINIMAP_CC_Shiny_Nugget_Cave_and_Cliffside_Beacon},
    {0x0a00, 0x0f00, 0x4100, 0x4380, 0x8000, 0x7fff, BIT_MINIMAP_Cape_Claw_Inside_Krazoa_Sculpture,      0, 0, TEXTABLE_537_MINIMAP_CC_Krazoa_Sculpture_to_SandWorm_Beach},
    {0x0a00, 0x0f00, 0x3c00, 0x4100, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_537_MINIMAP_CC_Krazoa_Sculpture_to_SandWorm_Beach},
    {0x0f00, 0x1900, 0x3c00, 0x4380, 0x8000, 0xff6a, BIT_ALWAYS_1, 0, 0, TEXTABLE_538_MINIMAP_CC_Coastline_Caves_and_Cannon_Island},
    {0x0c80, 0x1900, 0x3c00, 0x4100, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_53D_MINIMAP_CC_Coastline_Ruins_and_Cannon_Island},
    {0x0780, 0x0a00, 0x3e80, 0x4100, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_53E_MINIMAP_CC_Waterfall_Cave},
    {0x0500, 0x0c80, 0x4100, 0x4d80, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_53F_MINIMAP_CC_Main_Bay},
    {0x0c80, 0x1400, 0x4100, 0x4d80, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_540_MINIMAP_CC_Courtyard_and_Ruins_East_and_GuardClaw},
    {0x1400, 0x1900, 0x4100, 0x4d80, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_541_MINIMAP_CC_Courtyard_and_Ruins_West}
};

/*0xA44*/ static MinimapSection sTilesMMP[] = {
    {0xe700, 0xee80, 0x4880, 0x4b00, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_559_MINIMAP_MMP_Approach}, 
    {0xdd00, 0xe700, 0x4600, 0x4b00, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_55A_MINIMAP_MMP_Gateway},
    {0xdd00, 0xe700, 0x3c00, 0x4600, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_55B_MINIMAP_MMP_Ravine},
    {0xe200, 0xe480, 0x3700, 0x3c00, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_55C_MINIMAP_MMP_Outside_Shrine} //Depicts extra features which aren't found outside the current version of the entrance. The HITS for this older layout still seem to be present!
};
/*0xA8C*/ static MinimapSection sTilesIM[] = {
    {0x0000, 0x0280, 0x0780, 0x0a00, 0x8000, 0x193c, BIT_ALWAYS_1, 0, 0, TEXTABLE_543_MINIMAP_IM_Campfire_Cave},
    {0xfb00, 0x0500, 0x0500, 0x0f00, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_542_MINIMAP_IM_Summit},
    {0x0500, 0x0780, 0x0c80, 0x1900, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_544_MINIMAP_IM_Slope_Section_1_Icicle_Arches},
    {0x0280, 0x0500, 0x0f00, 0x1900, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_545_MINIMAP_IM_Slope_Section_2_Felled_Tree},         //Tile doesn't show in-game because Ice Mountain is split over 3 mapIDs.
    {0x0280, 0x0500, 0x1680, 0x1900, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_546_MINIMAP_IM_Slope_Section_2_Crossover},           //Tile doesn't show in-game because Ice Mountain is split over 3 mapIDs.
    {0xf600, 0x0280, 0x0f00, 0x1680, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_547_MINIMAP_IM_Slope_Section_3_Branching},           //Tile doesn't show in-game because Ice Mountain is split over 3 mapIDs.
    {0xf380, 0xf880, 0x0f00, 0x1680, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_548_MINIMAP_IM_Slope_Section_4_Loop},                //Tile doesn't show in-game because Ice Mountain is split over 3 mapIDs.
    {0xf600, 0xfd80, 0x0780, 0x1180, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_549_MINIMAP_IM_Slope_Section_5_End},                 //Tile doesn't show in-game because Ice Mountain is split over 3 mapIDs.
    {0xf600, 0xfd80, 0x0c80, 0x1180, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_54A_MINIMAP_IM_Slope_Section_5_End_Cave_Interior}    //Tile doesn't show in-game because Ice Mountain is split over 3 mapIDs.
};
/*0xB30*/ static MinimapSection sTilesWM[] = {
    {0x3200, 0x3700, 0x0280, 0x0940, 0x01ae, 0x0316, BIT_ALWAYS_1, 0, 0, TEXTABLE_551_MINIMAP_WM_Main_Chamber_Upper_Walkways},
    {0x3200, 0x3480, 0x0940, 0x0a00, 0x01ae, 0x0316, BIT_ALWAYS_1, 0, 0, TEXTABLE_551_MINIMAP_WM_Main_Chamber_Upper_Walkways},
    {0x3480, 0x3700, 0x0280, 0x0340, 0x02bc, 0x0316, BIT_ALWAYS_1, 0, 0, TEXTABLE_551_MINIMAP_WM_Main_Chamber_Upper_Walkways},
    {0x3200, 0x3700, 0xfd80, 0x0500, 0x02ee, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_553_MINIMAP_WM_Krystal_Path_to_Summit},
    {0x3200, 0x3700, 0x0500, 0x0c80, 0x0226, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_554_MINIMAP_WM_Sabre_Path_to_Summit},
    {0x3480, 0x3700, 0xfd80, 0x0280, 0x0172, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_550_MINIMAP_WM_Krystal_Warp_Crystal_Hall},
    {0x3200, 0x3e80, 0x0280, 0x0a00, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_54D_MINIMAP_WM_Main_Chamber_and_Sabre_GuardClaw_Hall_and_Orrery},
    {0x3200, 0x3980, 0xfd80, 0x0500, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_54B_MINIMAP_WM_Krystal_Skydock_and_Entrance_Tunnel},
    {0x2a80, 0x3200, 0x0000, 0x0780, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_54C_MINIMAP_WM_Krystal_Randorn_Hall_and_Orrery},
    {0x2800, 0x3200, 0x0780, 0x1180, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_54E_MINIMAP_WM_Sabre_Waterfall_Bay}, //Depicts an island entrance that doesn't appear in the level model itself! An unused BLOCKS model does exist for a similar (though not identical) WM island with an archway.
    {0x3200, 0x3e80, 0x0a00, 0x1180, 0x8000, 0x7fff, BIT_ALWAYS_1, 0, 0, TEXTABLE_54F_MINIMAP_WM_Sabre_Seadock}
};

/*0xBF8*/ static MinimapLevel sMinimapLevels[30] = {
{sTilesVFPT,         BIT_Map_VFPT,                MAP_VOLCANO_FORCE_POINT_TEMPLE,       ARRAYCOUNT(sTilesVFPT)},
{sTilesDIMExterior,  BIT_Map_DIM_Exterior,        MAP_DARK_ICE_MINES_1,                 ARRAYCOUNT(sTilesDIMExterior)},
{sTilesDIMExterior,  BIT_Map_DIM_Exterior,        MAP_DARK_ICE_MINES_CAVES,             ARRAYCOUNT(sTilesDIMExterior)},
{sTilesDIMInterior,  BIT_Map_DIM_Interior,        MAP_DARK_ICE_MINES_2,                 ARRAYCOUNT(sTilesDIMInterior)},
{sTilesGaladon,      BIT_Map_DIM_Interior,        MAP_BOSS_GALADON,                     ARRAYCOUNT(sTilesGaladon)},
{sTilesSW,           BIT_Map_SW,                  MAP_SNOWHORN_WASTES,                  ARRAYCOUNT(sTilesSW)},
{sTilesIMHotSpringA, BIT_ALWAYS_1,                MAP_ICE_MOUNTAIN_3,                   ARRAYCOUNT(sTilesIMHotSpringA)}, //Single tile of the hot spring at the end of IM
{sTilesIMHotSpringB, BIT_ALWAYS_1,                NO_MAP_ID,                            ARRAYCOUNT(sTilesIMHotSpringB)}, //NOTE: duplicate, not viewable since no mapID assigned?
{sTilesCRF,          BIT_Map_CRF,                 MAP_CLOUDRUNNER_FORTRESS,             ARRAYCOUNT(sTilesCRF)},
{sTilesCRF,          BIT_Map_CRF,                 MAP_CLOUDRUNNER_DUNGEON,              ARRAYCOUNT(sTilesCRF)},
{sTilesCRF,         BIT_Map_CRF,                 MAP_CLOUDRUNNER_TREASURE,             ARRAYCOUNT(sTilesCRF)},
{sTilesSC,          BIT_Map_SC,                  MAP_SWAPSTONE_CIRCLE,                 ARRAYCOUNT(sTilesSC)},
{sTilesSH,          BIT_Map_SH,                  MAP_SWAPSTONE_HOLLOW,                 ARRAYCOUNT(sTilesSH)},
{sTilesSHWell,      BIT_Map_SH,                  MAP_SWAPSTONE_HOLLOW_2,               ARRAYCOUNT(sTilesSHWell)},
{sTilesDF,          BIT_Map_DF,                  MAP_DISCOVERY_FALLS,                  ARRAYCOUNT(sTilesDF)},
{sTilesCRFTrack,    BIT_Map_CRF,                 MAP_CLOUDRUNNER_RACETRACK,            ARRAYCOUNT(sTilesCRFTrack)},
{sTilesDRTop,       BIT_Map_DR_Top,              MAP_DRAGON_ROCK_TOP,                  ARRAYCOUNT(sTilesDRTop)},
{sTilesDRBottom,    BIT_Map_DR_Bottom,           MAP_DRAGON_ROCK_BOTTOM,               ARRAYCOUNT(sTilesDRBottom)},
{sTilesGP,          BIT_Map_GP,                  MAP_GOLDEN_PLAINS,                    ARRAYCOUNT(sTilesGP)},
{sTilesBWC,         BIT_Map_BWC,                 MAP_BLACKWATER_CANYON,                ARRAYCOUNT(sTilesBWC)},
{sTilesKP,          BIT_Map_KP,                  MAP_KRAZOA_PALACE,                    ARRAYCOUNT(sTilesKP)},
{sTilesWG,          BIT_Map_WG,                  MAP_WILLOW_GROVE,                     ARRAYCOUNT(sTilesWG)},
{sTilesDFPTTop,     BIT_Map_DFPT,                MAP_DESERT_FORCE_POINT_TEMPLE_TOP,    ARRAYCOUNT(sTilesDFPTTop)},
{sTilesDFTPBottom,  BIT_Map_DFPT,                MAP_DESERT_FORCE_POINT_TEMPLE_BOTTOM, ARRAYCOUNT(sTilesDFTPBottom)},
{sTilesWC,          BIT_Map_WC,                  MAP_WALLED_CITY,                      ARRAYCOUNT(sTilesWC)},
{sTilesDB,          BIT_Map_DB,                  MAP_DIAMOND_BAY,                      ARRAYCOUNT(sTilesDB)},
{sTilesCC,          BIT_Map_CC,                  MAP_CAPE_CLAW,                        ARRAYCOUNT(sTilesCC)},
{sTilesMMP,         BIT_Map_MMP,                 MAP_MOON_MOUNTAIN_PASS,               ARRAYCOUNT(sTilesMMP)},
{sTilesWM,          BIT_Map_WM,                  MAP_WARLOCK_MOUNTAIN,                 ARRAYCOUNT(sTilesWM)},
{sTilesIM,          BIT_Map_IM,                  MAP_ICE_MOUNTAIN_1,                   ARRAYCOUNT(sTilesIM)}
};

// offset: 0x0 | ctor
void minimap_ctor(void *dll) {
    sMarkerSidekick = tex_load_deferred(TEXTABLE_3E1_MINIMAP_Green_Dot);
    sMarkerPlayer = tex_load_deferred(TEXTABLE_467_MINIMAP_Blue_Diamond);
}

// offset: 0x5C | dtor
void minimap_dtor(void *dll) {
    if (sMapTile) {
        tex_free(sMapTile);
    }
    tex_free(sMarkerSidekick);
    tex_free(sMarkerPlayer);
    sMapTile = 0;
    sMarkerSidekick = NULL;
    sMarkerPlayer = NULL;
}

// offset: 0xE4 | func: 0 | export: 0
/** 
  * Checks if a "Toggle Map" item was used in the cmdmenu, and toggles the map's visibility.
  * This inventory item/option was either scrapped or yet to be added, but unused text also exists for it! 
  */
void minimap_toggle_via_cmdmenu(void) {
    if (gDLL_1_UI->vtbl->func_DF4(BIT_Toggle_Minimap)) {
        sMinimapVisible = 1 - sMinimapVisible;
    }
}

// offset: 0x148 | func: 1 | export: 1
/**
  * Main minimap draw function.
  */
s32 minimap_print(Gfx **gdl, s32 arg1) {
    Object* player;
    Object* sidekick;
    s32 loadTextureID;
    s16 playerX;
    s16 playerZ;
    s16 playerY;
    u8 index;
    u8 index2;
    u16 texTableID;
    s16 tileCount;
    MinimapSection* mapTiles;
    u8 mapID;
    u8 tileIndex;
    s8 mapFound;
    MinimapLevel* mapLevel;
    MinimapSection* tile;

    loadTextureID = 0;
    mapFound = FALSE;
    index = 0;
    index2 = 0;
    mapID = 0;
    player = get_player();
    sidekick = get_sidekick();
    if (player != NULL) {
        //Get mapID from player's coords, or from the mobile map Object they're parented to
        if (player->parent) {
            mapID = player->parent->mapID;
        } else {
            mapID = map_get_map_id_from_xz_ws(player->srt.transl.x, player->srt.transl.z);
        }

        //Iterate over the map definitions until the one with the relevant mapID is found
        while (mapFound == FALSE && (u32)index < ARRAYCOUNT(sMinimapLevels)){
            if (mapID == sMinimapLevels[index].mapID) { 
                mapFound = TRUE;
            } else {
                index++;
            }
        }
        
        if (mapFound) {
            playerX = player->positionMirror.x;
            playerZ = player->positionMirror.z;
            playerY = player->positionMirror.y;
            
            //Iterate over the map's tiles, until finding a tile whose bounding volume contains the player coords
            mapLevel = &sMinimapLevels[index];
            mapTiles = mapLevel->tiles;
            for (mapFound = FALSE, index = 0; index2 < mapLevel->tileCount; index2++){
                if ((playerX >= mapTiles[index2].minX) && (playerX < mapTiles[index2].maxX) && 
                    (playerZ >= mapTiles[index2].minZ) && (playerZ < mapTiles[index2].maxZ) && 
                    (playerY >= mapTiles[index2].minY) && (playerY < mapTiles[index2].maxY) && 
                    main_get_bits(mapTiles[index2].gamebitSection)) { //Make sure the tile's gamebit is set

                    tileIndex = 0;
                    mapFound = TRUE;
                    loadTextureID = 0;
                    //Store the tile's texTableID, for loading a TEX0 minimap texture
                    if (mapTiles[index2].texTableID) {
                        loadTextureID = mapTiles[index2].texTableID;
                    }
                    
                    //@bug?: calculates the bounds every frame
                    if (sLoadedTexTableID == mapTiles[index2].texTableID) {
                        tileCount = mapLevel->tileCount;
                        sLevelMaxX = -0x8000;
                        sLevelMaxZ = -0x8000;
                        sLevelMinX = 0x7FFF;
                        sLevelMinZ = 0x7FFF;

                        //Getting minX/maxX, minZ/maxZ for whole map
                        while (tileIndex < tileCount){
                            if (loadTextureID == mapTiles[tileIndex].texTableID) {
                                sLevelMinX = (mapTiles[tileIndex].minX < sLevelMinX) ? mapTiles[tileIndex].minX : sLevelMinX;
                                sLevelMaxX = (mapTiles[tileIndex].maxX > sLevelMaxX) ? mapTiles[tileIndex].maxX : sLevelMaxX;
                                sLevelMinZ = (mapTiles[tileIndex].minZ < sLevelMinZ) ? mapTiles[tileIndex].minZ : sLevelMinZ;
                                sLevelMaxZ = (mapTiles[tileIndex].maxZ > sLevelMaxZ) ? mapTiles[tileIndex].maxZ : sLevelMaxZ;
                            }
                            tileIndex++;
                        }

                        //Getting gridX and gridZ
                        sGridX = ((sLevelMaxX - sLevelMinX) * 8) / BLOCKS_GRID_UNIT;
                        if (sGridX > 24) {
                            sGridX = 24;
                        }
                        sGridZ = ((sLevelMaxZ - sLevelMinZ) * 8) / BLOCKS_GRID_UNIT;
                        if (sGridZ > 24) {
                            sGridZ = (sGridZ * 2) - 24;
                        }

                        //Store screen offset for the tile
                        sOffsetX = mapTiles[index2].screenOffsetX;
                        sOffsetY = mapTiles[index2].screenOffsetY;
                    }
                    break;                
                }
            }
            
        }
        
        //Set/get minimap gamebits
        main_set_bits(BIT_Toggle_Minimap, mapFound);
        if (sMinimapVisible == FALSE || main_get_bits(BIT_Hide_Minimap)) {
            loadTextureID = 0;
        }
        
        //Hide during cutscenes
        if (camera_get_letterbox()) {
            loadTextureID = 0;
            sOpacity = 0;
        }

        //Fade minimap in/out
        if (loadTextureID == sLoadedTexTableID) {
            sOpacity += 0x20; //@framerate-dependent
            if (sOpacity > 0xFF) {
                sOpacity = 0xFF;
            }
        } else {
            sOpacity -= 0x20; //@framerate-dependent
            if (sOpacity < 0) {
                sOpacity = 0;
                if (sMapTile && (loadTextureID || sOpacity == 0)) {
                    tex_free(sMapTile);
                    sMapTile = NULL;
                    sLoadedTexTableID = 0;
                }
                if (loadTextureID) {
                    sMapTile = tex_load_deferred(loadTextureID);
                    sLoadedTexTableID = loadTextureID;
                }
            }
        }

        if (sMapTile && sOpacity) {
            //Draw minimap tile
            func_8003825C(gdl, sMapTile, 
                    (MINIMAP_SCREEN_X + sOffsetX - sGridX),
                    (MINIMAP_SCREEN_Y + sOffsetY - sGridZ),
                    0, 0, sOpacity, 0);

            //Draw player marker
            func_8003825C(gdl, sMarkerPlayer, 
                    (MINIMAP_SCREEN_X - sGridX - (player->positionMirror.x - sLevelMaxX) * 0.025f) - 4.0f,
                    (MINIMAP_SCREEN_Y - sGridZ - (player->positionMirror.z - sLevelMaxZ) * 0.025f) - 4.0f,
                    0, 0, sOpacity, 0);

            //Draw sidekick marker (if the sidekick's somewhere inside the current map's extended bounds)
            if (sidekick != NULL) {
                if (
                    (sLevelMinX - BLOCKS_GRID_UNIT_HALF < sidekick->positionMirror.x) && (sidekick->positionMirror.x < sLevelMaxX + BLOCKS_GRID_UNIT_HALF) &&
                    (sLevelMinZ - BLOCKS_GRID_UNIT_HALF < sidekick->positionMirror.z) && (sidekick->positionMirror.z < sLevelMaxZ + BLOCKS_GRID_UNIT_HALF)
                ) {
                    func_8003825C(gdl, sMarkerSidekick, 
                        (MINIMAP_SCREEN_X - sGridX - (sidekick->positionMirror.x - sLevelMaxX) * 0.025f) - 4.0f,
                        (MINIMAP_SCREEN_Y - sGridZ - (sidekick->positionMirror.z - sLevelMaxZ) * 0.025f) - 4.0f,
                        0, 0, sOpacity, 0);
                }
            }
        }
    }
    return 0;
}
