/** Graphics backend (RSP/RDP, etc)
 */
extern s32 gMainGfx[], gMainMtx[], gMainVtx[], gMainPol[]; //likely pointers
extern Gfx *gCurGfx;
extern u8 gFrameBufIdx;
extern float aspectRatioFloat; //1.121212 for PAL, 1.333 for NTSC/MPAL.
