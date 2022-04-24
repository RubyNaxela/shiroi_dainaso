#ifndef DAINASO_MACROS_H
#define DAINASO_MACROS_H

#include <stdio.h>
#include <stdlib.h>

///
/// \brief Keywords directly borrowed from C++.
///
#ifndef __cplusplus
#define and &&
#define false 0
#define not !
#define or ||
#define true 1
typedef int bool;
#endif

///
/// \brief Additional keywords for better readability.
///
#define constructor(type) contract_new type* type##_create
#define extends(type) type##_BASE
#define _new(type) type##_create
#define null (void*) 0
#define pass __ds_nullFunction
#define super __base__

///
/// \brief A function that does nothing. Useful when a function expression
///        is required semantically but no code needs to be executed.
///
void __ds_nullFunction() {
}

///
/// \brief Annotation denoting a type or function that should be treated as abstract.
///
#define abstract

///
/// \brief Annotation denoting a function that should be treated as static.
///
#define _static

///
/// \brief A function marked with contract_new annotation should
///        return a pointer to memory allocated within the function.
///        A function marked with contract_pure annotation should be deterministic, only read the stack
///        memory and have no side effects (throwing an exception is not considered a side effect).
///
#if defined(_MSC_VER)
#define contract_new __result_use_check
#define contract_pure __pure
#elif defined(__GNUC__)
#define contract_new __attribute__((malloc)) __attribute__((unused))
#define contract_pure __attribute__((const))
#else
#define contract_new
#define contract_pure
#endif

///
/// \brief Ensures that the specified condition is met.
///
/// \throws specified exception if the condition is not met
///
#define ASSERT(condition, exception) if (!(condition)) {\
                                         fprintf(stderr, "%s: %s:%d\n", #exception, __FILE__, __LINE__);\
                                         exit(11);\
                                     }

///
/// \brief Ensures that the specified pointer is not null.
///
/// \throws dsNullPointerException if the pointer is null
///
#define ASSERT_NOT_NULL(pointer) if (pointer == null) {\
                                     fprintf(stderr, "dsNullPointerException: %s:%d\n", __FILE__, __LINE__);\
                                     exit(11);\
                                 }

///
/// \brief Ensures that the file under the specified pathname exists.
///
/// \throws dsNullPointerException if the file does not exist
///
#define ASSERT_FILE_EXISTS(filename) if (fopen(filename, "r") == null) {\
                                         fprintf(stderr, "dsFileNotFoundException: %s\n", filename);\
                                         exit(11);\
                                     }

#define INSTANCE_OF(pointer, objectType) (pointer->type == dsScrollingObjectT\
                                            ? ((dsScrollingObject*) pointer)->scrollingType == objectType##T\
                                            : pointer->type == objectType##T)

#endif //DAINASO_MACROS_H
