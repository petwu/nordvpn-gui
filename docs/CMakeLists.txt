# --------------------------------------------------------------------------------
# generate documentation with doxygen
# --------------------------------------------------------------------------------
find_package(Doxygen)

if(Doxygen_FOUND)
  configure_file(doxyfile.in ${CMAKE_BINARY_DIR}/doxyfile @ONLY)
  add_custom_target(docs
    COMMAND ${DOXYGEN_EXECUTABLE} doxyfile
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
  )
else()
  add_custom_target(docs
    COMMAND ${CMAKE_COMMAND} -E echo "Error: Doxygen not found" 1>&2
  )
endif()
