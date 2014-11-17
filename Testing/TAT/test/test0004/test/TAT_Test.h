/* TAT TEST SUITE HEADER FILE */
#ifndef TAT_TAT_Test_H
#define TAT_TAT_Test_H
#ifndef TAT_INSTANCE_SUPPORT
#error TAT_INSTANCE_SUPPORT define not set for compilation
#endif
#ifndef TAT_TIME_SUPPORT
#error TAT_TIME_SUPPORT define not set for compilation
#endif
#include <testkern.h>

#define TAT_I_IUT_ID 1
#define TAT_I_Test_ID 2

typedef char *  CharString;

int TAT_Dump_CharString(void *data, char *dest);

int TAT_Free_CharString(CharString * x);

CharString TAT_Copy_CharString(CharString src);

int TAT_Compare_CharString(CharString x1, CharString x2);

#define TAT_S_A_ID 1
typedef struct {
	CharString Param1;
	CharString Param2;
} TAT_SIG_A;

extern TAT_TEST_RESULT TAT_Send_A ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, CharString Param1, CharString Param2);

#define TAT_S_B_ID 2
typedef struct {
	CharString Param1;
	CharString Param2;
} TAT_SIG_B;


extern TAT_TEST_RESULT TAT_Init_TAT_Test (TAT_TEST_INSTANCE * id);
extern TAT_TEST_TABLE TAT_TAT_Test_TestTable[];
extern TAT_TEST_RESULT TAT_TI_TAT_Test_test( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_TAT_Test_test( TAT_TEST_INSTANCE* id );
#endif
