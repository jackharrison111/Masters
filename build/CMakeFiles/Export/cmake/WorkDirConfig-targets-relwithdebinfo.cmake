#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "WorkDir::DiTauLib" for configuration "RelWithDebInfo"
set_property(TARGET WorkDir::DiTauLib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(WorkDir::DiTauLib PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libDiTauLib.so"
  IMPORTED_SONAME_RELWITHDEBINFO "libDiTauLib.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS WorkDir::DiTauLib )
list(APPEND _IMPORT_CHECK_FILES_FOR_WorkDir::DiTauLib "${_IMPORT_PREFIX}/lib/libDiTauLib.so" )

# Import target "WorkDir::DiTau" for configuration "RelWithDebInfo"
set_property(TARGET WorkDir::DiTau APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(WorkDir::DiTau PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libDiTau.so"
  IMPORTED_NO_SONAME_RELWITHDEBINFO "TRUE"
  )

list(APPEND _IMPORT_CHECK_TARGETS WorkDir::DiTau )
list(APPEND _IMPORT_CHECK_FILES_FOR_WorkDir::DiTau "${_IMPORT_PREFIX}/lib/libDiTau.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
