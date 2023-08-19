cmake_minimum_required(VERSION 3.21)

# Let's choose a name and version for the extension. Change the name to your
# liking. The version should be a string like "1.0".
set(PROJECT_NAME "RadioButtons") # Change this to your liking
project(${PROJECT_NAME} VERSION 1.5)

# Import the SDK
include(FetchContent)
FetchContent_Declare(
  gp-sdk
  GIT_REPOSITORY https://github.com/gigperformer/gp-sdk.git
  GIT_TAG 26bc1ed2568b94e1c95ca2348f03afcbb0d8fb16 # v46
)
FetchContent_MakeAvailable(gp-sdk)

# Define our library including sources, include directories and dependencies
add_library(${PROJECT_NAME} SHARED)
target_sources(
  ${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/LibMain.cpp"
                          "${CMAKE_CURRENT_LIST_DIR}/src/LibMain.h"
                          "${CMAKE_CURRENT_LIST_DIR}/src/RadioButton.cpp"
                          "${CMAKE_CURRENT_LIST_DIR}/src/RadioButton.h"
                          "${CMAKE_CURRENT_LIST_DIR}/src/Panels.h"
                          )
target_link_libraries(${PROJECT_NAME} PRIVATE gigperformer::sdk::cpp)
target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Language options: this will be a pure C++20 project
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES CXX_STANDARD 20
             CXX_STANDARD_REQUIRED ON
             CXX_EXTENSIONS NO)

# Install the extension on the development machine
install(
  TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION "${GIG_PERFORMER_EXTENSIONS_DIRECTORY}"
  RUNTIME DESTINATION "${GIG_PERFORMER_EXTENSIONS_DIRECTORY}")