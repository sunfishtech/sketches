#include <Process.h>
#include <YunMqtt.h>

void YunMqtt::begin(){
    Process::begin("mosquitto_sub");
}

void YunMqtt::setHost(const String& host) {
    Process::addParameter("-h\xFE" + host);
}

void YunMqtt::setPort(const int port){
    Process::addParameter("-p\xFE"+String(port));
}

void YunMqtt::setWillTopic(const String& willTopic){
    Process::addParameter("--will-topic\xFE"+willTopic);
}

void YunMqtt::setWill(const String& will){
    Process::addParameter("--will-payload\xFE"+will);
}

void YunMqtt::setQos(const int qos){
    Process::addParameter("-q\xFE"+String(qos));
}

void YunMqtt::addTopic(const String& topic){
    Process::addParameter("-t\xFE"+topic);
}

void YunMqtt::start(){
    Process:runAsynchronously();
}

