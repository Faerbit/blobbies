cmake_minimum_required(VERSION 3.1)
project(blobbies)

find_package(OpenGL REQUIRED)
if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    pkg_search_module(GLM REQUIRED glm)
    pkg_search_module(FREETYPE REQUIRED freetype2)
endif()

add_executable(blobbies
        src/main.cpp
        src/Blob.cpp
        src/PlayerBlob.cpp
        src/globals.cpp
        src/AiBlob.cpp
        )

set_property(TARGET blobbies PROPERTY CXX_STANDARD 14)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_FREETYPE=1 -s USE_GLFW=3")
endif()

target_include_directories(blobbies PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        ${GLM_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIRS}
)

if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    target_link_libraries(blobbies
            ${GLFW_LIBRARIES}
            ${OPENGL_gl_LIBRARY}
            ${FREETYPE_LIBRARIES}
    )
endif()

execute_process(COMMAND xxd -i NotoSans-Black.ttf
        OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/font.h"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/font")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    configure_file(src/runner.html "." COPYONLY)
endif()
