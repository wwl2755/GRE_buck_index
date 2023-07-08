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


# hint options
# set mod_hash as default
set(HINT_MODE "murmur_hash" CACHE STRING "Choose the hash mode: mod_hash, cl_hash, murmur_hash, model_predict, no_hash")

if(HINT_MODE STREQUAL "mod_hash")
    add_definitions(-DHINT_MOD_HASH)
elseif(HINT_MODE STREQUAL "cl_hash")
    add_definitions(-DHINT_CL_HASH)
elseif(HINT_MODE STREQUAL "murmur_hash")
    add_definitions(-DHINT_MURMUR_HASH)
elseif(HINT_MODE STREQUAL "model_predict")
    add_definitions(-DHINT_MODEL_PREDICT)
elseif(HINT_MODE STREQUAL "no_hash")
    add_definitions(-DNO_HINT)
else()
    message(STATUS "HINT_MODE variable is not set or not valid. Default configuration murmur_hash will be used.")
endif()


option(BUCKINDEX_DEBUG "Enable debug mode for BuckIndex" OFF)

option(BUCKINDEX_USE_SIMD "Enable SIMD in BuckIndex" OFF)

# option(BUCKINDEX_USE_LINEAR_REGRESSION "Enable linear regression in BuckIndex" ON)
option(BUCKINDEX_NOT_USE_LINEAR_REGRESSION "Disable linear regression and use endpoint linear model in BuckIndex" OFF)

add_definitions(-DBUCKINDEX_USE_LINEAR_REGRESSION)


if(BUCKINDEX_DEBUG)
    add_definitions(-DBUCKINDEX_DEBUG)
endif()


if(BUCKINDEX_USE_SIMD)
    add_definitions(-DBUCKINDEX_USE_SIMD)
endif()
message(STATUS "BUCKINDEX_USE_SIMD: ${BUCKINDEX_USE_SIMD}")

if(BUCKINDEX_NOT_USE_LINEAR_REGRESSION)
    remove_definitions(-DBUCKINDEX_USE_LINEAR_REGRESSION)
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
