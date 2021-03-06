/*
  ==============================================================================

    SynthVoice.h
    Created: 27 Feb 2021 6:50:44pm
    Author:  Tim Leete

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;
    
    void updateADSR (const float attack, const float decay, const float sustain, const float release);
    void updateGain (const float newOsc1Gain, const float newOsc2Gain, const float newSubOscGain, const float newNoiseGain);
    
private:
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    juce::AudioBuffer<float> synthBuffer;
    
    juce::dsp::Oscillator<float> osc1 { [](float x) {return x / juce::MathConstants<float>::pi;}, 200};
    juce::dsp::Oscillator<float> osc2 { [](float x) {return x / juce::MathConstants<float>::pi;}, 200};
    juce::dsp::Oscillator<float> subOsc { [](float x) {return std::sin(x);}, 200};
    juce::dsp::Oscillator<float> noise { [](float x) {return x < 0.0f ? -1.0f : 1.0f;}, 200};
    // return std::sin(x); // Sin wave
    // return x / juce::MathConstants<float>::pi; // Saw
    // return x < 0.0f ? -1.0f : 1.0f; // Square
    
    
    juce::dsp::Gain<float> osc1Gain;
    juce::dsp::Gain<float> osc2Gain;
    juce::dsp::Gain<float> subOscGain;
    juce::dsp::Gain<float> noiseGain;
    
    bool isPrepared {false};
    
};
