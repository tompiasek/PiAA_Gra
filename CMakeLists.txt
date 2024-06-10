cmake_minimum_required(VERSION "3.10.0")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


project("PiAA-Project_3-Game")

set(CMAKE_CXX_STANDARD 17)

add_library(game_lib "src/game.cpp" "src/board.cpp" "src/piece.cpp")
target_include_directories(game_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_library(interface_lib "src/startScreen.cpp" "src/gameScreen.cpp" "src/settingsScreen.cpp")
target_include_directories(interface_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/interface/")
target_include_directories(interface_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_executable("PiAA-Project_3-Game" "src/main.cpp" "src/web_ver.cpp" "src/game.cpp" "src/board.cpp" "src/piece.cpp" "src/startScreen.cpp" "src/gameScreen.cpp" "src/settingsScreen.cpp")
target_link_libraries("PiAA-Project_3-Game" game_lib)
target_link_libraries("PiAA-Project_3-Game" interface_lib)
target_include_directories("PiAA-Project_3-Game" PRIVATE "/src/")
target_include_directories("PiAA-Project_3-Game" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories("PiAA-Project_3-Game" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external/img/")

# Set the path to the SFML directory
set(SFML_DIR "C:/Users/tompi/documents/SFML-2.6.1/lib/cmake/SFML/")

# Set the prefix path for SFML
set(CMAKE_PREFIX_PATH "C:/Users/tompi/documents/SFML-2.6.1/")

# Find the SFML package
find_package(SFML 2.5...2.6.1 COMPONENTS graphics window system REQUIRED)

# Include SFML headers
include_directories(${SFML_INCLUDE_DIR})

# Link SFML libraries
target_link_libraries(PiAA-Project_3-Game sfml-graphics sfml-window sfml-system)
target_link_libraries(interface_lib sfml-graphics sfml-window sfml-system)

# Output some debug information
message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARIES}")