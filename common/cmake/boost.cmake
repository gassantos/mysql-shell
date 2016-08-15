# Copyright (c) 2015, Oracle and/or its affiliates. All rights reserved.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#
#  Setup boost for targets in given folder and its sub-folders.
#
MACRO(SETUP_BOOST)
  INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIR})
  LINK_DIRECTORIES(${BOOST_LIB_DIR})
  IF(WIN32)
    #
    # Otherwise boost headers generate warnings
    #
    ADD_DEFINITIONS(-D_WIN32_WINNT=0x0501)
  ENDIF()
ENDMACRO(SETUP_BOOST)

#
# Add minimal required boost libraries to given target. This assumes that
# Boost was set-up in given folder.
#
MACRO(ADD_BOOST target)
  IF(NOT WIN32)
    TARGET_LINK_LIBRARIES(${target} -lboost_system)
  ENDIF()
ENDMACRO(ADD_BOOST)


#
# The minimal required version of boost: major version must match, minor version
# can be later than required.
#
SET(BOOST_REQUIRED_VERSION_MAJOR 1)
SET(BOOST_REQUIRED_VERSION_MINOR 55)


# The boost tarball is fairly big, and takes several minutes
# to download. So we recommend downloading/unpacking it
# only once, in a place visible from any bzr sandbox.
# We use only header files, so there should be no binary dependencies.

# Downloading the tarball takes about 5 minutes here at the office.
# Here are some size/time data for unpacking the tarball on my desktop:
#  size tarball-name
#  67M boost_1_55_0.tar.gz  unzipping headers    ~2 seconds 117M
#                           unzipping everything ~3 seconds 485M
# 8,8M boost_headers.tar.gz unzipping everything <1 second

SET(BOOST_PACKAGE_NAME "boost_${BOOST_REQUIRED_VERSION_MAJOR}_${BOOST_REQUIRED_VERSION_MINOR}_0")
SET(BOOST_TARBALL "${BOOST_PACKAGE_NAME}.tar.gz")
SET(BOOST_DOWNLOAD_URL
  "http://sourceforge.net/projects/boost/files/boost/1.55.0/${BOOST_TARBALL}"
  )

MACRO(COULD_NOT_FIND_BOOST)
  MESSAGE(STATUS "Could not find (the correct version of) boost.\n")
  MESSAGE(FATAL_ERROR
    "You can download it with -DDOWNLOAD_BOOST=1 -DWITH_BOOST=<directory>\n"
    "This CMake script will look for boost in <directory>. "
    "If it is not there, it will download and unpack it "
    "(in that directory) for you.\n"
    "If you are inside a firewall, you may need to use an http proxy:\n"
    "export http_proxy=http://example.com:80\n"
    )
ENDMACRO()

# Pick value from environment if not set on command line.
IF(DEFINED ENV{WITH_BOOST} AND NOT DEFINED WITH_BOOST)
  SET(WITH_BOOST "$ENV{WITH_BOOST}")
ENDIF()

# Pick value from environment if not set on command line.
IF(DEFINED ENV{BOOST_ROOT} AND NOT DEFINED WITH_BOOST)
  SET(WITH_BOOST "$ENV{BOOST_ROOT}")
ENDIF()

# Update the cache, to make it visible in cmake-gui.
SET(WITH_BOOST ${WITH_BOOST} CACHE PATH
  "Path to boost sources: a directory, or a tarball to be unzipped.")

# If the value of WITH_BOOST changes, we must unset all dependent variables:
IF(OLD_WITH_BOOST)
  IF(NOT "${OLD_WITH_BOOST}" STREQUAL "${WITH_BOOST}")
    UNSET(BOOST_INCLUDE_DIR)
    UNSET(BOOST_INCLUDE_DIR CACHE)
    UNSET(LOCAL_BOOST_DIR)
    UNSET(LOCAL_BOOST_DIR CACHE)
    UNSET(LOCAL_BOOST_ZIP)
    UNSET(LOCAL_BOOST_ZIP CACHE)
  ENDIF()
ENDIF()

SET(OLD_WITH_BOOST ${WITH_BOOST} CACHE INTERNAL
  "Previous version of WITH_BOOST" FORCE)

IF (WITH_BOOST)
  ## Did we get a full path name, including file name?
  IF (${WITH_BOOST} MATCHES ".*\\.tar.gz" OR ${WITH_BOOST} MATCHES ".*\\.zip")
    GET_FILENAME_COMPONENT(BOOST_DIR ${WITH_BOOST} PATH)
    GET_FILENAME_COMPONENT(BOOST_ZIP ${WITH_BOOST} NAME)
    FIND_FILE(LOCAL_BOOST_ZIP
              NAMES ${BOOST_ZIP}
              PATHS ${BOOST_DIR}
              NO_DEFAULT_PATH
             )
  ENDIF()
  ## Did we get a path name to the directory of the .tar.gz or .zip file?
  FIND_FILE(LOCAL_BOOST_ZIP
            NAMES "${BOOST_PACKAGE_NAME}.tar.gz" "${BOOST_PACKAGE_NAME}.zip"
            PATHS ${WITH_BOOST}
            NO_DEFAULT_PATH
           )
  ## Did we get a path name to the directory of an unzipped version?
  FIND_FILE(LOCAL_BOOST_DIR
            NAMES "${BOOST_PACKAGE_NAME}"
            PATHS ${WITH_BOOST}
            NO_DEFAULT_PATH
           )
  ## Did we get a path name to an unzippped version?
  FIND_PATH(LOCAL_BOOST_DIR
            NAMES "boost/version.hpp"
            PATHS ${WITH_BOOST}
            NO_DEFAULT_PATH
           )
  MESSAGE(STATUS "Local boost dir ${LOCAL_BOOST_DIR}")
  MESSAGE(STATUS "Local boost zip ${LOCAL_BOOST_ZIP}")
ENDIF()

# There is a similar option in unittest/gunit.
# But the boost tarball is much bigger, so we have a separate option.
OPTION(DOWNLOAD_BOOST "Download boost from sourceforge." OFF)

# If we could not find it, then maybe download it.
IF(WITH_BOOST AND NOT LOCAL_BOOST_ZIP AND NOT LOCAL_BOOST_DIR)
  IF(NOT DOWNLOAD_BOOST)
    MESSAGE(STATUS "WITH_BOOST=${WITH_BOOST}")
    COULD_NOT_FIND_BOOST()
  ENDIF()
  # Download the tarball
  MESSAGE(STATUS "Downloading ${BOOST_TARBALL} to ${WITH_BOOST}")
  FILE(DOWNLOAD ${BOOST_DOWNLOAD_URL}
    ${WITH_BOOST}/${BOOST_TARBALL}
    TIMEOUT 600
    STATUS ERR
    SHOW_PROGRESS
  )
  IF(ERR EQUAL 0)
    SET(LOCAL_BOOST_ZIP "${WITH_BOOST}/${BOOST_TARBALL}")
    SET(LOCAL_BOOST_ZIP "${WITH_BOOST}/${BOOST_TARBALL}" CACHE INTERNAL "")
  ELSE()
    MESSAGE(STATUS "Download failed, error: ${ERR}")
    # A failed DOWNLOAD leaves an empty file, remove it
    FILE(REMOVE ${WITH_BOOST}/${BOOST_TARBALL})
  ENDIF()
ENDIF()

IF(LOCAL_BOOST_ZIP AND NOT LOCAL_BOOST_DIR)
  GET_FILENAME_COMPONENT(LOCAL_BOOST_DIR ${LOCAL_BOOST_ZIP} PATH)
  IF(NOT EXISTS "${LOCAL_BOOST_DIR}/${BOOST_PACKAGE_NAME}")
    MESSAGE(STATUS "cd ${LOCAL_BOOST_DIR}; tar xfz ${LOCAL_BOOST_ZIP}")
    EXECUTE_PROCESS(
      COMMAND ${CMAKE_COMMAND} -E tar xfz "${LOCAL_BOOST_ZIP}"
      WORKING_DIRECTORY "${LOCAL_BOOST_DIR}"
      RESULT_VARIABLE tar_result
      )
    IF (tar_result MATCHES 0)
      SET(BOOST_FOUND 1 CACHE INTERNAL "")
    ELSE()
      MESSAGE(STATUS "WITH_BOOST ${WITH_BOOST}.")
      MESSAGE(STATUS "Failed to extract files.\n"
        "   Please try downloading and extracting yourself.\n"
        "   The url is: ${BOOST_DOWNLOAD_URL}")
      MESSAGE(FATAL_ERROR "Giving up.")
    ENDIF()
  ENDIF()
ENDIF()

# Search for the version file, first in LOCAL_BOOST_DIR or WITH_BOOST
FIND_PATH(BOOST_INCLUDE_DIR
  NAMES boost/version.hpp
  NO_DEFAULT_PATH
  PATHS ${LOCAL_BOOST_DIR}
        ${LOCAL_BOOST_DIR}/${BOOST_PACKAGE_NAME}
        ${WITH_BOOST}
)
# Then search in standard places (if not found above).
FIND_PATH(BOOST_INCLUDE_DIR
  NAMES boost/version.hpp
)

IF(NOT BOOST_INCLUDE_DIR)
  MESSAGE(STATUS
    "Looked for boost/version.hpp in ${LOCAL_BOOST_DIR} and ${WITH_BOOST}")
  COULD_NOT_FIND_BOOST()
ENDIF()

# Verify version number. Version information looks like:
# //  BOOST_VERSION % 100 is the patch level
# //  BOOST_VERSION / 100 % 1000 is the minor version
# //  BOOST_VERSION / 100000 is the major version
# #define BOOST_VERSION 105500
FILE(STRINGS "${BOOST_INCLUDE_DIR}/boost/version.hpp"
  BOOST_VERSION_NUMBER
  REGEX "^#define[\t ]+BOOST_VERSION[\t ][0-9]+.*"
)
STRING(REGEX REPLACE
  "^.*BOOST_VERSION[\t ]([0-9][0-9])([0-9][0-9])([0-9][0-9]).*$" "\\1"
  BOOST_MAJOR_VERSION "${BOOST_VERSION_NUMBER}")
STRING(REGEX REPLACE
  "^.*BOOST_VERSION[\t ]([0-9][0-9])([0-9][0-9])([0-9][0-9]).*$" "\\2"
  BOOST_MINOR_VERSION "${BOOST_VERSION_NUMBER}")

MESSAGE(STATUS "BOOST_VERSION_NUMBER is ${BOOST_VERSION_NUMBER}")

IF(NOT BOOST_MAJOR_VERSION EQUAL 10)
  COULD_NOT_FIND_BOOST()
ENDIF()

IF(BOOST_MINOR_VERSION LESS 55)
  MESSAGE(WARNING "Boost minor version found is ${BOOST_MINOR_VERSION} "
    "we need at least 55"
    )
  COULD_NOT_FIND_BOOST()
ENDIF()

MESSAGE(STATUS "BOOST_INCLUDE_DIR ${BOOST_INCLUDE_DIR}")

#
# TODO: proper search for boost libraries (or, try using headers only)
#

SET(BOOST_LIB_DIR ${BOOST_INCLUDE_DIR}/lib)
MESSAGE(STATUS "BOOST_LIB_DIR ${BOOST_LIB_DIR}")

