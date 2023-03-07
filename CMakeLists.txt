#********************************************************************************
#  Copyright 2023 Yahoo Mike from XDA-developers.com
#  This file is part of snimg.
# 
#  snimg is free software: you can redistribute it and/or modify it under the
#  terms of the GNU General Public License as published by the Free Software
#  Foundation, either version 3 of the License, or (at your option) any later
#  version.
# 
#  snimg is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
#  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#  for more details.
# 
#  You should have received a copy of the GNU General Public License along
#  with snimg.  If not, see <https://www.gnu.org/licenses/>.
#********************************************************************************
cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(snimg)

# SOURCE FILES
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# EXECUTABLE
add_executable(${PROJECT_NAME} ${SRC_FILES})

# INCLUDE DIRECTORIES
target_include_directories(${PROJECT_NAME}
        PRIVATE h )

# OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto)
