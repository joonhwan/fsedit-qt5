# Translations
# Get translations directory
get_target_property(_qmake Qt5::qmake LOCATION)
execute_process(COMMAND ${_qmake} -query
  QT_INSTALL_TRANSLATIONS OUTPUT_VARIABLE QT_TRANSLATIONS_DIR OUTPUT_STRIP_TRAILING_WHITESPACE
  )

# Call this function with all sources that can have strings to translate
function(ADD_TR_SOURCES)
	get_property(translation_sources GLOBAL PROPERTY translation_sources)
	foreach(file ${ARGN})
		list(APPEND translation_sources ${CMAKE_CURRENT_SOURCE_DIR}/${file})
	endforeach(file ${ARGN})
	set_property(GLOBAL PROPERTY translation_sources ${translation_sources})
endfunction(ADD_TR_SOURCES)

# Call this to retrieve the list of sources to translate
macro(GET_TR_SOURCES _ts_source_list)
	get_property(translation_sources GLOBAL PROPERTY ${_ts_source_list})
endmacro(GET_TR_SOURCES _ts_source_list)

# The only difference here is that we use MAIN_DEPENDENCY to get the source file in the sources list
macro(MY_QT5_ADD_TRANSLATION _qm_files)
  foreach (_current_FILE ${ARGN})
    get_filename_component(_abs_FILE ${_current_FILE} ABSOLUTE)
    get_filename_component(qm ${_abs_FILE} NAME_WE)
    get_source_file_property(output_location ${_abs_FILE} OUTPUT_LOCATION)
    if(output_location)
      file(MAKE_DIRECTORY "${output_location}")
      set(qm "${output_location}/${qm}.qm")
    else(output_location)
      set(qm "${CMAKE_CURRENT_BINARY_DIR}/${qm}.qm")
    endif(output_location)

    add_custom_command(OUTPUT ${qm}
       COMMAND ${Qt5_LRELEASE_EXECUTABLE}
       ARGS ${_abs_FILE} -qm ${qm}
       MAIN_DEPENDENCY ${_abs_FILE}
    )
    set(${_qm_files} ${${_qm_files}} ${qm})
  endforeach (_current_FILE)
endmacro(MY_QT5_ADD_TRANSLATION)
