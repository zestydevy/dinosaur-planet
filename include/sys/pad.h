extern OSThread controller_thread;
extern u16 buttonInput0[4];
extern u16 buttonInput1[4];
extern u16 buttonMask[4]; // starts as 0xFFFF, buttons sometimes get AND'ed with it.
extern u8 controllerPortList[4]; // inits to 0,1,2,3. used in input getters.
extern s8 joyXMirror[4];
extern s8 joyYMirror[4];
extern u8 joyXHoldTimer[4]; // resets after hitting MenuInputDelay's value.
extern u8 joyYHoldTimer[4];
extern s8 joyXSign[4]; //1, 0, or -1 based on input.
extern s8 joyYSign[4];
extern s8 some_controller_init_val;
