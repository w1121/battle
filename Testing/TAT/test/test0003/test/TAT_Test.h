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
	int * data;
	int counter;
} Array1;

typedef struct {
	Array1 * data;
	int counter;
} Array2;

typedef struct {
	Array2 * data;
	int counter;
} Array3;

int TAT_Dump_Array1(void *data, char *dest);
int TAT_Dump_Array2(void *data, char *dest);
int TAT_Dump_Array3(void *data, char *dest);

int TAT_Free_Array1(Array1 * x);
int TAT_Free_Array2(Array2 * x);
int TAT_Free_Array3(Array3 * x);

Array1 TAT_Copy_Array1(Array1 src);
Array2 TAT_Copy_Array2(Array2 src);
Array3 TAT_Copy_Array3(Array3 src);

int TAT_Compare_Array1(Array1 x1, Array1 x2);
int TAT_Compare_Array2(Array2 x1, Array2 x2);
int TAT_Compare_Array3(Array3 x1, Array3 x2);

#define TAT_S_A_ID 1
typedef struct {
	Array3 Param1;
} TAT_SIG_A;

extern TAT_TEST_RESULT TAT_Send_A ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, Array3 Param1);

#define TAT_S_B_ID 2
typedef struct {
	Array3 Param1;
} TAT_SIG_B;


extern TAT_TEST_RESULT TAT_Init_TAT_Test (TAT_TEST_INSTANCE * id);
extern TAT_TEST_TABLE TAT_TAT_Test_TestTable[];
extern TAT_TEST_RESULT TAT_TI_TAT_Test_test( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_TAT_Test_test( TAT_TEST_INSTANCE* id );
#endif
