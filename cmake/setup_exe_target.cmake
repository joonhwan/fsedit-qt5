# Call this macro with the name of a target that is an executable.
# This will put the exe file in builddir/bin and (on visual studio) add the build
# type to the file name for all other than Release.
macro(SETUP_EXE_TARGET targetname)
	set_target_properties(${targetname} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
	if(MSVC)
		set_target_properties(${targetname} PROPERTIES
			DEBUG_OUTPUT_NAME ${targetname}_debug
			MINSIZEREL_OUTPUT_NAME ${targetname}_minsizerel
			RELWITHDEBINFO_OUTPUT_NAME ${targetname}_relwithdebinfo
			PREFIX "../"
		)
	endif(MSVC)
endmacro(SETUP_EXE_TARGET)
