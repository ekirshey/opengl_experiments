message("--- ${PROJECT_SOURCE_DIR}")

include_directories(
	${SDL2_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
	${EXTERNAL_DEPENDS}
	${HELPERS}
	${PROJECT_SOURCE_DIR}/experiments/lighting/
)

file( GLOB SRCS 
	#Includes
	${HELPERS}
	${PROJECT_SOURCE_DIR}/experiments/lighting/*.h

	#Source
	${PROJECT_SOURCE_DIR}/experiments/lighting/*.cpp
)

add_executable(LightingTest
	${SRCS} 
)

target_link_libraries (LightingTest LINK_PUBLIC ${SDL2_LIBRARY} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY})