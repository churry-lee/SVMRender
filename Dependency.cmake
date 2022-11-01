#set(SPD_INCLUDE_DIR /opt/homebrew/opt/spdlog/include)
#set(SPD_LIBRARY_DIR /opt/homebrew/opt/spdlog/lib)
#set(SPD_LIBRARY libspdlog.dylib)

#set(FMT_INCLUDE_DIR /opt/homebrew/opt/fmt/include)
#set(FMT_LIBRARY_DIR /opt/homebrew/opt/fmt/lib)
#set(FMT_LIBRARY libfmt.dylib)

set(GLFW_INCLUDE_DIR /opt/homebrew/opt/glfw/include)
set(GLFW_LIBRARY_DIR /opt/homebrew/opt/glfw/lib)
set(GLFW_LIBRARY libglfw.dylib)

set(ASSIMP_INCLUDE_DIR /opt/homebrew/opt/assimp/include)
set(ASSIMP_LIBRARY_DIR /opt/homebrew/opt/assimp/lib)
set(ASSIMP_LIBRARY libassimp.dylib)

set(LIBXML2_INCLUDE_DIR /opt/homebrew/opt/libxml2/include)
set(LIBXML2_LIBRARY_DIR /opt/homebrew/opt/libxml2/lib)
set(LIBXML2_LIBRARY libxml2.dylib)

set(LOCAL_INCLUDE_DIR /usr/local/include)

find_package(OpenCV REQUIRED)

# Dependency 관련 변수 설정
set(DEP_INCLUDE_DIRS
#		${SPD_INCLUDE_DIR} ${FMT_INCLUDE_DIR}
		${GLFW_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR}
		${LOCAL_INCLUDE_DIR}
		${OpenCV_INCLUDE_DIRS})
set(DEP_LIBRARY_DIRS
#		${SPD_LIBRARY_DIR} ${FMT_LIBRARY_DIR}
		${GLFW_LIBRARY_DIR} ${ASSIMP_LIBRARY_DIR} ${LIBXML2_LIBRARY_DIR})
set(DEP_LIBRARIES
#		${SPD_LIBRARY} ${FMT_LIBRARY}
		${GLFW_LIBRARY} ${ASSIMP_LIBRARY} ${LIBXML2_LIBRARY} ${OpenCV_LIBS})
