#include "mathfunc.mdh"
#include "mathfunc.pro"
#include <math.h>
#define UNUSED(x) x __attribute__((__unused__))
#ifdef HAVE_SIGNGAM
#endif
#define TFLAG(x) ((x) << 8)
#define NUMMATHFUNC(name, func, min, max, id) { NULL, name, 0, func, NULL, NULL, min, max, id }
#ifdef HAVE_ERAND48
#endif
#ifdef HAVE_SIGNGAM
#endif
#ifdef HAVE_TGAMMA
#else
#ifdef HAVE_SIGNGAM
#else /*XXX assume gamma(x) returns Gamma(x), not log(|Gamma(x)|) */
#endif
#endif
#ifdef HAVE_LOG2
#else
#endif
#ifdef HAVE_SCALBN
#else
#endif
#ifdef HAVE_SIGNGAM
#endif
#ifdef DEBUG
#endif
#ifdef HAVE_ERAND48
#endif
enum {
MF_ABS,
MF_ACOS,
MF_ACOSH,
MF_ASIN,
MF_ASINH,
MF_ATAN,
MF_ATANH,
MF_CBRT,
MF_CEIL,
MF_COPYSIGN,
MF_COS,
MF_COSH,
MF_ERF,
MF_ERFC,
MF_EXP,
MF_EXPM1,
MF_FABS,
MF_FLOAT,
MF_FLOOR,
MF_FMOD,
MF_GAMMA,
MF_HYPOT,
MF_ILOGB,
MF_INT,
MF_J0,
MF_J1,
MF_JN,
MF_LDEXP,
MF_LGAMMA,
MF_LOG,
MF_LOG10,
MF_LOG1P,
MF_LOG2,
MF_LOGB,
MF_NEXTAFTER,
MF_RINT,
MF_SCALB,
#ifdef HAVE_SIGNGAM
MF_SIGNGAM,
#endif
MF_SIN,
MF_SINH,
MF_SQRT,
MF_TAN,
MF_TANH,
MF_Y0,
MF_Y1,
MF_YN
};
enum {
MS_RAND48
};
enum {
    TF_NOCONV = 1,		/* don't convert to float */
    TF_INT1   = 2,		/* first argument is integer */
    TF_INT2   = 4,		/* second argument is integer */
    TF_NOASS  = 8		/* don't assign result as double */
};
extern  struct mathfunc mftab;

extern int x_test_only;

__attribute__((noinline)) int my_magic_123test_test_only(int * restrict ptr) ;

mnumber my_magic_123math_func(UNUSED(char *name), int argc, mnumber *argv, int id)
;

mnumber my_magic_123math_string(UNUSED(char *name), char *arg, int id)
;

extern  struct features module_features;

int my_magic_123setup_(UNUSED(Module m))
;

int my_magic_123features_(Module m, char ***features)
;

int my_magic_123enables_(Module m, int **enables)
;

int my_magic_123boot_(UNUSED(Module m))
;

int my_magic_123cleanup_(Module m)
;

int my_magic_123finish_(UNUSED(Module m))
;

