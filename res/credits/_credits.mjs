/**
 * This modules is imported by src/ui/windows/credits.qml in order to display
 * credits/attribution for open source software (OSS) used by this project.
 * 
 * How to add another attribution:
 * 
 *    1.) All a .mjs (JavaScript Module) with the same content as the others.
 *    2.) Add the .mjs file to ../res.qrc.
 *    3.) Import the added .mjs here.
 *    4.) Add the imported module to the exported array.
 */

import flagkit from 'flagkit.mjs'
import fontawesome from 'fontawesome.mjs'
import libcurl from 'libcurl.mjs'
import nlohmannJson from 'nlohmann-json.mjs'
import qt from 'qt.mjs'
import simpleWorldMap from 'simple-world-map.mjs'

export default [
  flagkit,
  fontawesome,
  libcurl,
  nlohmannJson,
  qt,
  simpleWorldMap,
]
