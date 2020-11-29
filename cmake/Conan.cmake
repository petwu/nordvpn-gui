#[============================================================================[.rst

Conan
=====

This module provides a CMake wrapper for `Conan <https://conan.io>`_, a C/C++
package manager. This module is just responsible for downloading the actual
module provided by Conan and including it. For documentation about the
available command see `here <https://github.com/conan-io/cmake-conan>`_.

.. note::

  When upgrading the download URL below to a new version, always use a tagged
  release and not the one from the master or development branch. This ensures
  controlled upgrades, reproducible builds and prevents unexpected behavior due
  to possible bugs in the master/development version of the module.

#]============================================================================]
if(NOT EXISTS ${CMAKE_BINARY_DIR}/conan.cmake)
  file(DOWNLOAD
    "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.15/conan.cmake"
    ${CMAKE_BINARY_DIR}/conan.cmake
  )
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)
