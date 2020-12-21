# --------------------------------------------------------------------------------
# copy resources that are not compiled in by rcc (qrc)
# --------------------------------------------------------------------------------
file(COPY data/ DESTINATION ${CMAKE_BINARY_DIR}/${RESOURCES_DIR_NAME})
install(DIRECTORY data/ DESTINATION ${RESOURCES_DIR_NAME})

# --------------------------------------------------------------------------------
# source files
# --------------------------------------------------------------------------------
target_sources(${PROJECT_NAME} PRIVATE
  res.qrc
  ${CMAKE_CURRENT_BINARY_DIR}/translations/translations.qrc
)

# --------------------------------------------------------------------------------
# translations
# --------------------------------------------------------------------------------
include(TranslationHelpers)
target_translation_sources(${PROJECT_NAME}
  translations/de.ts
  translations/en.ts
)
configure_file(
  translations/translations.qrc.in
  translations/translations.qrc
  COPYONLY
)
add_custom_target(lupdate
  COMMAND ${Qt5_LUPDATE_EXECUTABLE}
    -source-language en_US                          # for new files
    -no-obsolete                                    # drop obsolete strings
    -recursive src                                  # input
    -ts ${CMAKE_CURRENT_LIST_DIR}/translations/*.ts # output
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Updating translations res/translations/*.ts"
)

# --------------------------------------------------------------------------------
# desktop/start menu entry + icons
# --------------------------------------------------------------------------------
add_subdirectory(freedesktop)
