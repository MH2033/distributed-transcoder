cmake_minimum_required(VERSION 3.10.0)
project(dtrancoder VERSION 0.8.0)

#find boost program options
find_package(Boost 1.65 COMPONENTS program_options REQUIRED)

#find MPI
find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

#Find and link to gst-lib
find_package(PkgConfig) 
pkg_search_module(GLIB REQUIRED glib-2.0) 
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GST_APP REQUIRED gstreamer-app-1.0)
pkg_check_modules(GST_VIDEO REQUIRED gstreamer-video-1.0)

#Add all .cpp and .c files in src folder to compile list
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp
                    ${PROJECT_SOURCE_DIR}/src/*.c
    )

#Add all header files located include to headers list
include_directories(${Boost_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories("/usr/include/gstreamer-1.0")
include_directories("/usr/include/glib-2.0")
include_directories("/usr/lib/aarch64-linux-gnu/glib-2.0/include")
include_directories("/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include")
include_directories("/usr/lib/x86_64-linux-gnu/glib-2.0/include")

#Setting target name
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME}
                      ${GST_APP_LIBRARIES}
                      ${GST_VIDEO_LIBRARIES}
                      ${MPI_CXX_LIBRARIES}
                      ${Boost_LIBRARIES}
                      pthread
                      )
