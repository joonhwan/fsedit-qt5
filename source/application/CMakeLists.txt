include(setup_exe_target)

set(sources
	main.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/source)

# set(QT_USE_QTMAIN TRUE)
# include(${QT_USE_FILE})

add_executable(fseditor WIN32 MACOSX_BUNDLE ${sources})
target_link_libraries(fseditor
  ui
  Qt5::Widgets # ${QT_LIBRARIES}
  )
setup_exe_target(fseditor)

install(TARGETS fseditor DESTINATION bin)

include(tr_sources)
add_tr_sources(${sources})
