#!/usr/bin/env python3
import argparse
from io import BufferedReader
import os
from pathlib import Path
import struct
from typing import TypedDict

from objlist import read_objects, Obj

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
BIN_ASSETS_DIR = Path("bin/assets")

class ObjSetupBit:
    bit: int
    desc: str

    def __init__(self, bit: int, desc: str) -> None:
        self.bit = bit
        self.desc = desc

class ObjSetupField:
    offset: int
    length: int
    signed: bool
    name: str

    def __init__(self, offset: int, name: str, length: int=2, signed: bool=True) -> None:
        self.offset = offset
        self.name = name
        self.length = length
        self.signed = signed

class ObjSetup:
    fields: list[ObjSetupField]

    def __init__(self, fields: list[ObjSetupField]) -> None:
        self.fields = fields

BADDIE_SETUP = ObjSetup([
    ObjSetupField(0x18, "unk18 (dead?, BaddieControl)"),
    ObjSetupField(0x1A, "unk1A (BaddieControl)"),
    ObjSetupField(0x30, "unk30 (BaddieControl)"),
])

# dllno -> setup def
OBJ_SETUPS: dict[int, ObjSetup] = {
    # SharpClaw
    215: BADDIE_SETUP,
    # SnowWorm
    216: BADDIE_SETUP,
    # GuardClaw
    217: BADDIE_SETUP,
    # Chuka
    220: ObjSetup([
        ObjSetupField(0x18, "gamebitDead"),
    ]),
    # SnowWormSmall
    222: BADDIE_SETUP,
    # SabreBaddie
    225: BADDIE_SETUP,
    # Caictua
    228: BADDIE_SETUP,
    # VampireBat
    232: BADDIE_SETUP,
    # BigScorpionRobot
    233: BADDIE_SETUP,
    # ScorpionRobot
    234: BADDIE_SETUP,
    # WG_Triffid
    236: BADDIE_SETUP,
    # Lunaimar
    243: BADDIE_SETUP,
    # BalloonBaddie
    245: BADDIE_SETUP,
    # PirahnaBaddie
    248: BADDIE_SETUP,
    # TurtleBaddie
    250: BADDIE_SETUP,
    # Tesla
    257: ObjSetup([
        ObjSetupField(0x1C, "unk1C"),
    ]),
    # SHvines
    265: ObjSetup([
        ObjSetupField(0x1E, "gamebitBurnt"),
    ]),
    # SfxPlayer
    266: ObjSetup([
        ObjSetupField(0x18, "flagPlay"),
    ]),
    # sideload
    269: ObjSetup([
        ObjSetupField(0x18, "gamebitUnlocked"),
    ]),
    # collectable
    272: ObjSetup([
        ObjSetupField(0x1C, "gamebitCollected"),
        ObjSetupField(0x24, "gamebitSecondary"),
        ObjSetupField(0x2C, "gamebitCount"),
    ]),
    # EffectBox
    273: ObjSetup([
        ObjSetupField(0x20, "gamebitEnable"),
    ]),
    # pushpull
    274: ObjSetup([
        ObjSetupField(0x18, "unk18"),
    ]),
    # WarpPoint
    275: ObjSetup([
        ObjSetupField(0x20, "gamebit"),
    ]),
    # Door
    279: ObjSetup([
        ObjSetupField(0x1A, "unk1A"),
        ObjSetupField(0x20, "unk20"),
    ]),
    # Crate
    282: ObjSetup([
        ObjSetupField(0x1E, "gamebitDestroyed"),
    ]),
    # LevelName
    284: ObjSetup([
        ObjSetupField(0x18, "gamebitShown"),
    ]),
    # ProjectileSwitch
    285: ObjSetup([
        ObjSetupField(0x18, "gamebit"),
    ]),
    # BlownUpTarget
    286: ObjSetup([
        ObjSetupField(0x18, "gamebit"),
    ]),
    # PressureSwitch
    287: ObjSetup([
        ObjSetupField(0x1A, "gameBitPressed"),
        ObjSetupField(0x20, "gamebitActivated"),
    ]),
    # CClogpush
    289: ObjSetup([
        ObjSetupField(0x18, "gamebit1"),
        ObjSetupField(0x1A, "gamebit2"),
        ObjSetupField(0x1C, "gamebit3"),
    ]),
    # TrickyWarp
    292: ObjSetup([
        ObjSetupField(0x20, "gamebit"),
    ]),
    # TrickyGuard
    293: ObjSetup([
        ObjSetupField(0x1A, "gamebit"),
    ]),
    # SmallBasket
    295: ObjSetup([
        ObjSetupField(0x1E, "gamebit"),
    ]),
    # MediumCrate
    296: ObjSetup([
        ObjSetupField(0x1E, "gamebit"),
    ]),
    # FallLadders
    302: ObjSetup([
        ObjSetupField(0x1E, "gamebitRaise"),
        ObjSetupField(0x20, "gamebitFall"),
    ]),
    # PortalSpellDoor
    305: ObjSetup([
        ObjSetupField(0x1E, "gamebitActivated"),
    ]),
    # MMPBridge
    306: ObjSetup([
        ObjSetupField(0x1E, "gamebitVisible"),
    ]),
    # SeqDoor
    307: ObjSetup([
        ObjSetupField(0x18, "gamebitOpenA"),
        ObjSetupField(0x1A, "gamebitRestoreState"),
        ObjSetupField(0x22, "gamebitOpenB"),
        ObjSetupField(0x24, "gamebitCameraBack"),
        ObjSetupField(0x26, "gamebitCameraFront"),
    ]),
    # USEOBJ
    308: ObjSetup([
        ObjSetupField(0x1C, "gamebitUsed"),
        ObjSetupField(0x1E, "gamebitRequiredItem"),
        ObjSetupField(0x22, "gamebitEnabled"),
    ]),
    # SEQOBJ
    309: ObjSetup([
        ObjSetupField(0x18, "gamebitHasPlayed"),
        ObjSetupField(0x1A, "gamebitPlay"),
    ]),
    # IMMultiSeq
    311: ObjSetup([
        ObjSetupField(0x18, "gamebits1[0]"),
        ObjSetupField(0x1A, "gamebits1[1]"),
        ObjSetupField(0x1C, "gamebits1[2]"),
        ObjSetupField(0x1E, "gamebits1[3]"),
        ObjSetupField(0x20, "gamebits2[0]"),
        ObjSetupField(0x22, "gamebits2[1]"),
        ObjSetupField(0x24, "gamebits2[2]"),
        ObjSetupField(0x26, "gamebits2[3]"),
    ]),
    # NWMultiSeq
    312: ObjSetup([
        ObjSetupField(0x18, "playedBits[0]"),
        ObjSetupField(0x1A, "playedBits[1]"),
        ObjSetupField(0x1C, "playedBits[2]"),
        ObjSetupField(0x1E, "playedBits[3]"),
        ObjSetupField(0x20, "playedBits[4]"),
        ObjSetupField(0x22, "playedBits[5]"),
        ObjSetupField(0x24, "playedBits[6]"),
        ObjSetupField(0x26, "playedBits[7]"),
        ObjSetupField(0x28, "playBits[0]"),
        ObjSetupField(0x2A, "playBits[1]"),
        ObjSetupField(0x2C, "playBits[2]"),
        ObjSetupField(0x2E, "playBits[3]"),
        ObjSetupField(0x30, "playBits[4]"),
        ObjSetupField(0x32, "playBits[5]"),
        ObjSetupField(0x34, "playBits[6]"),
        ObjSetupField(0x36, "playBits[7]"),
    ]),
    # Crate2
    320: ObjSetup([
        ObjSetupField(0x1C, "unk1C"),
    ]),
    # Duster
    321: ObjSetup([
        ObjSetupField(0x24, "gamebit"),
    ]),
    # Trigger
    325: ObjSetup([
        ObjSetupField(0x44, "bitFlagID"),
        ObjSetupField(0x48, "conditionBitFlagIDs[0]"),
        ObjSetupField(0x4A, "conditionBitFlagIDs[1]"),
        ObjSetupField(0x4C, "conditionBitFlagIDs[2]"),
        ObjSetupField(0x4E, "conditionBitFlagIDs[3]"),
    ]),
    # (unnamed)
    327: ObjSetup([
        ObjSetupField(0x20, "gamebitVisible"),
    ]),
    # CampFire
    330: ObjSetup([
        ObjSetupField(0x18, "gamebitID"),
    ]),
    # GenProps
    331: ObjSetup([
        ObjSetupField(0x1E, "gamebitA"),
        ObjSetupField(0x20, "gamebitB"),
    ]),
    # FXEmit
    332: ObjSetup([
        ObjSetupField(0x1E, "toggleGamebit"),
        ObjSetupField(0x20, "disableGamebit"),
    ]),
    # EnvEmitter
    333: ObjSetup([
        ObjSetupField(0x1C, "gamebitActivate"),
    ]),
    # Transporter
    334: ObjSetup([
        ObjSetupField(0x20, "gamebitEnabled"),
    ]),
    # DoorOpen
    341: ObjSetup([
        ObjSetupField(0x20, "gamebit"),
    ]),
    # LightPole
    343: ObjSetup([
        ObjSetupField(0x1E, "gamebitFinished"),
        ObjSetupField(0x20, "gamebitStart"),
    ]),
    # texscroll
    348: ObjSetup([
        ObjSetupField(0x1A, "gamebitActivate"),
    ]),
    # AlphaAnimator
    350: ObjSetup([
        ObjSetupField(0x18, "gamebitActivate"),
        ObjSetupField(0x1A, "gamebitActivated"),
    ]),
    # GroundAnimator
    351: ObjSetup([
        ObjSetupField(0x18, "gamebitDug"),
    ]),
    # HitAnimator
    353: ObjSetup([
        ObjSetupField(0x18, "gamebitActivate"),
    ]),
    # VisAnimator
    354: ObjSetup([
        ObjSetupField(0x18, "gamebitID"),
    ]),
    # WallAnimator
    355: ObjSetup([
        ObjSetupField(0x18, "gamebitDug"),
    ]),
    # ExplodeAnimator
    357: ObjSetup([
        ObjSetupField(0x32, "gamebitExploded"),
        ObjSetupField(0x34, "gamebitExplodeTrigger"),
    ]),
    # TexFrameAnimator
    361: ObjSetup([
        ObjSetupField(0x1E, "gamebitFinished"),
        ObjSetupField(0x20, "gamebitPlay"),
    ]),
    # PortalTexAnimator
    362: ObjSetup([
        ObjSetupField(0x18, "gamebitEnable"),
    ]),
    # WindLift
    370: ObjSetup([
        ObjSetupField(0x1C, "unk1C"),
        ObjSetupField(0x1E, "unk1E"),
    ]),
    # CFPowerBase
    371: ObjSetup([
        ObjSetupField(0x1E, "gamebit"),
    ]),
    # CFCloudBaby
    373: ObjSetup([
        ObjSetupField(0x1E, "rescuedGamebit"),
        ObjSetupField(0x22, "reachedPerchGamebit"),
    ]),
    # LaserBeam
    374: ObjSetup([
        ObjSetupField(0x1E, "gamebit"),
    ]),
    # RobotAnimPatrol
    381: ObjSetup([
        ObjSetupField(0x1E, "unk1E"),
        ObjSetupField(0x20, "unk20"),
    ]),
    # CFCage
    384: ObjSetup([
        ObjSetupField(0x18, "gamebit"),
    ]),
    # ToggleSwitch
    386: ObjSetup([
        ObjSetupField(0x1E, "enabledGamebit"),
        ObjSetupField(0x20, "stateGamebit"),
    ]),
    # CFbigdoorswitch
    387: ObjSetup([
        ObjSetupField(0x1E, "gamebitUnlocked"),
        ObjSetupField(0x20, "gamebitState"),
    ]),
    # BlastedObject
    389: ObjSetup([
        ObjSetupField(0x1E, "blastedGamebit"),
        ObjSetupField(0x20, "hitCountGamebit"),
    ]),
    # CFMainSlideDoor
    393: ObjSetup([
        ObjSetupField(0x18, "unk18"),
        ObjSetupField(0x1A, "unk1A"),
        ObjSetupField(0x22, "unk22"),
    ]),
    # CFTreasureDoor
    394: ObjSetup([
        ObjSetupField(0x1A, "gamebitRestoreState"),
    ]),
    # CFTreasureChestKey
    399: ObjSetup([
        ObjSetupField(0x1C, "itemGamebit"),
        ObjSetupField(0x24, "guardClawDefeatedGamebit"),
    ]),
    # CFTreasSharpy
    401: ObjSetup([
        ObjSetupField(0x1E, "unk1E"),
        ObjSetupField(0x20, "unk20"),
    ]),
    # CFCheapGalleon
    406: ObjSetup([
        ObjSetupField(0x1E, "gamebitForceLowDetail"),
    ]),
    # IMicicle
    409: ObjSetup([
        ObjSetupField(0x1C, "gamebitEnabled"),
    ]),
    # DFriverflow
    418: ObjSetup([
        ObjSetupField(0x1C, "toggleGamebit"),
    ]),
    # DFbarrelcreator
    425: ObjSetup([
        ObjSetupField(0x1A, "gamebitStop"),
    ]),
    # DFmole
    428: BADDIE_SETUP,
    # DFSH_door1Special
    429: ObjSetup([
        ObjSetupField(0x18, "gamebitOpened"),
        ObjSetupField(0x1A, "gamebitDoorState"),
        ObjSetupField(0x22, "gamebitLit"),
    ]),
    # DFSH_door2Special
    430: ObjSetup([
        ObjSetupField(0x18, "gamebitOpened"),
        ObjSetupField(0x1A, "gamebitDoorState"),
        ObjSetupField(0x22, "gamebitLit"),
    ]),
    # MMP_gyservent
    446: ObjSetup([
        ObjSetupField(0x1E, "gamebitOff"),
    ]),
    # CCfirecrystal
    452: ObjSetup([
        ObjSetupField(0x18, "gamebitCollected"),
    ]),
    # CCkrazoaTablet
    459: ObjSetup([
        ObjSetupField(0x1E, "gamebit"),
    ]),
    # CCdockdoor
    465: ObjSetup([
        ObjSetupField(0x1E, "unk1E"),
    ]),
    # NWsfx
    493: ObjSetup([
        ObjSetupField(0x18, "gamebitDisable"),
    ]),
    # NWtreebridge
    494: ObjSetup([
        ObjSetupField(0x1E, "gamebit"),
    ]),
    # SHmushroom
    502: ObjSetup([
        ObjSetupField(0x1A, "gamebitCollected"),
    ]),
    # SHkillermushroom
    503: ObjSetup([
        ObjSetupField(0x1C, "gamebitAttacked"),
    ]),
    # SHrocketmushroom
    504: ObjSetup([
        ObjSetupField(0x1C, "gamebitGrow"),
    ]),
    # SHPlantSpore
    506: ObjSetup([
        ObjSetupField(0x1E, "unk1E"),
        ObjSetupField(0x20, "unk20"),
    ]),
    # SHroot
    510: ObjSetup([
        ObjSetupField(0x1E, "unk1E"),
    ]),
    # SCbabyLightFoot
    517: BADDIE_SETUP,
    # SCcollectables
    523: ObjSetup([
        ObjSetupField(0x1C, "gamebitCollected"),
        ObjSetupField(0x24, "gamebitSecondary"),
        ObjSetupField(0x2C, "gamebitCount"),
    ]),
    # SC_meterblock
    526: ObjSetup([
        ObjSetupField(0x1C, "gamebitDeactivated"),
        ObjSetupField(0x24, "gamebitActivated"),
        ObjSetupField(0x2C, "gamebitMeterProgress"),
    ]),
    # DIMLavaBallGenerator
    530: ObjSetup([
        ObjSetupField(0x1E, "gamebit1"),
        ObjSetupField(0x22, "gamebit2"),
        ObjSetupField(0x24, "gamebit3"),
    ]),
    # DIMicewall
    535: ObjSetup([
        ObjSetupField(0x1E, "unk1E"),
    ]),
    # DIMCannon
    537: ObjSetup([
        ObjSetupField(0x18, "gamebitSiloCoverOpen"),
        ObjSetupField(0x1A, "gamebitCannonClawDead"),
        ObjSetupField(0x1C, "gamebitCannonClawAboard"),
        ObjSetupField(0x1E, "gamebitCannonClawTruce"),
        ObjSetupField(0x20, "gamebitSiloEnter"),
        ObjSetupField(0x22, "gamebitSiloExit"),
    ]),
    # DIMWoodDoor
    542: ObjSetup([
        ObjSetupField(0x1E, "gamebitDestroyed"),
    ]),
    # DIMTent
    545: ObjSetup([
        ObjSetupField(0x1E, "gamebitBurnt"),
    ]),
    # DIMBikeDoor
    546: ObjSetup([
        ObjSetupField(0x1E, "unk1E"),
    ]),
    # GP_ShrinePillar
    549: ObjSetup([
        ObjSetupField(0x18, "gamebitRise"),
        ObjSetupField(0x1A, "gamebitRaised"),
        ObjSetupField(0x22, "gamebitDoorOpen"),
    ]),
    # WGBouncyVine
    550: ObjSetup([
        ObjSetupField(0x1A, "gamebitA"),
        ObjSetupField(0x1C, "gamebitB"),
        ObjSetupField(0x1E, "gamebitHurtPlayer"),
    ]),
    # DIM2Icicle
    562: ObjSetup([
        ObjSetupField(0x1E, "gamebitFell"),
    ]),
    # DIM_Boss
    565: BADDIE_SETUP,
    # DIMbosscrackparticles
    571: ObjSetup([
        ObjSetupField(0x1E, "gamebit"),
    ]),
    # SB_ShipGunBroke
    589: ObjSetup([
        ObjSetupField(0x1E, "gunDestroyedGamebit"),
    ]),
    # WL_lasertarget
    595: ObjSetup([
        ObjSetupField(0x1E, "gamebitA"),
        ObjSetupField(0x20, "gamebitB"),
    ]),
    # WL_PressureSwitch
    596: ObjSetup([
        ObjSetupField(0x1C, "gameBitPressed"),
    ]),
    # WL_WallTorch
    604: ObjSetup([
        ObjSetupField(0x1E, "gamebit"),
    ]),
    # WL_spiritplace
    610: ObjSetup([
        ObjSetupField(0x1E, "bit1"),
        ObjSetupField(0x20, "bit2"),
    ]),
    # WL_seqpoint
    611: ObjSetup([
        ObjSetupField(0x1E, "conditionBit"),
        ObjSetupField(0x20, "triggeredBit"),
    ]),
    # WL_SpiritSet
    613: ObjSetup([
        ObjSetupField(0x1E, "gamebit"),
    ]),
    # VFP_Statue
    622: ObjSetup([
        ObjSetupField(0x1E, "gamebit"),
    ]),
    # VFP_PodiumPoint
    633: ObjSetup([
        ObjSetupField(0x1E, "setGamebit"),
        ObjSetupField(0x20, "conditionGamebit"),
    ]),
    # VFP_flamepoint
    634: ObjSetup([
        ObjSetupField(0x1E, "gamebitFlamed"),
    ]),
    # VFP_SpellPlace
    637: ObjSetup([
        ObjSetupField(0x1E, "unk1E"),
        ObjSetupField(0x20, "unk20"),
    ]),
    # DFPLift
    641: ObjSetup([
        ObjSetupField(0x20, "gamebitActivated"),
    ]),
    # DFP_floorbar
    644: ObjSetup([
        ObjSetupField(0x20, "gamebitLowered"),
    ]),
    # DFP_RotatePuzzle
    647: ObjSetup([
        ObjSetupField(0x20, "gamebitSpin"),
    ]),
    # DFP_Statue1
    648: ObjSetup([
        ObjSetupField(0x20, "gamebitMove"),
    ]),
    # DFP_PerchSwitch
    649: ObjSetup([
        ObjSetupField(0x1E, "gamebitLocked"),
        ObjSetupField(0x20, "gamebitPulled"),
    ]),
    # DFP_SpellPlace
    652: ObjSetup([
        ObjSetupField(0x1E, "unk1E"),
        ObjSetupField(0x20, "unk20"),
    ]),
    # textblock
    654: ObjSetup([
        ObjSetupField(0x1E, "gamebitTranslated"),
        ObjSetupField(0x20, "gamebitInteractable"),
    ]),
    # DFP_Platform1
    655: ObjSetup([
        ObjSetupField(0x1E, "gamebitMoveZ"),
        ObjSetupField(0x20, "gamebitMoveY"),
    ]),
    # DBTrigger
    659: ObjSetup([
        ObjSetupField(0x1E, "gamebit"),
    ]),
    # DBSpike
    660: ObjSetup([
        ObjSetupField(0x1E, "gamebit"),
    ]),
    # DBPlaceHolder
    661: ObjSetup([
        ObjSetupField(0x18, "gamebit1"),
        ObjSetupField(0x1A, "gamebit2"),
    ]),
    # DBEgg
    662: ObjSetup([
        ObjSetupField(0x1C, "unk1C"),
        ObjSetupField(0x24, "unk24"),
    ]),
    # DBMagicBridge
    667: ObjSetup([
        ObjSetupField(0x1A, "gamebit1A"),
        ObjSetupField(0x1E, "gamebitVisible"),
        ObjSetupField(0x20, "gamebit20"),
    ]),
    # DBdiamond
    668: ObjSetup([
        ObjSetupField(0x1C, "flag1"),
        ObjSetupField(0x22, "flag2"),
    ]),
    # DBbignest
    673: ObjSetup([
        ObjSetupField(0x18, "gamebitB"),
        ObjSetupField(0x1A, "gamebitRestoreState"),
        ObjSetupField(0x22, "gamebitC"),
    ]),
    # DBwaterplant
    674: ObjSetup([
        ObjSetupField(0x1E, "gamebitRestoreState"),
        ObjSetupField(0x20, "gamebitDamaged"),
    ]),
    # DBplatform
    676: ObjSetup([
        ObjSetupField(0x1E, "gamebitStandingOn"),
        ObjSetupField(0x20, "gamebitStateTransition"),
    ]),
    # DBbridgeanim
    678: ObjSetup([
        ObjSetupField(0x20, "gamebitStart"),
    ]),
    # DBDustGeezer
    684: ObjSetup([
        ObjSetupField(0x20, "gamebitHidden"),
    ]),
    # DBstealerworm
    688: BADDIE_SETUP,
    # BossDrakor
    697: BADDIE_SETUP,
    # KTrex
    702: BADDIE_SETUP,
    # KT_RexFloorSwitch
    703: ObjSetup([
        ObjSetupField(0x1A, "unk1A"),
        ObjSetupField(0x1C, "unk1C"),
    ]),
    # KT_Laserwall
    704: ObjSetup([
        ObjSetupField(0x1A, "unk1A"),
        ObjSetupField(0x1E, "unk1E"),
    ]),
    # KT_Fallingrocks
    706: ObjSetup([
        ObjSetupField(0x24, "gamebitFalling"),
    ]),
    # IMSnowBike
    711: ObjSetup([
        ObjSetupField(0x1A, "unk1A"),
        ObjSetupField(0x1E, "unk1E"),
    ]),
    # DRcloudrunner
    714: ObjSetup([
        ObjSetupField(0x1E, "unk1E"),
    ]),
    # CRspellStone
    730: ObjSetup([
        ObjSetupField(0x1E, "gamebitVehicleHit"),
        ObjSetupField(0x20, "gamebitDisable"),
    ]),
    # CRFuelTank
    731: ObjSetup([
        ObjSetupField(0x1E, "gamebit"),
    ]),
    # CFsnowbike
    732: ObjSetup([
        ObjSetupField(0x1A, "gamebitUnlocked"),
        ObjSetupField(0x1E, "gamebitFinished"),
    ]),
    # DR_PushCart
    734: ObjSetup([
        ObjSetupField(0x20, "unk20"),
    ]),
    # DRLavaControl
    737: ObjSetup([
        ObjSetupField(0x1E, "gameBitFrozen"),
    ]),
    # DR_SupGuardClaw
    739: ObjSetup([
        ObjSetupField(0x1E, "gamebitFinished"),
    ]),
    # DR_ExplodeDoor
    742: ObjSetup([
        ObjSetupField(0x1E, "gamebitExplode"),
    ]),
    # DR_IceFire
    751: ObjSetup([
        ObjSetupField(0x18, "gamebitFrozen"),
    ]),
    # DR_CloudPerch
    764: ObjSetup([
        ObjSetupField(0x1E, "gamebitEnabled"),
    ]),
    # DR_EarthCallPad
    765: ObjSetup([
        ObjSetupField(0x20, "gamebitEnabled"),
    ]),
    # WCBeacon
    780: ObjSetup([
        ObjSetupField(0x1E, "gamebitLit"),
        ObjSetupField(0x20, "gamebitRise"),
    ]),
    # WCpressureswitch
    781: ObjSetup([
        ObjSetupField(0x1A, "gameBitPressed"),
        ObjSetupField(0x20, "gamebitActivated"),
    ]),
    # WCtrexstatue
    784: ObjSetup([
        ObjSetupField(0x1E, "gamebitActivated"),
    ]),
    # WCuseobj
    785: ObjSetup([
        ObjSetupField(0x1C, "gamebitInteracted"),
        ObjSetupField(0x1E, "gamebitUsedItem"),
        ObjSetupField(0x22, "gamebitUnlocked"),
    ]),
    # WCApertureSymbol
    787: ObjSetup([
        ObjSetupField(0x1E, "gamebitViewed"),
        ObjSetupField(0x20, "gamebitEnabled"),
    ]),
    # WCSunTempleLaser
    788: ObjSetup([
        ObjSetupField(0x1E, "gamebitEnabled"),
    ]),
    # WCTempleDial
    789: ObjSetup([
        ObjSetupField(0x1E, "gamebitFinished"),
    ]),
    # WCTempleBridge
    790: ObjSetup([
        ObjSetupField(0x1E, "gamebitVisible"),
    ]),
}

class MapObject(TypedDict):
    map_id: int
    map_name: str
    obj_id: int
    obj_name: str
    dll_id: int
    uid: int
    acts: list[int]
    load_flags: int
    byte5: int
    byte6: int
    fade_dist: int
    x: float
    y: float
    z: float

def check_trigger_object(data: bytes, offset: int, bits: set[int], found: list[ObjSetupBit]):
    for cmdi in range(8):
        cond, cmdid, param1, param2 = struct.unpack_from(">BBBB", data, offset + 0x18 + (cmdi * 4))
    
        if cmdid == 0x12 or cmdid == 0x21:
            bit = param2 | (param1 << 8)

            action: str
            if cmdid == 0x12:
                action_int = bit >> 14
                if action_int == 0:
                    action = "Unset"
                elif action_int == 1:
                    action = "Set"
                else:
                    action = "Invert"
                bit = bit & 0x3FFF
            else:
                action = f"Toggle bit {bit >> 13}"
                bit = bit & 0x1FFF

            if bit not in bits:
                continue

            conddetail = []
            if cond & 1:
                conddetail.append("IN")
            if cond & 2:
                conddetail.append("OUT")
            if cond & 4:
                conddetail.append("REENTER")
            if cond & 8:
                conddetail.append("REEXIT")
            if cond & 0x10:
                conddetail.append("CONTINUOUS")
            if cond & 0x20:
                conddetail.append("RESTORE")

            found.append(ObjSetupBit(bit, f"commands[{cmdi}] ({action}; {', '.join(conddetail)})"))

def length_to_struct_format(length: int, signed: bool):
    if length == 1:
        return "B" if not signed else "b"
    elif length == 2:
        return "H" if not signed else "h"
    elif length == 4:
        return "I" if not signed else "i"
    else:
        raise NotImplementedError()

last_map_id = -1
def check_map_object(data: bytes, offset: int, romdef: MapObject, bits: set[int]):
    global last_map_id
    found: list[ObjSetupBit] = []

    dll_no = romdef["dll_id"] - 0x8000 + 210 - 1

    basic_setup = OBJ_SETUPS.get(dll_no, None)
    if basic_setup != None:
        for field in basic_setup.fields:
            value = struct.unpack_from(
                f">{length_to_struct_format(field.length, field.signed)}", 
                data, 
                offset + field.offset)[0]
            if value in bits:
                found.append(ObjSetupBit(value, field.name))

    if dll_no == 325:
        check_trigger_object(data, offset, bits, found)

    if len(found) == 0:
        return

    if last_map_id != romdef["map_id"]:
        last_map_id = romdef["map_id"]
        print(f"{romdef['map_name']}:")

    print(f"  {romdef['obj_name']} 0x{romdef['obj_id']:X} (UID 0x{romdef['uid']:X}):")
    print(f"    acts: {','.join([str(s) for s in romdef['acts']])}")
    print(f"    load flags: 0x{romdef['load_flags']:X}  fade flags: 0x{romdef['byte5'] & 0xF}")
    if romdef['load_flags'] & 0x10:
        print(f"    object group: {romdef['byte6']}  fade distance: {romdef['fade_dist']}")
    else:
        print(f"    load distance: {romdef['byte6']}  fade distance: {romdef['fade_dist']}")
    print(f"    pos: {romdef['x']},{romdef['y']},{romdef['z']}")

    for bit in found:
        print(f"    {bit.desc}: 0x{bit.bit:X}")

def parse_act_exclusions_as_inclusions(exclusions1: int, exclusions2: int):
    acts: list[int] = []
    for i in range(8):
        if not (exclusions1 & (1 << i)):
            acts.append(i + 1)
    for i in range(4):
        if not (exclusions2 & (1 << (7 - i))):
            acts.append(i + 9)

    return acts

def check_maps(maps: BufferedReader, maps_tab: BufferedReader, mapinfo: BufferedReader, objmap: dict[int, Obj], bits: set[int]):
    offsets: list[int] = []
    while True:
        offset = struct.unpack(">I", maps_tab.read(4))[0]
        if offset == 0xFFFFFFFF:
            break
        offsets.append(offset)
    
    names: list[str] = []
    for i in range(120):
        if i >= 98:
            names.append("")
        else:
            names.append(mapinfo.read(0x20)[:0x1C].rstrip(b'\0').decode())

    i = 0
    m = 0
    while (i + 4) < len(offsets):
        maps.seek(offsets[i + 4])
        data = maps.read(offsets[i + 5] - offsets[i + 4])

        offset = 0
        while offset < len(data):
            obj_id, quarter_size = struct.unpack_from(">hB", data, offset + 0x0)
            if quarter_size == 0:
                break
            if (quarter_size * 4) >= 0x18 and m != 17:
                exclusions1 = struct.unpack_from(">B", data, offset + 0x3)[0]
                load_flags, byte5 = struct.unpack_from(">BB", data, offset + 0x4)
                byte6, fade_dist = struct.unpack_from(">BB", data, offset + 0x6)
                x, y, z = struct.unpack_from(">fff", data, offset + 0x8)
                uid = struct.unpack_from(">i", data, offset + 0x14)[0]
                acts = parse_act_exclusions_as_inclusions(exclusions1, byte5)

                obj = objmap.get(obj_id, None)

                romdef: MapObject = {
                    "map_id": m,
                    "map_name": names[m],
                    "obj_id": obj_id,
                    "obj_name": obj['name'] if obj != None else "(none)",
                    "dll_id": obj["dll_id"] if obj != None else -1,
                    "uid": uid,
                    "acts": acts,
                    "load_flags": load_flags,
                    "byte5": byte5,
                    "byte6": byte6,
                    "fade_dist": fade_dist,
                    "x": x,
                    "y": y,
                    "z": z
                }

                check_map_object(data, offset, romdef, bits)

            offset += (quarter_size * 4)

        i += 7
        m += 1

class Curve(TypedDict):
    events: list[bytes]

class ObjSeq(TypedDict):
    curves: list[Curve]

def __read_next_anim_curve(animcurves_tab: BufferedReader, animcurves_bin: BufferedReader) -> Curve:
    file_size, event_count, bin_offset = struct.unpack(">HHI", animcurves_tab.read(8))

    animcurves_bin.seek(bin_offset)

    events: list[bytes] = []
    for _ in range(event_count):
        evt = animcurves_bin.read(4)
        events.append(evt)
    
    keyframe_count = (file_size - (event_count * 4)) // 8
    animcurves_bin.seek(8 * keyframe_count, os.SEEK_CUR)  

    return { "events": events }

def seqs_from_bin(
        objseq2curve_tab: BufferedReader, 
        animcurves_tab: BufferedReader,
        animcurves_bin: BufferedReader) -> list[ObjSeq]:
    objseq2curve_count = 0
    while True:
        idx = struct.unpack(">H", objseq2curve_tab.read(2))[0]
        if idx == 0xFFFF:
            break
        objseq2curve_count += 1
    objseq2curve_count -= 1
    objseq2curve_tab.seek(0)

    animcurves_tab.seek(0, os.SEEK_END)
    animcurves_tab.seek(0)

    objseq_curves_start = struct.unpack(">H", objseq2curve_tab.read(2))[0]
    objseq2curve_tab.seek(0)
    standalone_curves_count = objseq_curves_start

    for _ in range(standalone_curves_count):
        __read_next_anim_curve(animcurves_tab, animcurves_bin)
    
    seqs: list[ObjSeq] = []
    for i in range(objseq2curve_count):
        curves_start_idx, curves_end_idx = struct.unpack(">HH", objseq2curve_tab.read(4))
        objseq2curve_tab.seek(-2, os.SEEK_CUR)
        num_curves = curves_end_idx - curves_start_idx

        animcurves_tab.seek(curves_start_idx * 8)

        curves: list[Curve] = []
        for _ in range(num_curves):
            curves.append(__read_next_anim_curve(animcurves_tab, animcurves_bin))

        seqs.append({ "curves": curves })

    return seqs

class EventBit:
    bit: int
    desc: str

    def __init__(self, bit: int, desc: str) -> None:
        self.bit = bit
        self.desc = desc

def check_events(events: list[bytes], bits: set[int]):
    found: list[EventBit] = []

    time_offset = 0
    k = 0
    num_events = len(events)
    while k < num_events:
        event = events[k]
        evt_type, delay, params = struct.unpack(">bBh", event)
        
        if evt_type == 13:
            envfx_type = (params >> 0xC) & 0xF

            if envfx_type == 11: # SET_BIT
                _, _, bit = struct.unpack(">bBh", events[k + 1])
                if bit in bits:
                    found.append(EventBit(bit, f"events[{k}] (SET_BIT @ {time_offset})"))
            elif envfx_type == 12: # CLEAR_BIT
                _, _, bit = struct.unpack(">bBh", events[k + 1])
                if bit in bits:
                    found.append(EventBit(bit, f"events[{k}] (CLEAR_BIT @ {time_offset})"))

        k += 1

        if evt_type == 0: # settime event
            time_offset = params
        elif evt_type != 15: # sfx_with_duration event
            time_offset += delay

        if evt_type == 11: # code event
            j = 0
            while j < params and k < num_events:
                k += 1
                j += 1
    
    return found

def check_seqs(seqs: list[ObjSeq], bits: set[int]):
    for seqno, seq in enumerate(seqs):
        first = True

        for i in range(len(seq["curves"])):
            curve = seq["curves"][i]
            found = check_events(curve["events"], bits)

            if len(found) == 0:
                continue

            if first:
                first = False
                print(f"ObjSeq 0x{seqno:X}:")

            print(f"  Actor {i}:")

            for evt in found:
                print(f"    {evt.desc}: 0x{evt.bit:X}")

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project.", default=str(SCRIPT_DIR.joinpath("..")))
    parser.add_argument("bits", nargs="+", type=str, help="Bit IDs to search for.")

    args = parser.parse_args()

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    bits = set([int(bit, base=0) for bit in args.bits])

    objmap: dict[int, Obj] = {}
    with open(BIN_ASSETS_DIR.joinpath("OBJECTS.bin"), "rb") as objects_bin, \
         open(BIN_ASSETS_DIR.joinpath("OBJECTS.tab"), "rb") as objects_tab, \
         open(BIN_ASSETS_DIR.joinpath("OBJINDEX.bin"), "rb") as objects_idx:
        objects = read_objects(objects_bin, objects_tab, objects_idx)
        for obj in objects:
            objmap[obj["id"]] = obj

    with open(BIN_ASSETS_DIR.joinpath("MAPS.bin"), "rb") as maps, \
         open(BIN_ASSETS_DIR.joinpath("MAPS.tab"), "rb") as maps_tab, \
         open(BIN_ASSETS_DIR.joinpath("MAPINFO.bin"), "rb") as mapinfo:
        check_maps(maps, maps_tab, mapinfo, objmap, bits)

    with open(BIN_ASSETS_DIR.joinpath("OBJSEQ2CURVE.tab"), "rb") as objseq2curve_tab_file, \
        open(BIN_ASSETS_DIR.joinpath("ANIMCURVES.tab"), "rb") as animcurves_tab_file, \
        open(BIN_ASSETS_DIR.joinpath("ANIMCURVES.bin"), "rb") as animcurves_bin_file:
        seqs = seqs_from_bin(
            objseq2curve_tab_file,
            animcurves_tab_file, animcurves_bin_file)
        check_seqs(seqs, bits)

if __name__ == "__main__":
    main()
