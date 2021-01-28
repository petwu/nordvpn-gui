# --------------------------------------------------------------------------------
# .desktop file
# --------------------------------------------------------------------------------
# Desktop Entry Specification:
# https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest
configure_file(nordvpn-gui.desktop.in nordvpn-gui.desktop)
include(GNUInstallDirs)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/nordvpn-gui.desktop
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications
)

# --------------------------------------------------------------------------------
# generate bitmaps using rsvg-convert
# --------------------------------------------------------------------------------
find_program(rsvg_EXECUTABLE
  NAMES rsvg-convert
  REQUIRED
)
# Icons are typically under /usr/share/icons/<theme>. According to the
# freedesktops' Icon Theme Specification, hicolor is the theme where 3rd-party
# apps can install their icons, see:
# https://specifications.freedesktop.org/icon-theme-spec/icon-theme-spec-latest
set(_icon_theme_path ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor)
# input file
set(_icon_svg ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.svg)
# generate png bitmaps of different sizes from the input svg vector graphic
foreach(_size IN ITEMS 16 22 24 32 48 64 128 256 512)
  # output directory and path
  set(_out_dir ${_size}x${_size}/apps)
  set(_out ${_out_dir}/${PROJECT_NAME}.png)
  # command calling rsvg-convert
  add_custom_command(
    OUTPUT ${_out}
    COMMAND ${rsvg_EXECUTABLE}
      -w ${_size}             # png size
      -o ${_out}              # output png
      ${_icon_svg}            # input svg
    COMMENT "Generating bitmap: ${_out}"
  )
  add_custom_target(freedesktop_icon_${_size} ALL DEPENDS ${_out})
  # install the generated png file
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${_out}
    DESTINATION ${_icon_theme_path}/${_out_dir}
  )
endforeach()
# install the svg file
install(FILES ${_icon_svg} DESTINATION ${_icon_theme_path}/scalable/apps)
