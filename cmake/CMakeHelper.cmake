# wrapper for test
macro(H2O_ADD_TEST TARGET_NAME TARGET_SRC)
  if(H2O_BUILD_TEST)
    # ${ARGN} will store the list of source files passed to this function.
    add_definitions(-DH2O_DATA_DIR="${H2O_DATA_DIR}")
    add_executable(${TARGET_NAME} ${TARGET_SRC})
    target_link_libraries(${TARGET_NAME} ${ARGN})
    enable_testing()
    add_test("test/${TARGET_NAME}" ${TARGET_NAME}
             WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "tests/")
  endif()
endmacro(H2O_ADD_TEST)


# Add library function
function(h2o_add_library library_name)
  set(singleValues "")
  set(multipleValues SOURCES PUBLIC_LINKS PRIVATE_LINKS PUBLIC_INCLUDE_DIRS PRIVATE_INCLUDE_DIRS PUBLIC_DEFINITIONS PRIVATE_DEFINITIONS)

  cmake_parse_arguments(LIBRARY "${options}" "${singleValues}" "${multipleValues}" ${ARGN})

  if(NOT library_name)
    message(FATAL_ERROR "You must provide the library name in 'h2o_add_library'")
  endif()

  if(NOT LIBRARY_SOURCES)
    message(FATAL_ERROR "You must provide the library SOURCES in 'h2o_add_library'")
  endif()

  # Generate Windows versioning information
  if(WIN32)
    set(H2O_INSTALL_VERSION_MAJOR ${H2O_VERSION_MAJOR})
    set(H2O_INSTALL_VERSION_MINOR ${H2O_VERSION_MINOR})
    set(H2O_INSTALL_VERSION_REVISION ${H2O_VERSION_REVISION})
    set(H2O_INSTALL_NAME ${library_name})
    set(H2O_INSTALL_LIBRARY 1)
    configure_file(
      "${CMAKE_SOURCE_DIR}/cmake/version.rc.in"
      "${CMAKE_CURRENT_BINARY_DIR}/${library_name}_version.rc"
      @ONLY
    )
    list(APPEND LIBRARY_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/${library_name}_version.rc")
  endif()
  add_library(${library_name} ${LIBRARY_SOURCES})

  target_link_libraries(${library_name}
  PUBLIC ${LIBRARY_PUBLIC_LINKS}
  PRIVATE ${LIBRARY_PRIVATE_LINKS}
  )

  target_include_directories(${library_name}
    PUBLIC $<BUILD_INTERFACE:${H2O_INCLUDE_DIR}>
           $<BUILD_INTERFACE:${generatedDir}>
           $<INSTALL_INTERFACE:include>
           ${LIBRARY_PUBLIC_INCLUDE_DIRS}

    PRIVATE ${LIBRARY_PRIVATE_INCLUDE_DIRS}
  )

  target_compile_definitions(${library_name}
    PUBLIC ${LIBRARY_PUBLIC_DEFINITIONS}
    PRIVATE ${LIBRARY_PRIVATE_DEFINITIONS}
  )

  set_property(TARGET ${library_name}
    PROPERTY FOLDER "H2O"
  )

  set_target_properties(${library_name}
    PROPERTIES SOVERSION ${H2O_VERSION_MAJOR}
    VERSION "${H2O_VERSION_MAJOR}.${H2O_VERSION_MINOR}"
  )

  install(TARGETS ${library_name}
    EXPORT h2o-targets
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
endfunction()

# Add software function
function(h2o_add_software software_name)
  set(options "")
  set(multipleValues SOURCES LINKS INCLUDE_DIRS)

  cmake_parse_arguments(SOFTWARE "${options}" "${singleValues}" "${multipleValues}" ${ARGN})

  if(NOT software_name)
    message(FATAL_ERROR "You must provide the software name in 'h2o_add_software'")
  endif()

  if(NOT SOFTWARE_SOURCES)
    message(FATAL_ERROR "You must provide the software SOURCE in 'h2o_add_software'")
  endif()

  # Generate Windows versioning information
  if(WIN32)
    set(H2O_INSTALL_VERSION_MAJOR ${H2O_VERSION_MAJOR})
    set(H2O_INSTALL_VERSION_MINOR ${H2O_VERSION_MINOR})
    set(H2O_INSTALL_VERSION_REVISION ${H2O_VERSION_REVISION})
    set(H2O_INSTALL_NAME ${software_name})
    set(H2O_INSTALL_LIBRARY 1)
    configure_file(
      "${CMAKE_SOURCE_DIR}/cmake/version.rc.in"
      "${CMAKE_CURRENT_BINARY_DIR}/${software_name}_version.rc"
      @ONLY
    )
    list(APPEND SOFTWARE_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/${software_name}_version.rc")
  endif()

  add_executable(${software_name} ${SOFTWARE_SOURCES})

  target_link_libraries(${software_name}
    PUBLIC ${SOFTWARE_LINKS}
  )

  target_include_directories(${software_name}
    PUBLIC ${SOFTWARE_INCLUDE_DIRS}
  )

  set_target_properties(${software_name}
     PROPERTIES SOVERSION ${H2O_VERSION_MAJOR}
     VERSION "${H2O_VERSION_MAJOR}.${H2O_VERSION_MINOR}"
  )

  install(TARGETS ${software_name}
    RUNTIME
      DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
  if(WIN32)
    install(CODE "
      include(BundleUtilities)
      fixup_bundle(\"${CMAKE_INSTALL_PREFIX}/bin/${software_name}.exe\" \"\" \"\")
    " COMPONENT Runtime)
  endif()
endfunction()