# OpenDDS Basic Pub/Sub Example

This repository provides a simple demonstration of a Publisher/Subscriber (Pub/Sub) application using [OpenDDS](https://opendds.org/) in C++. It features a `SmartCityTemperature::TemperatureData` topic defined in IDL and illustrates how to publish and subscribe to messages with OpenDDS.

## Directory Overview

- **SmartCityTemperature.idl**: Defines the `TemperatureData` topic in IDL.
- **publisher.cpp**: Sends `TemperatureData` samples.
- **subscriber.cpp**: Receives and displays `TemperatureData` samples.
- **SmartCityTemperatureReaderListener.h/cpp**: Implements the listener for the subscriber's DataReader.
- **CMakeLists.txt**: Configuration file for building the project.
- **generate_idl**: Script to generate OpenDDS and TAO code from the IDL file.
- **rtps.ini**: Sample configuration for OpenDDS TCP transport.

## Build Steps

1. **Generate IDL files:**
   ```sh
   ./generate_idl SmartCityTemperature
   ```

2. **Build the project:**
   ```sh
   mkdir -p build
   cd build
   cmake ..
   make
   ```

## How to Run

Use **three separate terminals** to execute the following commands:

1. **Terminal 1: Launch the OpenDDS InfoRepo**
   ```sh
   $DDS_ROOT/dds/InfoRepo/DCPSInfoRepo -ORBEndpoint iiop://localhost:12345 -d domain_ids
   ```

2. **Terminal 2: Start the Publisher**
   ```sh
   ./build/publisher -DCPSConfigFile ../rtps.ini
   ```

3. **Terminal 3: Start the Subscriber**
   ```sh
   ./build/subscriber -DCPSConfigFile ../rtps.ini
   ```

---

This example is designed for educational purposes and to explore OpenDDS Pub/Sub functionality.