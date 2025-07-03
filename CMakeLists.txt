cmake_minimum_required(VERSION 3.10)
project(DDS_PubSub)

configure_file(SmartCityTemperature.idl ${CMAKE_CURRENT_BINARY_DIR}/SmartCityTemperature.idl COPYONLY)
configure_file(DataReaderListener.cpp ${CMAKE_CURRENT_BINARY_DIR}/DataReaderListener.cpp COPYONLY)
configure_file(DataReaderListener.h ${CMAKE_CURRENT_BINARY_DIR}/DataReaderListener.h COPYONLY)

find_package(OpenDDS REQUIRED)

set(IDL_NAME SmartCityTemperature)

set(GENERATED_SRCS
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL_NAME}C.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL_NAME}C.h
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL_NAME}S.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL_NAME}S.h
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL_NAME}TypeSupport.idl
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL_NAME}TypeSupportC.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL_NAME}TypeSupportC.h
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL_NAME}TypeSupportImpl.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL_NAME}TypeSupportImpl.h
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL_NAME}TypeSupportS.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL_NAME}TypeSupportS.h
)

add_custom_command(
    OUTPUT ${GENERATED_SRCS}
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate_idl ${CMAKE_CURRENT_SOURCE_DIR}/${IDL_NAME}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${IDL_NAME}.idl
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating IDL files for ${IDL_NAME}"
)

add_custom_target(generate_idl ALL DEPENDS ${GENERATED_SRCS})


add_executable(publisher
    ${CMAKE_CURRENT_SOURCE_DIR}/publisher.cpp
    ${GENERATED_SRCS}
    ${CMAKE_CURRENT_BINARY_DIR}/DataReaderListener.cpp
)

target_include_directories(publisher PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(publisher PRIVATE OpenDDS::Dcps)
add_executable(subscriber
    ${CMAKE_CURRENT_SOURCE_DIR}/subscriber.cpp
    ${GENERATED_SRCS}
    ${CMAKE_CURRENT_BINARY_DIR}/DataReaderListener.cpp
)

target_include_directories(subscriber PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(subscriber PRIVATE OpenDDS::Dcps)
