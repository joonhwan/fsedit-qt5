include(tr_sources)

get_tr_sources(translation_sources)

set(ts_files
  fseditor_da.ts
  fseditor_en.ts
  )

# Enable this to only build the .qm files
# With this, it's safe to run "rebuild"
qt5_add_translation(qm_files ${ts_files})

# Enable this to update the .ts file.
# WARNING: This will delete current .ts file contents on a make clean
qt5_create_translation(qm_files ${translation_sources} ${ts_files})

# Get all the Qt translations
foreach(ts_file ${ts_files})
  # Figure out the filename if the Qt .ts and .qm files
  string(REPLACE fseditor qt qt_ts_file ${ts_file})
  string(REPLACE .ts .qm qt_qm_file ${qt_ts_file})
  string(REGEX REPLACE ".*_([a-zA-Z][a-zA-Z])\\.ts" "\\1" ts_lang ${ts_file})
  message("ts_lang = ${ts_lang}")
  if(ts_lang STREQUAL "en")
	message("skip passing qt_en.qm...")
  else()
	if (EXISTS ${QT_TRANSLATIONS_DIR}/${qt_qm_file})
	  # Copy the .qm file into place
	  add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${qt_qm_file}"
		COMMAND cmake -E copy "${QT_TRANSLATIONS_DIR}/${qt_qm_file}"
		"${CMAKE_CURRENT_BINARY_DIR}/${qt_qm_file}"
		DEPENDS "${QT_TRANSLATIONS_DIR}/${qt_qm_file}"
		)
	  list(APPEND qm_files "${CMAKE_CURRENT_BINARY_DIR}/${qt_qm_file}")
	else()
	  # This will try adding the qt_XY.ts files
	  string(REPLACE fseditor qt qt_ts_file ${ts_file})
	  list(APPEND qt_ts_files ${QT_TRANSLATIONS_DIR}/${qt_ts_file})
	endif()
  endif()
endforeach(ts_file ${ts_files})

qt5_add_translation(qt_qm_files ${qt_ts_files})

# Add the translations files to the file list
source_group("Translation Files" REGULAR_EXPRESSION ".*.ts$")

# Create the translations build target
add_custom_target(translations DEPENDS ${qm_files} ${qt_qm_files})
target_link_libraries(translations
  Qt5::Linguist
  )
# Make fseditor depend on translations
add_dependencies(fseditor translations)

install(FILES ${qm_files} ${qt_qm_files} DESTINATION translations)
