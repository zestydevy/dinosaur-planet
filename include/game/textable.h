#ifndef _GAME_TEXTABLE_H
#define _GAME_TEXTABLE_H

typedef enum {

    MINIMAP_Green_Dot = 0x3E1,          //sidekick marker
    MINIMAP_Blue_Diamond = 0x467,       //player marker

    MINIMAP_WM_Main_Chamber_Upper_Walkways = 0x551,
    MINIMAP_WM_Krystal_Path_to_Summit = 0x553,
    MINIMAP_WM_Sabre_Path_to_Summit = 0x554,
    MINIMAP_WM_Krystal_Warp_Crystal_Hall = 0x550,
    MINIMAP_WM_Main_Chamber_and_Sabre_GuardClaw_Hall_and_Orrery = 0x54d,
    MINIMAP_WM_Krystal_Skydock_and_Entrance_Tunnel = 0x54b,
    MINIMAP_WM_Krystal_Randorn_Hall_and_Orrery = 0x54c,
    MINIMAP_WM_Sabre_Waterfall_Bay = 0x54e, //NOTE: includes a scrapped island in the minimap!
    MINIMAP_WM_Sabre_Seadock = 0x54f,

    MINIMAP_CRF_Track_Entrance = 0x460,
    MINIMAP_CRF_Track_Loops = 0x461,

    MINIMAP_GP_Krazoa_Shrine = 0x50c,
    MINIMAP_GP_East_Oasis_and_Sand_Arches = 0x509,
    MINIMAP_GP_West_Dunes_and_Bonfires = 0x50a,
    MINIMAP_GP_DFPT_Entrance = 0x50b,

    MINIMAP_DFPT_Top = 0x0522,
    MINIMAP_DFPT_Bottom = 0x0523,
    MINIMAP_DFPT_Bottom_3rd_SpellStone_Holder = 0x0524,

    MINIMAP_BWC_Entrance_LightFoot_Village = 0x50d,
    MINIMAP_BWC_Entrance_Rib_Cage_River = 0x50e,
    MINIMAP_BWC_River_of_the_Dead_Start = 0x514,
    MINIMAP_BWC_Tree_of_Life_and_Bone_Serpent = 0x50f,
    MINIMAP_BWC_Tomb_and_Boss_Room = 0x510,
    MINIMAP_BWC_River_of_the_Dead_Rapids = 0x511,
    MINIMAP_BWC_River_of_the_Dead_Loop = 0x512,
    MINIMAP_BWC_Tree_Trunk_Spiral = 0x513,

    MINIMAP_IM_Hot_Spring = 0x44b,
    MINIMAP_SW_Ice_Cave = 0x44c,
    MINIMAP_CRF_Treasure_Room_Tunnels = 0x44d,
    MINIMAP_CRF_Dungeon = 0x44e,
    MINIMAP_CRF_Skydock = 0x44f,
    MINIMAP_CRF_Courtyard = 0x450,
    MINIMAP_CRF_Entrance_Canyon = 0x451,
    MINIMAP_SC_SwapStone_Area = 0x452,
    MINIMAP_SC_LightFoot_Village = 0x453,
    MINIMAP_SC_LightFoot_Village_Inside_Mound = 0x454,

    MINIMAP_CC_Krazoa_Sculpture_Transporter = 0x539,
    MINIMAP_CC_Ruins_Secret_Room_Krazoa_Tablet = 0x53a,
    MINIMAP_CC_Courtyard_Well_Shrine_Entrance = 0x53b,
    MINIMAP_CC_Shiny_Nugget_Cave_and_Cliffside_Beacon = 0x53c,
    MINIMAP_CC_Krazoa_Sculpture_to_SandWorm_Beach = 0x537, //uses gamebit
    MINIMAP_CC_Coastline_Caves_and_Cannon_Island = 0x538,
    MINIMAP_CC_Coastline_Ruins_and_Cannon_Island = 0x53d,
    MINIMAP_CC_Waterfall_Cave = 0x53e,
    MINIMAP_CC_Main_Bay = 0x53f,
    MINIMAP_CC_Courtyard_and_Ruins_East_and_GuardClaw = 0x540,
    MINIMAP_CC_Courtyard_and_Ruins_West = 0x541,

    MINIMAP_DF_Cave_Burrows_Upper = 0x0552,
    MINIMAP_DF_Cave_Burrows_Lower = 0x045f,
    MINIMAP_DF_Lower_Falls = 0x045d,
    MINIMAP_DF_Upper_Falls_Shrine_and_Whirlpool_Cave = 0x045c,
    MINIMAP_DF_Cascading_Approach = 0x045e,
    MINIMAP_DF_Toxic_Cave = 0x0464,

    MINIMAP_MMP_Approach = 0x559,
    MINIMAP_MMP_Gateway = 0x55A,
    MINIMAP_MMP_Ravine = 0x55B,
    MINIMAP_MMP_Outside_Shrine = 0x55C,

    MINIMAP_KP_Central_Atrium_and_Lift_Corridor = 0x515,
    MINIMAP_KP_Elemental_Reactor_SE_Magic = 0x516,
    MINIMAP_KP_Elemental_Reactor_SW_Fire = 0x517,
    MINIMAP_KP_Elemental_Reactor_NE_Water = 0x518,
    MINIMAP_KP_Elemental_Reactor_NW_Earth = 0x519,
    MINIMAP_KP_Exterior_Deep_Sea = 0x51a,
    MINIMAP_KP_Grand_Hall_Above_Sea = 0x51b,

    MINIMAP_IM_Campfire_Cave = 0x0543,
    MINIMAP_IM_Summit = 0x0542,
    MINIMAP_IM_Slope_Section_1_Icicle_Arches = 0x0544,
    MINIMAP_IM_Slope_Section_2_Felled_Tree = 0x0545,
    MINIMAP_IM_Slope_Section_2_Crossover = 0x0546,
    MINIMAP_IM_Slope_Section_3_Branching = 0x0547,
    MINIMAP_IM_Slope_Section_4_Loop = 0x0548,
    MINIMAP_IM_Slope_Section_5_End = 0x0549,
    MINIMAP_IM_Slope_Section_5_End_Cave_Interior = 0x054a,

    MINIMAP_SW_Garunda_Te_Waterfall_Area = 0x448,
    MINIMAP_SW_Geyser_and_Tricky_Tutorial = 0x44a,
    MINIMAP_SW_Meandering_Path_Icy_Platforms_and_Tree_Bridge = 0x449,
    MINIMAP_SW_Ice_Cave_Krazoa_Shrine_Entrance = 0x44c,

    MINIMAP_SH_River_Crossing_and_Walled_City_Gateway = 0x455,
    MINIMAP_SH_Tree_Hollow_Exterior = 0x457,
    MINIMAP_SH_Burrows = 0x458,
    MINIMAP_SH_SwapStone_Pond = 0x456,

    MINIMAP_SH_Well_Upper_Floor = 0x459,
    MINIMAP_SH_Well_Lower_Floor = 0x45a,

    MINIMAP_DIM_Snow_House_GuardClaw = 0x041e,
    MINIMAP_DIM_Snow_House_Bonfires_Upper = 0x041f,
    MINIMAP_DIM_Snow_House_Bonfires_Lower = 0x0420,
    MINIMAP_DIM_Snow_Houses_Area = 0x041a,
    MINIMAP_DIM_Jetbike_Cave = 0x041c,
    MINIMAP_DIM_Blizzard_Plains_Entrance = 0x041d,
    MINIMAP_DIM_Main_Area_and_Leap_of_Faith = 0x041b,

    MINIMAP_DIM_Pressure_Switch_Room_Upper = 0x0442, //misplaced way to south-east, and takes priority over entire lower portion of room
    MINIMAP_DIM_Waterfall_Cave = 0x0443, //shows up too far south
    MINIMAP_DIM_Upper_Cavern = 0x0444,
    MINIMAP_DIM_Lower_Cavern = 0x0447,
    MINIMAP_DIM_Pressure_Switch_Room_and_Dark_Passage = 0x0445, //doesn't seem to show up in dark room (maybe intentionally?)
    MINIMAP_DIM_Chasm_and_Magma_Pit_and_Spiral_Descent = 0x0446,

    MINIMAP_DIM_Boss_Room = 0x0441,

    MINIMAP_DB_Spike_Trap_Corridor = 0x0558,
    MINIMAP_DB_River_Rapids_Upper = 0x0555,
    MINIMAP_DB_River_Rapids_Lower_and_Light_Cave = 0x0556,
    MINIMAP_DB_Main_Bay = 0x0557,

    MINIMAP_VFPT_Entrance_Area_Old = 0x03e0, //NOTE: shows an older level design, and is positioned one block west
    MINIMAP_VFPT_Lower_Floor = 0x0415, //NOTE: shows the scrapped winding lava path connected into the south side of the entrance hall
    MINIMAP_VFPT_Winding_Lava_Path = 0x0416, //NOTE: a scrapped section leading from/to the south of Diamond Bay (its HITS still exist, and a single polygon from each of its BLOCKS)
    MINIMAP_VFPT_Upper_Floor_West = 0x0417, //VFPT Act 3
    MINIMAP_VFPT_Upper_Caldera = 0x0418,
    MINIMAP_VFPT_Upper_Floor_East = 0x0419, //VFPT Act 1 & 2

    MINIMAP_WC_Central_Ziggurat_Tunnels = 0x052f, //shown in-game but misplaced (slightly too far south)
    MINIMAP_WC_Sun_Temple_Interior = 0x052c,
    MINIMAP_WC_Moon_Temple_Interior = 0x052e,
    MINIMAP_WC_Jungle_Door_and_Approach = 0x0533,
    MINIMAP_WC_Central_Ziggurat = 0x0534,
    MINIMAP_WC_Sun_Temple_Exterior = 0x0535,
    MINIMAP_WC_Moon_Temple_Exterior = 0x0536,
    MINIMAP_WC_Central_Ziggurat_RedEye_Chamber = 0x052d,
    MINIMAP_WC_Outskirts_Start = 0x0530,
    MINIMAP_WC_Outskirts_Middle = 0x0531,
    MINIMAP_WC_Outskirts_End = 0x0532,

    MINIMAP_WG_Uppermost_Tier = 0x051c, //Scrapped section? Can't reach this height in the Dec 2000 layout.
    MINIMAP_WG_Upper_Tier = 0x051d, //SwapStone Hollow entrance is on this floor
    MINIMAP_WG_Middle_Tier = 0x051e, //Next tier down
    MINIMAP_WG_Bottom_Tier_Path_to_Dragon_Rock = 0x0520,
    MINIMAP_WG_Shabunga_Home_Interior = 0x0521, //NOTE: shows an older location/orientation for Shabunga's home - doesn't line up with Dec 2000 design
    MINIMAP_WG_Lower_Tier_and_Shabunga_Home = 0x051f, //NOTE: shows an older and wider layout for Shabunga's area, with its entrance facing south instead of east

    MINIMAP_DR_CloudRunner_Prison = 0x501,
    MINIMAP_DR_Plains_South_East = 0x502,
    MINIMAP_DR_Plains_South_West = 0x503,
    MINIMAP_DR_Plains_North_East_and_HighTop_Prison = 0x504,
    MINIMAP_DR_Plains_North_West = 0x505,

    MINIMAP_DR_Bottom_Central_Mine_Mezzanine = 0x508,
    MINIMAP_DR_Bottom_Central_Mine = 0x506,
    MINIMAP_DR_Bottom_Quarry_and_Skeetla_Cave = 0x507
    
} TexTable_Indices;

#endif // _GAME_TEXTABLE_H
