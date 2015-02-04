# =====================================================================
# Define minimum cmake version
# =====================================================================
cmake_minimum_required(VERSION 2.8.6)


# =====================================================================
# Project Name
# =====================================================================
project (MyModifyXMP)


# =====================================================================
# Add source files to the project
# Here: ${SAMPLE_SOURCE_ROOT} = <xmpsdk>/samples/source/
# =====================================================================
file (GLOB SOURCE_FILES ${SAMPLE_SOURCE_ROOT}/MyModifyXMP.cpp)
source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Common Files" FILES ${COMMON_FILES})


# =====================================================================
# Add include directories to the project
# Here: ${XMP_SDK_ROOT} = <xmpsdk>
#       ${PUBLIC_INCLUDE}=<xmpsdk>/public/include
# =====================================================================
include_directories( ${XMP_ROOT} )
include_directories( ${PUBLIC_INCLUDE} )


# =====================================================================
# The base name of the executable to be created from the source files
# =====================================================================
add_executable(${PROJECT_NAME} ${SOURCE_FILES} )


# =====================================================================
# Set the XMP_BUILDMODE_DIR variable to Debug/Release
# =====================================================================
SetupInternalBuildDirectory()


# =====================================================================
# Add link paths for required libraries and set the output paths for all platforms
# Here we link all of the XMP libs, and set the output paths based on SAMPLE_SOURCE_ROOT
# =====================================================================
if(UNIX)

    if(APPLE) #For Mac
        target_link_libraries(${PROJECT_NAME}
${XMP_ROOT}/public/libraries/${PLATFORM_FOLDER}/${XMP_BUILDMODE_DIR}/lib${XMPCORE_LIB}${BUILD_MODE_LIBNAME}${LIB_EXT}
${XMP_ROOT}/public/libraries/${PLATFORM_FOLDER}/${XMP_BUILDMODE_DIR}/lib${XMPFILES_LIB}${BUILD_MODE_LIBNAME}${LIB_EXT} )
        set(OUTPUT_DIR ${SAMPLE_SOURCE_ROOT}/../target/${PLATFORM_FOLDER}/ )
        set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_DIR})
    else(APPLE) #For Linux
        SetPlatformLinkFlags(${PROJECT_NAME} "" "")
         target_link_libraries(${PROJECT_NAME}
${XMP_SDK_ROOT}/public/libraries/${PLATFORM_FOLDER}/${XMP_BUILDMODE_DIR}/${XMPCORE_LIB}${BUILD_MODE_LIBNAME}${LIB_EXT}
${XMP_SDK_ROOT}/public/libraries/${PLATFORM_FOLDER}/${XMP_BUILDMODE_DIR}/${XMPFILES_LIB}${BUILD_MODE_LIBNAME}${LIB_EXT} )
        set(OUTPUT_DIR ${SAMPLE_SOURCE_ROOT}/../target/${PLATFORM_FOLDER}/${XMP_BUILDMODE_DIR} )
        set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_DIR})
    endif(APPLE)

else(UNIX) #For Windows
    target_link_libraries(${PROJECT_NAME}
${XMP_SDK_ROOT}/public/libraries/${PLATFORM_FOLDER}/${XMP_BUILDMODE_DIR}/${XMPCORE_LIB}${BUILD_MODE_LIBNAME}${LIB_EXT}
${XMP_SDK_ROOT}/public/libraries/${PLATFORM_FOLDER}/${XMP_BUILDMODE_DIR}/${XMPFILES_LIB}${BUILD_MODE_LIBNAME}${LIB_EXT}Rpcrt4.lib)
        set(OUTPUT_DIR ${SAMPLE_SOURCE_ROOT}/../target/${PLATFORM_FOLDER}/ )
        set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_DIR})
endif(UNIX)

#adding Cocoa for Mac
ADD_FRAMEWORK(Cocoa ${PROJECT_NAME})
