cmake_minimum_required (VERSION 3.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
project(AIAssistant)
set(TARGET_NAME AIAssistant)
find_package(Qt5 COMPONENTS Widgets Network Concurrent WebEngineWidgets)
# include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})

# find_package(Qt5 COMPONENTS Widgets REQUIRED)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/protobuf/lib/cmake/protobuf;${CMAKE_SOURCE_DIR}/protobuf/lib/cmake/absl;${CMAKE_SOURCE_DIR}/protobuf/lib/cmake/utf8_range)
message("CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
# Find required protobuf package
find_package(protobuf CONFIG REQUIRED)

if(protobuf_VERBOSE)
  message(STATUS "Using Protocol Buffers ${protobuf_VERSION}")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# http://www.cmake.org/Wiki/CMake_FAQ#How_can_I_build_my_MSVC_application_with_a_static_runtime.3F
if(MSVC AND protobuf_MSVC_STATIC_RUNTIME)
  foreach(flag_var
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
  endforeach()
endif()

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
	${CMAKE_CURRENT_SOURCE_DIR}/version.rc
	@ONLY)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(. DIR_SRCS)
aux_source_directory(./AIAssistantPage DIR_SRCS)
aux_source_directory(./base DIR_SRCS)
aux_source_directory(./chat DIR_SRCS)
aux_source_directory(./utils DIR_SRCS)
aux_source_directory(./protobuf DIR_SRCS)
aux_source_directory(./lang DIR_SRCS)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")# .exe and .dll
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")# .so and .dylib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")# .lib and .a




if(WIN32 OR MINGW)
    add_executable(${TARGET_NAME} WIN32
        ${DIR_SRCS}
        AIAssistant.qrc
        # version.rc
        logo.rc
        )
endif()

target_link_libraries(AIAssistant PUBLIC 
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    Qt5::Concurrent
    Qt5::WebEngineWidgets
    protobuf::libprotobuf
)

if(COPY_LIB)
    if(APPLE)
        add_custom_command( TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${TARGET_NAME}>" "${REVOSCAN_BUNDLE_CONTENTS}/MacOS/${TARGET_NAME}"
        # COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_BUNDLE_CONTENT_DIR:${TARGET_NAME}>" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RevoScan5.app/Contents/Frameworks/${TARGET_NAME}"
        # COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TARGET_FILE}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RevoScan5.app/Contents/Frameworks"

        )
    elseif(WIN32 OR MINGW)
        add_custom_command( TARGET ${TARGET_NAME} POST_BUILD
        #qt打包
            COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${TARGET_NAME}>

        )
    endif()
endif()