#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "sys/curves.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/route.h"
#include "dll.h"

static const char str_80099db0[] = "****** GOAL FOUND Iterations=%d ******\n";
static const char str_80099dd8[] = " VOXMAPS : Overflow in Route Points for Net Route Finding \n";
static const char str_80099e14[] = " Curve Node %i \n";
static const char str_80099e28[] = " Curve Node %i \n";
static const char str_80099e3c[] = "net_route: No support for this type of node %d x=%f y=%f z=%f\n";
static const char str_80099e7c[] = "**** HEAP INSERT ****\n";
static const char str_80099e94[] = "**** NODE FIND ****\n";

extern CurveSetup* D_800B4920;

void route_scan_neighbors(Route* route, RoutePoint* basePoint, s32 baseCurveIdx);
void route_add_neighbor(Route* route, RoutePoint* prevPoint, s32 prevPointIdx, u32 dist, CurveSetup* curve);
void route_clear(Route* route);
void route_heap_insert(RouteHeapElement* heap, s16* heapLengthPtr, u16 pointIdx, u32 dist);
void route_heap_change_priority(RouteHeapElement* heap, s32 heapLength, u16 pointIdx, u32 newPriority);
s32 route_heap_extract(RouteHeapElement* heap, s16* heapLengthPtr);
RoutePoint* route_add_point(Route* route, CurveSetup *curve, u32 dist, u16 prevPointIdx);
s32 route_find_point(Route* route, CurveSetup* curve, s32* outVisited);
s32 route_is_goal(Route* route, RoutePoint* point);

// public
void route_init(Route *route) {
    route->routePoints = (RoutePoint*)mmAlloc(
        (sizeof(RoutePoint) * ROUTE_HEAP_CAPACITY) + (sizeof(RouteHeapElement) * ROUTE_HEAP_CAPACITY) + (sizeof(CurveSetup*) * ROUTE_MAX_LENGTH), 
        ALLOC_TAG_VOX_COL, 
        NULL);
    route->heap = (RouteHeapElement*)(route->routePoints + ROUTE_HEAP_CAPACITY);
    route->route = (CurveSetup**)(route->heap + ROUTE_HEAP_CAPACITY);
}

// public
void route_free(Route *route) {
    if (route->routePoints != NULL) {
        mmFree(route->routePoints);
        route->routePoints = NULL;
    }
}

// public
s32 route_setup(Route* route, CurveSetup* startCurve, Vec3f* goalPos, void *goal, s32 arg5) {
    RoutePoint* startPoint;

    route_clear(route);
    route->startCurve = startCurve;
    route->goalPos = goalPos;
    route->goal = goal;
    route->unk28 = arg5 & 1;
    route->unk24 = 10000;
    startPoint = route_add_point(route, startCurve, 0, 0xFF);
    route_heap_insert(route->heap, &route->heapLength, (route->routePointsLength - 1), startPoint->goalDist + startPoint->netDist);
    return 0;
}

// public
s32 route_find(Route* route, s32 maxIterations) {
    RoutePoint* point;
    s32 pointIdx;
    s32 done;
    s32 result;

    done = 0;
    result = 0;
    while ((done == 0) && (maxIterations != 0)) {
        // get next point from priority queue
        pointIdx = route_heap_extract(route->heap, &route->heapLength);
        if (pointIdx >= 0) {
            route->lastPointIdx = pointIdx;
            point = &route->routePoints[pointIdx];
            if (route_is_goal(route, point) != 0) {
                // goal found?
                done = 1;
                result = 1;
            } else {
                point->visited = TRUE;
                route_scan_neighbors(route, point, pointIdx);
            }
        } else {
            done = 1;
            result = -1;
        }
        maxIterations -= 1;
    }
    return result;
}

// public
s32 route_reconstruct(Route* route) {
    CurveSetup** temp_t6;
    RoutePoint* var_v1;
    RoutePoint* var_v1_2;
    s32 var_v0;
    s32 var_a1;
    s32 var_a2;

    var_v0 = route->lastPointIdx;
    var_v1 = &route->routePoints[var_v0];
    var_v1->nextPointIdx = 0xFF;
    while (var_v1->prevPointIdx != 0xFF) {
        var_a2 = var_v1->prevPointIdx;
        var_v1 = &route->routePoints[var_v1->prevPointIdx];
        var_v1->nextPointIdx = var_v0;
        var_v0 = var_a2;
        var_a2 = var_v1->prevPointIdx;
    }
    if (var_v1->nextPointIdx == 0xFF) {
        var_v1_2 = NULL;
    } else {
        var_v1_2 = &route->routePoints[var_v1->nextPointIdx];
    }
    var_a1 = 0;
    while (var_v1_2 != NULL) {
        route->route[var_a1] = var_v1_2->curve;
        var_a1 += 1;
        if (var_a1 >= ROUTE_MAX_LENGTH) {
            var_v1_2 = NULL;
        } else if (var_v1_2->nextPointIdx == 0xFF) {
            var_v1_2 = NULL;
        } else {
            var_v1_2 = &route->routePoints[var_v1_2->nextPointIdx];
        }
    }
    route->routeLength = var_a1;
    route->routeCurrIdx = 0;
    return var_a1;
}

// public
CurveSetup* route_next(Route* route) {
    if (route->routeCurrIdx < route->routeLength) {
        return route->route[route->routeCurrIdx++];
    }
    return NULL;
}

s32 route_is_allowed_kyte_curve(CurveSetup* arg0, UnkVoxmap2Struct *arg1) {
    if ((arg0->type22.unk30 == -1 || main_get_bits(arg0->type22.unk30) != 0) && 
        (arg0->type22.usedBit == -1 || main_get_bits(arg0->type22.usedBit) == 0)) {
        if (arg0->base_type22.unk4 == arg1->unk0) {
            return TRUE;
        }
        if (arg0->unk1A < 3) {
            return TRUE;
        }
        if (arg1->unk4 != NULL) {
            if (arg0 == arg1->unk4) {
                return TRUE;
            }
        } else {
            if (arg1->unk2 == arg0->base_type22.unk4) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void route_scan_neighbors(Route* route, RoutePoint* basePoint, s32 baseCurveIdx) {
    CurveSetup* base;
    CurveSetup* neighbor;
    s32 neighborCurveUID;
    s32 i;
    u8 var_fp;

    base = basePoint->curve;
    if (route->unk28 != 0) {
        var_fp = base->unk1B;
    } else {
        var_fp = ~base->unk1B;
    }
    
    for (i = 0; i < 4; i++) {
        neighborCurveUID = base->unk1C[i];
        if ((neighborCurveUID >= 0) && (var_fp & (1 << i))) {
            neighbor = gDLL_26_Curves->vtbl->func_39C(neighborCurveUID);
            if (neighbor != NULL) {
                switch (neighbor->unk19) {
                case 0x22: // Kyte
                    if (route_is_allowed_kyte_curve(neighbor, (UnkVoxmap2Struct*)route->goal) != 0) {
                        route_add_neighbor(route, basePoint, baseCurveIdx, 
                            (u32) (vec3_distance_squared(&base->pos, &neighbor->pos) + (f32) basePoint->netDist), 
                            neighbor);
                    }
                    break;
                case 0x24: // Tricky
                    main_get_bits(BIT_4E2);
                    if ((neighbor->type22.unk30 == -1 || main_get_bits(neighbor->type22.unk30) != 0) &&
                           (neighbor->type22.usedBit == -1 || main_get_bits(neighbor->type22.usedBit) == 0)) {
                        route_add_neighbor(route, basePoint, baseCurveIdx, 
                            (u32) (vec3_distance_squared(&base->pos, &neighbor->pos) + (f32) basePoint->netDist), 
                            neighbor);
                    }
                    break;
                default:
                    D_800B4920 = neighbor;
                    // STUBBED_PRINTF("net_route: No support for this type of node %d x=%f y=%f z=%f\n");
                }
            }
        }
    }
}

void route_add_neighbor(Route* route, RoutePoint* prevPoint, s32 prevPointIdx, u32 dist, CurveSetup* curve) {
    s32 visited;
    s32 pointIdx;
    s32 _pad;
    RoutePoint* point;

    if (route_is_goal(route, prevPoint) != 0) {
        pointIdx = route->routePointsLength;
        route_add_point(route, curve, dist, (u16) prevPointIdx);
        route_heap_insert(route->heap, &route->heapLength, pointIdx, 1);
    }
    pointIdx = route_find_point(route, curve, &visited);
    if ((pointIdx >= 0) && (visited == FALSE)) {
        point = &route->routePoints[pointIdx];
        if (dist < point->netDist) {
            // Better route found
            point->prevPointIdx = (u8) prevPointIdx;
            point->netDist = dist;
            route_heap_change_priority(route->heap, route->heapLength, pointIdx, point->goalDist + dist);
        }
    } else if (pointIdx < 0) {
        pointIdx = route->routePointsLength;
        point = route_add_point(route, curve, dist, (u16) prevPointIdx);
        if (point != NULL) {
            if (route->unk24 < point->goalDist) {
                route_heap_insert(route->heap, &route->heapLength, pointIdx, point->goalDist + point->netDist);
            } else {
                if (point->goalDist < route->unk24) {
                    route->unk24 = point->goalDist;
                }
                route_heap_insert(route->heap, &route->heapLength, pointIdx, point->goalDist + point->netDist);
            }
        }
    }
}

void route_clear(Route* route) {
    s32 i;

    route->heapLength = 0;
    route->routePointsLength = 0;
    for (i = 0; i < ROUTE_HEAP_CAPACITY; i++) {
        route->heap[i].priority = 0;
        route->routePoints[i].visited = FALSE;
    }
}

void route_up_heap(RouteHeapElement* heap, s32 idx) {
    u32 priority;
    s32 parent;
    s32 pointIdx;

    priority = heap[idx].priority;
    pointIdx = heap[idx].pointIdx;
    heap[0].priority = -1;
    parent = idx >> 1;
    while (heap[parent].priority < priority) {
        heap[idx].pointIdx = heap[parent].pointIdx;
        heap[idx].priority = heap[parent].priority;
        idx = parent;
        parent = parent >> 1;
    }
    heap[idx].priority = priority;
    heap[idx].pointIdx = pointIdx;
}

void route_down_heap(RouteHeapElement* heap, s32 heapLength, s32 idx) {
    s32 var_t1;
    s32 pointIdx;
    u32 priority;

    priority = heap[idx].priority;
    pointIdx = heap[idx].pointIdx;
    while ((heapLength >> 1) >= idx) {
        // find smaller child
        var_t1 = idx + idx;
        if (var_t1 < heapLength) {
            if (heap[var_t1].priority < heap[var_t1 + 1].priority) {
                var_t1 = var_t1 + 1;
            }
        }

        if (priority >= heap[var_t1].priority) {
            break;
        }

        heap[idx].priority = heap[var_t1].priority;
        heap[idx].pointIdx = heap[var_t1].pointIdx;
        idx = var_t1;
    }
    heap[idx].priority = priority;
    heap[idx].pointIdx = pointIdx;
}

// insert curve into priority queue
void route_heap_insert(RouteHeapElement* heap, s16* heapLengthPtr, u16 pointIdx, u32 dist) {
    *heapLengthPtr += 1;
    heap[*heapLengthPtr].pointIdx = pointIdx;
    // invert the dist. this is a max-heap, so smaller distances should be higher priority
    heap[*heapLengthPtr].priority = -1 - dist;
    route_up_heap(heap, *heapLengthPtr);
}

void route_heap_change_priority(RouteHeapElement* heap, s32 heapLength, u16 pointIdx, u32 newPriority) {
    s32 idx;
    s32 i;
    u32 prevPriority;

    // find curve in heap
    for (i = 0; heapLength >= i; i++) {
        if (pointIdx == heap[i].pointIdx) {
            idx = i;
            i = heapLength + 1; // break
        }
    }
    // change priority
    prevPriority = heap[idx].priority;
    heap[idx].priority = newPriority;
    // fix heap
    if (newPriority < prevPriority) {
        route_down_heap(heap, heapLength, idx);
    } else if (prevPriority < newPriority) {
        route_up_heap(heap, idx);
    }
}

s32 route_heap_extract(RouteHeapElement* heap, s16* heapLengthPtr) {
    u16 pointIdx;
    
    if (*heapLengthPtr == 0) {
        return -1;
    }

    pointIdx = heap[1].pointIdx;
    heap[1].priority = heap[*heapLengthPtr].priority;
    heap[1].pointIdx = heap[*heapLengthPtr].pointIdx;
    *heapLengthPtr -= 1;
    route_down_heap(heap, *heapLengthPtr, 1);
    return (s32) pointIdx;
}

RoutePoint* route_add_point(Route* route, CurveSetup *curve, u32 dist, u16 prevPointIdx) {
    RoutePoint* point;

    if (route->routePointsLength == ROUTE_HEAP_CAPACITY) {
        return NULL;
    }
    point = &route->routePoints[route->routePointsLength++];
    point->curve = curve;
    point->netDist = dist;
    point->prevPointIdx = (u8) prevPointIdx;
    point->goalDist = (u32) vec3_distance_squared(&curve->pos, route->goalPos);
    return point;
}

s32 route_find_point(Route* route, CurveSetup* curve, s32* outVisited) {
    RoutePoint* point;
    s32 i;

    for (i = 0; i < route->routePointsLength; i++) {
        point = &route->routePoints[i];
        if (curve == point->curve) {
            *outVisited = point->visited;
            return i;
        }
    }
    return -1;
}

s32 route_is_goal(Route* route, RoutePoint* point) {
    UnkVoxmap2Struct *new_var;
    CurveSetup* temp_a1;
    void *temp_v0;
    s32 temp3;

    temp_v0 = route->goal;
    switch (point->curve->unk19) {
    case 0x22: // Kyte
        new_var = (UnkVoxmap2Struct*)route->goal;
        if (new_var->unk4 != NULL) {
            return new_var->unk4 == point->curve;
        }
        return new_var->unk2 == point->curve->base_type22.unk4;
    case 0x24: // Tricky
        if (!(point->prevPointIdx & 0x80)) {
            temp3 = point->curve->unk3;
            if (temp3 != 0) {
                return (s32)temp_v0 == point->curve->unk3;
            }

            if (route->routePoints[point->prevPointIdx].curve) {}

            temp_a1 = route->routePoints[point->prevPointIdx].curve;
            if (point->curve->uID == temp_a1->unk1C[0]) {
                return (s32)temp_v0 == temp_a1->base_type24.unk4;
            }
            if (point->curve->uID == temp_a1->unk1C[1]) {
                return (s32)temp_v0 == temp_a1->base_type24.unk5;
            }
            if (point->curve->uID == temp_a1->unk1C[2]) {
                return (s32)temp_v0 == temp_a1->base_type24.unk6;
            }
            if (point->curve->uID == temp_a1->unk1C[3]) {
                return (s32)temp_v0 == temp_a1->base_type24.unk7;
            }
        }
        return 0;
    default:
      return ((CurveSetup*)temp_v0) == (point->curve);
    }
}
