#ifndef ENCODER_H    // #include guard
#define ENCODER_H

#include <stdint.h>     // for uint8_t

extern       unsigned int  framerate;
extern const unsigned int  SCR_WIDTH;
extern const unsigned int  SCR_HEIGHT;


namespace Encoder {
    bool initializeEncoder(const char* filename);
    bool encodeFrame(const uint8_t* rgbData, float crntTime);
    void finalizeEncoder();
}

#endif /* ENCODER_H */