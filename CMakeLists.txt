project(demo)
cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(WIN32)
  set(CMAKE_LIBRARY_PATH  "C:\\Program Files (x86)\\Windows Kits\\8.0\\Lib\\win8\\um\\x64")
  cmake_policy(SET CMP0020 NEW)   # to avoid cmake warning
endif()

#-------------------------------------------------------------------------------
# Find packages
#-------------------------------------------------------------------------------

# Qt5
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
if(UNIX)
  #find_package(Qt5X11Extras REQUIRED)
endif()

# OpenGL
find_package(OpenGL REQUIRED)

# Boost
if(UNIX)
  #find_package(Qt5X11Extras REQUIRED)
  find_package(Boost REQUIRED COMPONENTS system thread)
endif()

# Ogre3d
# setup some possible locations for OGRE
if(WIN32)
  getenv_path(OGRE_HOME)
  getenv_path(OGRE_SDK)
  getenv_path(PROGRAMFILES)

  set(OGRE_ROOT_GUESSES
    ${ENV_PROGRAMFILES}/OGRE
    C:/OgreSDK
    $ENV_OGRE_HOME
    $ENV_OGRE_SDK
    ${OGRE_HOME}
    ${OGRE_SDK}
  )
elseif(UNIX)
  set(OGRE_ROOT_GUESSES
    ${OGRE_HOME}
    ${OGRE_SDK}
    /opt/ogre
    /opt/OGRE
    /usr/lib/OGRE/cmake
    /usr/lib${LIB_SUFFIX}/ogre
    /usr/lib${LIB_SUFFIX}/OGRE
    /usr/local/lib/OGRE/cmake
    /usr/local/lib${LIB_SUFFIX}/ogre
    /usr/local/lib${LIB_SUFFIX}/OGRE
    /usr/share/OGRE/cmake
    /usr/share/OGRE/cmake/modules
    $ENV{HOME}/ogre
    $ENV{HOME}/OGRE
    $ENV_OGRE_HOME
    $ENV_OGRE_SDK
    $ENV{OGRE_HOME}/CMake/
  )
endif()

# find Ogre cmake modules path
find_path(OGRE_CMAKE_MODULE_PATH FindOGRE.cmake
    HINTS ${OGRE_ROOT_GUESSES}
)
if(OGRE_CMAKE_MODULE_PATH-NOTFOUND)
  message(SEND_ERROR "Failed to find OGRE module path.")
else()
  set(CMAKE_MODULE_PATH ${OGRE_CMAKE_MODULE_PATH} ${CMAKE_MODULE_PATH})
endif()

find_package(OGRE  REQUIRED)
find_package(OIS REQUIRED)

message("OGRE_LIBRARY_DIR_REL: " ${OGRE_LIBRARY_DIR_REL})
message("OGRE_LIBRARY_DIR_DBG: " ${OGRE_LIBRARY_DIR_DBG})
message("OGRE_LIBRARY_DIRS: " ${OGRE_LIBRARY_DIRS})

set(OGRE_RESOURCE_FOLDER ${PROJECT_SOURCE_DIR}/resources)

if(UNIX)
  include_directories(/usr/include/OGRE)
  set(BOOST_LINK_LIBS
      boost_system
      boost_thread
  )
  set(OGRE_PLUGIN_FOLDER "${OGRE_LIBRARY_DIR_DBG}/OGRE-1.9.0")
elseif(WIN32)
  include_directories(
      "${OGRE_HOME}\\include"
      "${OGRE_HOME}\\include\\OGRE"
      "${OGRE_HOME}\\include\\OGRE\\RenderSystems\\GL"
      "${OGRE_HOME}\\boost"
  )
  set(BOOST_LINK_LIBS
      libboost_system-vc100-mt-1_55
      libboost_thread-vc100-mt-1_55
  )

  link_directories(
      ${LINK_DIRECORIES}
      "${OGRE_HOME}\\lib\\Release"
      "${OGRE_HOME}\\boost\\lib"
  )
  set(OGRE_PLUGIN_FOLDER "${OGRE_HOME}\\bin\\Release")
endif()

add_executable(QOgreWidgetDemo   
  QOgreWidget.cpp 
  QOgreWidgetDemo.cpp
  main.cpp
  )

if(UNIX)
  #qt5_use_modules(QOgreWidgetDemo OpenGL Widgets X11Extras)
  qt5_use_modules(QOgreWidgetDemo OpenGL Widgets)
elseif(WIN32)
  qt5_use_modules(QOgreWidgetDemo OpenGL Widgets)
endif()

link_directories("${CMAKE_INSTALL_PREFIX}/lib")
link_directories("${OGRE_LIBRARY_DIRS}")

target_link_libraries(QOgreWidgetDemo
    ${OPENGL_LIBRARIES}
    ${BOOST_LINK_LIBS}
    ${OGRE_LIBRARIES}
  )

configure_file(
  "${PROJECT_SOURCE_DIR}/plugins.cfg.in"
  "${PROJECT_BINARY_DIR}/plugins.cfg"
  )

configure_file(
  "${PROJECT_SOURCE_DIR}/resources.cfg.in"
  "${PROJECT_BINARY_DIR}/resources.cfg"
)

install(TARGETS QOgreWidgetDemo RUNTIME DESTINATION bin)
