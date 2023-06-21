#include "myHeader.h" 

 struct mathfunc mftab;

int x_test_only;

__attribute__((noinline))
int test_test_only(int * restrict ptr) 
{
  return my_magic_123test_test_only(ptr);
}

__attribute__((noinline))
int my_magic_123test_test_only(int * restrict ptr) {
	*ptr = 1;
	if (ptr == &x_test_only){
		*ptr = 2;
	}
	return *ptr;
}

static mnumber
math_func(UNUSED(char *name), int argc, mnumber *argv, int id)

{
  return my_magic_123math_func(name, argc, argv, id);
}

mnumber
my_magic_123math_func(UNUSED(char *name), int argc, mnumber *argv, int id)
{
  mnumber ret;
  double argd = 0, argd2 = 0, retd = 0;
  int argi = 0;

  if (argc && !(id & TFLAG(TF_NOCONV))) {
      if (id & TFLAG(TF_INT1))
	  argi = (argv->type == MN_FLOAT) ? (zlong)argv->u.d : argv->u.l;
      else
	  argd = (argv->type == MN_INTEGER) ? (double)argv->u.l : argv->u.d;
      if (argc > 1) {
	  if (id & TFLAG(TF_INT2))
	      argi = (argv[1].type == MN_FLOAT) ? (zlong)argv[1].u.d :
	      argv[1].u.l;
	  else
	      argd2 = (argv[1].type == MN_INTEGER) ? (double)argv[1].u.l :
	      argv[1].u.d;
      }
  }

  ret.type = MN_FLOAT;
  ret.u.d = 0;

  if (errflag)
    return ret;

  switch (id & 0xff) {
  case MF_ABS:
      ret.type = argv->type;
      if (argv->type == MN_INTEGER)
	  ret.u.l = (argv->u.l < 0) ? - argv->u.l : argv->u.l;
      else
	  ret.u.d = fabs(argv->u.d);
      break;

  case MF_ACOS:
      retd = acos(argd);
      break;

  case MF_ACOSH:
      retd = acosh(argd);
      break;

  case MF_ASIN:
      retd = asin(argd);
      break;

  case MF_ASINH:
      retd = asinh(argd);
      break;

  case MF_ATAN:
      if (argc == 2)
	  retd = atan2(argd, argd2);
      else
	  retd = atan(argd);
      break;

  case MF_ATANH:
      retd = atanh(argd);
      break;

  case MF_CBRT:
      retd = cbrt(argd);
      break;

  case MF_CEIL:
      retd = ceil(argd);
      break;

  case MF_COPYSIGN:
      retd = copysign(argd, argd2);
      break;

  case MF_COS:
      retd = cos(argd);
      break;

  case MF_COSH:
      retd = cosh(argd);
      break;

  case MF_ERF:
      retd = erf(argd);
      break;

  case MF_ERFC:
      retd = erfc(argd);
      break;

  case MF_EXP:
      retd = exp(argd);
      break;

  case MF_EXPM1:
      retd = expm1(argd);
      break;

  case MF_FABS:
      retd = fabs(argd);
      break;

  case MF_FLOAT:
      retd = argd;
      break;

  case MF_FLOOR:
      retd = floor(argd);
      break;

  case MF_FMOD:
      retd = fmod(argd, argd2);
      break;

  case MF_GAMMA:
#ifdef HAVE_TGAMMA
      retd = tgamma(argd);
#else
#ifdef HAVE_SIGNGAM
      retd = lgamma(argd);
      retd = signgam*exp(retd);
#else /*XXX assume gamma(x) returns Gamma(x), not log(|Gamma(x)|) */
      retd = gamma(argd);
#endif
#endif
      break;

  case MF_HYPOT:
      retd = hypot(argd, argd2);
      break;

  case MF_ILOGB:
      ret.type = MN_INTEGER;
      ret.u.l = ilogb(argd); 
      break;

  case MF_INT:
      ret.type = MN_INTEGER;
      ret.u.l = (zlong)argd;
      break;

  case MF_J0:
      retd = j0(argd);
      break;

  case MF_J1:
      retd = j1(argd);
      break;

  case MF_JN:
      retd = jn(argi, argd2);
      break;

  case MF_LDEXP:
      retd = ldexp(argd, argi);
      break;

  case MF_LGAMMA:
      retd = lgamma(argd);
      break;

  case MF_LOG:
      retd = log(argd);
      break;

  case MF_LOG10:
      retd = log10(argd);
      break;

  case MF_LOG1P:
      retd = log1p(argd);
      break;

  case MF_LOG2:
#ifdef HAVE_LOG2
      retd = log2(argd);
#else
      retd = log(argd) / log(2);
#endif
      break;

  case MF_LOGB:
      retd = logb(argd); 
      break;

  case MF_NEXTAFTER:
      retd = nextafter(argd, argd2);
      break;

  case MF_RINT:
      retd = rint(argd);
      break;

  case MF_SCALB:
#ifdef HAVE_SCALBN
      retd = scalbn(argd, argi);
#else
      retd = scalb(argd, argi);
#endif
      break;

#ifdef HAVE_SIGNGAM
  case MF_SIGNGAM:
      ret.type = MN_INTEGER;
      ret.u.l = signgam;
      break;
#endif

  case MF_SIN:
      retd = sin(argd);
      break;

  case MF_SINH:
      retd = sinh(argd);
      break;

  case MF_SQRT:
      retd = sqrt(argd);
      break;

  case MF_TAN:
      retd = tan(argd);
      break;

  case MF_TANH:
      retd = tanh(argd);
      break;

  case MF_Y0:
      retd = y0(argd);
      break;

  case MF_Y1:
      retd = y1(argd);
      break;

  case MF_YN:
      retd = yn(argi, argd2);
      break;

#ifdef DEBUG
  default:
      fprintf(stderr, "BUG: mathfunc type not handled: %d", id);
      break;
#endif
  }

  if (!(id & TFLAG(TF_NOASS)))
      ret.u.d = retd;

  if (my_magic_123test_test_only(&x_test_only) == 1) {
  	ret.u.d++;
  }

  return ret;
}

static mnumber
math_string(UNUSED(char *name), char *arg, int id)

{
  return my_magic_123math_string(name, arg, id);
}

mnumber
my_magic_123math_string(UNUSED(char *name), char *arg, int id)
{
    mnumber ret = zero_mnumber;
    char *send;
    /*
     * Post-process the string argument, which is just passed verbatim.
     * Not clear if any other functions that use my_magic_123math_string() will
     * want this, but assume so for now.
     */
    while (iblank(*arg))
	arg++;
    send = arg + strlen(arg);
    while (send > arg && iblank(send[-1]))
	send--;
    *send = '\0';

    switch (id)
    {
#ifdef HAVE_ERAND48
    case MS_RAND48:
	{
	    static unsigned short seedbuf[3];
	    static int seedbuf_init;
	    unsigned short tmp_seedbuf[3], *seedbufptr;
	    int do_init = 1;

	    if (*arg) {
		/* Seed is contained in parameter named by arg */
		char *seedstr;
		seedbufptr = tmp_seedbuf;
		if ((seedstr = getsparam(arg)) && strlen(seedstr) >= 12) {
		    int i, j;
		    do_init = 0;
		    /*
		     * Decode three sets of four hex digits corresponding
		     * to each unsigned short.
		     */
		    for (i = 0; i < 3 && !do_init; i++) {
			unsigned short *seedptr = seedbufptr + i;
			*seedptr = 0;
			for (j = 0; j < 4; j++) {
			    if (idigit(*seedstr))
				*seedptr += *seedstr - '0';
			    else if (tolower(*seedstr) >= 'a' &&
				     tolower(*seedstr) <= 'f')
				*seedptr += tolower(*seedstr) - 'a' + 10;
			    else {
				do_init = 1;
				break;
			    }
			    seedstr++;
			    if (j < 3)
				*seedptr *= 16;
			}
		    }
		}
		else if (errflag)
		    break;
	    }
	    else
	    {
		/* Use default seed: must be initialised. */
		seedbufptr = seedbuf;
		if (!seedbuf_init)
		    seedbuf_init = 1;
		else
		    do_init = 1;
	    }
	    if (do_init) {
		seedbufptr[0] = (unsigned short)rand();
		seedbufptr[1] = (unsigned short)rand();
		seedbufptr[2] = (unsigned short)rand();
		/*
		 * Some implementations of rand48() need initialization.
		 * This is likely to be harmless elsewhere, since
		 * according to the documentation erand48() normally
		 * doesn't look at the seed set in this way.
		 */
		(void)seed48(seedbufptr);
	    }
	    ret.type = MN_FLOAT;
	    ret.u.d = erand48(seedbufptr);

	    if (*arg)
	    {
		char outbuf[13];
		sprintf(outbuf, "%04x%04x%04x", (int)seedbufptr[0],
			(int)seedbufptr[1], (int)seedbufptr[2]);
		setsparam(arg, ztrdup(outbuf));
	    }
	}
	break;
#endif
    }

    return ret;
}

 struct features module_features;

int
setup_(UNUSED(Module m))

{
  return my_magic_123setup_(m);
}

int
my_magic_123setup_(UNUSED(Module m))
{
    return 0;
}

int
features_(Module m, char ***features)

{
  return my_magic_123features_(m, features);
}

int
my_magic_123features_(Module m, char ***features)
{
    *features = featuresarray(m, &module_features);
    return 0;
}

int
enables_(Module m, int **enables)

{
  return my_magic_123enables_(m, enables);
}

int
my_magic_123enables_(Module m, int **enables)
{
    return handlefeatures(m, &module_features, enables);
}

int
boot_(UNUSED(Module m))

{
  return my_magic_123boot_(m);
}

int
my_magic_123boot_(UNUSED(Module m))
{
    return 0;
}

int
cleanup_(Module m)

{
  return my_magic_123cleanup_(m);
}

int
my_magic_123cleanup_(Module m)
{
    return setfeatureenables(m, &module_features, NULL);
}

int
finish_(UNUSED(Module m))

{
  return my_magic_123finish_(m);
}

