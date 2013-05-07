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

include_directories(${GSOAP_INCLUDE_DIRS}
                    ${ARES_INCLUDE_DIRS}
                    ${GSOAP_PLUGIN_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS}
                    ${GLOBUS_CORE_INCLUDE_DIRS}
                    ${PROJECT_SOURCE_DIR}/interface
                    ${CMAKE_CURRENT_BINARY_DIR}/${AUTODIR})

add_custom_command(OUTPUT ${AUTODIR}/monitor_client/CEMON_CLIENTC.cpp
                          ${AUTODIR}/monitor_client/CEMON_CLIENTClient.cpp
                          ${AUTODIR}/monitor_consumer/CEMON_CONSUMERC.cpp
                          ${AUTODIR}/monitor_consumer/CEMON_CONSUMERServer.cpp
                   DEPENDS ${WSDL_PATH}/org.glite.ce-monitor_service.wsdl
                           ${WSDL_PATH}/org.glite.ce-monitor_consumer_service.wsdl
                           ${WSDL_PATH}/org.glite.ce-monitor_types.wsdl
                           ${WSDL_PATH}/org.glite.ce-monitor_faults.wsdl
                           ${WSDL_PATH}/org.glite.ce-faults.xsd
                   COMMAND mkdir -p ${AUTODIR}/glite/ce/monitor-client-api-c ${AUTODIR}/monitor_client
                   COMMAND wsdl2h -o ${AUTODIR}/glite/ce/monitor-client-api-c/monitor_service.h
                                  -t ${CMAKE_CURRENT_SOURCE_DIR}/typemap.dat
                                  ${WSDL_PATH}/org.glite.ce-monitor_service.wsdl
                   COMMAND soapcpp2 -I/usr/include -I/usr/include/gsoap -I/usr/share/gsoap/import
                                    -p CEMON_CLIENT -n -d ${AUTODIR}/monitor_client
                                    ${AUTODIR}/glite/ce/monitor-client-api-c/monitor_service.h
                   COMMAND sed 's|CEMON_CLIENTH.h|glite/ce/monitor-client-api-c/monitor_client_CEMON_CLIENTH.h|g'
                               ${AUTODIR}/monitor_client/CEMON_CLIENT.nsmap
                           > ${AUTODIR}/glite/ce/monitor-client-api-c/CEMON_CLIENT.nsmap
                   COMMAND cp ${AUTODIR}/monitor_client/CEMON_CLIENTStub.h
                              ${AUTODIR}/glite/ce/monitor-client-api-c/monitor_client_CEMON_CLIENTStub.h
                   COMMAND sed 's|CEMON_CLIENTStub.h|glite/ce/monitor-client-api-c/monitor_client_CEMON_CLIENTStub.h|g'
                               ${AUTODIR}/monitor_client/CEMON_CLIENTH.h
                           > ${AUTODIR}/glite/ce/monitor-client-api-c/monitor_client_CEMON_CLIENTH.h
                   COMMAND mkdir -p ${AUTODIR}/glite/ce/monitor-client-api-c ${AUTODIR}/monitor_consumer
                   COMMAND wsdl2h -o ${AUTODIR}/glite/ce/monitor-client-api-c/monitor_consumer_service.h
                                  -t ${CMAKE_CURRENT_SOURCE_DIR}/typemap.dat
                                  ${WSDL_PATH}/org.glite.ce-monitor_consumer_service.wsdl
                   COMMAND soapcpp2 -I/usr/include -I/usr/include/gsoap -I/usr/share/gsoap/import
                                    -p CEMON_CONSUMER -n -d ${AUTODIR}/monitor_consumer
                                    ${AUTODIR}/glite/ce/monitor-client-api-c/monitor_consumer_service.h
                   COMMAND sed 's|CEMON_CONSUMERH.h|glite/ce/monitor-client-api-c/monitor_consumer_CEMON_CONSUMERH.h|g'
                               ${AUTODIR}/monitor_consumer/CEMON_CONSUMER.nsmap
                           > ${AUTODIR}/glite/ce/monitor-client-api-c/CEMON_CONSUMER.nsmap
                   COMMAND cp ${AUTODIR}/monitor_consumer/CEMON_CONSUMERStub.h
                              ${AUTODIR}/glite/ce/monitor-client-api-c/monitor_consumer_CEMON_CONSUMERStub.h
                   COMMAND sed 's|CEMON_CONSUMERStub.h|glite/ce/monitor-client-api-c/monitor_consumer_CEMON_CONSUMERStub.h|g'
                               ${AUTODIR}/monitor_consumer/CEMON_CONSUMERH.h
                           > ${AUTODIR}/glite/ce/monitor-client-api-c/monitor_consumer_CEMON_CONSUMERH.h)

add_custom_target(cemonitor_stubs ALL DEPENDS ${AUTODIR}/monitor_consumer/CEMON_CONSUMERC.cpp
                                              ${AUTODIR}/monitor_consumer/CEMON_CONSUMERServer.cpp
                                              ${AUTODIR}/monitor_client/CEMON_CLIENTC.cpp
                                              ${AUTODIR}/monitor_client/CEMON_CLIENTClient.cpp)



add_library(glite_ce_monitor_client SHARED ${AUTODIR}/monitor_client/CEMON_CLIENTC.cpp
                                           ${AUTODIR}/monitor_client/CEMON_CLIENTClient.cpp
                                           Action.cpp
                                           Property.cpp
                                           Topic.cpp
                                           Dialect.cpp
                                           Parameter.cpp
                                           Policy.cpp
                                           Query.cpp
                                           AbsRequest.cpp
                                           CEEvent.cpp
                                           CEInfo.cpp
                                           CEPing.cpp
                                           CESubscription.cpp
                                           CESubscriptionMgr.cpp
                                           CETopics.cpp)


set_target_properties(glite_ce_monitor_client PROPERTIES VERSION 0.0.0 SOVERSION 0)

target_link_libraries(glite_ce_monitor_client ${GSOAP_LIBRARIES}
                                              ${GSOAP_PLUGIN_LIBRARIES}
                                              ${ARES_LIBRARIES}
                                              ${Boost_LIBRARIES})

add_library(glite_ce_monitor_consumer SHARED ${AUTODIR}/monitor_consumer/CEMON_CONSUMERC.cpp
                                             ${AUTODIR}/monitor_consumer/CEMON_CONSUMERServer.cpp
                                             CEConsumer.cpp
                                             Topic.cpp
                                             Dialect.cpp)

set_target_properties(glite_ce_monitor_consumer PROPERTIES VERSION 0.0.0 SOVERSION 0)

target_link_libraries(glite_ce_monitor_consumer ${GSOAP_LIBRARIES}
                                                ${GSOAP_PLUGIN_LIBRARIES}
                                                ${ARES_LIBRARIES}
                                                ${Boost_LIBRARIES})



install(TARGETS glite_ce_monitor_client
        LIBRARY DESTINATION usr/${LIBSUBDIR})

install(TARGETS glite_ce_monitor_consumer
        LIBRARY DESTINATION usr/${LIBSUBDIR})

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${AUTODIR}/glite/ce/monitor-client-api-c/
        DESTINATION usr/include/glite/ce/monitor-client-api-c
        FILES_MATCHING PATTERN "*.h")

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${AUTODIR}/glite/ce/monitor-client-api-c/
        DESTINATION usr/include/glite/ce/monitor-client-api-c
        FILES_MATCHING PATTERN "*.nsmap")

add_subdirectory(pkgconfig)


