cmake_minimum_required (VERSION 3.14)
project (GRE)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(FetchContent)
include(ExternalProject)

find_package(OpenMP)
find_package(MKL REQUIRED)
find_package(JeMalloc REQUIRED)
find_package(TBB REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${MKL_INCLUDE_DIRS})
include_directories(${TBB_INCLUDE_DIRS})
include_directories(${JEMALLOC_INCLUDE_DIR})
include_directories(SYSTEM src/competitor/hot/src/libs/hot/commons/include)
include_directories(SYSTEM src/competitor/hot/src/libs/hot/rowex/include)
include_directories(SYSTEM src/competitor/hot/src/libs/hot/single-threaded/include)
include_directories(SYSTEM src/competitor/hot/src/libs/idx/content-helpers/include)
include_directories(${CMAKE_SOURCE_DIR}/src/competitor/buckindex/src/include)
include_directories(${CMAKE_SOURCE_DIR}/src/competitor/buckindex/src/src)
include_directories(/opt/intel/oneapi/vtune/latest/sdk/include/) # vtune headers

# vtune libraries
set(VTUNE_LIB_PATH /opt/intel/oneapi/vtune/latest/sdk)
find_library(ADVISOR_LIB NAMES advisor PATHS ${VTUNE_LIB_PATH}/lib64 ${VTUNE_LIB_PATH}/lib32 NO_DEFAULT_PATH)
find_library(ITTNOTIFY_LIB NAMES ittnotify PATHS ${VTUNE_LIB_PATH}/lib64 ${VTUNE_LIB_PATH}/lib32 NO_DEFAULT_PATH)
find_library(JITPROFILING_LIB NAMES jitprofiling PATHS ${VTUNE_LIB_PATH}/lib64 ${VTUNE_LIB_PATH}/lib32 NO_DEFAULT_PATH)
message(STATUS "VTune ADVISOR_LIB: ${ADVISOR_LIB}")
message(STATUS "VTune ITTNOTIFY_LIB: ${ITTNOTIFY_LIB}")
message(STATUS "VTune JITPROFILING_LIB: ${JITPROFILING_LIB}")

add_compile_options(-faligned-new -march=native -g -pg -O3)

option(BUCKINDEX_DEBUG "Enable debug mode for BuckIndex" OFF)
option(BUCKINDEX_HINT_HASH "Enable hash hint has for BuckIndex" ON)

if(BUCKINDEX_DEBUG)
    add_definitions(-DBUCKINDEX_DEBUG)
endif()

if(BUCKINDEX_HINT_HASH)
    add_definitions(-DBUCKINDEX_HINT_HASH)
endif()

# add_subdirectory(src/competitor/wormhole)
# add_subdirectory(src/competitor/wormhole_u64)
# add_subdirectory(src/competitor/masstree)
# add_subdirectory(src/competitor/buckindex)


# microbench
add_executable(microbench
        ${CMAKE_CURRENT_SOURCE_DIR}/src/benchmark/microbench.cpp
    )

# target_link_libraries(microbench
#         PRIVATE
#         # WORMHOLE
#         # WORMHOLEU64
#         # MASSTREE
# )

target_link_libraries(microbench
        PUBLIC OpenMP::OpenMP_CXX ${JEMALLOC_LIBRARIES}
        ${MKL_LIBRARIES} ${TBB_LIBRARIES}
        ${ADVISOR_LIB} ${ITTNOTIFY_LIB} ${JITPROFILING_LIB}
        dl
)
