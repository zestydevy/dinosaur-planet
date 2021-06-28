#define MAIN_GFX_BUF_SIZE 4500
#define MAIN_MTX_BUF_SIZE 1100
#define MAIN_VTX_BUF_SIZE 300

extern void* framebufferNext;
extern void* framebufferCurrent;
extern OSViMode OSViMode_Custom;
extern u32 CurrentResolutionH[2]; //picked from resolutionArray[video_mode % 7]
extern u32 CurrentResolutionV[2]; //one for each FB, apparently
extern void* framebufferPointers[2]; //{FBaddr,FBaddr*H*V*2}
extern u32 framebufferChoice;
extern u32 video_mode;
extern float aspectRatioFloat; //1.121212 for PAL, 1.333 for NTSC/MPAL.
extern s32 gFrameCount;
extern u8 gFrameBufIdx;
extern Gfx *gMainGfx[2], *gCurGfx;
extern Mtx *gMainMtx[2], *gCurMtx;
extern Vtx *gMainVtx[2], *gCurVtx;
extern void *gMainPol[2], *gCurPol; //XXX type

void alloc_frame_buffers(void);
