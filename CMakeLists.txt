cmake_minimum_required(VERSION 2.8)

project(opengl)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(
        -std=c++14
    )
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(IMGUI_DIR imgui-1.49)
set(GLFW_DIR glfw-3.2.1)
set(GLM_DIR glm-0.9.8.3)
set(GLAD_DIR glad)
set(JSON_DIR json-2.0.9)

add_subdirectory(third-party/${GLFW_DIR})

find_package(OpenGL REQUIRED)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(c2ba-glsl-shaders)
include(c2ba-copy-dll-post-build)

if(MSVC)
    set(SHADER_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/\$\(Configuration\)/shaders)
else()
    set(SHADER_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/shaders)
endif()

include_directories(
    glmlv
    PUBLIC 
    ${OPENGL_INCLUDE_DIRS}
    third-party/${GLFW_DIR}/include
    third-party/${GLM_DIR}
    third-party/${GLAD_DIR}/include
    third-party/${JSON_DIR}
    third-party/${IMGUI_DIR}
    lib/include
)

file(
    GLOB_RECURSE
    GLMLV_LIB_FILES
    lib/*.cpp lib/*.hpp lib/*.glsl
)

add_library(
    glmlv
    ${GLMLV_LIB_FILES}
)

c2ba_add_shader_directory(${CMAKE_CURRENT_SOURCE_DIR}/lib/shaders ${SHADER_OUTPUT_PATH}/glmlv)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/shaders/
    DESTINATION shaders/glmlv
    FILES_MATCHING PATTERN "*.glsl"
)

set(
    IMGUI_SRC_FILES
    third-party/${IMGUI_DIR}/imgui.cpp
    third-party/${IMGUI_DIR}/imgui_demo.cpp
    third-party/${IMGUI_DIR}/imgui_draw.cpp
)

set(
    THIRD_PARTY_SRC_FILES
    ${IMGUI_SRC_FILES}
    third-party/${GLAD_DIR}/src/glad.c
)

set(
    LIBRARIES
    ${OPENGL_LIBRARIES}
    glfw
    glmlv
)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(LIBRARIES ${LIBRARIES} stdc++fs)
endif()

source_group ("glsl" REGULAR_EXPRESSION "*/*.glsl")
source_group ("third-party" REGULAR_EXPRESSION "third-party/*.*")

file(GLOB APP_DIRECTORIES "apps/*")
foreach(DIR ${APP_DIRECTORIES})
    get_filename_component(APP ${DIR} NAME)

    file(
        GLOB_RECURSE
        SRC_FILES
        apps/${APP}/*.cpp apps/${APP}/*.hpp apps/${APP}/*.glsl
    )

    add_executable(
        ${APP}
        ${SRC_FILES}
        ${THIRD_PARTY_SRC_FILES}
    )

    target_link_libraries(
        ${APP}
        ${LIBRARIES}
    )

    install(
        TARGETS ${APP}
        DESTINATION .
    )

    c2ba_add_shader_directory(${DIR}/shaders ${SHADER_OUTPUT_PATH}/${APP})

    install(
        DIRECTORY ${DIR}/shaders/
        DESTINATION shaders/${APP}
        FILES_MATCHING PATTERN "*.glsl"
    )
endforeach()