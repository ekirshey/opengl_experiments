message("--- ${PROJECT_SOURCE_DIR}")

include_directories(
	${SDL2_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
	${EXTERNAL_DEPENDS}
	${HELPERS}
	${PROJECT_SOURCE_DIR}/experiments/camera/
)

file( GLOB SRCS 
	#Includes
	${HELPERS}
	${PROJECT_SOURCE_DIR}/experiments/camera/*.h

	#Source
	${PROJECT_SOURCE_DIR}/experiments/camera/*.cpp
)

add_executable(CameraTest
	${SRCS} 
)

target_link_libraries (CameraTest LINK_PUBLIC ${SDL2_LIBRARY} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY})