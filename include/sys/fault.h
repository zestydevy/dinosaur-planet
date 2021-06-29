//use for crash screens
struct ErrString{
	u32 code1;
	u32 code2;
	char* text;
};

void create_crash_thread(OSSched *scheduler);
