/* Test file for mpfr_version.

Copyright 2004-2016 Free Software Foundation, Inc.
Contributed by the AriC and Caramba projects, INRIA.

This file is part of the GNU MPFR Library.

The GNU MPFR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MPFR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MPFR Library; see the file COPYING.LESSER.  If not, see
http://www.gnu.org/licenses/ or write to the Free Software Foundation, Inc.,
51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA. */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "mpfr-intmax.h"
#include "mpfr-test.h"

#define STRINGIZE(S) #S
#define MAKE_STR(S) STRINGIZE(S)

int
main (void)
{
  int err = 0;

  /* Test the GMP and MPFR versions. */
  if (test_version ())
    exit (1);

  printf ("[tversion] MPFR %s\n", MPFR_VERSION_STRING);

  /* TODO: We may want to output info for non-GNUC-compat compilers too. See:
   * http://sourceforge.net/p/predef/wiki/Compilers/
   * http://nadeausoftware.com/articles/2012/10/c_c_tip_how_detect_compiler_name_and_version_using_compiler_predefined_macros
   *
   * For ICC, do not check the __ICC macro as it is obsolete and not always
   * defined.
   */
#define COMP "[tversion] Compiler: "
#ifdef __INTEL_COMPILER
# ifdef __VERSION__
#  define ICCV " [" __VERSION__ "]"
# else
#  define ICCV ""
# endif
  printf (COMP "ICC %d.%d.%d" ICCV "\n", __INTEL_COMPILER / 100,
          __INTEL_COMPILER % 100, __INTEL_COMPILER_UPDATE);
#elif (defined(__GNUC__) || defined(__clang__)) && defined(__VERSION__)
# ifdef __clang__
#  define COMP2 COMP
# else
#  define COMP2 COMP "GCC "
# endif
  printf (COMP2 "%s\n", __VERSION__);
#endif

#ifdef __MPIR_VERSION
  printf ("[tversion] MPIR: header %d.%d.%d, library %s\n",
          __MPIR_VERSION, __MPIR_VERSION_MINOR, __MPIR_VERSION_PATCHLEVEL,
          mpir_version);
#else
#ifdef MPFR_USE_MINI_GMP
  printf ("[tversion] mini-gmp\n");
#else
  printf ("[tversion] GMP: header %d.%d.%d, library %s\n",
          __GNU_MP_VERSION, __GNU_MP_VERSION_MINOR, __GNU_MP_VERSION_PATCHLEVEL,
          gmp_version);
#endif
#endif

  /* The following output is also useful under Unix, where one should get:
     WinDLL: __GMP_LIBGMP_DLL = 0, MPFR_WIN_THREAD_SAFE_DLL = undef
     If this is not the case, something is probably broken. We cannot test
     automatically as some MS Windows implementations may declare some Unix
     (POSIX) compatibility; for instance, Cygwin32 defines __unix__ (but
     Cygwin64 does not, probably because providing both MS Windows API and
     POSIX API is not possible with a 64-bit ABI, since MS Windows is LLP64
     and Unix is LP64). */
  printf ("[tversion] WinDLL: __GMP_LIBGMP_DLL = "
#if defined(__GMP_LIBGMP_DLL)
          MAKE_STR(__GMP_LIBGMP_DLL)
#else
          "undef"
#endif
          ", MPFR_WIN_THREAD_SAFE_DLL = "
#if defined(MPFR_WIN_THREAD_SAFE_DLL)
          MAKE_STR(MPFR_WIN_THREAD_SAFE_DLL)
#else
          "undef"
#endif
          "\n");

  if (
#ifdef MPFR_USE_THREAD_SAFE
      !
#endif
      mpfr_buildopt_tls_p ())
    {
      printf ("ERROR! mpfr_buildopt_tls_p() and macros"
              " do not match!\n");
      err = 1;
    }

  if (
#ifdef MPFR_WANT_FLOAT128
      !
#endif
      mpfr_buildopt_float128_p ())
    {
      printf ("ERROR! mpfr_buildopt_float128_p() and macros"
              " do not match!\n");
      err = 1;
    }

  if (
#ifdef MPFR_WANT_DECIMAL_FLOATS
      !
#endif
      mpfr_buildopt_decimal_p ())
    {
      printf ("ERROR! mpfr_buildopt_decimal_p() and macros"
              " do not match!\n");
      err = 1;
    }

  if (
#if defined(MPFR_HAVE_GMP_IMPL) || defined(WANT_GMP_INTERNALS)
      !
#endif
      mpfr_buildopt_gmpinternals_p ())
    {
      printf ("ERROR! mpfr_buildopt_gmpinternals_p() and macros"
              " do not match!\n");
      err = 1;
    }

  printf ("[tversion] TLS = %s, float128 = %s, decimal = %s,"
          " GMP internals = %s\n",
          mpfr_buildopt_tls_p () ? "yes" : "no",
          mpfr_buildopt_float128_p () ? "yes" : "no",
          mpfr_buildopt_decimal_p () ? "yes" : "no",
          mpfr_buildopt_gmpinternals_p () ? "yes" : "no");

  printf ("[tversion] intmax_t = "
#if defined(_MPFR_H_HAVE_INTMAX_T)
          "yes"
#else
          "no"
#endif
          ", printf = "
#if defined(HAVE_STDARG) && !defined(MPFR_USE_MINI_GMP)
          "yes"
#else
          "no"
#endif
          "\n");

  printf ("[tversion] gmp_printf: hhd = "
#if defined(NPRINTF_HH)
          "no"
#else
          "yes"
#endif
          ", lld = "
#if defined(NPRINTF_LL)
          "no"
#else
          "yes"
#endif
          ", jd = "
#if defined(NPRINTF_J)
          "no"
#else
          "yes"
#endif
          ", td = "
#if defined(NPRINTF_T)
          "no"
#elif defined(PRINTF_T)
          "yes"
#else
          "?"
#endif
          ", Ld = "
#if defined(NPRINTF_L)
          "no"
#elif defined(PRINTF_L)
          "yes"
#else
          "?"
#endif
          "\n");

  if (strcmp (mpfr_buildopt_tune_case (), MPFR_TUNE_CASE) != 0)
    {
      printf ("ERROR! mpfr_buildopt_tune_case() and MPFR_TUNE_CASE"
              " do not match!\n  %s\n  %s\n",
              mpfr_buildopt_tune_case (), MPFR_TUNE_CASE);
      err = 1;
    }
  else
    printf ("[tversion] MPFR tuning parameters from %s\n", MPFR_TUNE_CASE);

  if (strcmp (mpfr_get_patches (), "") != 0)
    printf ("[tversion] MPFR patches: %s\n", mpfr_get_patches ());

  tests_start_mpfr ();
  if (locale != NULL)
    printf ("[tversion] Locale: %s\n", locale);
  /* The memory limit should not be changed for "make check".
     The warning below signals a possible user mistake.
     Do not use "%zu" because it is not available in C90;
     the type mpfr_ueexp_t should be sufficiently large. */
  if (tests_memory_limit != DEFAULT_MEMORY_LIMIT)
    printf ("[tversion] Warning! Memory limit changed to %" MPFR_EXP_FSPEC
            "u\n", (mpfr_ueexp_t) tests_memory_limit);
  tests_end_mpfr ();

  return err;
}
