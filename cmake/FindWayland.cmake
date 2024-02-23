# Source: https://github.com/4J-company/mr-window/blob/master/cmake/FindWayland.cmake
# Source: https://github.com/KhronosGroup/Vulkan-Tools/blob/sdk-1.3.261/cmake/FindWayland.cmake

# Use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(PKG_WAYLAND QUIET wayland-client wayland-server)

SET(WAYLAND_DEFINITIONS ${PKG_WAYLAND_CFLAGS})

# Find include directories
FIND_PATH(WAYLAND_CLIENT_INCLUDE_DIR  NAMES wayland-client.h HINTS ${PKG_WAYLAND_INCLUDE_DIRS})
FIND_PATH(WAYLAND_SERVER_INCLUDE_DIR  NAMES wayland-server.h HINTS ${PKG_WAYLAND_INCLUDE_DIRS})

# Find library directories
FIND_LIBRARY(WAYLAND_CLIENT_LIBRARIES NAMES wayland-client libwayland-client  HINTS ${PKG_WAYLAND_LIBRARY_DIRS})
FIND_LIBRARY(WAYLAND_SERVER_LIBRARIES NAMES wayland-server libwayland-server  HINTS ${PKG_WAYLAND_LIBRARY_DIRS})

# Expose wayland-client includes and libs as new library
if(WAYLAND_CLIENT_INCLUDE_DIR AND WAYLAND_CLIENT_LIBRARY)
	add_library(wayland::client UNKNOWN IMPORTED)

	set_target_properties(
		wayland::client
		PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES "${WAYLAND_CLIENT_INCLUDE_DIR}"
			IMPORTED_LINK_INTERFACE_LANGUAGES "C"
			IMPORTED_LOCATION "${WAYLAND_CLIENT_LIBRARY}"
	)
endif()

# Expose wayland-server includes and libs as new library
if(WAYLAND_SERVER_INCLUDE_DIR AND WAYLAND_SERVER_LIBRARY)
	add_library(wayland::server UNKNOWN IMPORTED)

	set_target_properties(
		wayland::server
		PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${WAYLAND_SERVER_INCLUDE_DIR}"
		IMPORTED_LINK_INTERFACE_LANGUAGES "C"
		IMPORTED_LOCATION "${WAYLAND_SERVER_LIBRARY}"
	)
endif()

# Merge client and server directories into one
set(WAYLAND_INCLUDE_DIR ${WAYLAND_CLIENT_INCLUDE_DIR} ${WAYLAND_SERVER_INCLUDE_DIR})
list(REMOVE_DUPLICATES WAYLAND_INCLUDE_DIR)

# Merge client and server libraries into one
set(WAYLAND_LIBRARIES ${WAYLAND_CLIENT_LIBRARIES} ${WAYLAND_SERVER_LIBRARIES})

include(FindPackageHandleStandardArgs)
set(FPHSA_NAME_MISMATCHED ON)

# Expose variables for cmake for wayland client
find_package_handle_standard_args(WAYLAND_CLIENT
	REQUIRED_VARS 
		WAYLAND_CLIENT_LIBRARY
		WAYLAND_CLIENT_INCLUDE_DIR
)

# Expose variables for cmake for wayland server
find_package_handle_standard_args(WAYLAND_SERVER
	REQUIRED_VARS
		WAYLAND_SERVER_LIBRARY
		WAYLAND_SERVER_INCLUDE_DIR
)

# Expose variables for cmake for wayland all
find_package_handle_standard_args(WAYLAND
	DEFAULT_MSG
		WAYLAND_LIBRARIES
		WAYLAND_INCLUDE_DIR)

unset(FPHSA_NAME_MISMATCHED)

mark_as_advanced(
	WAYLAND_INCLUDE_DIR         WAYLAND_LIBRARIES
	WAYLAND_CLIENT_INCLUDE_DIR  WAYLAND_CLIENT_LIBRARY
	WAYLAND_SERVER_INCLUDE_DIR  WAYLAND_SERVER_LIBRARY
)