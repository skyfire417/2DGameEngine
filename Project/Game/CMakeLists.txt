cmake_minimum_required(VERSION 3.11...3.13)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(PlatformerMakerGameCastle VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(SDL2_VERSION 2.0.9)
set(SDL2_IMAGE_VERSION 2.0.4)
set(SDL2_TTF_VERSION 2.0.14)
set(SDL2_MIXER_VERSION 2.0.4)
set(SDL2PP_VERSION 0.16.0)
set(CATCH2_VERSION 2.6.0)
set(DOXYGEN_VERSION 1.8.0)
set(JSONCPP_VERSION 1.8.4)
set(SPDLOG_VERSION 1.3.1)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/temp/fetched-content)

## Set a default build type if none was specified
set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

## Get FindSDL2XX scripts
include(FetchContent)
FetchContent_Declare(
    sdl2_cmake_scripts
    GIT_REPOSITORY  https://github.com/tcbrindle/sdl2-cmake-scripts
)
FetchContent_GetProperties(sdl2_cmake_scripts)
if(NOT sdl2_cmake_scripts_POPULATED)
    FetchContent_Populate(sdl2_cmake_scripts)
endif()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${sdl2_cmake_scripts_SOURCE_DIR}")

## Get targets for SDL2
find_package(SDL2 ${SDL2_VERSION} REQUIRED)
find_package(SDL2_image ${SDL2_IMAGE_VERSION} REQUIRED)
find_package(SDL2_mixer ${SDL2_MIXER_VERSION} REQUIRED)
find_package(SDL2_ttf ${SDL2_TTF_VERSION} REQUIRED)

## Get SDL2pp library
# Can't call find_package here since SDL2PP is compiled at build time
set(project_sdl2pp_dir ${PROJECT_SOURCE_DIR}/lib/sdl2pp/${SDL2PP_VERSION})
include(ExternalProject)
ExternalProject_Add(
    sdl2pp
    PREFIX          ${PROJECT_SOURCE_DIR}/temp/sdl2pp/${SDL2PP_VERSION}
    GIT_REPOSITORY  https://github.com/libSDL2pp/libSDL2pp
    GIT_TAG         ${SDL2PP_VERSION}
    CMAKE_ARGS      -D SDL2PP_CXXSTD=c++${CMAKE_CXX_STANDARD}
                    -D SDL2PP_WITH_IMAGE=ON
                    -D SDL2PP_WITH_TTF=ON
                    -D SDL2PP_WITH_MIXER=ON
                    -D SDL2PP_STATIC=ON
                    -D SDL2PP_WITH_EXPERIMENTAL_OPTIONAL=OFF
                    -D CMAKE_INSTALL_PREFIX=${project_sdl2pp_dir}
)
# Manually set paths
set(SDL2PP_INCLUDE_DIRS ${project_sdl2pp_dir}/include)
set(SDL2PP_LIBRARIES ${project_sdl2pp_dir}/lib/libSDL2pp.a)

## Get jsoncpp library
set(project_jsoncpp_dir ${PROJECT_SOURCE_DIR}/lib/jsoncpp/${JSONCPP_VERSION})
ExternalProject_Add(
    jsoncpp
    PREFIX              ${PROJECT_SOURCE_DIR}/temp/jsoncpp/${JSONCPP_VERSION}
    GIT_REPOSITORY      https://github.com/open-source-parsers/jsoncpp
    GIT_TAG             ${JSONCPP_VERSION}
    CONFIGURE_COMMAND   ""
    BUILD_COMMAND       python amalgamate.py -s ${project_jsoncpp_dir}/dist/jsoncpp.cpp -i ${project_jsoncpp_dir}/json/json.h
    BUILD_IN_SOURCE     YES
    INSTALL_COMMAND     ""
)
set(JSONCPP_SOURCE ${project_jsoncpp_dir}/dist/jsoncpp.cpp)
set(JSONCPP_INCLUDE_DIR ${project_jsoncpp_dir})
file(MAKE_DIRECTORY ${project_jsoncpp_dir}/dist)
file(TOUCH ${JSONCPP_SOURCE})

## Get spdlog library
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY  https://github.com/gabime/spdlog
    GIT_TAG         v${SPDLOG_VERSION}
)
FetchContent_GetProperties(spdlog)
if(NOT spdlog_POPULATED)
    FetchContent_Populate(spdlog)
endif()
set(SPDLOG_INCLUDE_DIR ${spdlog_SOURCE_DIR}/include)

set(main_exe main)
function(get_src_list)
    cmake_parse_arguments(
        GET_SRC_LIST
        ""
        "NAME"
        "NAMES" # custom include
        ${ARGN}
    )
    if(GET_SRC_LIST_NAME)
        list(APPEND SRC_LIST ${GET_SRC_LIST_NAME})
    endif()
    if(GET_SRC_LIST_NAMES)
        list(APPEND SRC_LIST ${GET_SRC_LIST_NAMES})
    endif()

    list(TRANSFORM SRC_LIST PREPEND "src/")
    list(TRANSFORM SRC_LIST APPEND ".cpp")
    set(PROJECT_SRC_LIST ${SRC_LIST} PARENT_SCOPE)
    #add_executable(${GET_SRC_LIST_EXE} ${SRC_LIST})
endfunction()

## Gets a list of source files except for the main one
# Add only cpp filenames here
get_src_list(
    NAMES
    resource/base
    resource/image
    resource/game_image
    resource/text
    resource/game_text
    resource/game_font
    resource/game_audio
    resource/game_config
    object/object
    object/fallable_object
    object/tile
    object/tile_manager
    object/border
    object/border_manager
    world/world
    world/level_manager
    world/player
    world/player_input
    world/camera_system
    physics/collision
    game
    start_menu
    utility
)
# Must add main.cpp first or program will crash on quit
add_executable(${main_exe} src/${main_exe}.cpp)
add_dependencies(${main_exe} sdl2pp jsoncpp)
target_sources(${main_exe} PRIVATE ${PROJECT_SRC_LIST})
target_sources(${main_exe} PUBLIC ${JSONCPP_SOURCE})

target_include_directories(${main_exe}
    PRIVATE
      "src"
      ${SDL2_INCLUDE_DIR}
      ${SDL2_IMAGE_INCLUDE_DIRS}
      ${SDL2_TTF_INCLUDE_DIRS}
      ${SDL2_MIXER_INCLUDE_DIRS}
      ${SDL2PP_INCLUDE_DIRS}
      ${JSONCPP_INCLUDE_DIR}
      ${SPDLOG_INCLUDE_DIR}
)
target_link_libraries(${main_exe}
    PRIVATE
      ${SDL2_LIBRARY}
      ${SDL2_IMAGE_LIBRARIES}
      ${SDL2_TTF_LIBRARIES}
      ${SDL2_MIXER_LIBRARIES}
      ${SDL2PP_LIBRARIES}
)

## Doxygen documentation
find_package(Doxygen ${DOXYGEN_VERSION})
if(DOXYGEN_FOUND)
    set(DOXYGEN_EXTRACT_ALL NO)
    set(DOXYGEN_EXTRACT_PRIVATE YES)
    doxygen_add_docs(
        doxygen
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/include
        COMMENT "Generate HTML pages"
    )
else()
    message(STATUS "Doxygen not found, not generating documentation")
endif()

## Testing in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    FetchContent_Declare(
        catch2
        GIT_REPOSITORY  https://github.com/catchorg/Catch2.git
        GIT_TAG         v${CATCH2_VERSION}
    )
    FetchContent_GetProperties(catch2)
    if(NOT catch2_POPULATED)
        FetchContent_Populate(catch2)
        add_subdirectory(${catch2_SOURCE_DIR})
    endif()

    set(test_exe tests-main)
    add_executable(${test_exe} src/test/${test_exe}.cpp)
    add_dependencies(${test_exe} sdl2pp jsoncpp)
    target_sources(${test_exe} PRIVATE src/test/tests-math.cpp)
    target_sources(${test_exe} PRIVATE ${PROJECT_SRC_LIST})
    target_sources(${test_exe} PUBLIC ${JSONCPP_SOURCE})
    target_include_directories(${test_exe}
        PRIVATE
          "src"
          ${SDL2_INCLUDE_DIR}
          ${SDL2_IMAGE_INCLUDE_DIRS}
          ${SDL2_TTF_INCLUDE_DIRS}
          ${SDL2_MIXER_INCLUDE_DIRS}
          ${SDL2PP_INCLUDE_DIRS}
          ${JSONCPP_INCLUDE_DIR}
          ${SPDLOG_INCLUDE_DIR}
    )
    target_link_libraries(${test_exe}
        PRIVATE
          Catch2::Catch2
          ${SDL2_LIBRARY}
          ${SDL2_IMAGE_LIBRARIES}
          ${SDL2_TTF_LIBRARIES}
          ${SDL2_MIXER_LIBRARIES}
          ${SDL2PP_LIBRARIES}
    )

    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${catch2_SOURCE_DIR}/contrib)
    include(CTest)
    include(Catch)
    catch_discover_tests(tests-main)
endif()

## Copy game executable to GameBuild directory
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_command(TARGET ${main_exe}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/GameBuild"
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:${main_exe}>
            ${PROJECT_SOURCE_DIR}/GameBuild/
    )
endif()

## Generates compilation database in debug mode for code completion
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Generating compilation databse")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()
