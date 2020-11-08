/*!
 * Translating a server name ain't that easy, since there are several possible combinations:
 *
 *  Format                                      Example
 *  ------------------------------------------------------------------------------
 *  <country> <#>                               Germany #123
 *  <countryA> - <countryB> <#>                 Switzerland - Netherlands #456
 *  <country> <technology> <#>                  Switzerland Onion #2
 *  <countryA> - <countryB> <technology> <#>    Switzerland - Netherlands SOCKS #4
 *
 * There can't be a translation for the complete string, since there are too many possible
 * combinations, especially with the number at the end.
 * This function therefore tries to split the server name s into these parts and translate
 * the <country> parts seperately. <technology> and <#> won't get translated but are kept
 * in the return value.
 */
function qsTrServer(s) {
    // extract and remove the number suffix (<#>)
    const num = s.trim().match(/\s+\#\d+$/)[0]
    s = s.substr(0, s.length - num.length)
    // translate each part seperately
    const parts = s.split(' - ')
    const translatedParts = []
    parts.forEach(p => {
                      let up = p.trim()
                      let tp = qsTranslate('Country', up)
                      let tries = 3 // keep this number low, since for English->English this would get slow otherwise
                      while (tp.length > 0 && tp === up && tries > 0) {
                          // while untranslated part 'up' equals translated part 'tp':
                          // - remove the last word of 'up'
                          // - translate 'up'
                          // try at max 3 times
                          tries--
                          up = up.replace(/\s+[^\s]+$/, '')
                          tp = qsTranslate('Country', up)
                      }
                      if (tries === 0 && tp === up)
                        translatedParts.push(p) // no translation found (or English->English translation)
                      else
                        translatedParts.push(tp + up.substr(tp.length)) // translation found
                  })
    return translatedParts.join(' - ') + num // re-join parts and re-append number suffix
}
