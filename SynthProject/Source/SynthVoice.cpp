/*
  ==============================================================================

    SynthVoice.cpp
    Created: 27 Feb 2021 6:50:44pm
    Author:  Tim Leete

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    osc1.setFrequency (juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber));
    osc2.setFrequency (juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber));
    subOsc.setFrequency (juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber - 12));
    noise.setFrequency (juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber));
    adsr.noteOn();
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff();
    
    if (! allowTailOff || ! adsr.isActive())
        clearCurrentNote();
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate (sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    osc1.prepare (spec);
    osc2.prepare (spec);
    subOsc.prepare (spec);
    noise.prepare (spec);
    
    osc1Gain.prepare(spec);
    osc2Gain.prepare(spec);
    subOscGain.prepare(spec);
    noiseGain.prepare(spec);
    
    isPrepared = true;
}
void SynthVoice::updateGain (const float newOsc1Gain, const float newOsc2Gain, const float newSubOscGain, const float newNoiseGain)
{
    osc1Gain.setGainLinear (newOsc1Gain);
    osc2Gain.setGainLinear (newOsc2Gain);
    subOscGain.setGainLinear (newSubOscGain);
    noiseGain.setGainLinear (newNoiseGain);
}

void SynthVoice::updateADSR (const float attack, const float decay, const float sustain, const float release)
{
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
    
    adsr.setParameters (adsrParams);
    
}

void SynthVoice::renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    jassert (isPrepared);
    
    if (! isVoiceActive())
        return;
    
    synthBuffer.setSize (outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlock { synthBuffer };
    
    osc1.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    osc2.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    subOsc.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    noise.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
    osc1Gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    osc2Gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    subOscGain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    noiseGain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom (channel, startSample, synthBuffer, channel, 0, numSamples);
        
            if (! adsr.isActive())
                clearCurrentNote();
    }
}
