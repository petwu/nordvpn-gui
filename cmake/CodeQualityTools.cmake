#[============================================================================[.rst

CodeQualityTools
================

This module provides functions to create targets that leverage clang tooling to
improve code quality. Supported tools:

- clang-tidy
- clang-format
- include-what-you-use (iwyu)

This module enables ``CMAKE_EXPORT_COMPILE_COMMANDS`` since the compilation
database (compile_controls.json) is used by various tools to determine the
correct compiler flags.

It does NOT set CMAKE_<LANG>_CLANG_TIDY since this would slow down the build
heavily. Also, they don't have any effect on increment build and e.g. QtCreator
does not remember the warnings of the last (incremental) build which is why the
will be lost after each build. Therefore it is more efficient to used the
command below and call the created targets explicitlay.

#]============================================================================]

#### determine the number of logical processors the build machine has (for iwyu)
include(ProcessorCount)
ProcessorCount(CPU_COUNT)
if(NOT CPU_COUNT)
  set(CPU_COUNT 8) # just a reasonable assumption about modern computers
endif()

#### create the log directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/log)

#### find all code quality tools (linter, formatter, ...)
# clang-tidy
find_program(CLANG_TIDY_EXECUTABLE
  NAMES clang-tidy
  REQUIRED
)
# clang-format
find_program(CLANG_FORMAT_EXECUTABLE
  NAMES clang-format
  REQUIRED
)
# iwyu is a executable but utility tools are python scripts
find_package (Python3 COMPONENTS Interpreter)
# ieyu compilation database driver
find_program(IWYU_TOOL_PY
  NAMES iwyu_tool.py
  REQUIRED
)
# ieyu auto fixer
find_program(IWYU_FIX_PY
  NAMES fix_includes.py
  REQUIRED
)

#### create compilation database
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
      > log/clang-tidy.log 2>&1
    COMMENT "Linting CXX code with clang-tidy (log/clang-tidy.log)"
  )

  #### add target to call clang-tidy and fix all suggestions
  add_custom_target(${_target}-fix
    COMMAND ${CLANG_TIDY_EXECUTABLE}
      -p ${CMAKE_BINARY_DIR}/compile_commands.json  # compilation database
      --fix                                         # auto-fix all suggestions
      ${_cxx_sources}                               # source files
      > log/clang-tidy-fix.log 2>&1
    COMMENT "Linting CXX code with clang-tidy and fixing all suggestions (log/clang-tidy-fix.log)"
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
      ${_CF_FILES_ABS}      # input files
      > log/clang-format.log 2>&1
    COMMENT "Formatting CXX code with clang-format (log/clang-format.log)"
  )

endfunction()

#[============================================================================[.rst

.. cmake:command:: add_iwyu

.. code-block:: cmake

  add_iwyu(<target>
    [MAPPING_FILE [<files>...]]
  )

This command creates a ``<target>`` that calls the *include-what-you-use (iwyu)*
tool. *iwyu* analyzes the used symbols in a file to determine the list of header
files that export those symbols. It then compares this list with the list of
actually included headers and suggests fixes for possible violations.
This command creates a second target named ``<target>-fix`` that automatically
applies all fixes.

*Options*

  ``MAPPING_FILE``
    Optional `mapping file`_ for *iwyu*.

.. mapping file:
  https://github.com/include-what-you-use/include-what-you-use/blob/master/docs/IWYUMappings.md

.. note::

  You probably have to use a specific *iwyu* version depending on your version
  of clang. There might be a possibility, you have to `build iwyu from source`_.

.. build iwyu from source:
  https://github.com/include-what-you-use/include-what-you-use#how-to-build

#]============================================================================]
function(add_iwyu _target)

  #### parse arguments
  set(_single_value_args MAPPING_FILE)
  cmake_parse_arguments(_IWYU "" "${_single_value_args}" "" ${ARGN})

  #### validate args
  if(_IWYU_MAPPING_FILE)
    if(NOT IS_ABSOLUTE ${_IWYU_MAPPING_FILE})
      set(_IWYU_MAPPING_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${_IWYU_MAPPING_FILE}")
    endif()
    set(_IWYU_MAPPING_FILE -Xiwyu --mapping_file=${_IWYU_MAPPING_FILE})
  endif()
  message(${_IWYU_MAPPING_FILE})

  #### analyze target
  add_custom_target(${_target}
    COMMAND Python3::Interpreter
      ${IWYU_TOOL_PY}                              # iwyu python script working off compile_commands.json
      -p ${CMAKE_BINARY_DIR}/compile_commands.json # compilation database
      -o iwyu                                      # output format (not output file!)
      -j ${CPU_COUNT}                              # number of parallel jobs
      --                                           # -- iwyu options --
      ${_IWYU_MAPPING_FILE}                        # -Xiwyu --mapping_file=... (optional)
      -Xiwyu --no_fwd_decls                        # use #includes, no forward declarations
      > log/iwyu.log 2>&1
    COMMENT "Analyzing CXX headers with include-what-you-use (log/iwyu.log)"
  )

  #### auto-fix target
  add_custom_target(${_target}-fix
    COMMAND Python3::Interpreter
      ${IWYU_FIX_PY}       # iwyu python script fixing all suggestions
      --nosafe_headers     # remove unused includes (default is to keep them)
      < log/iwyu.log       # input
      # > log/iwyu-fix.log # <-- for some reason this does not work as command
                           #     despite it works when executed as shell command
    COMMENT "Fixing CXX headers with include-what-you-use"
  )

  # log/iwyu.log needs to be available
  add_dependencies(${_target}-fix ${_target})

endfunction()
