
#ifndef __AUDIO_H
#define __AUDIO_H

#include <Adafruit_VS1053.h>

class Audio {
    public:
        void setup();
        void loop(int test);

    private:
        Adafruit_VS1053_FilePlayer *audioPlayerPtr;
};

#endif