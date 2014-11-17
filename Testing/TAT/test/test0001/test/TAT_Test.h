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
	int a;
	int b;
} Struct1;

typedef struct {
	Struct1 a;
	Struct1 b;
} Struct2;

typedef struct {
	Struct2 a;
	Struct2 b;
} Struct3;

int TAT_Dump_Struct1(void *data, char *dest);
int TAT_Dump_Struct2(void *data, char *dest);
int TAT_Dump_Struct3(void *data, char *dest);

int TAT_Free_Struct1(Struct1 * x);
int TAT_Free_Struct2(Struct2 * x);
int TAT_Free_Struct3(Struct3 * x);

Struct1 TAT_Copy_Struct1(Struct1 src);
Struct2 TAT_Copy_Struct2(Struct2 src);
Struct3 TAT_Copy_Struct3(Struct3 src);

int TAT_Compare_Struct1(Struct1 x1, Struct1 x2);
int TAT_Compare_Struct2(Struct2 x1, Struct2 x2);
int TAT_Compare_Struct3(Struct3 x1, Struct3 x2);

#define TAT_S_A_ID 1
typedef struct {
	Struct3 Param1;
} TAT_SIG_A;

extern TAT_TEST_RESULT TAT_Send_A ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, Struct3 Param1);

#define TAT_S_B_ID 2
typedef struct {
	Struct3 Param1;
} TAT_SIG_B;


extern TAT_TEST_RESULT TAT_Init_TAT_Test (TAT_TEST_INSTANCE * id);
extern TAT_TEST_TABLE TAT_TAT_Test_TestTable[];
extern TAT_TEST_RESULT TAT_TI_TAT_Test_test( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_TAT_Test_test( TAT_TEST_INSTANCE* id );
#endif
