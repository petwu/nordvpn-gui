const parseDronePipelineLog = (log, allSteps) => {
  const parsedLog = {}
  let prevStep = null
  let prevLineNum = -1
  log.split(/\r?\n/).forEach((line) => {
    const matches = /\[(.+):(\d+)\]\s*(.*)/.exec(line)
    if (matches && matches.length == 4) {
      const step = matches[1].trim()
      const lineNum = matches[2].trim()
      const lineText = matches[3].trim()
      let lineClass = lineText.startsWith('+ ') ? 'ms-text-info cmd' : 'out'
      if (parsedLog[step] === undefined) {
        parsedLog[step] = { status: 0, log: '' }
      }
      if (/[^\w]warn(ing)?[\.,:-_\s\]\)\}]/.test(line)) {
        parsedLog[step].status = 1
        lineClass = 'ms-text-warning wrn'
      }
      parsedLog[
        step
      ].log += `<span data-line='${lineNum}' class='log-line ${lineClass}'>${lineText}</span><br />`
      prevStep = step
      prevLineNum = lineNum
    } else {
      if (prevStep != 0 && /exit code [1-9][0-9]*/.test(line)) {
        parsedLog[prevStep].status = 2
        parsedLog[
          prevStep
        ].log += `<span data-line='${++prevLineNum}' class='log-line ms-text-danger err'>${line}</span><br />`
      }
    }
  })
  allSteps.forEach((step) => {
    if (parsedLog[step] === undefined) {
      parsedLog[step] = {
        status: -1,
        log: `<i class='fas fa-forward'></i> skipped`,
      }
    }
  })
  return parsedLog
}

const parseLog = (log) => {
  let parsedLog = ``
  log.split(/\r?\n/).forEach((line, i) => {
    let lineClass = ''
    if (/[^\w]warn(ing)?[\.,:-_\s\]\)\}]/.test(line)) {
      lineClass = 'ms-text-warning wrn'
    } else if (/[^\w]err(or)?[\.,:-_\s\]\)\}]/.test(line)) {
      lineClass = 'ms-text-danger err'
    }
    parsedLog += `<span data-line='${i}' class='log-line ${lineClass}'>${line}</span><br />`
  })
  return parsedLog
}

const app = new Vue({
  el: '#app',
  data() {
    const _pipelineLog = parseDronePipelineLog(pipelineLog, pipelineSteps)
    const firstStep = Object.keys(_pipelineLog)[0]
    const firstLog = Object.keys(logs)[0]
    const _logs = {}
    Object.keys(logs).map((key) => {
      _logs[key] = parseLog(logs[key])
    })
    const url = new URL(document.location)
    this.setTheme(url.searchParams.get('theme'))
    return {
      pipelineLog: _pipelineLog,
      logs: _logs,
      docIndex,
      activeTab: 'pipeline',
      selectedPipelineStep: firstStep,
      selectedPipelineStepLog: _pipelineLog[firstStep],
      selectedLog: firstLog,
      selectedLogContent: _logs[firstLog],
    }
  },
  methods: {
    onPipelineStepSelected(step) {
      this.selectedPipelineStep = step
      this.selectedPipelineStepLog = this.pipelineLog[step]
    },
    onLogFileSelected(logFile) {
      this.selectedLog = logFile
      this.selectedLogContent = this.logs[logFile]
    },
    onTabSelected(tab) {
      this.activeTab = tab
    },
    onThemeToggle() {
      const htmlTag = document.querySelector('html')
      let theme = htmlTag.dataset.theme === 'dark' ? 'light' : 'dark'
      this.setTheme(theme)
    },
    setTheme(theme) {
      if (!['light', 'dark'].includes(theme)) {
        theme = 'dark' // default theme
      }
      document.querySelector('html').dataset.theme = theme
      const url = new URL(document.location)
      url.searchParams.set('theme', theme)
      // document.location.search = url.search <-- would cause a reload
      window.history.replaceState(null, null, url.search)
    },
    statusToColor(status) {
      if (typeof status !== 'number') return ''
      if (status < 0) {
        return 'info'
      } else if (status === 0) {
        return 'success'
      } else if (status === 1) {
        return 'warning'
      } else {
        return 'danger'
      }
    }
  },
})
