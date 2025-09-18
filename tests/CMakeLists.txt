# Sets up projects for stb tests.
# The STB_SANITIZERS variable can be set to enable various sanitizers;
# for instance, to enable Undefined Behavior Sanitizer and Address Sanitizer on
# clang, use
# _build> CC=clang CXX=clang++ cmake .. -DSTB_SANITIZERS="-fsanitize=undefined,address"

cmake_minimum_required(VERSION 3.13)

project(stb_tests)
include_directories(${CMAKE_CURRENT_LIST_DIR}/..)

# Turn on multiprocess compilation by default on MSVC
if(MSVC)
  add_compile_options(/MP)
endif()

# Download files for testing that aren't included in the repository, to the
# CMake build directory
file(DOWNLOAD "https://nothings.org/stb_vorbis/samples/sketch008.ogg" ${CMAKE_BINARY_DIR}/sketch008.ogg)

# Currently using add_executable for tests which can be run, add_library for
# tests that don't have main() functions
add_executable(stb_c_lexer_test c_lexer_test.c)
set_target_properties(stb_c_lexer_test PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..) # Since it parses itself

add_executable(stb_connected_components_test grid_reachability.c test_c_compilation.c)
target_compile_definitions(stb_connected_components_test PRIVATE GRID_TEST STB_DEFINE)
target_include_directories(stb_connected_components_test PRIVATE ../deprecated)
set_target_properties(stb_connected_components_test PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..) # Since it loads data/map_03.png
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/output/stbcc)
if(UNIX)
  target_link_libraries(stb_connected_components_test PRIVATE m)
endif()

add_executable(stb_herringbone_generator herringbone_generator.c)
set_target_properties(stb_herringbone_generator PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "${CMAKE_CURRENT_LIST_DIR}/output/herringbone/tileset.png e 12 1 1 1 1 1 1 4 4")
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/output/herringbone)
add_executable(stb_herringbone_map herringbone_map.c)
set_target_properties(stb_herringbone_map PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "${CMAKE_CURRENT_LIST_DIR}/../data/herringbone/template_rooms_limit_connectivity.png 128 128 ${CMAKE_CURRENT_LIST_DIR}/output/herringbone/map.png")
if(UNIX)
  target_link_libraries(stb_herringbone_map PRIVATE m)
endif()

add_executable(stb_image_test image_test.c image_write_test.c)
target_compile_definitions(stb_image_test PRIVATE PNGSUITE_PRIMARY)
target_include_directories(stb_image_test PRIVATE ../deprecated)
set_target_properties(stb_image_test PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}) # Since it reads pngsuite/primary
if(UNIX)
  target_link_libraries(stb_image_test PRIVATE m)
endif()

add_library(stb_image_test_compilation test_image.c)
add_library(stb_image_write_test_compilation test_image_write.c)

add_executable(stb_resample_test resample_test.cpp)
add_executable(stb_resample_test_c resample_test_c.c)
target_include_directories(stb_resample_test_c PRIVATE ../deprecated)

# These fail on 64-bit systems, because stb_regex expects that pointers are 32-bit
# add_executable(stb_test stb.c)
# target_include_directories(stb_test PRIVATE ../deprecated)
# target_compile_definitions(stb_test PRIVATE MAIN_TEST STB_PERFECT_HASH)
# add_executable(stb_test_cpp stb_cpp.cpp)
# target_include_directories(stb_test_cpp PRIVATE ../deprecated)

add_library(stb_static stb_static.c)

add_executable(stb_lib_test stblib_test.c)
add_library(stb_lib_test_companion stblib_test_companion.c)

add_library(stb_test_c_compilation test_c_compilation.c)
add_library(stb_test_cpp_compilation test_cpp_compilation.cpp)

add_executable(stb_ds_test test_ds.c)
target_compile_definitions(stb_ds_test PRIVATE DS_TEST)
add_executable(stb_ds_test_cpp test_ds_cpp.cpp)
target_compile_definitions(stb_ds_test_cpp PRIVATE DS_TEST)

add_library(stb_dxt_test test_dxt.c)

add_library(stb_easyfont_test test_easyfont.c)

add_library(stb_perlin_test test_perlin.c)

add_executable(stb_png_paeth_test test_png_paeth.c)
add_executable(stb_png_regress_test test_png_regress.c)
set_target_properties(stb_png_regress_test PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}) # Since it reads and writes pngsuite/
if(UNIX)
  target_link_libraries(stb_png_regress_test PRIVATE m)
endif()

add_executable(stb_siphash_test test_siphash.c)
add_executable(stb_sprintf_test test_sprintf.c)
target_compile_definitions(stb_sprintf_test PRIVATE TEST_SPRINTF STB_SPRINTF_IMPLEMENTATION)

add_executable(stb_truetype_test test_truetype.c test_c_compilation.c)
target_compile_definitions(stb_truetype_test PRIVATE TT_TEST)
if(UNIX)
  target_link_libraries(stb_truetype_test PRIVATE m)
endif()

add_executable(stb_vorbis_test test_vorbis.c ../stb_vorbis.c)
target_include_directories(stb_vorbis_test PRIVATE ../deprecated)
target_compile_definitions(stb_vorbis_test PRIVATE VORBIS_TEST STB_DEFINE)
if(UNIX)
  target_link_libraries(stb_vorbis_test PRIVATE m)
endif()

add_library(stb_voxel_test test_voxel.c)

#-------------------------------------------------------------------------------
# Set up default sanitizers
if(NOT STB_SANITIZERS)
  if(MSVC)
    set(STB_SANITIZERS /fsanitize=address)
  else()
    set(STB_SANITIZERS -fsanitize=address,undefined)
  endif()
endif()

#-------------------------------------------------------------------------------
# stb_image regression test cases
add_executable(stb_image_test_issues
  image/image_test_issues.c
  image/1454_gif.h
  image/1838_gif.h
)
target_compile_options(stb_image_test_issues PRIVATE ${STB_SANITIZERS})
if(UNIX)
  target_link_libraries(stb_image_test_issues PRIVATE m)
  target_link_options(stb_image_test_issues PRIVATE ${STB_SANITIZERS})
endif()

#-------------------------------------------------------------------------------
# Vorbis test cases
add_executable(stb_vorbis_test_issues
  vorbis/vorbis_test_issues.c
  vorbis/1536.h
  vorbis/1537.h
  vorbis/1553.h
  vorbis/1554.h
  vorbis/1555.h
  vorbis/1556.h
  vorbis/1557.h
  vorbis/1558.h
  vorbis/1559.h
  vorbis/1560.h
)
target_compile_options(stb_vorbis_test_issues PRIVATE ${STB_SANITIZERS})
if(UNIX)
  target_link_libraries(stb_vorbis_test_issues PRIVATE m)
  target_link_options(stb_vorbis_test_issues PRIVATE ${STB_SANITIZERS})
endif()
# Other .ogg files are handled in the fuzzer.

#-------------------------------------------------------------------------------
# Configure fuzzers

# This script only knows that libFuzzer is supported on MSVC and Clang:
if((CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
  if(MSVC)
    set(_STB_SANITIZERS_ALL ${STB_SANITIZERS} /fsanitize=fuzzer)
  else()
    set(_STB_SANITIZERS_ALL ${STB_SANITIZERS} -fsanitize=fuzzer)
  endif()

  # Corpuses:
  # stb_image
  file(COPY pbm DESTINATION ${CMAKE_BINARY_DIR}/CORPUS_IMAGE)
  file(COPY pic DESTINATION ${CMAKE_BINARY_DIR}/CORPUS_IMAGE)
  file(COPY pngsuite DESTINATION ${CMAKE_BINARY_DIR}/CORPUS_IMAGE)

  if(NOT EXISTS ${CMAKE_BINARY_DIR}/gif.tar.gz)
    file(DOWNLOAD https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/imagetestsuite/imagetestsuite-gif-1.00.tar.gz ${CMAKE_BINARY_DIR}/gif.tar.gz SHOW_PROGRESS)
    file(ARCHIVE_EXTRACT INPUT ${CMAKE_BINARY_DIR}/gif.tar.gz DESTINATION ${CMAKE_BINARY_DIR}/CORPUS_IMAGE PATTERNS **.gif)
  endif()

  if(NOT EXISTS ${CMAKE_BINARY_DIR}/jpg.tar.gz)
    file(DOWNLOAD https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/imagetestsuite/imagetestsuite-jpg-1.00.tar.gz ${CMAKE_BINARY_DIR}/jpg.tar.gz SHOW_PROGRESS)
    file(ARCHIVE_EXTRACT INPUT ${CMAKE_BINARY_DIR}/jpg.tar.gz DESTINATION ${CMAKE_BINARY_DIR}/CORPUS_IMAGE PATTERNS **.jpg)
  endif()

  if(NOT EXISTS ${CMAKE_BINARY_DIR}/bmp.zip)
    file(DOWNLOAD http://entropymine.com/jason/bmpsuite/releases/bmpsuite-2.6.zip ${CMAKE_BINARY_DIR}/bmp.zip SHOW_PROGRESS)
    file(ARCHIVE_EXTRACT INPUT ${CMAKE_BINARY_DIR}/bmp.zip DESTINATION ${CMAKE_BINARY_DIR}/CORPUS_IMAGE PATTERNS **.bmp)
  endif()

  if(NOT EXISTS ${CMAKE_BINARY_DIR}/tga.zip)
    file(DOWNLOAD https://github.com/richgel999/tga_test_files/archive/master.zip ${CMAKE_BINARY_DIR}/tga.zip SHOW_PROGRESS)
    file(ARCHIVE_EXTRACT INPUT ${CMAKE_BINARY_DIR}/tga.zip DESTINATION ${CMAKE_BINARY_DIR}/CORPUS_IMAGE PATTERNS **.tga)
  endif()

  # stb_vorbis
  file(COPY vorbis
       DESTINATION ${CMAKE_BINARY_DIR}/CORPUS_VORBIS
       FILES_MATCHING PATTERN **.ogg)

  # Executables:
  macro(add_sanitizers TARGET_NAME)
    target_compile_options(${TARGET_NAME} PRIVATE ${_STB_SANITIZERS_ALL})
    if(NOT MSVC)
      target_link_options(${TARGET_NAME} PRIVATE ${_STB_SANITIZERS_ALL})
    endif()
  endmacro()

  add_executable(stb_c_lexer_test_fuzz stb_c_lexer_fuzzer.cpp)
  add_sanitizers(stb_c_lexer_test_fuzz)
  set_target_properties(stb_c_lexer_test_fuzz PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS ${CMAKE_BINARY_DIR}/CORPUS_C_LEXER)

  add_executable(stb_image_test_fuzz stbi_read_fuzzer.c)
  add_sanitizers(stb_image_test_fuzz)
  set_target_properties(stb_image_test_fuzz PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS --dict ${CMAKE_CURRENT_LIST_DIR}/stb_png.dict ${CMAKE_BINARY_DIR}/CORPUS_IMAGE)

  add_executable(stb_image_test_gif_fuzz stbi_read_gif_fuzzer.c)
  add_sanitizers(stb_image_test_gif_fuzz)
  set_target_properties(stb_image_test_gif_fuzz PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS ${CMAKE_BINARY_DIR}/CORPUS_IMAGE)

  add_executable(stb_vorbis_test_fuzz vorbis/vorbis_test_fuzzer.c)
  add_sanitizers(stb_vorbis_test_fuzz)
  set_target_properties(stb_vorbis_test_fuzz PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS ${CMAKE_BINARY_DIR}/CORPUS_VORBIS)
endif()
