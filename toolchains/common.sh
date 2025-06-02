#!/bin/bash

# $1 is the tag
getVersionFromTag()
{
    echo $(printenv MMT_TAG) | awk -F v '{print $2}'
}

getVersionMajorFromTag()
{
    local version_major=$($(printenv MMT_TAG) | awk -F v '{print $2}' | awk -F . '{print $1}')
    if [[ "" == ${version_major} ]];
    then
        echo "0"
    else
        echo ${version_major}
    fi
}

getVersionMinorFromTag()
{
    local version_minor=$($(printenv MMT_TAG) | awk -F v '{print $2}' | awk -F . '{print $2}')
    if [[ "" == ${version_minor} ]];
    then
        echo "0"
    else
        echo ${version_minor}
    fi
}

getVersionPatchFromTag()
{
    local version_patch=$($(printenv MMT_TAG) | awk -F v '{print $2}' | awk -F . '{print $3}')
    if [[ "" == ${version_patch} ]];
    then
        echo "0"
    else
        echo ${version_patch}
    fi
}
