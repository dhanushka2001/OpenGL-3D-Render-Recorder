#ifndef ENCODER_H    // #include guard
#define ENCODER_H

#include <stdint.h>     // for uint8_t
#include <mutex>

namespace Encoder {
    extern std::mutex encoderMutex;
    
    bool initializeEncoder(const char* filename);
    bool encodeFrame(const uint8_t* rgbData, float crntTime);
    void finalizeEncoder();
}

#endif /* ENCODER_H */