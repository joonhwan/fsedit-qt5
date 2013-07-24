set(headers
	fsconnect.h
)

set(moc_headers
	document.h
	documentmanager.h
)

set(sources
	document.cpp
	documentmanager.cpp
)

qt5_wrap_cpp(moc_generated ${moc_headers})

include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# set(QT_USE_QTTEST TRUE)
# include(${QT_USE_FILE})

source_group("Generated Files" REGULAR_EXPRESSION "(moc_.*cxx|fstest_.*cpp)$")

add_library(model STATIC ${headers} ${moc_headers} ${sources} ${moc_generated})
set_target_properties(model PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(model PROPERTIES PROJECT_LABEL "libmodel")
target_link_libraries(model
  Qt5::Widgets # ${QT_LIBRARIES}
  Qt5::Test
  )

include(fstest)
set(test_libmodel DocumentTest DocumentManagerTest)
#SET(test_libmodel_sources unittest/domoneyparse.h unittest/domoneyparse.cpp)
fstest(test_libmodel LibModel)

include(tr_sources)
add_tr_sources(${sources} ${headers} ${moc_headers})
