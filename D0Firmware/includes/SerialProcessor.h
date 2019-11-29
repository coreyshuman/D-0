
#include <Print.h>

#ifndef __SERIALPROCESSOR_H
#define __SERIALPROCESSOR_H

#define BUFFER_LENGTH_TX        64
#define BUFFER_LENGTH_RX        64
#define BUFFER_LENGTH_PARSED    64

#define PROCESSED_CALLBACK_FUNCTION(func, payload, length)  void (*func)(char * payload, uint8_t length)



class SerialProcessor : public Print {
    public:
        void setup();
        void loop(int test);
        void registerProcessedCallback(PROCESSED_CALLBACK_FUNCTION(func,,));
        virtual size_t write(uint8_t);
        virtual size_t write(const uint8_t *buffer, size_t size);


    private:
        char txBuffer[BUFFER_LENGTH_TX];
        char rxBuffer[BUFFER_LENGTH_RX];
        char parseBuffer[BUFFER_LENGTH_PARSED];
        byte txWriteIndex;
        byte txReadIndex;
        byte txCount;
        byte rxWriteIndex; 
        byte rxReadIndex; 
        byte rxCount;
        byte parseCount;

        enum SerialProcessorStateMachine {
            SPWaitForStartChar = 0,
            SPGotStartChar,
            SPGetContent,
            SPVerifyPayload
        };
        SerialProcessorStateMachine spState = SPWaitForStartChar;

        void parseReceived(char data);
        uint8_t calculateChecksum();
        PROCESSED_CALLBACK_FUNCTION(processedCallback,payload,length) = nullptr;
};

#endif