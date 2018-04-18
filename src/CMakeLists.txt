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
  add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
  #set(sources "${sources}; GUI/App.rc")
else()
  set(MATH_LIBS m)
endif()

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
  ${MATH_LIBS})

add_subdirectory("Bin")

if (DEFINED TESTS AND NOT TESTS EQUAL 0)
  add_subdirectory("Test")
endif()
