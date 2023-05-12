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

add_compile_options(-faligned-new -march=native -g -O3)

option(BUCKINDEX_DEBUG "Enable debug mode for BuckIndex" OFF)

if(BUCKINDEX_DEBUG)
    add_definitions(-DBUCKINDEX_DEBUG)
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

target_link_libraries(microbench PUBLIC OpenMP::OpenMP_CXX ${JEMALLOC_LIBRARIES} ${MKL_LIBRARIES} ${TBB_LIBRARIES})
