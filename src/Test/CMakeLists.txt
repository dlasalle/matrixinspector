function(setup_test test_file)
  file(GLOB source ${test_file}.cpp)
	add_executable(${test_file} ${source})
	target_link_libraries(${test_file} matrixinspector ${MATRIXINSPECTOR_LIBS})
	add_test(NAME ${test_file} COMMAND ${test_file})
endfunction()

include_directories(.)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)

setup_test(ReorderTest)
setup_test(StatsTest)
setup_test(SortTest)
setup_test(StringTest)
