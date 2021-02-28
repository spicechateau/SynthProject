/*
  ==============================================================================

    SynthSound.h
    Created: 27 Feb 2021 6:51:01pm
    Author:  Tim Leete

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote (int midiNoteNumber) override {return true;}
    bool appliesToChannel (int midiChannel) override {return true;}
};
