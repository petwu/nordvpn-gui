#[============================================================================[.rst

Git
===

This module provides commands to retrieve information from the version control
system Git.

#]============================================================================]

find_program(GIT_EXECUTABLE
  NAMES git
  REQUIRED
)

#[============================================================================[.rst

Commands
^^^^^^^^

.. cmake:command:: get_git_hash(<output-variable> <REF=HEAD> <SHORT|LONG>)

Gets the commit hash of a specified commit reference. If not specified, ``REF`` 
defaults to ``HEAD``. Set ``SHORT`` or ``LONG`` to get the short or long version
or the commit hash, default is ``SHORT``.

#]============================================================================]
function(get_git_hash _out_var)

  #### parse arguments
  cmake_parse_arguments(_ARG "SHORT;LONG" "RED" "" ${ARGN})

  #### validate arguments
  if(_ARG_SHORT OR NOT _ARG_LONG)
    set(_ARG_SHORT "--short")
  endif()
  if(NOT _ARG_REF)
    set(_ARG_REF "HEAD")
  endif()

  #### call git at configure time to retrieve the information
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse ${_ARG_SHORT} ${_ARG_REF}
    OUTPUT_VARIABLE _result
  )
    string(STRIP "${_result}" _result)

  #### return result
  set(${_out_var} "${_result}" PARENT_SCOPE)

endfunction()
