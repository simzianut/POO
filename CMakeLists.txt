cmake_minimum_required(VERSION 3.26)

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing executable name in this file
# for now, the project name is used as the executable name
set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")


project(${MAIN_PROJECT_NAME})
# set(CMAKE_PROJECT_VERSION_MAJOR 0)
# set(CMAKE_PROJECT_VERSION_MINOR 0)
# set(CMAKE_PROJECT_VERSION_PATCH 1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# You aren't supposed to be using this, but convenience...
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS RELATIVE ${CMAKE_SOURCE_DIR} "src/*")
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "include/*")
file(GLOB_RECURSE TESTS RELATIVE ${CMAKE_SOURCE_DIR} "test/*.cpp")
include_directories(include)
include_directories(src)

option(WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(USE_ASAN "Use Address Sanitizer" OFF)
option(USE_MSAN "Use Memory Sanitizer" OFF)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "install_dir" CACHE PATH "..." FORCE)
endif()

# disable sanitizers when releasing executables without explicitly requested debug info
# use generator expressions to set flags correctly in both single and multi config generators
set(is_debug "$<CONFIG:Debug>")
set(is_rel_with_deb "$<CONFIG:RelWithDebInfo>")
set(debug_mode "$<OR:${is_debug},${is_rel_with_deb}>")

###############################################################################

# custom functions

function(detect_libcpp)
    set(OLD_CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(TEST_STDLIB_SRC [=[
    #include <iostream>
    int main() {}
    ]=])
    try_compile(HAS_LIBCPP SOURCE_FROM_CONTENT test_stdlib.cpp ${TEST_STDLIB_SRC})
    set(CMAKE_CXX_FLAGS ${OLD_CMAKE_CXX_FLAGS})
    unset(OLD_CMAKE_CXX_FLAGS)
endfunction()


function(set_custom_stdlib_and_sanitizers target add_apple_asan)
    if(MSVC)
        # see https://gitlab.kitware.com/cmake/cmake/-/issues/24922
        set_target_properties(${target} PROPERTIES VS_USER_PROPS "${CMAKE_SOURCE_DIR}/disable_modules.props")
        target_compile_options(${target} PRIVATE /experimental:module-)
        if(USE_ASAN)
            target_compile_options(${target} PRIVATE "$<${debug_mode}:/fsanitize=address>")
        endif()
        return()
    endif()

    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND NOT WIN32)
        detect_libcpp()
        if(HAS_LIBCPP)
            # see also https://stackoverflow.com/a/70382484
            target_compile_options(${target} PRIVATE -stdlib=libc++)
            target_link_options(${target} PRIVATE -stdlib=libc++)
        else()
            # fall back to libstdc++
            target_compile_options(${target} PRIVATE -stdlib=libstdc++)
            target_link_options(${target} PRIVATE -stdlib=libstdc++)
        endif()
    endif()

    if(APPLE)
        # first check Apple since Apple is also a kind of Unix
        if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND add_apple_asan MATCHES true)
            if(USE_ASAN)
                target_compile_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
                target_link_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
            endif()
        endif()
    elseif(UNIX)
        if(USE_ASAN)
            # check leaks on Linux since macOS does not support the leaks sanitizer yet
            # leaks sanitizer is enabled by default on Linux, so we do not need to enable it explicitly
            target_compile_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
            target_link_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
        elseif(USE_MSAN)
            # use semi-colons instead of spaces to separate arguments
            # it is recommended to quote generator expressions in order to avoid unintentional splitting
            target_compile_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=memory,undefined;-fsanitize-recover=memory,undefined;-fsanitize-memory-track-origins>")
            target_link_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=memory,undefined;-fsanitize-recover=memory,undefined;-fsanitize-memory-track-origins;-Wl,-rpath,tools/llvm-project/build/lib>")
        endif()
    endif()
endfunction()
include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

###############################################################################

# external dependencies with FetchContent
include(FetchContent)
#
# set(FETCHCONTENT_QUIET OFF)
# set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
#
# NOTE: Also update env vars used for caching in
#         - .github/actions/configure-cmake/action.yml
#         - .github/workflows/cmake.yml
 FetchContent_Declare(
         googletest
         URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
 )
#

if(USE_MSAN AND CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND UNIX)
    # FetchContent dependencies must use the same standard library as the app.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

FetchContent_MakeAvailable(googletest)

# ── SFML 2.6.1 (static – no DLLs needed on Windows) ────────────────────────
set(BUILD_SHARED_LIBS  OFF CACHE BOOL "" FORCE)   # <-- key fix: static build
set(SFML_BUILD_AUDIO   OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.1
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SFML)

if(USE_MSAN AND CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND UNIX)
    # Instrument SFML System along with the app so MSan can track values across the library boundary.
    # SFML 2.6.1's global X11 strings trigger a libc++ false positive in its String.cpp locale path.
    target_compile_options(sfml-system PRIVATE
        "$<${debug_mode}:-fsanitize=memory,undefined;-fsanitize-recover=memory,undefined;-fsanitize-memory-track-origins>"
        "-fsanitize-ignorelist=${CMAKE_SOURCE_DIR}/cmake/msan-ignorelist.txt")
endif()

###############################################################################

# external dependencies with find_package

# find_package(Threads REQUIRED)

###############################################################################

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing name here
add_executable(${MAIN_EXECUTABLE_NAME} main.cpp ${SOURCES}
        include/Poop.h
        src/Poop.cpp
        include/BoardManager.h
        src/BoardManager.cpp
        include/Crate.h
        src/Crate.cpp
        include/Shop.h
        src/Shop.cpp
        include/Berry.h
        src/Berry.cpp
        include/Encyclopedia.h
        src/Encyclopedia.cpp
        src/GameException.cpp
        # ── SFML GUI layer ────────────────────────────────────────────────────
        include/AssetManager.h        src/AssetManager.cpp
        include/FloatingText.h        src/FloatingText.cpp
        include/EntitySprite.h        src/EntitySprite.cpp
        include/CrateSprite.h         src/CrateSprite.cpp
        include/BerrySprite.h         src/BerrySprite.cpp
        include/HudBar.h              src/HudBar.cpp
        include/OverlayScrollBar.h
        include/ShopOverlay.h         src/ShopOverlay.cpp
        include/EncyclopediaOverlay.h src/EncyclopediaOverlay.cpp
        include/GameWindow.h          src/GameWindow.cpp
)

# Match SFML's STL annotation settings on current MSVC runners.
if(MSVC)
    target_compile_definitions(${MAIN_EXECUTABLE_NAME} PRIVATE
        _DISABLE_STRING_ANNOTATION
        _DISABLE_VECTOR_ANNOTATION
    )
endif()

# NOTE: Add all defined targets (e.g. executables, libraries, etc. )
# NOTE: RUN_SANITIZERS is optional, if it's not present it will default to true
set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})
# set_compiler_flags(TARGET_NAMES ${MAIN_EXECUTABLE_NAME} ${FOO} ${BAR})
# where ${FOO} and ${BAR} represent additional executables or libraries
# you want to compile with the set compiler flags

###############################################################################

# use SYSTEM so cppcheck and clang-tidy do not report warnings from these directories
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE generated/include)

# ── Link SFML ────────────────────────────────────────────────────────────────
target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE sfml-graphics sfml-window sfml-system)

###############################################################################

# copy binaries to "bin" folder; these are uploaded as artifacts on each release
# DESTINATION_DIR is set as "bin" in cmake/Options.cmake:6
install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

###############################################################################
include(CTest)
if(BUILD_TESTING)
    add_executable(oop_tests ${HEADERS} test/game_logic_test.cpp ${SOURCES})
    target_compile_definitions(oop_tests PRIVATE PIGEON_ENABLE_TESTS)
    # Match SFML's STL annotation settings on current MSVC runners.
    if(MSVC)
        target_compile_definitions(oop_tests PRIVATE
            _DISABLE_STRING_ANNOTATION
            _DISABLE_VECTOR_ANNOTATION
        )
    elseif(MINGW)
        # Avoid loading a potentially incompatible libstdc++ from the runner PATH.
        target_link_options(oop_tests PRIVATE -static-libgcc -static-libstdc++)
    endif()
    target_include_directories(oop_tests SYSTEM PRIVATE generated/include)
    target_link_libraries(oop_tests PRIVATE
            GTest::gtest_main sfml-graphics sfml-window sfml-system)
    set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES oop_tests)

    include(GoogleTest)
    # Defer execution until CTest so Windows toolchain runtime paths are set up.
    gtest_discover_tests(oop_tests DISCOVERY_MODE PRE_TEST)
endif()
copy_files(FILES tastatura.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
# Copy assets directory next to the binary (textures, fonts, etc.)
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    copy_files(DIRECTORY assets COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
endif()
# copy_files(FILES tastatura.txt config.json DIRECTORY images sounds COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
# copy_files(DIRECTORY images sounds COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
