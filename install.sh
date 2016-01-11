#!/bin/bash
MY_DIR=$( pwd )

#----------------------------------------------------------------
# This function carry out the installation of the wcm
# platform
# Accepts 2 argument:
#      $1 - string containing path of the platform directory
#      $2 - string containing path of the vendor SDK
#----------------------------------------------------------------
install_platform()
{
    cd $2/..
    echo "Patch $2 folder with $1/WCM_DK1.patch"
    patch -Np1 < "$1/WCM_DK1.patch"

    echo "Success"
    cd $MY_DIR
}