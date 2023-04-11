# !/bin/bash

rm -rf build.lite.linux.x86.gcc.opencl
lite/tools/build_linux.sh --arch=x86 \
    --toolchain=gcc \
    --with_extra=ON \
    --with_python=ON \
    --python_version=`python --version | cut -d ' ' -f 2 | cut -d '.' -f -2` \
    --with_log=ON \
    --with_exception=ON \
    --with_profile=ON \
    --with_precision_profile=ON \
    --with_static_mkl=OFF \
    --with_avx=ON \
    --with_opencl=ON
find ./build.lite.linux.x86.gcc.opencl/ | egrep -i whl$
