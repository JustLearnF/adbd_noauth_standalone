cmake_minimum_required(VERSION 3.24)

set(CROSS_COMPILE_PREFIX "aarch64-none-linux-gnu-")

set(CMAKE_C_COMPILER "${CROSS_COMPILE_PREFIX}gcc")
set(CMAKE_CXX_COMPILER "${CROSS_COMPILE_PREFIX}g++")

project (adbd C CXX)

add_definitions (
	-Os
	-Wall -Wno-unused-parameter -Wno-deprecated-declarations
	-Wno-all -Wno-incompatible-pointer-types -Wno-deprecated-declarations -Wno-implicit-function-declaration
	-D_GNU_SOURCE -D_XOPEN_SOURCE
)

set(SOURCES
 ${PROJECT_SOURCE_DIR}/adbd/adb.c
 
 ${PROJECT_SOURCE_DIR}/adbd/fdevent.c
 
 ${PROJECT_SOURCE_DIR}/adbd/transport.c
 ${PROJECT_SOURCE_DIR}/adbd/transport_local.c
 ${PROJECT_SOURCE_DIR}/adbd/transport_usb.c
 
 ${PROJECT_SOURCE_DIR}/adbd/sockets.c
 ${PROJECT_SOURCE_DIR}/adbd/sockets_libcutils.c
 
 ${PROJECT_SOURCE_DIR}/adbd/usb_linux_client.c
 
 ${PROJECT_SOURCE_DIR}/adbd/services.c
 ${PROJECT_SOURCE_DIR}/adbd/file_sync_service.c
 ${PROJECT_SOURCE_DIR}/adbd/priv_service.c

 ${PROJECT_SOURCE_DIR}/adbd/propd.c
 ${PROJECT_SOURCE_DIR}/adbd/led.c
)

include_directories (${PROJECT_SOURCE_DIR}/adbd/)

add_executable(adbd ${SOURCES})
target_link_libraries(adbd -lpthread)
target_link_options(adbd PUBLIC -static)

