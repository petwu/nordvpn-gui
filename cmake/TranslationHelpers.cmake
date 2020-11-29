#[============================================================================[.rst

TranslationHelpers
==================

This module provides helper command to deal with Qt translation files.

Commands
^^^^^^^^

.. cmake:command:: target_translation_sources

For some reason qt5_add_translation does not work properly when used in a
subdirectory. There also isn't some AUTO... variable like AUTORCC for .qrc
files that can be set to ``ON`` so that CMake handles the translation files
automatically.

This function helps out by calling ``lrelease``, the Qt command line tool for
compile ``.ts`` translation files to ``.qm`` files, and creating custom targets
for them.

.. code-block:: cmake

  target_translation_sources(<target> [<ts-file>...])

Simple pass the target name as first argument followed by a list of translation
files.

#]============================================================================]
find_program(lrelease_EXECUTABLE lrelease)
mark_as_advanced(lrelease_EXECUTABLE)
function(target_translation_sources _target)
  # validate args
  if(NOT TARGET ${_target})
    message(FATAL_ERROR "${_target} is not a target name.")
  endif()
  # process args
  foreach(_ts_file IN ITEMS ${ARGN})
    # replace .ts extension with .qm to get the output file
    string(REGEX REPLACE "\\.[^\\.]+$" ".qm" _qm_file "${_ts_file}")
    # ensure absolute paths
    if(NOT IS_ABSOLUTE ${_ts_file})
      set(_ts_file "${CMAKE_CURRENT_SOURCE_DIR}/${_ts_file}")
    endif()
    if(NOT IS_ABSOLUTE ${_qm_file})
      set(_qm_file "${CMAKE_CURRENT_BINARY_DIR}/${_qm_file}")
    endif()
    # validate that file exists
    if(NOT EXISTS ${_ts_file})
      message(FATAL_ERROR "${_ts_file}: no such file or directory")
    endif()
    # custom command that calls lupdate to compile the translation file
    add_custom_command(
      OUTPUT ${_qm_file}
      DEPENDS ${_ts_file}
      COMMAND ${lrelease_EXECUTABLE} ${_ts_file} -qm ${_qm_file}
    )
    # add a target for the output .qm file and set it as dependency for _target
    string(REGEX REPLACE "[^a-zA-Z0-9_]" "_" _qm_target ${_qm_file})
    add_custom_target(${_qm_target} DEPENDS ${_qm_file})
    add_dependencies(${_target} ${_qm_target})
  endforeach()
endfunction()
