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

typedef struct _List1 * List1;

struct _List1 {
int  data;
List1 next;
};

extern List1 TAT_List1_Cur;

typedef struct _List2 * List2;

struct _List2 {
List1  data;
List2 next;
};

extern List2 TAT_List2_Cur;

typedef struct _ComplexData * ComplexData;

struct _ComplexData {
List2  data;
ComplexData next;
};

extern ComplexData TAT_ComplexData_Cur;

int TAT_Dump_ComplexData(void *data, char *dest);
int TAT_Dump_List1(void *data, char *dest);
int TAT_Dump_List2(void *data, char *dest);

int TAT_Free_ComplexData(ComplexData * x);
int TAT_Free_List1(List1 * x);
int TAT_Free_List2(List2 * x);

ComplexData TAT_Copy_ComplexData(ComplexData src);
List1 TAT_Copy_List1(List1 src);
List2 TAT_Copy_List2(List2 src);

int TAT_Compare_ComplexData(ComplexData x1, ComplexData x2);
int TAT_Compare_List1(List1 x1, List1 x2);
int TAT_Compare_List2(List2 x1, List2 x2);

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
