#include <stdio.h>
#include <string.h>
#include <errno.h>


#ifdef DEBUG
#define PRT_DBG(...)	\
	{\
		printf("erro %s %s %d\n",__FILE__,__FUNCTION__,__LINE__);\
		printf(s, ##__VA_ARGS__);		\
	}
#else
#define DEBUG
#define PRT_DBG(...)
#endif

