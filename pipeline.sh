#!/bin/bash

################################################################################
#
# This script ...
#
################################################################################

# ------------------------------------------------------------------------------
# variables
PIPELINE_FILE=.drone.yml
ENV_FILE=.drone.env
PIPELINE_LOG_FILE=pipeline.log
source $ENV_FILE
START_TIME=$( date +%Y-%m-%dT%H:%M:%S )

# ------------------------------------------------------------------------------
# utility functions
function log() {
    LONG_LINE=$(printf "%-80s" "-")
    LONG_LINE=${LONG_LINE// /-}
    echo
    echo $LONG_LINE
    echo "-- $@"
    echo $LONG_LINE
}

# ------------------------------------------------------------------------------
# build docker image if it does not exists yet or simply reuse an existing one
log "building docker image: $DOCKER_IMAGE"
if [[ "$( docker images -q $DOCKER_IMAGE 2>/dev/null )" == "" ]]; then
    docker build -t $DOCKER_IMAGE .
else
    echo "info: re-using existing image"
fi

# ------------------------------------------------------------------------------
# run pipeline
log "running drone pipeline: $PIPELINE_FILE"
rm -rf $ARTIFACTS_DIR
mkdir -p $ARTIFACTS_DIR
drone exec --env-file $ENV_FILE $PIPELINE_FILE 2>&1 \
    | tee $ARTIFACTS_DIR/$PIPELINE_LOG_FILE \
    || true

# ------------------------------------------------------------------------------
# post processing
mkdir -p $ARTIFACTS_DIR/_assets
cp -f ci/website/index.html $ARTIFACTS_DIR
cp -rf ci/website/_assets/* $ARTIFACTS_DIR/_assets
source ci/website/data2js.sh

# ------------------------------------------------------------------------------
# copy artifacts to timestamped directory
_ARTIFACTS_DIR=$ARTIFACTS_DIR-$START_TIME
cp -rf $ARTIFACTS_DIR $_ARTIFACTS_DIR
ARTIFACTS_DIR=$_ARTIFACTS_DIR
unset _ARTIFACTS_DIR
log "pipeline artifacts are stored in: $ARTIFACTS_DIR"
