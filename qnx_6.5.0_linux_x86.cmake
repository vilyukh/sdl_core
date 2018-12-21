set(CMAKE_SYSTEM_PROCESSOR ${ARCH_TYPE_OPTION})
set(CMAKE_SYSTEM_NAME QNX)

if(CMAKE_HOST_LINUX)
    set(HOST_EXECUTABLE_SUFFIX "")
endif(CMAKE_HOST_LINUX)

find_path(QNX_HOST
          NAME
          usr/bin/qcc${HOST_EXECUTABLE_SUFFIX}
          PATHS $ENV{QNX_HOST}
          NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH
)

find_path(QNX_TARGET
          NAME
          usr/include/qnx_errno.h
          PATHS $ENV{QNX_TARGET}
          NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH
)

if(CMAKE_HOST_LINUX)
    find_path(QNX_CONFIGURATION
              NAME
              /etc/qnx/bin/qnxactivate
              PATHS $ENV{QNX_CONFIGURATION} "$ENV{QNX_HOST}/usr/bin/qconfig"
              NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH
)
endif(CMAKE_HOST_LINUX)

set(ENV{QNX_HOST} ${QNX_HOST})
set(ENV{QNX_TARGET} ${QNX_TARGET})
if(CMAKE_HOST_LINUX)
    set(ENV{QNX_CONFIGURATION} ${QNX_CONFIGURATION})
    set(ENV{PATH} "$ENV{PATH};${QNX_HOST}/usr/bin")
endif(CMAKE_HOST_LINUX)

set(CMAKE_MAKE_PROGRAM "${QNX_HOST}/usr/bin/make${HOST_EXECUTABLE_SUFFIX}"
    CACHE PATH "QNX Make Program"
)
set(CMAKE_SH "${QNX_HOST}/usr/bin/sh${HOST_EXECUTABLE_SUFFIX}"
    CACHE PATH "QNX shell Program"
)
set(
    CMAKE_AR
    "${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-ar${HOST_EXECUTABLE_SUFFIX}"
    CACHE PATH "QNX ar Program"
)
set(
    CMAKE_RANLIB
    "${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-ranlib${HOST_EXECUTABLE_SUFFIX}"
    CACHE PATH "QNX ranlib Program"
)
set(
    CMAKE_NM
    "${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-nm${HOST_EXECUTABLE_SUFFIX}"
    CACHE PATH "QNX nm Program"
)
set(
    CMAKE_OBJCOPY
    "${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-objcopy${HOST_EXECUTABLE_SUFFIX}"
    CACHE PATH "QNX objcopy Program"
)
set(
    CMAKE_OBJDUMP
    "${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-objdump${HOST_EXECUTABLE_SUFFIX}"
    CACHE PATH "QNX objdump Program"
)
set(CMAKE_LINKER "${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-ld"
    CACHE PATH "QNX Linker Program"
)
set(
    CMAKE_STRIP
    "${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-strip${HOST_EXECUTABLE_SUFFIX}"
    CACHE PATH "QNX Strip Program"
)

set(
    CMAKE_C_COMPILER
    ${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-gcc${HOST_EXECUTABLE_SUFFIX}
    
)
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

set(
    CMAKE_CXX_COMPILER
    ${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-c++${HOST_EXECUTABLE_SUFFIX}
    
)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_FIND_ROOT_PATH ${QNX_TARGET})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
