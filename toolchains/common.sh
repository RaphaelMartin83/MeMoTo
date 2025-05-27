#!/bin/bash

# $1 is the tag
getVersionFromTag()
{
    echo $(printenv MMT_TAG) | awk -F v '{print $2}'
}

getVersionMajorFromTag()
{
    echo $(printenv MMT_TAG) | awk -F v '{print $2}' | awk -F . '{print $1}'
}

getVersionMinorFromTag()
{
    echo $(printenv MMT_TAG) | awk -F v '{print $2}' | awk -F . '{print $2}'
}

getVersionPatchFromTag()
{
    echo $(printenv MMT_TAG) | awk -F v '{print $2}' | awk -F . '{print $3}'
}
