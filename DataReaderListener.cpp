#include "DataReaderListener.h"
#include <ace/streams.h>

void DataReaderListener::on_data_available(DDS::DataReader_ptr reader) {
  SmartCity::TemperatureDataDataReader_var temp_reader = SmartCity::TemperatureDataDataReader::_narrow(reader);
  if (!temp_reader) return;

  SmartCity::TemperatureData temp_data;
  DDS::SampleInfo info;
  while (temp_reader->take_next_sample(temp_data, info) == DDS::RETCODE_OK) {
    if (info.valid_data) {
        ACE_DEBUG((LM_INFO, ACE_TEXT("Received: id=%d, city=%C, temperature=%.2f, unit=%C, timestamp=%llu\n"),
            temp_data.id, temp_data.city.in(), temp_data.temperature, temp_data.unit.in(), temp_data.timestamp));
    } else {
        ACE_DEBUG((LM_WARNING, ACE_TEXT("Sample received but not valid.\n")));
    }
  }
}