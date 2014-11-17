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

typedef struct {
	char* * data;
	int counter;
} ComplexData;

int TAT_Dump_ComplexData(void *data, char *dest);

int TAT_Free_ComplexData(ComplexData * x);

ComplexData TAT_Copy_ComplexData(ComplexData src);

int TAT_Compare_ComplexData(ComplexData x1, ComplexData x2);

#define TAT_S_A_ID 1
typedef struct {
	ComplexData Param1;
} TAT_SIG_A;

extern TAT_TEST_RESULT TAT_Send_A ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, ComplexData Param1);

#define TAT_S_B_ID 2
typedef struct {
	ComplexData Param1;
} TAT_SIG_B;


extern TAT_TEST_RESULT TAT_Init_TAT_Test (TAT_TEST_INSTANCE * id);
extern TAT_TEST_TABLE TAT_TAT_Test_TestTable[];
extern TAT_TEST_RESULT TAT_TI_TAT_Test_test( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_TAT_Test_test( TAT_TEST_INSTANCE* id );
#endif
