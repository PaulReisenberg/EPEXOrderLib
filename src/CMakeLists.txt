# get all source files
file(GLOB_RECURSE SRC_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# get all source files
file(GLOB_RECURSE PUBLIC_HEADERS 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
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
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  
        # Public headers location after install
        $<INSTALL_INTERFACE:include/epexlib>  
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

# Installation rules

# Install the library binary to the lib directory
install(TARGETS epexLib
        EXPORT epexLibTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

# Install the public headers to the include/epexlib directory
install(FILES ${PUBLIC_HEADERS}
        DESTINATION include/epexlib)

# Install the export file (for find_package)
install(EXPORT epexLibTargets
        FILE epexLibTargets.cmake
        NAMESPACE epexLib::
        DESTINATION lib/cmake/epexLib)

# Optionally, create and install a ConfigVersion file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/epexLibConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_file(epexLibConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/epexLibConfig.cmake"
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/epexLibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/epexLibConfigVersion.cmake"
    DESTINATION lib/cmake/epexLib)

# Export targets for other CMake projects to use
export(EXPORT epexLibTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/epexLibTargets.cmake"
       NAMESPACE epexLib::)
