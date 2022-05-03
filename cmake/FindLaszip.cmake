# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindLaszip
# --------
#
# Find the Laszip library (libLaszip).
#
# Imported targets
# ^^^^^^^^^^^^^^^^
#
# This module defines the following :prop_tgt:`IMPORTED` targets:
#
# ``Laszip::Laszip``
#   The Laszip library, if found.
#
# Result variables
# ^^^^^^^^^^^^^^^^
#
# This module will set the following variables in your project:
#
# ``Laszip_FOUND``
#   true if the Laszip headers and libraries were found
# ``Laszip_INCLUDE_DIR``
#   the directory containing the Laszip headers
# ``Laszip_INCLUDE_DIRS``
#   the directory containing the Laszip headers
# ``Laszip_LIBRARIES``
#   Laszip libraries to be linked
#
# Cache variables
# ^^^^^^^^^^^^^^^
#
# The following cache variables may also be set:
#
# ``Laszip_INCLUDE_DIR``
#   the directory containing the Laszip headers
# ``Laszip_LIBRARY``
#   the path to the Laszip library

find_path(Laszip_INCLUDE_DIR "laszip/laszip_api.h")

set(Laszip_NAMES ${Laszip_NAMES} "laszip" "laszip3")
foreach(name ${Laszip_NAMES})
  list(APPEND Laszip_NAMES_DEBUG "${name}" "${name}d")
endforeach()

if(NOT Laszip_LIBRARY)
  find_library(Laszip_LIBRARY_RELEASE NAMES ${Laszip_NAMES})
  find_library(Laszip_LIBRARY_DEBUG NAMES ${Laszip_NAMES_DEBUG})
  include(SelectLibraryConfigurations)
  select_library_configurations(Laszip)
  mark_as_advanced(Laszip_LIBRARY_RELEASE Laszip_LIBRARY_DEBUG)
endif()
unset(Laszip_NAMES)
unset(Laszip_NAMES_DEBUG)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Laszip
                                  REQUIRED_VARS Laszip_LIBRARY Laszip_INCLUDE_DIR
                                  VERSION_VAR Laszip_VERSION_STRING)

if(Laszip_FOUND)
  set(Laszip_LIBRARIES ${Laszip_LIBRARY})
  set(Laszip_INCLUDE_DIRS "${Laszip_INCLUDE_DIR}")

  if(NOT TARGET Laszip::Laszip)
    add_library(Laszip::Laszip UNKNOWN IMPORTED)
    if(Laszip_INCLUDE_DIRS)
      set_target_properties(Laszip::Laszip PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Laszip_INCLUDE_DIRS}")
    endif()
    if(EXISTS "${Laszip_LIBRARY}")
      set_target_properties(Laszip::Laszip PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${Laszip_LIBRARY}")
    endif()
    if(EXISTS "${Laszip_LIBRARY_RELEASE}")
      set_property(TARGET Laszip::Laszip APPEND PROPERTY
        IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(Laszip::Laszip PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
        IMPORTED_LOCATION_RELEASE "${Laszip_LIBRARY_RELEASE}")
    endif()
    if(EXISTS "${Laszip_LIBRARY_DEBUG}")
      set_property(TARGET Laszip::Laszip APPEND PROPERTY
        IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(Laszip::Laszip PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
        IMPORTED_LOCATION_DEBUG "${Laszip_LIBRARY_DEBUG}")
    endif()
  endif()
endif()

mark_as_advanced(Laszip_INCLUDE_DIR Laszip_LIBRARY)
