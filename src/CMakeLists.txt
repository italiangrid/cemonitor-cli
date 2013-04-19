#
#  Copyright (c) Members of the EGEE Collaboration. 2010.
#  See http://www.eu-egee.org/partners/ for details on the copyright
#  holders.
#  
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  
#     http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
#  implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#  

set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")
set(AUTODIR "autogen")

if(NOT DEFINED WSDL_PATH)
    message("Undefined variable WSDL_PATH, used /usr/share/wsdl/cream-ce")
    set(WSDL_PATH /usr/share/wsdl/cream-ce)
endif(NOT DEFINED WSDL_PATH)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSOAP gsoapssl++)
pkg_check_modules(ARES libcares)
pkg_check_modules(GSOAP_PLUGIN glite-security-gsoap-plugin++)
pkg_check_modules(GLOBUS_OPENSSL_MODULE globus-openssl-module)
pkg_check_modules(GLOBUS_CALLOUT globus-callout)
pkg_check_modules(GLOBUS_COMMON globus-common)
pkg_check_modules(GLOBUS_CORE globus-core)

if( ${CMAKE_VERSION} VERSION_LESS "2.8.2" )
    if( IS_DIRECTORY "/usr/lib64" )
        set(LIBSUBDIR "lib64")
    else( IS_DIRECTORY "/usr/lib64" )
        set(LIBSUBDIR "lib")
    endif( IS_DIRECTORY "/usr/lib64" )
else( ${CMAKE_VERSION} VERSION_LESS "2.8.2" )
    if( IS_DIRECTORY "/usr/lib64" AND NOT IS_SYMLINK "/usr/lib64" )
        set(LIBSUBDIR "lib64")
    else( IS_DIRECTORY "/usr/lib64" AND NOT IS_SYMLINK "/usr/lib64" )
        set(LIBSUBDIR "lib")
    endif( IS_DIRECTORY "/usr/lib64" AND NOT IS_SYMLINK "/usr/lib64" )
endif( ${CMAKE_VERSION} VERSION_LESS "2.8.2" )

if( NOT DEFINED BOOST_LIBRARYDIR AND IS_DIRECTORY "/usr/lib64" )
    set(BOOST_LIBRARYDIR "/usr/lib64")
endif( NOT DEFINED BOOST_LIBRARYDIR AND IS_DIRECTORY "/usr/lib64" )

set(Boost_USE_MULTITHREADED ON)
find_package( Boost COMPONENTS filesystem regex )

add_definitions( -pipe )

set(COMMON_LIBS ${GSOAP_LIBRARIES}
                ${GSOAP_PLUGIN_LIBRARIES}
                ${ARES_LIBRARIES}
                ${Boost_LIBRARIES})

include_directories(${GSOAP_INCLUDE_DIRS}
                    ${ARES_INCLUDE_DIRS}
                    ${GSOAP_PLUGIN_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS}
                    ${GLOBUS_CORE_INCLUDE_DIRS}
                    ${PROJECT_SOURCE_DIR}/interface
                    ${CMAKE_CURRENT_BINARY_DIR}/api/${AUTODIR})

link_directories(${CMAKE_BINARY_DIR}/api
                 ${Boost_LIBRARY_DIRS}
                 ${GSOAP_PLUGIN_LIBRARY_DIRS})

add_executable(glite-ce-monitor-consumer glite-ce-monitor-consumer.cpp)
target_link_libraries(glite-ce-monitor-consumer ${COMMON_LIBS}
                                                glite_ce_monitor_consumer) 

add_executable(glite-ce-monitor-getevent glite-ce-monitor-getevent.cpp cliUtils.cpp)
target_link_libraries(glite-ce-monitor-getevent ${COMMON_LIBS}
                                                glite_ce_monitor_client) 

add_executable(glite-ce-monitor-getinfo glite-ce-monitor-getinfo.cpp cliUtils.cpp)
target_link_libraries(glite-ce-monitor-getinfo ${COMMON_LIBS}
                                               glite_ce_monitor_client) 

add_executable(glite-ce-monitor-gettopics glite-ce-monitor-gettopics.cpp cliUtils.cpp)
target_link_libraries(glite-ce-monitor-gettopics ${COMMON_LIBS}
                                                 glite_ce_monitor_client) 

add_executable(glite-ce-monitor-pause glite-ce-monitor-pause.cpp cliUtils.cpp)
target_link_libraries(glite-ce-monitor-pause ${COMMON_LIBS}
                                             glite_ce_monitor_client) 

add_executable(glite-ce-monitor-resume glite-ce-monitor-resume.cpp cliUtils.cpp)
target_link_libraries(glite-ce-monitor-resume ${COMMON_LIBS}
                                              glite_ce_monitor_client) 

add_executable(glite-ce-monitor-subinfo glite-ce-monitor-subinfo.cpp cliUtils.cpp)
target_link_libraries(glite-ce-monitor-subinfo ${COMMON_LIBS}
                                               glite_ce_monitor_client) 

add_executable(glite-ce-monitor-subscribe glite-ce-monitor-subscribe.cpp cliUtils.cpp)
target_link_libraries(glite-ce-monitor-subscribe ${COMMON_LIBS}
                                                 glite_ce_monitor_client) 

add_executable(glite-ce-monitor-subupdate glite-ce-monitor-subupdate.cpp cliUtils.cpp)
target_link_libraries(glite-ce-monitor-subupdate ${COMMON_LIBS}
                                                 glite_ce_monitor_client) 

add_executable(glite-ce-monitor-unsubscribe glite-ce-monitor-unsubscribe.cpp cliUtils.cpp)
target_link_libraries(glite-ce-monitor-unsubscribe ${COMMON_LIBS}
                                                   glite_ce_monitor_client) 

install(TARGETS glite-ce-monitor-consumer
                glite-ce-monitor-getevent
                glite-ce-monitor-getinfo
                glite-ce-monitor-gettopics
                glite-ce-monitor-pause
                glite-ce-monitor-resume
                glite-ce-monitor-subinfo
                glite-ce-monitor-subscribe
                glite-ce-monitor-subupdate
                glite-ce-monitor-unsubscribe
        RUNTIME DESTINATION usr/bin)


add_subdirectory(api)


