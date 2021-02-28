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
    
private:
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    
    juce::dsp::Oscillator<float> osc{[](float x) {return x / juce::MathConstants<float>::pi;}, 200};
    // return std::sin(x); // Sin wave
    // return x / juce::MathConstants<float>::pi; // Saw
    // return x < 0.0f ? -1.0f : 1.0f; // Square
    juce::dsp::Gain<float> gain;
    
    bool isPrepared {false};
    
};
