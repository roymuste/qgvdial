#!/bin/bash

rm -f qgvdial.bar qgvdial-114.png
cp ../build-files/qgvdial/bb10/bar-descriptor.xml .
cp ../icons/114/qgvdial.png ./qgvdial-114.png

blackberry-nativepackager -devMode -debugToken /media/drobo/uv/work/bb_dev_cert/z10-dbgtoken.bar -package qgvdial.bar bar-descriptor.xml
