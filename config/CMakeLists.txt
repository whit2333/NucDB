set(exename "nucdb-config")
add_executable(${exename} ${exename}.cxx)

#target_link_libraries(${exename} fieldtest)

install(TARGETS ${exename}
  # IMPORTANT: Add the bar executable to the "export-set"
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)


