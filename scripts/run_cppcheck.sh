#!/usr/bin/bash

# Preserve public backend APIs; cppcheck misses their UI/test entry points across translation units.
cppcheck --enable=all \
    --inline-suppr \
    --project="${BUILD_DIR:-build}"/compile_commands.json \
    -i"${BUILD_DIR:-build}" --suppress="*:${BUILD_DIR:-build}/*" \
    -i"${EXT_DIR:-ext}" --suppress="*:${EXT_DIR:-ext}/*" \
    -i"${GEN_DIR:-generated}" --suppress="*:${GEN_DIR:-generated}/*" \
    --suppress=missingIncludeSystem \
    --suppress=unmatchedSuppression \
    --suppress=useStlAlgorithm \
    --suppress=unusedFunction:src/Berry.cpp \
    --suppress=unusedFunction:src/BoardManager.cpp \
    --suppress=unusedFunction:src/Encyclopedia.cpp \
    --suppress=unusedFunction:src/GameException.cpp \
    --suppress=unusedFunction:src/Pigeon.cpp \
    --suppress=unusedFunction:src/Shop.cpp \
    --check-level=exhaustive \
    --error-exitcode=1
