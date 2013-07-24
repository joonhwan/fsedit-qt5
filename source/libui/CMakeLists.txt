set(headers
)

set(moc_headers
	closedialog.h
	editwidget.h
	mainwindow.h
	translation.h
)

set(sources
	closedialog.cpp
	editwidget.cpp
	mainwindow.cpp
	translation.cpp
)

set(forms
	closedialog.ui
	helpdialog.ui
	mainwindow.ui
)

set(resources
	libui.qrc
)

qt5_wrap_cpp(moc_generated ${moc_headers})
qt5_wrap_ui(forms_generated ${forms})
qt5_add_resources(rc_generated ${resources})

include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

source_group("Generated Files" REGULAR_EXPRESSION "(moc_.*cxx|ui_.*h|qrc_.*cxx)$")
source_group("UI Files" REGULAR_EXPRESSION ".*.ui$")
source_group("RC Files" REGULAR_EXPRESSION ".*.qrc$")

# include(${QT_USE_FILE})

add_library(ui ${headers} ${moc_headers} ${sources} ${forms} ${moc_generated} ${forms_generated} ${rc_generated})
set_target_properties(ui PROPERTIES PROJECT_LABEL "libui")
target_link_libraries(ui
  model
  modelcommands
  Qt5::Widgets # ${QT_LIBRARIES}
  )

include(tr_sources)
add_tr_sources(${sources} ${headers} ${moc_headers} ${forms})
