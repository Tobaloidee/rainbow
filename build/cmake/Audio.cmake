if(ANDROID)
	# Just pass through.
elseif(EMSCRIPTEN)
	find_package(OpenAL REQUIRED)
	set(AUDIO_LIBRARIES ${OPENAL_LIBRARY})
elseif(USE_FMOD_STUDIO)
	add_definitions(-DUSE_FMOD_STUDIO)
	set(AUDIO_INCLUDE_DIRS ${LOCAL_LIBRARY}/FMOD/inc)
	if(WIN32)
		set(ARCH "windows")
		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(FMOD_LIBRARY_SUFFIX "64_vc.lib")
		else()
			set(FMOD_LIBRARY_SUFFIX "_vc.lib")
		endif()
	else()
		if(APPLE)
			set(ARCH "mac")
		else()
			if(CMAKE_SIZEOF_VOID_P EQUAL 8)
				set(ARCH "linux/x86_64")
			else()
				set(ARCH "linux/x86")
			endif()
		endif()
		set(FMOD_LIBRARY_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
	endif()
	set(FMOD_LIBRARY_PREFIX ${LOCAL_LIBRARY}/FMOD/lib/${ARCH}/${CMAKE_SHARED_LIBRARY_PREFIX})
	set(AUDIO_LIBRARIES ${FMOD_LIBRARY_PREFIX}fmod${FMOD_LIBRARY_SUFFIX}
	                    ${FMOD_LIBRARY_PREFIX}fmodstudio${FMOD_LIBRARY_SUFFIX})
	message(STATUS "Found FMOD Studio: ${AUDIO_LIBRARIES}")
else()
	if(WIN32)
		find_library(OPENAL_LIBRARY NAMES libOpenAL32 HINTS build/windows/lib)
		include_directories(build/windows/include)
	else()
		find_package(OpenAL REQUIRED)
		if(APPLE)
			find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox REQUIRED)
			set(PLATFORM_LIBRARIES ${AUDIOTOOLBOX_LIBRARY} ${PLATFORM_LIBRARIES})
		endif()
	endif()
	find_package(Vorbisfile REQUIRED)
	set(AUDIO_INCLUDE_DIRS ${VORBISFILE_INCLUDE_DIR})
	set(AUDIO_LIBRARIES ${VORBISFILE_LIBRARIES} ${OPENAL_LIBRARY})
endif()
