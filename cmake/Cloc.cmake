#[============================================================================[.rst

Cloc
====

This module provides a command to create a target for
`cloc <https://github.com/AlDanial/cloc>`_ (count lines of code) by AlDaniel.

Commands
^^^^^^^^

.. cmake:command:: add_cloc

.. code-block::

  add_cloc(<target>
    SOURCES <sources>
    [ARGS <args>]
  )

This command creates a new ``<target>`` that calls cloc with the given
``<args>`` (optional) and for the given ``<sources>``. The sources may include
directories and files.

#]============================================================================]
find_program(CLOC_EXECUTABLE NAMES cloc)

function(add_cloc _target)

  #### parse arguments
  set(_multi_value_args SOURCES ARGS)
  cmake_parse_arguments(_CLOC "" "" "${_multi_value_args}" ${ARGN})

  #### validate arguments
  # paths must be absolute
  foreach(_path IN LISTS _CLOC_SOURCES)
    if(IS_ABSOLUTE "${_path}")
      list(APPEND _sources "${_path}")
    else()
      list(APPEND _sources "${CMAKE_CURRENT_SOURCE_DIR}/${_path}")
    endif()
  endforeach()

  #### add target
  if(CLOC_EXECUTABLE MATCHES ".*-NOTFOUND")
    # dummy target if cloc is not available
    add_custom_target(${_target}
      COMMAND ${CMAKE_COMMAND} -E echo "error: cloc is not installed"
    )
  else()
    # actual target calling cloc
    add_custom_target(${_target}
      COMMAND ${CLOC_EXECUTABLE} ${_CLOC_ARGS} ${_sources}
    )
  endif()

endfunction()
