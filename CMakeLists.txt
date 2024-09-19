cmake_minimum_required (VERSION 3.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
project(AIAssistant)
set(TARGET_NAME AIAssistant)
find_package(Qt5 COMPONENTS Widgets Network Concurrent WebEngineWidgets)
# include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})

# find_package(Qt5 COMPONENTS Widgets REQUIRED)

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

#第三方库头文件路
include_directories(
	"${CMAKE_SOURCE_DIR}/protobuf/include"	
)

#设置链接的动态库文件目录
if(APPLE)
    link_directories(
        "${CMAKE_SOURCE_DIR}/protobuf/lib/windows" 
    )
elseif(WIN32 OR MINGW)
    link_directories(
        "${CMAKE_SOURCE_DIR}/protobuf/lib" 
    )
endif()

# if(APPLE)
#     set(CMAKE_EXE_LINKER_FLAGS "-framework Cocoa -framework CoreBluetooth -framework Foundation")
#     target_link_libraries(${TARGET_NAME} PRIVATE 
#         # optimized simpleble
#         # aeSharpnessFineAlgorithm
#         protobuf
#         # optimized ossLib
#         # omp
#         # z
#     )    
# elseif(WIN32 OR MINGW)
#     target_link_libraries(${TARGET_NAME} PRIVATE 
#         # debug simplebled
#         # optimized simpleble 
#         # debug 3DCamerad
#         debug protobuf
#         # debug ossLibd
#         # optimized ossLib
#         # aeSharpnessFineAlgorithm
#     )
# endif()

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
    abseil_dll
    abseil_test_dll
    absl_flags_commandlineflag
    absl_flags_commandlineflag_internal
    absl_flags_config
    absl_flags_internal
    absl_flags_marshalling
    absl_flags_parse
    absl_flags_private_handle_accessor
    absl_flags_program_name
    absl_flags_reflection
    absl_flags_usage
    absl_flags_usage_internal
    absl_log_flags
    absl_log_internal_fnmatch
    absl_string_view
    absl_vlog_config_internal
    gmock
    libprotobuf-lited
    libprotobufd
    libprotocd
    libupbd
    utf8_range
    utf8_validity
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