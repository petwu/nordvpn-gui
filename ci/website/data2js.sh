#!/bin/bash
set -u

################################################################################
#
# This script is intended to be called by pipeline.sh.
# The script collects data and puts it in a JavaScript file (data.js) to make it
# available to local HTML files, since reading local files with JavaScript is
# is not allowed by browsers for security issues. But including a script file is
# possible.
#
# The following variables have to be set:
#
# - ARTIFACTS_DIR       Path to the directory containing the pipeline artifacts.
# - PIPELINE_LOG_FILE   Path to the file containing the log of the pipeline run.
#
################################################################################

# ------------------------------------------------------------------------------
# go into artifacts dir
cd $ARTIFACTS_DIR

# ------------------------------------------------------------------------------
# create an empty JavaScript file to put all data into
DATA_FILE="data.js"
rm -f $DATA_FILE
touch $DATA_FILE
log "creating data file for CI website: $ARTIFACTS_DIR/$DATA_FILE"

# ------------------------------------------------------------------------------
# pipeline log
echo -n "const pipelineLog = \`" >> $DATA_FILE
PIPELINE_LOG=$( cat $PIPELINE_LOG_FILE )
PIPELINE_LOG=${PIPELINE_LOG/\`/\\\`}
echo "$PIPELINE_LOG" >> $DATA_FILE
echo "\`" >> $DATA_FILE

# ------------------------------------------------------------------------------
# get all steps in case the pipeline failed and the log hence does not contain
# all steps
cd - > /dev/null
# extract "steps: ..." part
ALL_PIPELINE_STEPS=$( grep -Pzo "(?s)steps:.*(\n[^\s].|\n--|\n\.\.)" .drone.yml | tr '\0' '\n' )
# extract "- name: ..." of each step
ALL_PIPELINE_STEPS=$( echo "$ALL_PIPELINE_STEPS" | grep "^- name: .*" )
# remove "-name: " to only have the step names
ALL_PIPELINE_STEPS=${ALL_PIPELINE_STEPS//- name: /}
# split string into array of lines
IFS=$'\n' ALL_PIPELINE_STEPS=($ALL_PIPELINE_STEPS)
cd $ARTIFACTS_DIR
# write the steps into the js file
echo "const pipelineSteps = [" >> $DATA_FILE
for STEP in "${ALL_PIPELINE_STEPS[@]}"; do
    echo "\`$STEP\`," >> $DATA_FILE
done
echo "]" >> $DATA_FILE

# ------------------------------------------------------------------------------
# log files
echo "const logs = {" >> $DATA_FILE
if [[ -d log ]]; then
    for LOG in log/*; do
        echo -n "\"${LOG/log\//}\": \`" >> $DATA_FILE
        cat $LOG >> $DATA_FILE
        echo -n "\`," >> $DATA_FILE
    done
fi
echo "
}" >> $DATA_FILE

# ------------------------------------------------------------------------------
# packages
echo "const packages = {" >> $DATA_FILE
if [[ -d packages ]]; then
    for PACKAGE in packages/*; do
        echo "\"${PACKAGE/packages\//}\": \`$PACKAGE\`," >> $DATA_FILE
    done
fi
echo "}" >> $DATA_FILE

# ------------------------------------------------------------------------------
# path to index.html of the generated documentation
echo -n "const docIndex = \`" >> $DATA_FILE
if [[ -d doc ]]; then
    echo -n $( find doc -name 'index.html' ) >> $DATA_FILE
fi
echo "\`" >> $DATA_FILE

cd - > /dev/null
