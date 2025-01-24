cmake_minimum_required(VERSION 3.1...3.15 FATAL_ERROR)

###################################################################################################
## These variables are passed to oatpp-module-install.cmake script
## use these variables to configure module installation

set(OATPP_THIS_MODULE_NAME oatpp-curl) ## name of the module (also name of folders in installation dirs)
set(OATPP_THIS_MODULE_VERSION "1.3.0") ## version of the module (also sufix of folders in installation dirs)
set(OATPP_THIS_MODULE_LIBRARIES oatpp-curl) ## list of libraries to find when find_package is called
set(OATPP_THIS_MODULE_TARGETS oatpp-curl) ## list of targets to install
set(OATPP_THIS_MODULE_DIRECTORIES oatpp-curl) ## list of directories to install

###################################################################################################

project(${OATPP_THIS_MODULE_NAME}
        VERSION ${OATPP_THIS_MODULE_VERSION}
        LANGUAGES CXX
        ## HOMEPAGE_URL "https://github.com/oatpp/oatpp-curl"
        ## DESCRIPTION "Something about curl"
)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(OATPP_DIR_SRC "Path to oatpp module directory (sources)")
option(OATPP_DIR_LIB "Path to directory with liboatpp (directory containing ex: liboatpp.so or liboatpp.dynlib)")
option(OATPP_BUILD_TESTS "Build tests for this module" ON)
option(OATPP_INSTALL "Install module binaries" ON)

option(OATPP_WITH_SHARED_LIBRARIES "Build shared version of the Oatpp libraries." OFF)
option(OATPP_WITH_VERBOSE_MESSAGE "Build Oatpp with verbose message." OFF)
option(OATPP_WITHOUT_INSTALL_ALL "Build Oatpp without installing all artifacts." OFF)
if(OATPP_WITH_SHARED_LIBRARIES)
    set(BUILD_SHARED_LIBS ON)
else()
    set(BUILD_SHARED_LIBS OFF)
endif()

set(OATPP_MODULES_LOCATION "SIBLING" CACHE STRING "Location where to find oatpp modules. can be [INSTALLED|EXTERNAL|SIBLING|CUSTOM]")

###################################################################################################
## get oatpp main module in specified location

set(OATPP_MODULES_LOCATION_INSTALLED INSTALLED)
set(OATPP_MODULES_LOCATION_EXTERNAL EXTERNAL)
set(OATPP_MODULES_LOCATION_SIBLING SIBLING)
set(OATPP_MODULES_LOCATION_CUSTOM CUSTOM)

if(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_INSTALLED)

    message("Finding oatpp in location=INSTALLED")

    find_package(oatpp ${OATPP_THIS_MODULE_VERSION} REQUIRED)

    get_target_property(OATPP_INCLUDE oatpp::oatpp INTERFACE_INCLUDE_DIRECTORIES)
    message("OATPP_INCLUDE=${OATPP_INCLUDE}")

    get_target_property(OATPP_TEST_INCLUDE oatpp::oatpp-test INTERFACE_INCLUDE_DIRECTORIES)
    message("OATPP_TEST_INCLUDE=${OATPP_TEST_INCLUDE}")

elseif(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_SIBLING)

    if(OATPP_WITH_VERBOSE_MESSAGE)
        message("Finding oatpp in location=SIBLING")
    endif()

    find_package(SiblingOatpp REQUIRED)

    get_target_property(OATPP_INCLUDE oatpp::oatpp INTERFACE_INCLUDE_DIRECTORIES)
    if(OATPP_WITH_VERBOSE_MESSAGE)
        message("OATPP_INCLUDE=${OATPP_INCLUDE}")
    endif()

    if(OATPP_BUILD_TESTS)
        get_target_property(OATPP_TEST_INCLUDE oatpp::oatpp-test INTERFACE_INCLUDE_DIRECTORIES)
        if(OATPP_WITH_VERBOSE_MESSAGE)
            message("OATPP_TEST_INCLUDE=${OATPP_TEST_INCLUDE}")
        endif()
    endif()

elseif(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_EXTERNAL)

    message("Finding oatpp in location=EXTERNAL")

    include(ExternalProject)

    set(MODULE_WAIT_DEPS ON)

    set(LIB_OATPP_EXTERNAL "lib_oatpp_external")
    ExternalProject_Add(${LIB_OATPP_EXTERNAL}
            GIT_REPOSITORY "https://github.com/oatpp/oatpp.git"
            GIT_TAG origin/master
            CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DOATPP_INSTALL=OFF -DOATPP_BUILD_TESTS=OFF
            INSTALL_COMMAND cmake -E echo "SKIP INSTALL '${LIB_OATPP_EXTERNAL}'"
    )

    ExternalProject_Get_Property(${LIB_OATPP_EXTERNAL} BINARY_DIR)
    set(OATPP_DIR_LIB ${BINARY_DIR}/src)

    ExternalProject_Get_Property(${LIB_OATPP_EXTERNAL} SOURCE_DIR)
    set(OATPP_DIR_SRC ${SOURCE_DIR}/src)

    message("OATPP_DIR_SRC --> '${OATPP_DIR_SRC}'")
    message("OATPP_DIR_LIB --> '${OATPP_DIR_LIB}'")

elseif(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_CUSTOM)

    message("Finding oatpp in location=CUSTOM")

    message("OATPP_DIR_SRC --> '${OATPP_DIR_SRC}'")
    message("OATPP_DIR_LIB --> '${OATPP_DIR_LIB}'")

else()
    message("FATAL_ERROR Unknown location to find oatpp '${OATPP_MODULES_LOCATION}'")
endif()

if(OATPP_DIR_LIB)
    link_directories(${OATPP_DIR_LIB})
endif()

###################################################################################################
## get dependencies

if(OATPP_WITH_VERBOSE_MESSAGE)
    message("\n############################################################################")
    message("## ${OATPP_THIS_MODULE_NAME} module. Resolving dependencies...\n")
endif()

##############################
## Find libcurl dependency

find_package(SiblingCURL)
if(CURL_FOUND)
    set(PKG_CURL_LIBRARIES ${CURL_LIBRARIES})
    set(PKG_CURL_INCLUDE_DIRS ${CURL_INCLUDE_DIRS})
else()
    include(FindPkgConfig)
    pkg_check_modules(PKG_CURL REQUIRED libcurl)

    if(OATPP_WITH_VERBOSE_MESSAGE)
        message("[libcurl] libcurl found:")
        message("[libcurl] LIBRARIES=${PKG_CURL_LIBRARIES}")
        message("[libcurl] LIBRARY_DIRS=${PKG_CURL_LIBRARY_DIRS}")
        message("[libcurl] INCLUDE_DIRS=${PKG_CURL_INCLUDE_DIRS}")
        message("[libcurl] VERSION=${PKG_CURL_VERSION}\n")
    endif()

    link_directories(
            ${PKG_CURL_LIBRARY_DIRS}
    )
endif()

if(OATPP_WITH_VERBOSE_MESSAGE)
    message("\n############################################################################\n")
endif()

###################################################################################################
## define targets

include(cmake/module-utils.cmake)

add_subdirectory("src")

if(OATPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory("test")
endif()
