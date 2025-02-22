// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include <PR/os.h>
#include "libnaudio/n_synthInternals.h"

void n_alSynRemovePlayer(ALPlayer *client) {
    ALPlayer *thing;
    ALPlayer *prev;
    OSIntMask mask;

    prev = NULL;

    if (n_syn->head != NULL) {
        mask = osSetIntMask(OS_IM_NONE);

        for (thing = n_syn->head; thing != NULL; thing = thing->next) {
            if (thing == client) {
                if (prev != NULL) {
                    prev->next = thing->next;
                } else {
                    n_syn->head = thing->next;
                }

                client->next = NULL;
                break;
            }

            prev = thing;
        }

        osSetIntMask(mask);
    }
}
