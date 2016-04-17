//
//  Entity.hpp
//  egg scramble
//
//  Created by Phoebe on 4/14/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#ifndef Entity_h
#define Entity_h

#include <stdio.h>
#include <iostream>
#include <string>

#include <audiodecoder/audiodecoder.h>
#include <portaudio.h>

///////////////// AUDIO STUFF ///////////////////////////

// All audio will be handled as stereo.
const int NUM_CHANNELS = 2;

// This is the function that gets called when we need to generate sound!
// In this example, we're going to decode some audio using libaudiodecoder
// and fill the "output" buffer with that. In other words, we're going to
// decode demo.mp3 and send that audio to the soundcard. Easy!
int audioCallback(const void *input, void *output,
                  unsigned long frameCount,
                  const PaStreamCallbackTimeInfo* timeInfo,
                  PaStreamCallbackFlags statusFlags,
                  void* userData);

////////////////////////////////////////////////////////////

class Entity {
public:
    Entity();
    
    int PlaySound(std::string sound_file);
    
    // Draw
    // Update

};


#endif /* Entity_hpp */
