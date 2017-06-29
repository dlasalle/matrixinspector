macro( addsubmodule name )
  include_directories(${name}/)
  file(GLOB ${name}_sources ${name}/*.cpp)
  message("Adding submodule ${name} : ${${name}_sources}")
  set(sources "${sources};${${name}_sources}")
endmacro()

include_directories(.)

addsubmodule(Data)
addsubmodule(GUI)
addsubmodule(View)
addsubmodule(Operations)
addsubmodule(Utilities)


file(GLOB base_sources *.cpp)


# add resources on windows
if (WIN32)
  set(sources "${sources}; GUI/App.rc")
endif(WIN32)

# library
add_library(matrixinspector STATIC
  ${base_sources}
  ${sources}
)


# configure libraries
list(APPEND MATRIXINSPECTOR_LIBS 
  matrixinspector
  wildriver
  ${wxWidgets_LIBRARIES}
  ${OPENGL_LIBRARY}
  m)

add_subdirectory("Bin")

if (DEFINED TESTS AND NOT TESTS EQUAL 0)
  add_subdirectory("Test")
endif()
