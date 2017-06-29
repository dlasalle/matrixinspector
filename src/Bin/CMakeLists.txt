macro( addbinary name )
  add_executable(${name}-bin
    ${name}.cpp
  )
  set_target_properties(${name}-bin PROPERTIES OUTPUT_NAME ${name})
  target_link_libraries(${name}-bin ${MATRIXINSPECTOR_LIBS})
  install(TARGETS ${name}-bin
    RUNTIME DESTINATION bin
  )
endmacro()


addbinary(matrixinspector)


