macro( addbinary name )
  add_executable(${name}-bin
    ${name}.cpp
  )
  set_target_properties(${name}-bin PROPERTIES OUTPUT_NAME ${name})
  target_link_libraries(${name}-bin ${MATRIXINSPECTOR_LIBS})
  install(TARGETS ${name}-bin
    RUNTIME DESTINATION bin
  )
  if (MSVC)
    set_property(TARGET name APPEND_STRING
        PROPERTY LINK_FLAGS " /NODEFAULTLIB:msvcrtd /NODEFAULTLIB:msvcprtd")
  endif()
endmacro()


addbinary(matrixinspector)


