#ifndef _YUNMQTT_H
#define _YUNMQTT_H
#include <Arduino.h>
#include <Process.h>

class YunMqtt : public Process {

    public:
        void begin();
        void setHost(const String& accountName);
        void setPort(const int port);
        void setWillTopic(const String& topic);
        void setWill(const String& will);
        void setQos(const int qos);
        void addTopic(const String& topic);
        

        void start();
};

#endif 