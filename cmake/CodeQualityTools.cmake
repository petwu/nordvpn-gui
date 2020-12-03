#[============================================================================[.rst

CodeQualityTools
================

This module provides functions to create targets that leverage clang tooling to
improve code quality. Supported tools:

- clang-tidy
- clang-format

This module enables ``CMAKE_EXPORT_COMPILE_COMMANDS`` since the compilation
database (compile_controls.json) is used by *clang-tidy* to determine the
correct compiler flags.

It does NOT set CMAKE_<LANG>_CLANG_TIDY since this would slow down the build
heavily. Also, they don't have any effect on increment build and e.g. QtCreator
does not remember the warnings of the last (incremental) build which is why the
will be lost after each build. Therefore it is more efficient to used the
command below and call the created targets explicitlay.

#]============================================================================]
find_program(CLANG_TIDY_EXECUTABLE
  NAMES clang-tidy
  REQUIRED
)
find_program(CLANG_FORMAT_EXECUTABLE
  NAMES clang-format
  REQUIRED
)

if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

#[============================================================================[.rst

Commands
^^^^^^^^

.. cmake:command:: add_clang_tidy

.. code-block:: cmake

  add_clang_tidy(<target>
    TARGET <target-name>
    SOURCES [<source-files>...]
  )

This command adds two targets to execute the C++ linter *clang-tidy*:

  ``<target>``
    Runs *clang-tidy* on all source files and outputs all suggestions.

  ``<target>-fix``
    Runs *clang-tidy* and automatically fixes all suggestions. Note that this
    might cause some serious changes in your source code. It might be a good
    idea to commit every others changes first, in order to review the changes
    made by *clang-tidy*.

*Options*

  ``TARGET``
    Name of a target whose ``SOURCES`` property will be passed to *clang-tidy*.
    You should make sure, that all source files are attached to the target,
    before calling this command.

  ``SOURCES``
    Manually specify the list of source files to lint.

Either ``TARGET`` or ``SOURCES`` need to be provided. Both may be set as well.

The source files passed to *clang-tidy* and *clang-format* are extracted from
the ``SOURCES`` property of ``<target-name>``. 

#]============================================================================]
function(add_clang_tidy _target)

  #### parse arguments
  set(_single_value_args TARGET)
  set(_multi_value_args SOURCES)
  cmake_parse_arguments(_CT "" "${_single_value_args}" "${_multi_value_args}" ${ARGN})

  #### validate and process arguments
  if(NOT _CT_TARGET AND NOT _CT_SOURCES)
    message(FATAL_ERROR "Either TARGET or SOURCES must be set.")
  endif()
  # get all C/C++ source files of target
  if(_CT_TARGET)
    if(NOT TARGET ${_CT_TARGET})
      message(FATAL_ERROR "${_CT_TARGET} is not a target.")
    else()
      get_target_property(_target_sources ${_CT_TARGET} SOURCES)
      foreach(_source_file IN LISTS _target_sources)
        if(_source_file MATCHES ".*\\.(c|C|cc|cpp|cxx|c\\+\\+)$")
          list(APPEND _cxx_sources ${_source_file})
        endif()
      endforeach()
    endif()
  endif()
  # filter SOURCES to only contain C/C++ files
  if(_CT_SOURCES)
    foreach(_source_file IN LISTS _CT_SOURCES)
      if(_source_file MATCHES ".*\\.(c|C|cc|cpp|cxx|c\\+\\+)$")
        list(APPEND _cxx_sources ${_source_file})
      endif()
    endforeach()
  endif()

  #### add target to call clang-tidy
  add_custom_target(${_target}
    COMMAND ${CLANG_TIDY_EXECUTABLE}
      -p ${CMAKE_BINARY_DIR}/compile_commands.json  # compilation database
      ${_cxx_sources}                               # source files
      -header-filter=.*
      2>&1 > clang-tidy.log
    COMMENT "Linting CXX code with clang-tidy (clang-tidy.log)"
  )

  #### add target to call clang-tidy and fix all suggestions
  add_custom_target(${_target}-fix
    COMMAND ${CLANG_TIDY_EXECUTABLE}
      -p ${CMAKE_BINARY_DIR}/compile_commands.json  # compilation database
      --fix                                         # auto-fix all suggestions
      ${_cxx_sources}                               # source files
      > clang-tidy-fix.log
    COMMENT "Linting CXX code with clang-tidy and fixing all suggestions (clang-tidy-fix.log)"
  )

endfunction()

#[============================================================================[.rst

.. cmake:command:: add_clang_format

.. code-block:: cmake

  add_clang_format(<target>
    FILES [<files>...]
  )

This command creates a ``<target>`` that calls *clang-format* to format the
provided ``<files>``. They should be C/C++ header or source files. Relative
paths in ``<files>`` are interpreted as being relative to
``CMAKE_CURRENT_SOURCE_DIR``.

#]============================================================================]
function(add_clang_format _target)

  #### parse arguments
  set(_multi_value_args FILES)
  cmake_parse_arguments(_CF "" "" "${_multi_value_args}" ${ARGN})

  #### path conversion
  foreach(_file IN LISTS _CF_FILES)
    if(NOT IS_ABSOLUTE _file)
      set(_file ${CMAKE_CURRENT_SOURCE_DIR}/${_file})
    endif()
    if(_file MATCHES "/\\*\\*/") # /**/
      string(REPLACE "**" "" _file "${_file}")
      file(GLOB_RECURSE _file LIST_DIRECTORIES False CONFIGURE_DEPENDS "${_file}")
    elseif(_file MATCHES "/\\*\\.") # /*.
      file(GLOB _file LIST_DIRECTORIES False CONFIGURE_DEPENDS "${_file}")
    endif()
    list(APPEND _CF_FILES_ABS ${_file})
  endforeach()

  #### add target to call clang-format
  add_custom_target(${_target}
    COMMAND ${CLANG_FORMAT_EXECUTABLE}
      -i                    # edit files in place
      --style=file          # format using .clang-format
      --fallback-style=LLVM # in case no .clang-format is found
      --verbose             # list the formatted files
      ${_CF_FILES_ABS}          # input files
    COMMENT "Formatting CXX code with clang-format"
  )

endfunction()
