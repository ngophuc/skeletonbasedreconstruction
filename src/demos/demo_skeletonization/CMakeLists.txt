include_directories(${CMAKE_SOURCE_DIR}/src/lib
					${CMAKE_SOURCE_DIR}/src/utils
					${Boost_INCLUDE_DIR})

set(source_files main.cpp)

#Déclaration de l'exécutable

set(EXEC_NAME demo_skeletonization)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/")

add_executable(${EXEC_NAME} ${source_files})

target_link_libraries(${EXEC_NAME} ${MATHTOOLS_LIB}
								   ${SHAPE_LIB}
								   ${BOUNDARY_LIB}
								   ${USERINPUT_LIB}
								   ${DISPLAYOCV_LIB}
								   ${ALGORITHM_LIB}
								   ${OpenCV_LIBS})

