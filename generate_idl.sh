#!/bin/bash

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <FILE_NAME>"
    exit 1
fi

IDL_FILE_NAME="$1.idl"
TYPE_SUPPORT_FILE_NAME="$1TypeSupport.idl"

tao_idl -I "$DDS_ROOT" -Sa -St -Sm -Sci -in --idl-version 4 --unknown-annotations ignore "$IDL_FILE_NAME"
opendds_idl "$IDL_FILE_NAME"
tao_idl -I "$DDS_ROOT" -Sa -St -Sm -Sci -in --idl-version 4 --unknown-annotations ignore "$TYPE_SUPPORT_FILE_NAME"

echo "Execution completed."