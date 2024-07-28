#!/bin/bash

execute_build() {
    build_mode=$1
    echo "Build firmware on ${build_mode} mode"
    cmake -B build/${build_mode} -DCMAKE_BUILD_TYPE=${build_mode}
    cmake --build build/${build_mode}
}

mkdir build

build_mode=("Debug" "Release" "Test")

parameter_mode=$1

for item in "${build_mode[@]}"; do
    if [ "$item" = "$parameter_mode" ]; then
        execute_build $parameter_mode
        break
    fi
done
