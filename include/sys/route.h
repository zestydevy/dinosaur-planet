#ifndef _SYS_ROUTE_H
#define _SYS_ROUTE_H

#include "PR/ultratypes.h"
#include "sys/curves.h"

#define ROUTE_HEAP_CAPACITY 254
#define ROUTE_MAX_LENGTH 100

// Kyte specific structure
typedef struct {
/*0*/ u16 unk0;
/*2*/ u16 unk2;
/*4*/ CurveSetup *unk4;
} UnkVoxmap2Struct;

// size: 0x10
typedef struct {
/*0*/ CurveSetup *curve; // aka: node
/*4*/ u32 goalDist;
/*8*/ u32 netDist;
/*C*/ u8 prevPointIdx;
/*D*/ u8 nextPointIdx;
/*E*/ u8 visited;
} RoutePoint;

// element of a binary max-heap/priority queue
// size: 0x8
typedef struct {
/*0*/ u32 priority;
/*4*/ u16 pointIdx; // index of route point this element represents
} RouteHeapElement;

// size: 0x30
typedef struct {
/*00*/ RoutePoint *routePoints; // len: 254
/*04*/ RouteHeapElement *heap; // len: 254
/*08*/ CurveSetup **route; // len: 100
/*0C*/ Vec3f *goalPos;
/*10*/ void *goal; // extra data related to goal, depends on curve type being routed
/*14*/ u8 _unk14[0x18 - 0x14];
/*18*/ CurveSetup *startCurve;
/*1C*/ s32 lastPointIdx;
/*20*/ s16 routePointsLength;
/*22*/ s16 heapLength;
/*24*/ u32 unk24; // dist of closest route point to goal pos
/*28*/ u8 unk28;
/*29*/ u8 _unk29[0x2A - 0x29];
/*2A*/ s16 routeLength;
/*2C*/ s16 routeCurrIdx;
/*2E*/ u8 _unk2E[0x30 - 0x2E];
} Route;

void route_init(Route *route);
void route_free(Route *route);
s32 route_setup(Route* route, CurveSetup* startCurve, Vec3f* goalPos, void *goal, s32 arg5);
/**
 * return values:
 *  1: route found
 *  0: ran out of iterations, inconclusive
 * -1: no route found
 */
s32 route_find(Route* route, s32 maxIterations);
/** Reconstruct route, returns number of nodes in route */
s32 route_reconstruct(Route* route);
/** 
 * Get next node in route
 *
 * Assumes route_reconstruct was previously called.
 */
CurveSetup* route_next(Route* route);

#endif
