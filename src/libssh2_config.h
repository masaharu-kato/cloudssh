/* to make a symbol visible */
/* #undef LIBSSH2_API */

/* Enable clearing of memory before being freed */
/* #undef LIBSSH2_CLEAR_MEMORY */

/* Enable "none" cipher -- NOT RECOMMENDED */
/* #undef LIBSSH2_CRYPT_NONE */

/* Enable newer diffie-hellman-group-exchange-sha1 syntax */
#define LIBSSH2_DH_GEX_NEW 1

/* Compile in zlib support */
/* #undef LIBSSH2_HAVE_ZLIB */

/* Use libgcrypt */
/* #undef LIBSSH2_LIBGCRYPT */

/* Enable "none" MAC -- NOT RECOMMENDED */
/* #undef LIBSSH2_MAC_NONE */

/* Use mbedtls */
/* #undef LIBSSH2_MBEDTLS */

/* Use openssl */
#define LIBSSH2_OPENSSL 1

/* Use wincng */
/* #undef LIBSSH2_WINCNG */

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Define to 1 if _REENTRANT preprocessor symbol must be defined. */
/* #undef NEED_REENTRANT */

/* Name of package */
#define PACKAGE "libssh2"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "libssh2-devel@cool.haxx.se"

/* Define to the full name of this package. */
#define PACKAGE_NAME "libssh2"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "libssh2 -"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "libssh2"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "-"

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at runtime.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "-"

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Enable large inode numbers on Mac OS X 10.5.  */
#ifndef _DARWIN_USE_64_BIT_INODE
# define _DARWIN_USE_64_BIT_INODE 1
#endif

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */
