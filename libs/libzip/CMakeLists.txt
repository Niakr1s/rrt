# TODO:
# create usable libtool .la file

CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

PROJECT(libzip C)

OPTION(ENABLE_COMMONCRYPTO "Enable use of CommonCrypto" ON)
OPTION(ENABLE_GNUTLS "Enable use of GnuTLS" ON)
OPTION(ENABLE_MBEDTLS "Enable use of mbed TLS" ON)
OPTION(ENABLE_OPENSSL "Enable use of OpenSSL" ON)
OPTION(ENABLE_WINDOWS_CRYPTO "Enable use of Windows cryptography libraries" ON)

OPTION(ENABLE_BZIP2 "Enable use of BZip2" ON)

OPTION(BUILD_TOOLS "Build tools in the src directory (zipcmp, zipmerge, ziptool)" ON)
OPTION(BUILD_REGRESS "Build regression tests" ON)
OPTION(BUILD_EXAMPLES "Build examples" ON)
OPTION(BUILD_DOC "Build documentation" ON)

INCLUDE(CheckFunctionExists)
INCLUDE(CheckIncludeFiles)
INCLUDE(CheckSymbolExists)
INCLUDE(CheckTypeSize)
INCLUDE(CheckCSourceRuns)
INCLUDE(CheckCSourceCompiles)
INCLUDE(CheckStructHasMember)
INCLUDE(TestBigEndian)
INCLUDE(GNUInstallDirs)
IF(ENABLE_COMMONCRYPTO)
  CHECK_INCLUDE_FILES(CommonCrypto/CommonCrypto.h COMMONCRYPTO_FOUND)
ELSE()
  SET(COMMONCRYPTO_FOUND FALSE)
ENDIF()
IF(ENABLE_GNUTLS)
  INCLUDE(FindNettle)
  INCLUDE(FindGnuTLS)
ELSE()
  SET(GNUTLS_FOUND FALSE)
ENDIF()
IF(ENABLE_MBEDTLS)
  FIND_PATH(MBEDTLS_INCLUDE_DIR mbedtls/aes.h)
  FIND_LIBRARY(MBEDTLS_LIBRARIES NAMES mbedtls)
ELSE()
  SET(MBEDTLS_LIBRARIES FALSE)
ENDIF()
IF(ENABLE_OPENSSL)
  INCLUDE(FindOpenSSL)
ELSE()
  SET(OPENSSL_FOUND FALSE)
ENDIF()
IF(WIN32)
  IF(ENABLE_WINDOWS_CRYPTO)
    SET(WINDOWS_CRYPTO_FOUND TRUE)
  ENDIF()
ELSE()
  SET(WINDOWS_CRYPTO_FOUND FALSE)
ENDIF()

OPTION(BUILD_SHARED_LIBS "Build shared libraries" ON)


OPTION(SHARED_LIB_VERSIONNING "Add SO version in .so build" ON)

FIND_PROGRAM(MDOCTOOL NAMES mandoc groff)
IF (MDOCTOOL)
  SET(DOCUMENTATION_FORMAT "mdoc" CACHE STRING "Documentation format")
ELSE()
  FIND_PROGRAM(MANTOOL NAMES nroff)
  IF (MANTOOL)
    SET(DOCUMENTATION_FORMAT "man" CACHE STRING "Documentation format")
  ELSE()
    SET(DOCUMENTATION_FORMAT "html" CACHE STRING "Documentation format")
  ENDIF()
ENDIF()

SET(PACKAGE "libzip")
SET(PACKAGE_NAME ${PACKAGE})
SET(PACKAGE_VERSION_MAJOR "1")
SET(PACKAGE_VERSION_MINOR "5")
SET(PACKAGE_VERSION_MICRO "2")
#SET(VERSION "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}")
SET(VERSION "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO}")
SET(PACKAGE_VERSION ${VERSION})
SET(LIBZIP_VERSION ${PACKAGE_VERSION})
SET(LIBZIP_VERSION_MAJOR ${PACKAGE_VERSION_MAJOR})
SET(LIBZIP_VERSION_MINOR ${PACKAGE_VERSION_MINOR})
SET(LIBZIP_VERSION_MICRO ${PACKAGE_VERSION_MICRO})
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

SET(ARCHIVE_NAME ${PACKAGE_NAME}-${PACKAGE_VERSION})
ADD_CUSTOM_TARGET(dist
  COMMAND git config tar.tar.xz.command "xz -c"
  COMMAND git archive --prefix=${ARCHIVE_NAME}/ -o ${ARCHIVE_NAME}.tar.gz HEAD
  COMMAND git archive --prefix=${ARCHIVE_NAME}/ -o ${ARCHIVE_NAME}.tar.xz HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
ADD_CUSTOM_TARGET(distcheck
  COMMAND chmod -R u+w ${ARCHIVE_NAME} ${ARCHIVE_NAME}-build ${ARCHIVE_NAME}-dest 2>/dev/null || true
  COMMAND rm -rf ${ARCHIVE_NAME} ${ARCHIVE_NAME}-build ${ARCHIVE_NAME}-dest
  COMMAND cmake -E tar xf ${ARCHIVE_NAME}.tar.gz
  COMMAND chmod -R u-w ${ARCHIVE_NAME}
  COMMAND mkdir ${ARCHIVE_NAME}-build
  COMMAND mkdir ${ARCHIVE_NAME}-dest
  COMMAND cd ${ARCHIVE_NAME}-build && cmake -DCMAKE_INSTALL_PREFIX=../${ARCHIVE_NAME}-dest ../${ARCHIVE_NAME}
  COMMAND cd ${ARCHIVE_NAME}-build && make -j4
  COMMAND cd ${ARCHIVE_NAME}-build && make test
  COMMAND cd ${ARCHIVE_NAME}-build && make install
#  COMMAND cd ${ARCHIVE_NAME}-build && make uninstall
#  COMMAND if [ `find ${ARCHIVE_NAME}-dest ! -type d | wc -l` -ne 0 ]; then echo leftover files in ${ARCHIVE_NAME}-dest; false; fi
  COMMAND cd ${ARCHIVE_NAME}-build && make clean
  COMMAND chmod -R u+w ${ARCHIVE_NAME} ${ARCHIVE_NAME}-build ${ARCHIVE_NAME}-dest
  COMMAND rm -rf ${ARCHIVE_NAME} ${ARCHIVE_NAME}-build ${ARCHIVE_NAME}-dest
  COMMAND echo "${ARCHIVE_NAME}.tar.gz is ready for distribution."
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
ADD_DEPENDENCIES(distcheck dist)

IF(BUILD_SHARED_LIBS)
  SET(HAVE_SHARED TRUE)
ELSE()
  SET(ZIP_STATIC TRUE)
ENDIF()

# Checks

CHECK_FUNCTION_EXISTS(_chmod HAVE__CHMOD)
CHECK_FUNCTION_EXISTS(_close HAVE__CLOSE)
CHECK_FUNCTION_EXISTS(_dup HAVE__DUP)
CHECK_FUNCTION_EXISTS(_fdopen HAVE__FDOPEN)
CHECK_FUNCTION_EXISTS(_fileno HAVE__FILENO)
CHECK_FUNCTION_EXISTS(_open HAVE__OPEN)
CHECK_FUNCTION_EXISTS(_setmode HAVE__SETMODE)
CHECK_FUNCTION_EXISTS(_snprintf HAVE__SNPRINTF)
CHECK_FUNCTION_EXISTS(_strdup HAVE__STRDUP)
CHECK_FUNCTION_EXISTS(_stricmp HAVE__STRICMP)
CHECK_FUNCTION_EXISTS(_strtoi64 HAVE__STRTOI64)
CHECK_FUNCTION_EXISTS(_strtoui64 HAVE__STRTOUI64)
CHECK_FUNCTION_EXISTS(_umask HAVE__UMASK)
CHECK_FUNCTION_EXISTS(_unlink HAVE__UNLINK)
CHECK_FUNCTION_EXISTS(clonefile HAVE_CLONEFILE)
CHECK_FUNCTION_EXISTS(explicit_bzero HAVE_EXPLICIT_BZERO)
CHECK_FUNCTION_EXISTS(explicit_memset HAVE_EXPLICIT_MEMSET)
CHECK_FUNCTION_EXISTS(fileno HAVE_FILENO)
CHECK_FUNCTION_EXISTS(fseeko HAVE_FSEEKO)
CHECK_FUNCTION_EXISTS(ftello HAVE_FTELLO)
CHECK_FUNCTION_EXISTS(getprogname HAVE_GETPROGNAME)
CHECK_FUNCTION_EXISTS(open HAVE_OPEN)
CHECK_FUNCTION_EXISTS(mkstemp HAVE_MKSTEMP)
CHECK_FUNCTION_EXISTS(setmode HAVE_SETMODE)
CHECK_FUNCTION_EXISTS(snprintf HAVE_SNPRINTF)
CHECK_FUNCTION_EXISTS(strcasecmp HAVE_STRCASECMP)
CHECK_FUNCTION_EXISTS(strdup HAVE_STRDUP)
CHECK_FUNCTION_EXISTS(stricmp HAVE_STRICMP)
CHECK_FUNCTION_EXISTS(strtoll HAVE_STRTOLL)
CHECK_FUNCTION_EXISTS(strtoull HAVE_STRTOULL)

CHECK_INCLUDE_FILES("sys/types.h;sys/stat.h;fts.h" HAVE_FTS_H)
CHECK_INCLUDE_FILES(stdbool.h HAVE_STDBOOL_H)
CHECK_INCLUDE_FILES(strings.h HAVE_STRINGS_H)
CHECK_INCLUDE_FILES(unistd.h HAVE_UNISTD_H)

CHECK_INCLUDE_FILES(inttypes.h HAVE_INTTYPES_H_LIBZIP)
CHECK_INCLUDE_FILES(stdint.h HAVE_STDINT_H_LIBZIP)
CHECK_INCLUDE_FILES(sys/types.h HAVE_SYS_TYPES_H_LIBZIP)

# TODO: fix test
# this test does not find __progname even when it exists
#CHECK_SYMBOL_EXISTS(__progname stdlib.h HAVE___PROGNAME)

CHECK_TYPE_SIZE(__int8 __INT8_LIBZIP)
CHECK_TYPE_SIZE(int8_t INT8_T_LIBZIP)
CHECK_TYPE_SIZE(uint8_t UINT8_T_LIBZIP)
CHECK_TYPE_SIZE(__int16 __INT16_LIBZIP)
CHECK_TYPE_SIZE(int16_t INT16_T_LIBZIP)
CHECK_TYPE_SIZE(uint16_t UINT16_T_LIBZIP)
CHECK_TYPE_SIZE(__int32 __INT32_LIBZIP)
CHECK_TYPE_SIZE(int32_t INT32_T_LIBZIP)
CHECK_TYPE_SIZE(uint32_t UINT32_T_LIBZIP)
CHECK_TYPE_SIZE(__int64 __INT64_LIBZIP)
CHECK_TYPE_SIZE(int64_t INT64_T_LIBZIP)
CHECK_TYPE_SIZE(uint64_t UINT64_T_LIBZIP)
CHECK_TYPE_SIZE("short" SHORT_LIBZIP)
CHECK_TYPE_SIZE("int" INT_LIBZIP)
CHECK_TYPE_SIZE("long" LONG_LIBZIP)
CHECK_TYPE_SIZE("long long" LONG_LONG_LIBZIP)
CHECK_TYPE_SIZE("off_t" SIZEOF_OFF_T)
CHECK_TYPE_SIZE("size_t" SIZE_T_LIBZIP)
CHECK_TYPE_SIZE("ssize_t" SSIZE_T_LIBZIP)

CHECK_C_SOURCE_COMPILES("#include <sys/ioctl.h>
#include <linux/fs.h>
int main(int argc, char *argv[]) { unsigned long x = FICLONERANGE; }" HAVE_FICLONERANGE)

CHECK_C_SOURCE_COMPILES("
int foo(char * _Nullable bar);
int main(int argc, char *argv[]) { }" HAVE_NULLABLE)

TEST_BIG_ENDIAN(WORDS_BIGENDIAN)

FIND_PACKAGE(ZLIB REQUIRED)
INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})
SET(CMAKE_REQUIRED_INCLUDES ${ZLIB_INCLUDE_DIR})
IF(ZLIB_VERSION_STRING VERSION_LESS "1.1.2")
  MESSAGE(FATAL_ERROR "-- ZLIB version too old, please install at least v1.1.2")
ENDIF(ZLIB_VERSION_STRING VERSION_LESS "1.1.2")

IF(ENABLE_BZIP2)
  FIND_PACKAGE(BZip2)
  IF(BZIP2_FOUND)
    SET (HAVE_LIBBZ2 1)

    INCLUDE_DIRECTORIES(${BZIP2_INCLUDE_DIR})
    SET (OPTIONAL_LIBRARY ${BZIP2_LIBRARY})
  ELSE()
    MESSAGE(WARNING "-- bzip2 library not found; bzip2 support disabled")
  ENDIF(BZIP2_FOUND)
ENDIF(ENABLE_BZIP2)

IF (COMMONCRYPTO_FOUND)
  SET (HAVE_CRYPTO 1)
  SET (HAVE_COMMONCRYPTO 1)
ELSEIF (WINDOWS_CRYPTO_FOUND)
  SET (HAVE_CRYPTO 1)
  SET (HAVE_WINDOWS_CRYPTO 1)
ELSEIF (GNUTLS_FOUND AND NETTLE_FOUND)
  SET (HAVE_CRYPTO 1)
  SET (HAVE_GNUTLS 1)
  INCLUDE_DIRECTORIES(${GNUTLS_INCLUDE_DIR} ${NETTLE_INCLUDE_DIR})
  SET (OPTIONAL_LIBRARY ${OPTIONAL_LIBRARY} ${GNUTLS_LIBRARY} ${NETTLE_LIBRARY})
ELSEIF (OPENSSL_FOUND)
  SET (HAVE_CRYPTO 1)
  SET (HAVE_OPENSSL 1)
  INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
  SET (OPTIONAL_LIBRARY ${OPTIONAL_LIBRARY} ${OPENSSL_LIBRARIES})
ELSEIF (MBEDTLS_LIBRARIES)
  SET (HAVE_CRYPTO 1)
  SET (HAVE_MBEDTLS 1)
  INCLUDE_DIRECTORIES(${MBEDTLS_INCLUDE_DIR})
  SET (OPTIONAL_LIBRARY ${OPTIONAL_LIBRARY} ${MBEDTLS_LIBRARIES})
ENDIF()

IF (NOT HAVE_CRYPTO)
  MESSAGE(WARNING "-- neither Common Crypto, GnuTLS, mbed TLS, OpenSSL, nor Windows Cryptography found; AES support disabled")
ENDIF()

IF(MSVC)
ADD_DEFINITIONS("-D_CRT_SECURE_NO_WARNINGS")
ADD_DEFINITIONS("-D_CRT_NONSTDC_NO_DEPRECATE")
ENDIF(MSVC)

if(WIN32)
  if(HAVE_WINDOWS_CRYPTO)
    SET (OPTIONAL_LIBRARY "${OPTIONAL_LIBRARY}" bcrypt)
  endif()
  if(CMAKE_SYSTEM_NAME MATCHES WindowsPhone OR CMAKE_SYSTEM_NAME MATCHES WindowsStore)
    ADD_DEFINITIONS(-DMS_UWP)
  else(CMAKE_SYSTEM_NAME MATCHES WindowsPhone OR CMAKE_SYSTEM_NAME MATCHES WindowsStore)
    SET (OPTIONAL_LIBRARY "${OPTIONAL_LIBRARY}" advapi32)
  endif(CMAKE_SYSTEM_NAME MATCHES WindowsPhone OR CMAKE_SYSTEM_NAME MATCHES WindowsStore)
endif(WIN32)

ADD_DEFINITIONS("-DHAVE_CONFIG_H")

# rpath handling: use rpath in installed binaries
IF(NOT CMAKE_SYSTEM_NAME MATCHES Linux)
	SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
	SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
ENDIF()

# Testing
ENABLE_TESTING()

# Targets
ADD_SUBDIRECTORY(lib)

#IF(BUILD_DOC)
#ADD_SUBDIRECTORY(man)
#ENDIF()

#IF(BUILD_TOOLS)
#ADD_SUBDIRECTORY(src)
#ENDIF()

#IF(BUILD_REGRESS)
#ADD_SUBDIRECTORY(regress)
#ENDIF()

#IF(BUILD_EXAMPLES)
#ADD_SUBDIRECTORY(examples)
#ENDIF()


# pkgconfig file
SET(prefix ${CMAKE_INSTALL_PREFIX})
SET(exec_prefix \${prefix})
SET(bindir \${exec_prefix}/${CMAKE_INSTALL_BINDIR})
SET(libdir \${exec_prefix}/${CMAKE_INSTALL_LIBDIR})
SET(includedir \${prefix}/${CMAKE_INSTALL_INCLUDEDIR})
IF(BZIP2_FOUND)
  SET(LIBS "${LIBS} -lbz2")
ENDIF()
SET(LIBS "${LIBS} -lz")
IF(CMAKE_SYSTEM_NAME MATCHES BSD)
  SET(PKG_CONFIG_RPATH "-Wl,-R\${libdir}")
ENDIF(CMAKE_SYSTEM_NAME MATCHES BSD)
CONFIGURE_FILE(libzip.pc.in libzip.pc @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/libzip.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

# fixed size integral types

IF(HAVE_STDINT_H_LIBZIP)
  SET(LIBZIP_TYPES_INCLUDE "#include <stdint.h>")
ELSEIF(HAVE_INTTYPES_H_LIBZIP)
  SET(LIBZIP_TYPES_INCLUDE "#include <inttypes.h>")
ELSEIF(HAVE_SYS_TYPES_H_LIBZIP)
  SET(LIBZIP_TYPES_INCLUDE "#include <sys/types.h>")
ENDIF()

IF(HAVE_INT8_T_LIBZIP)
  SET(ZIP_INT8_T int8_t)
ELSEIF(HAVE___INT8_LIBZIP)
  SET(ZIP_INT8_T __int8)
ELSE()
  SET(ZIP_INT8_T "signed char")
ENDIF()

IF(HAVE_UINT8_T_LIBZIP)
  SET(ZIP_UINT8_T uint8_t)
ELSEIF(HAVE___INT8_LIBZIP)
  SET(ZIP_UINT8_T "unsigned __int8")
ELSE()
  SET(ZIP_UINT8_T "unsigned char")
ENDIF()

IF(HAVE_INT16_T_LIBZIP)
  SET(ZIP_INT16_T int16_t)
ELSEIF(HAVE___INT16_LIBZIP)
  SET(INT16_T_LIBZIP __int16)
ELSEIF(SHORT_LIBZIP EQUAL 2)
  SET(INT16_T_LIBZIP short)
ENDIF()

IF(HAVE_UINT16_T_LIBZIP)
  SET(ZIP_UINT16_T uint16_t)
ELSEIF(HAVE___INT16_LIBZIP)
  SET(UINT16_T_LIBZIP "unsigned __int16")
ELSEIF(SHORT_LIBZIP EQUAL 2)
  SET(UINT16_T_LIBZIP "unsigned short")
ENDIF()

IF(HAVE_INT32_T_LIBZIP)
  SET(ZIP_INT32_T int32_t)
ELSEIF(HAVE___INT32_LIBZIP)
  SET(ZIP_INT32_T __int32)
ELSEIF(INT_LIBZIP EQUAL 4)
  SET(ZIP_INT32_T int)
ELSEIF(LONG_LIBZIP EQUAL 4)
  SET(ZIP_INT32_T long)
ENDIF()

IF(HAVE_UINT32_T_LIBZIP)
SET(ZIP_UINT32_T uint32_t)
ELSEIF(HAVE___INT32_LIBZIP)
SET(ZIP_UINT32_T "unsigned __int32")
ELSEIF(INT_LIBZIP EQUAL 4)
SET(ZIP_UINT32_T "unsigned int")
ELSEIF(LONG_LIBZIP EQUAL 4)
SET(ZIP_UINT32_T "unsigned long")
ENDIF()

IF(HAVE_INT64_T_LIBZIP)
  SET(ZIP_INT64_T int64_t)
ELSEIF(HAVE___INT64_LIBZIP)
  SET(ZIP_INT64_T __int64)
ELSEIF(LONG_LIBZIP EQUAL 8)
  SET(ZIP_INT64_T long)
ELSEIF(LONG_LONG_LIBZIP EQUAL 8)
  SET(ZIP_INT64_T "long long")
ENDIF()

IF(HAVE_UINT64_T_LIBZIP)
  SET(ZIP_UINT64_T uint64_t)
ELSEIF(HAVE___INT64_LIBZIP)
  SET(ZIP_UINT64_T "unsigned __int64")
ELSEIF(LONG_LIBZIP EQUAL 8)
  SET(ZIP_UINT64_T "unsigned long")
ELSEIF(LONG_LONG_LIBZIP EQUAL 8)
  SET(ZIP_UINT64_T "unsigned long long")
ENDIF()

IF(HAVE_NULLABLE)
  SET(ZIP_NULLABLE_DEFINES)
ELSE()
  SET(ZIP_NULLABLE_DEFINES "#define _Nullable
#define _Nonnull")
ENDIF()

# write out config file
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/cmake-config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/cmake-zipconf.h.in ${CMAKE_CURRENT_BINARY_DIR}/zipconf.h)

# for tests

SET(srcdir ${CMAKE_CURRENT_SOURCE_DIR}/regress)
SET(abs_srcdir ${CMAKE_CURRENT_SOURCE_DIR}/regress)
SET(top_builddir ${CMAKE_BINARY_DIR})

CONFIGURE_FILE(regress/runtest.in ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/runtest @ONLY)
FILE(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/runtest
  DESTINATION ${CMAKE_BINARY_DIR}/regress
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)


# installation

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/zipconf.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
