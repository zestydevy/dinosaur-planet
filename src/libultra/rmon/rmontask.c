// @DECOMP_OPT_FLAGS=-O1
#include "libultra/rmon/rmonint.h"

static const char str_800a2aa0[] = "StopThreads %d\n";
static const char str_800a2ab0[] = "Couldn't stop thread %d\n";
static const char str_800a2acc[] = "Couldn't stop thread %d\n";
static const char str_800a2ae8[] = "ListThreads\n";
static const char str_800a2af8[] = "ThreadStatus %d method %d\n";
static const char str_800a2b14[] = "StopThread %d\n";
static const char str_800a2b24[] = "RunThread %d\n";

void __rmonMaskIdleThreadInts(void) {
    register OSThread *thread;

    thread = __osGetActiveQueue();

    while (thread->priority != -1) {
        if (thread->priority == 0) {
            thread->context.sr &= ~OS_IM_CPU;
            thread->context.sr |= (OS_IM_CART | OS_IM_RDBWRITE | OS_IM_RDBREAD);
            break;
        }
        thread = thread->tlnext;
    }
}

OSThread* __rmonGetTCB(int threadNumber) {
    register OSThread *thread;

    thread = __osGetActiveQueue();

    if (threadNumber < 1){
        return NULL;
    }

    while (thread->priority != -1) {
        if (thread->id == threadNumber) {
            return thread;
        }
        thread = thread->tlnext;
    }

    return NULL;
}

int __rmonStopUserThreads(int whichThread) {
    register s32 var1;
    register OSThread *thread;

    var1 = 0;
    thread = __osGetActiveQueue();

    if (whichThread != 0) {
        while (thread->priority != -1) {
            if (thread->id == whichThread) {
                break;
            }
            thread = thread->tlnext;
        }

        if (thread->priority == -1) {
            return 0;
        }

        if (thread->priority > 0 && thread->priority < 128) {
            osStopThread(thread);
            var1 = whichThread;
        }
    } else {
        while (thread->priority != -1) {
            if (thread->priority > 0 && thread->priority < 128) {
                osStopThread(thread);
                var1 = -1;
            }
            thread = thread->tlnext;
        }
    }

    return var1;
}

int __rmonListThreads(KKHeader* req) {
    register KKObjectRequest *request = (KKObjectRequest*)req;
    KKObjsEvent *event;
    register OSThread *thread;

    event = (KKObjsEvent*)&__rmonUtilityBuffer;

    event->object = request->object == -1 ? 0x3ea : request->object;

    if (req->method == 1) {
        event->objs.number = 1;
        event->objs.objects[0] = 1000;
    } else {
        thread = __osGetActiveQueue();
        event->objs.number = 0;

        while (thread->priority != -1) {
            if (thread->id != 0) {
                event->objs.objects[event->objs.number] = thread->id;
                event->objs.number++;
            }
            thread = thread->tlnext;
        }
    }

    event->header.code = request->header.code;
    event->header.error = 0;
    __rmonSendReply((KKHeader *const)event, 0x18 + (event->objs.number - 1) * 4, 1);

    return 0;
}

int __rmonGetThreadStatus(int method, int id, KKStatusEvent* reply) {
    s32 sp1C;
    OSThread* sp18;

    reply->status.tid = id;
    if (method == 1) {
        reply->status.pid = 0x3E9;
    } else {
        reply->status.pid = 0x3EA;
    }
    reply->status.why = 1;
    reply->status.what = 0;
    reply->status.info.major = 0;
    reply->status.info.minor = 0;
    reply->status.rv = 0;
    
    if (method == 1) {
        reply->status.start = 0x04001000;
        reply->status.priority = 0x2A;
        if (__rmonRCPrunning() != 0) {
            reply->status.flags = 4;
            reply->status.info.addr = NULL;
            reply->status.instr = 0;
        } else {
            reply->status.flags = 1;
            reply->status.info.addr = __rmonReadWordAt((u32*)0x04080000) + 0x04001000;
            sp1C = __rmonReadWordAt((u32*)reply->status.info.addr);
            if ((sp1C & 0xFC00003F) == 0xD) {
                sp1C = 0xD;
            }
            if (__rmonRcpAtBreak != 0) {
                reply->status.why = 2;
                reply->status.info.major = 2;
                reply->status.info.minor = 4;
            }
            reply->status.instr = sp1C;
        }
    } else {
        sp18 = __osGetActiveQueue();
    
        while (sp18->priority != -1) {
            if (sp18->id == id) {
                break;  
            }
            sp18 = sp18->tlnext;
        }
        
        if (sp18->priority == -1) {
            return -2;
        }
        reply->status.priority = sp18->priority;
        
        if (sp18->state != 0) {
            reply->status.flags = (s32) sp18->state;
        } else {
            reply->status.flags = 1;
        }
        reply->status.info.addr = sp18->context.pc;
        sp1C = *(s32*)sp18->context.pc;
        if ((sp1C & 0xFC00003F) == 0xD) {
            sp1C = 0xD;
        }
        
        reply->status.instr = sp1C;
        reply->status.start = (int)sp18;
        if (sp18->flags & 1) {
            reply->status.why = 2;
            reply->status.info.major = 2;
            reply->status.info.minor = 4;
        } else if (sp18->flags & 2) {
            reply->status.why = 2;
            reply->status.info.major = 1;
            reply->status.info.minor = 2;
        }
    }

    return 0;
}

int __rmonThreadStatus(KKHeader* req) {
    KKObjectRequest *request = (KKObjectRequest*)req;
    KKStatusEvent event;

    if (__rmonGetThreadStatus(req->method, request->object, &event)) {
        return -2;
    }

    event.header.code = request->header.code;
    event.header.error = 0;
    __rmonSendReply((KKHeader *const)&event, 0x4c, 1);

    return 0;
}

int __rmonStopThread(KKHeader* req) {
    KKObjectRequest *request = (KKObjectRequest*)req;
    KKStatusEvent event;
    u32 *var;

    switch (req->method) {
        case 0:
            __rmonStopUserThreads(request->object);
            break;
        case 1:
            if (__rmonRCPrunning()) {
                __rmonIdleRCP();

                var = (u32*)__rmonReadWordAt((u32*)0x4080000);
                if (var == NULL) {
                    break;
                }

                var--;

                if ((__rmonGetBranchTarget(1, 1000, (char*)((u32)var + 0x04001000)) & 3) == 0) {
                    __rmonStepRCP();
                }
            }
            break;
        default:
            return -4;
    }

    if (__rmonGetThreadStatus(req->method, request->object, &event)) {
        return -2;
    }

    event.header.code = request->header.code;
    event.header.error = 0;
    __rmonSendReply((KKHeader *const)&event, 0x4c, 1);

    if (event.status.flags == 1) {
        event.header.code = 4;
        __rmonSendReply((KKHeader *const)&event, 0x4c, 2);
    }

    return 0;
}

int __rmonRunThread(KKHeader* req) {
    KKRunThreadRequest *request = (KKRunThreadRequest*)req;
	KKObjectEvent event1;
	KKStatusEvent event2;
	register OSThread *thread;
	register s32 var;

    var = 0;

    switch (req->method) {
        case 0:
            thread = __osGetActiveQueue();
            
            while (thread->priority != -1) {
                if (thread->id == (s32)request->tid) {
                    break;
                }
                thread = thread->tlnext;
            }

            if (thread->priority == -1) {
                return -2;
            }

            if (thread->state != 1) {
                return -4;
            }

            thread->flags &= ~3;

            if (request->actions.flags & 2) {
                thread->context.pc = request->actions.vaddr;
            }

            if (request->actions.flags & 1 && __rmonSetSingleStep(request->tid, (u32*)thread->context.pc) == 0) {
                return -4;
            }

            var = 1;
            break;
        case 1:
            if (__rmonRCPrunning()) {
                return -4;
            }

            if (request->actions.flags & 2) {
                __rmonWriteWordTo((u32*)0x4080000, request->actions.vaddr - 0x4001000);
            }

            if (request->actions.flags & 1) {
                if ((__rmonGetBranchTarget(1, 1000, (char*)(__rmonReadWordAt((u32*)0x4080000) + 0x4001000)) & 3) == 0) {
                    __rmonStepRCP();
                }
                __rmonStepRCP();
                __rmonRcpAtBreak = 1;
            } else {
                __rmonRcpAtBreak = 0;
                __rmonRunRCP();
            }

            event1.header.code = request->header.code;
            event1.header.error = 0;
            event1.object = request->tid;
            __rmonSendReply((KKHeader *const)&event1, 0x10, 1);

            if (request->actions.flags & 1) {
                __rmonGetThreadStatus(1, 1000, &event2);
                __rmonGetExceptionStatus(&event2);
                __rmonSendReply((KKHeader *const)&event2, 0x4c, 2);
            }

            return 0;
        default:
            return -4;
    }

    event1.header.code = request->header.code;
    event1.header.error = 0;
    event1.object = request->tid;
    __rmonSendReply((KKHeader *const)&event1, 0x10, 1);

    if (var != 0) {
        osStartThread(thread);
    }

    return 1;
}
