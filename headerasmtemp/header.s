/*
 * Dinosaur Planet ROM Header
 */

.byte  0x80, 0x37, 0x12, 0x40   /* PI BSD Domain 1 register */
.word  0x0000000F               /* Clockrate setting*/
.word  romMain                  /* Entrypoint */

/* Revision */
    .word  0x00001449

.word  0xD0F6A741               /* Checksum 1 */
.word  0x7D57761E               /* Checksum 2 */
.word  0x00000000               /* Unknown */
.word  0x00000000               /* Unknown */
.ascii "DINO PLANET         "   /* Internal ROM name */
.word  0x00000000               /* Unknown */
.word  0x0000004E               /* 1st letter of cartridge ID (N) [I wonder if this can replaced with .ascii "N"?] */
.ascii "DP"                     /* Next 2 letters of cartridge ID */

/* Region */
.ascii "E"                  /* 4th letter of cartridge ID indicates game region, E = NTSC-U (North America) */

.byte  0x00                 /* Version number, some games use this, DP's ROM header leaves it blank. */
