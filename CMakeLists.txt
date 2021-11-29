cmake_minimum_required(VERSION 3.10.0)
project(dtrancoder VERSION 0.1.0)

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
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${GST_INCLUDE_DIRS})

#Setting target name
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME}
                      ${GST_APP_LIBRARIES}
                      ${GST_VIDEO_LIBRARIES}
                      )



