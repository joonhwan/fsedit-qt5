# Before calling this macro, set ${testproject}_tests to all the test cases.
# Optional: Set ${testproject}_sources to the extra files that should be compiled in
# Optional: Set ${testproject}_libs to the extra libraries that should be linked in
macro(FSTEST testproject testname)
	set(fstest_include_string "#include <libfstest/fstest.h>\n")
	set(fstest_code_string "")
	foreach(test ${${testproject}})
		# Add the filenames for this test
		string(TOLOWER ${test} filename)
		set(headerfile unittest/${filename}.h)
		set(cppfile unittest/${filename}.cpp)
		set(${testproject}_headers ${${testproject}_headers} ${headerfile})
		set(${testproject}_sources ${${testproject}_sources} ${cppfile})

		# Add the lines to the generated file
		set(fstest_include_string "${fstest_include_string}#include \"${headerfile}\"\n")
		set(fstest_code_string "${fstest_code_string}FSTest::TestFactory<${test}> testFactory${test}(\"${test}\");\n")
	endforeach(test)

	# Create a file that defines the entire test suite
	set(fstest_filename ${CMAKE_CURRENT_BINARY_DIR}/fstest_${testproject}.cpp)
	file(WRITE ${fstest_filename} "${fstest_include_string}${fstest_code_string}")

	qt5_wrap_cpp(${testproject}_sources ${${testproject}_headers})

	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	add_executable(${testproject} ${${testproject}_sources} ${${testproject}_headers} ${fstest_filename})
	target_link_libraries(${testproject} ${${testproject}_libs} model fstest ${QT_LIBRARIES})

	foreach(test ${${testproject}})
		add_test(${testname}-${test} ${testproject} -dir=${CMAKE_SOURCE_DIR}/testfiles -test=${test})
	endforeach(test ${modeltests})
endmacro(FSTEST)
