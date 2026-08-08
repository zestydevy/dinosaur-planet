#include "dlls/objects/437_EWTrobotpatrol.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/curves.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/rand.h"
#include "dll.h"
#include "macros.h"

/** @file EWTrobotpatrolB(ase) ? */

#define MAX_ROBOS 12
#define MAX_NODES 24

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 unk18; // unused due to, presumably, missing code. likely the number of robos to spawn 
/*19*/ u8 numNodes; // set but unused, the dll discovers this count on its own
/*1A*/ u8 roboFadeDistance;
/*1B*/ u8 maxSearchTime; // maximum search time (after aggro) divided by 64
} EWTrobotpatrolB_Setup;

typedef struct {
/*00*/ Object* robos[MAX_ROBOS];
/*30*/ CurveSetup* nodes[MAX_NODES];
/*90*/ u8 numNodes;
/*91*/ u8 engagingPlayer;
/*92*/ u8 searchingForPlayer;
/*94*/ s16 aggroCounter;
/*96*/ s16 lastAggroCounter;
/*98*/ f32 searchTimeElapsed;
} EWTrobotpatrolB_Data;

/*0x0*/ static CurveSetup* sPathfindVisited[MAX_NODES];
/*0x60*/ static CurveSetup* sPathfindGoal;
/*0x64*/ static Object* sPathfindSelf;

static void EWTrobotpatrolB_roboCallback(Object* self, Object* robot, s32 aggro, u32 currUID);
static void EWTrobotpatrolB_pathRoboRandom(Object* self, Object* robo, u32 currUID);
static void EWTrobotpatrolB_pathRoboToPlayer(Object* self, Object* robo, u32 currUID);
static CurveSetup* EWTrobotpatrolB_getPathNode(Object* self, u32 uID);
static s16 EWTrobotpatrolB_calcLinkPathDistance(s32 searchIdx, CurveSetup* currNode, s32 linkUID, s32 iteration);
static void EWTrobotpatrolB_initCurveNetworkSearchLink(CurveSetup** nodes, u8* count, s32 srcUID, s32 linkUID, u8 count2);
static void EWTrobotpatrolB_findNodesClosestToPlayer(CurveSetup** closest, CurveSetup** nodes, Object* player);
static f32 EWTrobotpatrolB_objDist2Node(CurveSetup* node, Object* obj);

// offset: 0x0 | ctor
void EWTrobotpatrolB_ctor(void* dll) { }

// offset: 0xC | dtor
void EWTrobotpatrolB_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void EWTrobotpatrolB_obj_Setup(Object* self, ObjSetup* setup, s32 reset) { }

// offset: 0x2C | func: 1 | export: 1
void EWTrobotpatrolB_obj_Control(Object* self) { }

// offset: 0x38 | func: 2 | export: 2
void EWTrobotpatrolB_obj_Update(Object* self) {
    EWTrobotpatrolB_Data* objdata;
    EWTrobotpatrolB_Setup* setup;

    objdata = self->data;
    setup = (EWTrobotpatrolB_Setup*)self->setup;
    // If any robo has aggro, engage the player
    if (objdata->aggroCounter == objdata->lastAggroCounter) {
        objdata->engagingPlayer = FALSE;
    } else {
        objdata->engagingPlayer = TRUE;
        objdata->lastAggroCounter = objdata->aggroCounter;
    }
    // After aggro, enter a search phase for some time
    if (!objdata->engagingPlayer && objdata->searchingForPlayer) {
        objdata->searchTimeElapsed += gUpdateRateF;
        if ((f32) (setup->maxSearchTime << 6) < objdata->searchTimeElapsed) {
            objdata->searchingForPlayer = FALSE;
            objdata->searchTimeElapsed = 0.0f;
        }
    } else {
        objdata->searchingForPlayer = objdata->engagingPlayer;
    }
}

// offset: 0xE0 | func: 3 | export: 3
void EWTrobotpatrolB_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x134 | func: 4 | export: 4
void EWTrobotpatrolB_obj_Free(Object* self, s32 onlySelf) {
    s32 i;
    EWTrobotpatrolB_Data* objdata;

    objdata = self->data;
    if (onlySelf == 0) {
        for (i = 0; i < MAX_ROBOS; i++) {
            if (objdata->robos[i] != NULL) {
                objFreeObject(objdata->robos[i]);
            }
        }
    }
}

// offset: 0x1AC | func: 5 | export: 5
u32 EWTrobotpatrolB_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1BC | func: 6 | export: 6
u32 EWTrobotpatrolB_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(EWTrobotpatrolB_Data);
}

// offset: 0x1D0 | func: 7
void EWTrobotpatrolB_spawnRobo(Object* self, CurveSetup* startNode) {
    EWTrobotpatrolB_Data* objdata;
    EWTrobotpatrolB_Setup* setup;
    ObjSetup* roboSetup;
    Object* robo;
    s32 i;

    objdata = self->data;
    setup = (EWTrobotpatrolB_Setup*)self->setup;
    i = 0;
    while (objdata->robos[i] != NULL) {
        i++;
    }
    roboSetup = objAllocSetup(sizeof(EWTrobotpatrol_Setup), OBJ_EWTrobotpatrol);
    roboSetup->actExclusions1 = setup->base.actExclusions1;
    roboSetup->loadFlags = OBJSETUP_LOAD_MANUAL;
    roboSetup->fadeDistance = setup->roboFadeDistance;
    roboSetup->x = startNode->pos.x;
    roboSetup->y = startNode->pos.y;
    roboSetup->z = startNode->pos.z;
    robo = objSetupObject(roboSetup, OBJINIT_STANDALONE | OBJINIT_FLAG4, -1, -1, self->parent);
    objdata->robos[i] = robo;
    ((DLL_437_EWTrobotpatrol*)robo->dll)->vtbl->SetBase(robo, self, EWTrobotpatrolB_roboCallback);
    EWTrobotpatrolB_pathRoboRandom(self, robo, startNode->uID);
}

// offset: 0x30C | func: 8
static void EWTrobotpatrolB_roboCallback(Object* self, Object* robot, s32 aggro, u32 currUID) {
    EWTrobotpatrolB_Data* objdata = self->data;
    
    if (aggro == 0) {
        if (objdata->engagingPlayer || objdata->searchingForPlayer) {
            EWTrobotpatrolB_pathRoboToPlayer(self, robot, currUID);
        } else {
            EWTrobotpatrolB_pathRoboRandom(self, robot, currUID);
        }
    } else if (aggro == 1) {
        // Can see player
        objdata->aggroCounter++;
    }
}

// offset: 0x3A4 | func: 9
static void EWTrobotpatrolB_pathRoboRandom(Object* self, Object* robo, u32 currUID) {
    CurveSetup* node;
    u8 max;

    max = 0;
    node = EWTrobotpatrolB_getPathNode(self, currUID);
    while (node->links[max + 1] != -1 && max != 3) {
        max++;
    }
    ((DLL_437_EWTrobotpatrol*)robo->dll)->vtbl->MoveTo(robo, node->links[mathRnd(0, max)]);
}

// offset: 0x470 | func: 10
static void EWTrobotpatrolB_pathRoboToPlayer(Object* self, Object* robo, u32 currUID) {
    EWTrobotpatrolB_Data* objdata = self->data;
    CurveSetup* currNode;
    CurveSetup* closestNodes[2];
    s16 linkPathDists[4];
    s16 shortestDist;
    s32 bestLinkIdx;
    s32 i;

    // Calculate distance from current node to node closest to player via each link
    currNode = EWTrobotpatrolB_getPathNode(self, currUID);
    EWTrobotpatrolB_findNodesClosestToPlayer(closestNodes, objdata->nodes, objGetPlayer());
    sPathfindVisited[0] = currNode;
    sPathfindSelf = self;
    if (currNode == closestNodes[0]) {
        sPathfindGoal = closestNodes[1];
    } else {
        sPathfindGoal = closestNodes[0];
    }
    for (i = 0; i < 4; i++) {
        if (currNode->links[i] != -1) {
            linkPathDists[i] = EWTrobotpatrolB_calcLinkPathDistance(1, currNode, currNode->links[i], 1);
        } else {
            linkPathDists[i] = -1;
        }
    }
    // Choose the link with the shortest path
    shortestDist = linkPathDists[0];
    bestLinkIdx = 0;
    i = 1;
    while (i < 4) {
        if ((linkPathDists[i] != -1) && ((shortestDist == -1) || (linkPathDists[i] < shortestDist))) {
            shortestDist = linkPathDists[i];
            bestLinkIdx = i;
        }
        i += 1;
    }
    ((DLL_437_EWTrobotpatrol*)robo->dll)->vtbl->MoveTo(robo, currNode->links[bestLinkIdx]);
}

// offset: 0x60C | func: 11
static s16 EWTrobotpatrolB_calcLinkPathDistance(s32 searchIdx, CurveSetup* currNode, s32 linkUID, s32 iteration) {
    CurveSetup* linkNode = EWTrobotpatrolB_getPathNode(sPathfindSelf, linkUID);
    s16 linkPathDists[] = {-1, -1, -1};
    s16 shortestDist;
    u8 i;
    u8 nlinks;
    
    i = 0;
    nlinks = 0;
    if (linkNode == sPathfindGoal) {
        // Found goal
        while (currNode->uID != (u32)linkNode->links[i]) {
            i++;
        }
        return linkNode->type1E.linkDist[i];
    }
    if (iteration == 6) {
        // Too many iterations, give up
        return -1;
    }
    while (i < searchIdx) {
        if (linkNode == sPathfindVisited[i]) {
            // Already visited node, bail
            return -1;
        }
        i++;
    }
    sPathfindVisited[searchIdx++] = linkNode;
    // Recursively calculate the dist to target for each link
    i = 0;
    while (i < 4) {
        if ((currNode->uID != (u32)linkNode->links[i]) && (linkNode->links[i] != -1)) {
            linkPathDists[nlinks] = EWTrobotpatrolB_calcLinkPathDistance(searchIdx, linkNode, linkNode->links[i], iteration + 1);
            nlinks++;
        }
        i++;
    }
    // Return the shortest path dist, as that's the only one we care about
    shortestDist = linkPathDists[0];
    i = 1;
    while (i < 3) {
        if ((linkPathDists[i] != -1) && ((shortestDist == -1) || (linkPathDists[i] < shortestDist))) {
            shortestDist = linkPathDists[i];
        }
        i++;
    }
    if (shortestDist == -1) {
        return -1;
    }
    i = 0;
    while (currNode->uID != (u32)linkNode->links[i]) {
        i++;
    }
    return linkNode->type1E.linkDist[i] + shortestDist;
}

// offset: 0x88C | func: 12
void EWTrobotpatrolB_initCurveNetwork(Object* self) {
    EWTrobotpatrolB_Data* objdata;
    s8 i;
    s32 nodeTypes[] = {0x1E};
    s32 uid;
    s8 k;
    CurveSetup* temp;

    objdata = self->data;
    // Reset node list
    for (i = 0; i < MAX_NODES; i++) {
        objdata->nodes[i] = NULL;
    }
    // Get nearest 0x1E type curve node
    uid = gDLL_26_Curves->vtbl->func_1E4(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, nodeTypes, 1, -1);
    objdata->nodes[0] = gDLL_26_Curves->vtbl->func_39C(uid);
    if (uid == -1) {
        STUBBED_PRINTF(" Could Not find a nearest node ");
        return;
    }
    if (objdata->nodes[0] == NULL) {
        STUBBED_PRINTF(" Could Not get Nearest node ");
        return;
    }
    // Recursively discover curve network
    objdata->numNodes = 1;
    for (i = 0; i < 4; i++) {
        if (objdata->nodes[0]->links[i] != -1) {
            EWTrobotpatrolB_initCurveNetworkSearchLink(objdata->nodes, &objdata->numNodes, uid, objdata->nodes[0]->links[i], 1);
        }
    }
    // Sort nodes by UID ascending
    for (i = 0; i < (objdata->numNodes - 1); i++) {
        k = objdata->numNodes - 2;
        while (k >= i) {
            if (objdata->nodes[k + 1]->uID < objdata->nodes[k]->uID) {
                temp = objdata->nodes[k];
                objdata->nodes[k] = objdata->nodes[k + 1];
                objdata->nodes[k + 1] = temp;
            }
            k -= 1;
        }
    }
}

// offset: 0xA60 | func: 13
static void EWTrobotpatrolB_initCurveNetworkSearchLink(CurveSetup** nodes, u8* count, s32 srcUID, s32 linkUID, u8 count2) {
    CurveSetup* linkNode;
    s32 i;

    linkNode = gDLL_26_Curves->vtbl->func_39C(linkUID);
    for (i = 0; i < MAX_NODES; i++) {
        if (linkNode == nodes[i]) {
            // Path loops back on itself
            return;
        }
    }
    nodes[*count] = linkNode;
    *count += 1;
    if (count2 != MAX_NODES) {
        // Recurse
        for (i = 0; i != 4; i++) {
            if ((srcUID != linkNode->links[i]) && (linkNode->links[i] != -1)) {
                EWTrobotpatrolB_initCurveNetworkSearchLink(nodes, count, linkUID, linkNode->links[i], count2 + 1);
            }
        }
    }
}

// offset: 0xBA0 | func: 14
static void EWTrobotpatrolB_findNodesClosestToPlayer(CurveSetup** closest, CurveSetup** nodes, Object* player) {
    s32 i;
    s32 indices[2];
    f32 temp;
    f32 dists[2];
    f32 dist;

    dists[0] = EWTrobotpatrolB_objDist2Node(nodes[0], player);
    dists[1] = EWTrobotpatrolB_objDist2Node(nodes[1], player);
    indices[0] = 0;
    indices[1] = 1;
    temp = dists[0];
    if (dists[1] < dists[0]) {
        indices[0] = 1;
        indices[1] = 0;
        dists[0] = dists[1];
        dists[1] = temp;
    }
    i = 2;
    while (i != MAX_NODES) {
        if (nodes[i] != NULL) {
            dist = EWTrobotpatrolB_objDist2Node(nodes[i], player);
            temp = dists[0];
            if (dist < dists[0]) {
                indices[1] = indices[0];
                indices[0] = i;
                dists[0] = dist;
                dists[1] = temp;
            } else if (dist < dists[1]) {
                indices[1] = i;
                dists[1] = dist;
            }
        }
        i += 1;
    }
    closest[0] = nodes[indices[0]];
    closest[1] = nodes[indices[1]];
}

// offset: 0xD10 | func: 15
static f32 EWTrobotpatrolB_objDist2Node(CurveSetup* node, Object* obj) {
    f32 xDiff;
    f32 yDiff;
    f32 zDiff;

    xDiff = obj->globalPosition.x - node->pos.x;
    yDiff = obj->globalPosition.y - node->pos.y;
    zDiff = obj->globalPosition.z - node->pos.z;
    return sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
}

// offset: 0xD7C | func: 16
static CurveSetup* EWTrobotpatrolB_getPathNode(Object* self, u32 uID) {
    EWTrobotpatrolB_Data* objdata = self->data;
    u8 currentIdx;
    u8 min;
    u8 max;
    
    // binary search
    max = objdata->numNodes - 1;
    min = 0;
    while (1) {
        currentIdx = (max + min) >> 1;
        if (objdata->nodes[currentIdx]->uID < uID) {
            min = currentIdx + 1;
        } else if (objdata->nodes[currentIdx]->uID > uID) {
            max = currentIdx - 1;
        } else {
            return objdata->nodes[currentIdx];
        }
    }
}
