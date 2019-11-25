
#ifndef __SERIALPROCESSOR_H
#define __SERIALPROCESSOR_H

#define BUFFER_LENGTH_TX        64
#define BUFFER_LENGTH_RX        64
#define BUFFER_LENGTH_PARSED    64

class SerialProcessor {
    public:
        void setup();
        void loop(int test);
        void writeBytes(const byte *payload, uint8_t length);
        void writeString(const char *payload);


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
        void receivedPayloadCallback();
};

#endif