/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SynthProjectAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SynthProjectAudioProcessorEditor (SynthProjectAudioProcessor&);
    ~SynthProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    
    void setSliderParams (juce::Slider& slider);
    
    // Gen Params
    
    // Osc
    juce::ComboBox osc1Selector;
    
    std::unique_ptr<ComboBoxAttachment> osc1SelectorAttachment;
    
    // ADSR
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;

    // Output Params
    
    SynthProjectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthProjectAudioProcessorEditor)
};
