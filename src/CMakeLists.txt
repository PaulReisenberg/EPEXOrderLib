# get all source files
file(GLOB_RECURSE SRC_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# remove MolSim.cpp from source files
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

# create library
add_library(
    epexLib
    STATIC
    ${SRC_FILES}
)

# include headers
target_include_directories(
    epexLib
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

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

