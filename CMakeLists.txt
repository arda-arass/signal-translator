cmake_minimum_required(VERSION 3.14)

project(signal-translator)

add_executable(${PROJECT_NAME} src/main.cpp src/dsp.cpp)

# DOWNLOAD ALL THE SUBMODULES
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --remote
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

# CHECK ALL THE SUBMODULES
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/lib/pub/matplotplusplus/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

find_package(Wget REQUIRED)

message("FFTW3 download and extract process has been started...")

set(URL "https://fftw.org/pub/fftw/fftw-3.3.10.tar.gz")
set(DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/pub/fftw3")
set(FILENAME "fftw-3.3.10.tar.gz")
set(FULLPATH "${DESTINATION}/${FILENAME}")
set(EXTRACT_DIR "${DESTINATION}")

# Create the destination directory if it doesn't exist
file(MAKE_DIRECTORY ${DESTINATION})

# check if the file already exists
if(EXISTS ${FULLPATH})
    message("File ${FILENAME} already exists in ${DESTINATION}")
else()
    # download the file using wget
    execute_process(
        COMMAND wget ${URL} -O ${FULLPATH}
        RESULT_VARIABLE result
    )

    if(result)
        message(FATAL_ERROR "Failed to download file from ${URL}")
    endif()
endif()

# check if the directory is already populated
file(GLOB DIR_CONTENTS ${EXTRACT_DIR}/*)
list(FILTER DIR_CONTENTS EXCLUDE REGEX "${EXTRACT_DIR}/${FILENAME}")
if(DIR_CONTENTS)
    message("Archive ${FILENAME} already extracted to ${EXTRACT_DIR}")
else()
    # extract the archive using tar
    execute_process(
        COMMAND tar xf ${FULLPATH} -C ${EXTRACT_DIR}
        RESULT_VARIABLE result
    )

    if(result)
        message(FATAL_ERROR "Failed to extract ${FILENAME}")
    endif()
endif()

# Matplot++ library flags configuration
set(MATPLOTPP_BUILD_EXAMPLES OFF)
set(MATPLOTPP_BUILD_TESTS OFF)
set(INTERPROCEDURAL_OPTIMIZATION ON)

add_subdirectory(lib/pub/matplotplusplus)
# add_subdirectory(lib/pub/fftw3/fftw-3.3.10)

target_include_directories(${PROJECT_NAME}
    PRIVATE lib/priv
)

target_link_libraries(${PROJECT_NAME} PUBLIC matplot fftw3)
