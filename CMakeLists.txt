cmake_minimum_required(VERSION 3.10)
project(open_dds_qos LANGUAGES CXX)

enable_testing()
find_package(OpenDDS REQUIRED)
include(opendds_testing)

# -------------------------------
# IDL: Generate TypeSupport files
# -------------------------------
set(IDL_NAME SmartCityTemperature)

# Generate source/header from IDL
add_library(${IDL_NAME}_idl)

opendds_target_sources(${IDL_NAME}_idl PUBLIC ${IDL_NAME}.idl)

# Include path untuk file hasil generate
target_include_directories(${IDL_NAME}_idl PUBLIC
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${IDL_NAME}_idl PUBLIC OpenDDS::Dcps)

# -------------------------------
# Shared Libraries
# -------------------------------
set(opendds_libs
  OpenDDS::Dcps
  OpenDDS::InfoRepoDiscovery
  OpenDDS::Tcp
  OpenDDS::Rtps
  OpenDDS::Rtps_Udp
  ${IDL_NAME}_idl
)

# -------------------------------
# Publisher Executable
# -------------------------------
add_executable(publisher
  Publisher.cpp
)

target_link_libraries(publisher PRIVATE ${opendds_libs})

# -------------------------------
# Subscriber Executable
# -------------------------------
add_executable(subscriber
  Subscriber.cpp
  DataReaderListenerImpl.cpp
)

target_link_libraries(subscriber PRIVATE ${opendds_libs})

# -------------------------------
# Test files
# -------------------------------
configure_file(run_test.pl . COPYONLY)
configure_file(rtps.ini . COPYONLY)

opendds_add_test(NAME info_repo COMMAND perl run_test.pl)
opendds_add_test(NAME rtps COMMAND perl run_test.pl --rtps)
