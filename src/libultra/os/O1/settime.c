#include <PR/os_internal.h>
#include "libultra/os/os_int.h"

void osSetTime(OSTime time){
    __osCurrentTime = time;
}
