// exp_lottery.h

#ifdef _DLL_LOTTERY_
#define	EXT_LOTTERY		__declspec(dllexport)
#else
#define	EXT_LOTTERY		__declspec(dllimport)
#endif//_DLL_LOTTERY_
