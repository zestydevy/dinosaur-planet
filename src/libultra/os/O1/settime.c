#include <PR/os_internal.h>
#include "libultra/os/osint.h"

void osSetTime(OSTime time){
    __osCurrentTime = time;
}
