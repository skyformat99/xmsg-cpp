find_package(Doxygen)

if(DOXYGEN_FOUND)

  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )

  option(DOXYGEN_WITH_CPPREFERENCE_LINKS
    "Link C++ standard library classes to http://cppreference.com documentation."
    TRUE
  )

  set(DOXYGEN_TAGFILES)
  if(DOXYGEN_WITH_CPPREFERENCE_LINKS)
    # download Doxygen tags from cppreference.com
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/cppreference-doxygen-web.tag.xml
      COMMAND ${CMAKE_COMMAND}
      -D DEST_DIR=${CMAKE_CURRENT_BINARY_DIR}
      -P ${CMAKE_CURRENT_SOURCE_DIR}/get_cppreference_tags.cmake
      COMMENT "Getting cppreference.com doxygen tags...")
    add_custom_target(get-ccpreference-tags DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/cppreference-doxygen-web.tag.xml)
    add_dependencies(doc get-ccpreference-tags)
    set(DOXYGEN_TAGFILES
      "${DOXYGEN_TAGFILES} \"${CMAKE_CURRENT_BINARY_DIR}/cppreference-doxygen-web.tag.xml=http://en.cppreference.com/w/\"")
  endif()

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

endif()
