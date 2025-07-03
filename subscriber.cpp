#include "SmartCityTemperatureTypeSupportImpl.h"
#include "DataReaderListener.h"
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <ace/streams.h>
#include <ace/OS_NS_unistd.h>

int main(int argc, char* argv[]) {
    try {
        // Inisialisasi DomainParticipantFactory
        DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);

        // Membuat DomainParticipant untuk domain ID 42
        DDS::DomainParticipant_var participant = dpf->create_participant(
            42, PARTICIPANT_QOS_DEFAULT, nullptr, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (!participant) {
            std::cerr << "ERROR: Gagal membuat DomainParticipant!" << std::endl;
            return 1;
        }

        // Registrasi TypeSupport untuk TemperatureData
        SmartCity::TemperatureDataTypeSupport_var ts = new SmartCity::TemperatureDataTypeSupportImpl();
        if (ts->register_type(participant, "") != DDS::RETCODE_OK) {
            std::cerr << "ERROR: Gagal mendaftarkan TypeSupport!" << std::endl;
            return 1;
        }

        // Membuat topik untuk data suhu
        CORBA::String_var type_name = ts->get_type_name();
        DDS::Topic_var topic = participant->create_topic(
            "SmartCityTemperatureTopic", type_name, TOPIC_QOS_DEFAULT, nullptr, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (!topic) {
            std::cerr << "ERROR: Gagal membuat topik!" << std::endl;
            return 1;
        }

        // Membuat Subscriber
        DDS::Subscriber_var subscriber = participant->create_subscriber(
            SUBSCRIBER_QOS_DEFAULT, nullptr, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (!subscriber) {
            std::cerr << "ERROR: Gagal membuat Subscriber!" << std::endl;
            return 1;
        }

        // Membuat DataReader dengan listener khusus
        DDS::DataReaderListener_var listener(new DataReaderListener());
        DDS::DataReader_var reader = subscriber->create_datareader(
            topic, DATAREADER_QOS_DEFAULT, listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (!reader) {
            std::cerr << "ERROR: Gagal membuat DataReader!" << std::endl;
            return 1;
        }

        // Menunggu data diterima
        std::cout << "Subscriber aktif. Menunggu data..." << std::endl;
        for (int i = 0; i < 60; ++i) { // Tunggu selama 60 detik
            ACE_OS::sleep(1);
        }

        // Membersihkan entitas DDS
        std::cout << "Membersihkan entitas DDS..." << std::endl;
        participant->delete_contained_entities();
        dpf->delete_participant(participant);
        TheServiceParticipant->shutdown();

    } catch (const CORBA::Exception& e) {
        std::cerr << "Exception caught: " << e << std::endl;
        return 1;
    }

    return 0;
}