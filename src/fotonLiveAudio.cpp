//
//  fotonLiveAudio.cpp
//  fotonLive
//
//  Created by MORIOKAToyoshi on 2017/05/15.
//
//

#include "fotonLiveAudio.h"

void fotonLiveAudio::setup(){
    
    // 0 output channels,
    // 2 input channels
    // 44100 samples per second
    // 256 samples per buffer
    // 4 num buffers (latency)
    
    soundStream.printDeviceList();
    
    //if you want to set a different device id
    //soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
    soundStream.setDeviceID(2);
    
    // Setup Audio Input
    //    ofSoundStreamSetup(0, 1, sampleRate, bufferSize, 4);
    soundStream.setup(this, 0, 2, fotonLiveAudio_SampleRate, fotonLiveAudio_BufferSize, 4);
//    soundStream.start();
    
    smoothedVol = 0.0;
    scaledVol = 0.0;
    
    // ofxBTrack
    
    // Setup frame size
    bTrack.setup(fotonLiveAudio_BufferSize);
    
    // FIX: this is experimental inplementation.
    bTrack.setConfidentThreshold(fotonLiveAudio_BTrackThreshold);
    
    // ofxFft
    fft = ofxFft::create(fotonLiveAudio_BufferSize, OF_FFT_WINDOW_HAMMING);
    rawSpectrum.resize(fft->getBinSize());
    normalSpectrum.resize(fft->getBinSize());
    
    leftInput.resize(0);
    rightInput.resize(0);
    
}

void fotonLiveAudio::audioIn(float *input, int bufferSize, int nChannels){
    float maxValue = 0.0;
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    soundMutex.lock();
    
    leftInput.resize(0);
    rightInput.resize(0);
    
    for (int i = 0; i < bufferSize; i++){
        float left	= input[i*2];
        float right	= input[i*2+1];
        
        leftInput.push_back(left);
        rightInput.push_back(right);
        
        if(abs(left) > maxValue) {
            maxValue = abs(left);
        }
        if(abs(right) > maxValue) {
            maxValue = abs(right);
        }
        
        curVol += (left * 0.5) * (left * 0.5);
        curVol += (right * 0.5) * (right * 0.5);
        numCounted+=2;
    }
    
    soundMutex.unlock();
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    
    // feed audio frame into ofxBTrack object
    bTrack.audioIn(input, bufferSize, nChannels);
    
    // fft
    for(int i = 0; i < bufferSize; i++) {
        input[i] /= maxValue;
    }
    
    fft->setSignal(input);
    
    float* curFft = fft->getAmplitude();
    memcpy(&rawSpectrum[0], curFft, sizeof(float) * fft->getBinSize());
    
    maxValue = 0;
    for(int i = 0; i < fft->getBinSize(); i++) {
        if(abs(rawSpectrum[i]) > maxValue) {
            maxValue = abs(rawSpectrum[i]);
        }
    }
    for(int i = 0; i < fft->getBinSize(); i++) {
        normalSpectrum[i] = rawSpectrum[i] / maxValue;
    }
    //soundMutex.unlock();
    
    // soundMutex.lock();
    // soundMutex.unlock();
}
