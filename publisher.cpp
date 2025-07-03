#include "SmartCityTemperatureTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/PublisherImpl.h>
#include <dds/DCPS/WaitSet.h>
#include <dds/DCPS/StaticIncludes.h>
#include <ace/streams.h>
#include <vector>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        // Inisialisasi DomainParticipantFactory
        DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);

        // Membuat DomainParticipant
        DDS::DomainParticipant_var participant =
            dpf->create_participant(42, PARTICIPANT_QOS_DEFAULT, nullptr, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (!participant) {
            std::cerr << "Gagal membuat DomainParticipant." << std::endl;
            return 1;
        }

        // Registrasi TypeSupport
        SmartCity::TemperatureDataTypeSupport_var ts = new SmartCity::TemperatureDataTypeSupportImpl();
        if (ts->register_type(participant, "") != DDS::RETCODE_OK) {
            std::cerr << "Gagal mendaftarkan TypeSupport." << std::endl;
            return 1;
        }

        // Membuat Topik
        CORBA::String_var type_name = ts->get_type_name();
        DDS::Topic_var topic = participant->create_topic("SmartCityTemperatureTopic", type_name, TOPIC_QOS_DEFAULT, nullptr, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (!topic) {
            std::cerr << "Gagal membuat topik." << std::endl;
            return 1;
        }

        // Membuat Publisher
        DDS::Publisher_var publisher = participant->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (!publisher) {
            std::cerr << "Gagal membuat Publisher." << std::endl;
            return 1;
        }

        // Membuat DataWriter
        DDS::DataWriter_var writer = publisher->create_datawriter(topic, DATAWRITER_QOS_DEFAULT, nullptr, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (!writer) {
            std::cerr << "Gagal membuat DataWriter." << std::endl;
            return 1;
        }

        SmartCity::TemperatureDataDataWriter_var temp_writer = SmartCity::TemperatureDataDataWriter::_narrow(writer);
        if (!temp_writer) {
            std::cerr << "ERROR: DataWriter tidak dapat di-narrow." << std::endl;
            return 1;
        }

        // Data yang akan diterbitkan
        SmartCity::TemperatureData data_array[] = {
            {1, "New York", 25.5, "Celsius", 1672531200},
            {2, "Los Angeles", 30.2, "Celsius", 1672534800},
            {3, "Chicago", 15.8, "Celsius", 1672538400},
            {4, "Houston", 28.4, "Celsius", 1672542000},
            {5, "Phoenix", 35.1, "Celsius", 1672545600}
        };

        // Mengirimkan data satu per satu
        for (size_t i = 0; i < sizeof(data_array) / sizeof(data_array[0]); ++i) {
            DDS::ReturnCode_t result = temp_writer->write(data_array[i], DDS::HANDLE_NIL);
            if (result != DDS::RETCODE_OK) {
                std::cerr << "ERROR: Gagal mengirim data untuk kota: " << data_array[i].city.in() << std::endl;
            } else {
                std::cout << "Data berhasil diterbitkan: ID=" << data_array[i].id
                          << ", Kota=" << data_array[i].city.in()
                          << ", Suhu=" << data_array[i].temperature
                          << ", Satuan=" << data_array[i].unit.in()
                          << ", Timestamp=" << data_array[i].timestamp << std::endl;
            }
        }

        // Membersihkan entitas DDS
        participant->delete_contained_entities();
        dpf->delete_participant(participant);
        TheServiceParticipant->shutdown();
    } catch (const CORBA::Exception& e) {
        std::cerr << "Exception tertangkap: " << e << std::endl;
        return 1;
    }

    return 0;
}