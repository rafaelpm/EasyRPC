#ifndef _TEST_ALL_H
#define _TEST_ALL_H
/* ---------------------------------------------------------------------------*/
//#include "HelperClientConnection.h"
#include "test_builded_client_function.h"
#include "test_wrap.h"
#include "libdebug.h"
#include "Helpers.h"
#include "HelperBind.h"

void test_all() {
	

	test_set_get_float(nameFunction1, 1.5, 2.5);
	test_set_get_int(nameFunction2, 3, 3.5);
	test_set_get_bool(nameFunction3, true, 1024);
	test_set_get_string_array(nameFunction1, &nameFunction2[0], (uint8_t *) &nameFunction3[0], (int)strlen(nameFunction3));
	test_bind_search_function();
	test_bind();
}
/* ---------------------------------------------------------------------------*/
#endif
