# SPDX-FileCopyrightText: 2025 Matthew McDade <nvllmnd@pm.me>
# SPDX-License-Identifier: AGPL-3.0-or-later

project(
  'aoc2025',
  'c',
  meson_version: '>= 1.3.0',
  version: files('.version'),
  default_options: [
    'c_std=c2x,c23,c2y',
    'warning_level=3',
    'werror=true',
    'b_ndebug=if-release',
    'prefer_static=true',
    'default_library=static',
  ],
)

add_global_arguments('-Wno-static-in-inline', language: 'c')
add_global_arguments('-Wno-gnu-statement-expression', language: 'c')

#################################################################################
######################### External Meson Dependencies ###########################
#################################################################################




#################################################################################
######################### External Dependencies #################################
#################################################################################

# cchan_lib = static_library(
#   'cchan',
#   files(
#     'src/extern/cchan/cchan.c',
#   ),
#   dependencies: [mimalloc_dep, thread_dep],
#   include_directories: include_directories('include/extern'),
#   install: true,
#   pic: true,
# )

# cchan_dep = declare_dependency(
#   include_directories: include_directories('include/extern'),
#   link_with: cchan_lib,
# )



#################################################################################
########################## Resource Files  ######################################
#################################################################################


#################################################################################
######################### Project Includes ######################################
#################################################################################

inc = include_directories(
  'include',
  'include/deps',
)

aoc_lib = static_library(
  'aoc_2025',
  files(
  ),
  dependencies: [],
  include_directories: inc,
  install: true,
  pic: true,
  c_pch: 'pch/aoc_pch.h'
)

aoc_dep = declare_dependency(
  include_directories: inc,
  link_with: aoc_lib,
)

aoc_exe = executable(
  'aoc',
  files('src/main.c'),
  dependencies: [aoc_dep ],
  include_directories: inc,
  install: true,
  c_pch: 'pch/aoc_pch.h',
)


subdir('tests')


