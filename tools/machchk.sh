#!/bin/bash

getdefs() {
    gcc -dM -E - </dev/null | grep -E \
        "(__x86_64__|__amd64__|__i386__|__arm__|\
        __aarch64__|__linux__|__WIN32__|__APPLE__)"
}

echo "Dumping GCC machdep defines..."
echo "****************************************************"
getdefs
echo "****************************************************"
