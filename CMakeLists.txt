cmake_minimum_required(VERSION 3.16)
project(SVMRender_LCH_ver1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_VERBOSE_MAKEFILE True)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(OUTPUT_ELF "${CMAKE_PROJECT_NAME}.out" )

set(SRC_FILES
        sources/common/src/glad/glad.c
        sources/src/svmCore.cpp
        sources/common/src/svmShader.cpp
        sources/common/src/svmProgram.cpp
        sources/common/src/svmBuffer.cpp
        sources/common/src/svmVertexLayout.cpp
        sources/src/svmModel.cpp
        sources/common/src/svmMesh.cpp
        sources/common/src/svmXML.cpp
        sources/src/svmMain.cpp
        sources/src/svmBowlMesh.cpp
        sources/src/svmContext.cpp
        sources/common/src/svmTex.cpp
        )


add_executable( ${OUTPUT_ELF} ${SRC_FILES} )


if(${APPLE})
    set(CMAKE_APPLE_SILICON_PROCESSOR arm64)

    include(Dependency.cmake)

    target_include_directories( ${OUTPUT_ELF} PUBLIC
            ${DEP_INCLUDE_DIRS}
            sources/inc
            sources/common/inc
            sources/common/inc/KHR
            sources/common/inc/glad
            )
    target_link_directories( ${OUTPUT_ELF} PUBLIC ${DEP_LIBRARY_DIRS})
    target_link_libraries( ${OUTPUT_ELF} ${DEP_LIBRARIES} )
else()
    target_include_directories( ${OUTPUT_ELF} PUBLIC
            sources/inc
            sources/common/inc
            sources/common/inc/KHR
            sources/common/inc/glad
            D:/local_lib/glfw/include
            D:/local_lib/glm
            D:/local_lib/assimp/include
            D:/local_lib/opencv-454-win/include
            D:/local_lib/libxml2/include
            )

    target_link_directories( ${OUTPUT_ELF} PUBLIC
            D:/local_lib/glfw/lib-vc2022
            D:/local_lib/assimp/lib/Debug
            D:/local_lib/opencv-454-win/lib
            D:/local_lib/libxml2/lib
            D:/local_lib/libxml2/lib
            )

    target_link_libraries( ${OUTPUT_ELF} PUBLIC
            glfw3.lib
            assimpd.lib
            opencv_world454d.lib
            libxml2d.lib
            )
endif()

if(${APPLE})
    set(WINDOW_NAME "${CMAKE_PROJECT_NAME}")
    set(WINDOW_WIDTH 960)
    set(WINDOW_HEIGHT 540)
else()
    set(WINDOW_NAME "${CMAKE_PROJECT_NAME}")
    set(WINDOW_WIDTH 1920)
    set(WINDOW_HEIGHT 1080)
endif()

target_compile_definitions( ${OUTPUT_ELF} PUBLIC
        CAMERA
#        IMAGE
        WINDOW_NAME="${WINDOW_NAME}"
        WINDOW_WIDTH=${WINDOW_WIDTH}
        WINDOW_HEIGHT=${WINDOW_HEIGHT}

        PROJECT_SRC_DIR="${PROJECT_SOURCE_DIR}"
        )

if (MSVC)
    target_compile_options(${OUTPUT_ELF} PUBLIC /wd4819)
endif()
