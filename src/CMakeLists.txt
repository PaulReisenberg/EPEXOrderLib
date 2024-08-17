
# get all source files
file(GLOB_RECURSE SRC_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# get all header files
file(GLOB_RECURSE PUBLIC_HEADERS 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

# remove main.cpp from source files
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

# create library
add_library(
    epexLib
    STATIC
    ${SRC_FILES}
)

# Specify the include directories
target_include_directories(epexLib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  # For building the library
        $<INSTALL_INTERFACE:include/epexlib>            # For users after installation
)

# Enable all warnings
target_compile_options(epexLib PRIVATE -Wall -Wextra)

# Define the main executable
add_executable(main main.cpp)
target_link_libraries(main epexLib)

# Installation rules

# Install the library binary to the lib directory
install(TARGETS epexLib
        EXPORT epexLibTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

# Install the public headers to the include/epexlib directory
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp"
)

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
