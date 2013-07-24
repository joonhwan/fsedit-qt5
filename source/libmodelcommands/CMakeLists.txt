set(headers
	editcommands.h
	editorinterface.h
)

set(sources
	editcommands.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/source)

# INCLUDE(${QT_USE_FILE})

add_library(modelcommands STATIC ${headers} ${sources})
set_target_properties(modelcommands PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(modelcommands PROPERTIES PROJECT_LABEL "libmodelcommands")
target_link_libraries(modelcommands
  model
  Qt5::Widgets # ${QT_LIBRARIES}
  )

include(tr_sources)
add_tr_sources(${sources} ${headers})
