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
extern Gfx *D_800AE680;