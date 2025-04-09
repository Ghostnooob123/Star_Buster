# CMakeList.txt : CMake project for C, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_TOOLCHAIN_FILE "vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain file")

project ("C")

# Add source to this project's executable.
add_executable (C "C.cpp" "C.h" "Engine.h" "Engine.cpp" "Player.h" "Player.cpp" "Consts.h" "Meteor.cpp" 
"Meteor.h" "templateFunc.h" "Enemy.h" "Enemy.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET C PROPERTY CXX_STANDARD 20)
endif()

find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(C
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )

find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(C PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

find_package(SDL2_ttf CONFIG REQUIRED)
target_link_libraries(C PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)