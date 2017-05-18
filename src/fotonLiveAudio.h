//
//  fotonLiveAudio.h
//  fotonLive
//
//  Created by MORIOKAToyoshi on 2017/05/15.
//
//

#ifndef fotonLiveAudio_h
#define fotonLiveAudio_h

#include "ofMain.h"
#include "ofxBTrack.h"
#include "ofxFft.h"
#include "fotonConst.h"

class fotonLiveAudio : public ofBaseApp{
    
public:
    float smoothedVol;
    float scaledVol;
    
    void setup();
    
    bool hasBeat(){
        return bTrack.hasBeat();
    }
    
    float bpm(){
        return bTrack.getEstimatedBPM();
    }
    
    float confidence(){
        return bTrack.getConfidence();
    }
    
    vector<float> getLeftInput(){
        soundMutex.lock();
        vector<float> result = leftInput;
        soundMutex.unlock();
        return result;
    }
    
    vector<float> getRightInput(){
        soundMutex.lock();
        vector<float> result = rightInput;
        soundMutex.unlock();
        return result;
    }
    
    vector<float> getRawSpectrum(){
        soundMutex.lock();
        vector<float> result = rawSpectrum;
        soundMutex.unlock();
        return result;
    }
    
    vector<float> getNormalSpectrum(){
        soundMutex.lock();
        vector<float> result = normalSpectrum;
        soundMutex.unlock();
        return result;
    }
    
private:
    void audioIn(float *input, int bufferSize, int nChannels);
    ofxBTrack bTrack;
    ofSoundStream soundStream;
    ofxFft* fft;
    ofMutex soundMutex;
    vector<float> rawSpectrum, normalSpectrum;
    vector<float> leftInput, rightInput;
};

#endif /* fotonLiveAudio_h */
