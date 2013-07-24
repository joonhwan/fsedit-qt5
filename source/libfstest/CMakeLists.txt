# set(QT_USE_QTTEST TRUE)
# include(${QT_USE_FILE})

add_library(fstest STATIC fstest.h fstest.cpp)
target_link_libraries(fstest
  Qt5::Widgets # ${QT_LIBRARIES}
  )
set_target_properties(fstest
  PROPERTIES PROJECT_LABEL "libfstest"
  )
