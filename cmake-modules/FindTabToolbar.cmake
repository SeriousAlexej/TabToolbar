set(TabToolbar_ROOT "" CACHE PATH "Path to TabToolbar root directory.")

set(TabToolbar_FIND_DIRS
    ${TabToolbar_ROOT}
    $ENV{TabToolbar_ROOT}
    /usr/local
    /usr
    /opt/local
    /opt
)

find_path(TabToolbar_INCLUDE_DIR
          TabToolbar/TabToolbar.h
          PATH_SUFFIXES include
          PATHS ${TabToolbar_FIND_DIRS}
)

# release library
find_library(TabToolbar_LIBRARY_RELEASE
             NAMES TabToolbar libTabToolbar
			 PATH_SUFFIXES lib64 lib
			 PATHS ${TabToolbar_FIND_DIRS})

# debug library
find_library(TabToolbar_LIBRARY_DEBUG
			 NAMES TabToolbard libTabToolbard
			 PATH_SUFFIXES lib64 lib
			 PATHS ${TabToolbar_FIND_DIRS})

if (TabToolbar_LIBRARY_DEBUG OR TabToolbar_LIBRARY_RELEASE)
	set(TabToolbar_FOUND TRUE)

	# if both are found, set TabToolbar_LIBRARY to contain both
	if (TabToolbar_LIBRARY_DEBUG AND TabToolbar_LIBRARY_RELEASE)
		set(TabToolbar_LIBRARY debug     ${TabToolbar_LIBRARY_DEBUG}
							   optimized ${TabToolbar_LIBRARY_RELEASE})
	endif()

	# if only one debug/release variant is found, set the other to be equal to the found one
	if (TabToolbar_LIBRARY_DEBUG AND NOT TabToolbar_LIBRARY_RELEASE)
		# debug and not release
		set(TabToolbar_LIBRARY_RELEASE ${TabToolbar_LIBRARY_DEBUG})
		set(TabToolbar_LIBRARY         ${TabToolbar_LIBRARY_DEBUG})
	endif()
	if (TabToolbar_LIBRARY_RELEASE AND NOT TabToolbar_LIBRARY_DEBUG)
		# release and not debug
		set(TabToolbar_LIBRARY_DEBUG ${TabToolbar_LIBRARY_RELEASE})
		set(TabToolbar_LIBRARY       ${TabToolbar_LIBRARY_RELEASE})
	endif()
else()
	set(TabToolbar_FOUND FALSE)
endif()

MARK_AS_ADVANCED(TabToolbar_LIBRARY
				 TabToolbar_LIBRARY_RELEASE
				 TabToolbar_LIBRARY_DEBUG
)

if (TabToolbar_FOUND)
  if (NOT TabToolbar_FIND_QUIETLY)
	message(STATUS "Found TabToolbar library: ${TabToolbar_LIBRARY}")
  endif (NOT TabToolbar_FIND_QUIETLY)
else (TabToolbar_FOUND)
  if (TabToolbar_FIND_REQUIRED)
	message(FATAL_ERROR "Could not find TabToolbar library")
  endif (TabToolbar_FIND_REQUIRED)
endif (TabToolbar_FOUND)
