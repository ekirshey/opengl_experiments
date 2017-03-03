message("--- ${PROJECT_SOURCE_DIR}")

include_directories(
	${SDL2_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
	${EXTERNAL_DEPENDS}
	${HELPERS}
	${PROJECT_SOURCE_DIR}/experiments/modelloading/
)

file( GLOB SRCS 
	#Includes
	${HELPERS}
	${PROJECT_SOURCE_DIR}/experiments/modelloading/*.h

	#Source
	${PROJECT_SOURCE_DIR}/experiments/modelloading/*.cpp
)

add_executable(ModelLoading
	${SRCS} 
)

target_link_libraries (ModelLoading LINK_PUBLIC ${SDL2_LIBRARY} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY})