# get all source files
file(GLOB_RECURSE SRC_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${EpexOrderLib_SOURCE_DIR}/include/epexLib/epexLib.hpp"
)
# remove MolSim.cpp from source files
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

# create library
add_library(
    epexLib
    STATIC
    ${SRC_FILES}
)

target_include_directories(epexLib
    PUBLIC
        # Public headers location during build
        $<BUILD_INTERFACE:${EpexOrderLib_SOURCE_DIR}/include>  
        # Public headers location after install
        $<INSTALL_INTERFACE:include>  
        # Private headers location
        ${CMAKE_CURRENT_SOURCE_DIR}  
)

target_compile_options(epexLib PRIVATE -Wall -Wextra)

# define main target
add_executable(main main.cpp)
target_link_libraries(main epexLib)
# depending on the compiler different flags are used
target_compile_options(main
        PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall>
            # disable some spam warnings for icpc...
            $<$<CXX_COMPILER_ID:Intel>:-w3 -wd383,981,1418,1572,2259>
)

