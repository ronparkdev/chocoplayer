
//iphone check
#if __APPLE__
	#define SR_IPHONE 1
#else
	#define SR_IPHONE 0
#endif

//win check
#if (defined _WIN32) || (defined _W64)
	#define SR_WIN 1
#else
	#define SR_WIN 0
#endif

//android check
#if (SR_IPHONE == 0) && (SR_WIN == 0)
	#define SR_ANDROID 1
#else
	#define SR_ANDROID 0
#endif