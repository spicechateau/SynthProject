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
    void setComboBoxParams (juce::ComboBox& comboBox);
    
    // Gen Params
    juce::ComboBox stabilityRateSelector;
    
    juce::Slider stabilitySlider;
    
    std::unique_ptr<ComboBoxAttachment> stabilityRateSelectorAttachment;
    
    std::unique_ptr<SliderAttachment> stabilitySliderAttachment;
    
    // Osc
    juce::ComboBox osc1Selector;
    juce::ComboBox osc2Selector;
    
    juce::Slider osc1GainSlider;
    juce::Slider osc2GainSlider;
    juce::Slider subOscGainSlider;
    juce::Slider noiseGainSlider;
    
    std::unique_ptr<ComboBoxAttachment> osc1SelectorAttachment;
    std::unique_ptr<ComboBoxAttachment> osc2SelectorAttachment;
    
    std::unique_ptr<SliderAttachment> osc1GainAttachment;
    std::unique_ptr<SliderAttachment> osc2GainAttachment;
    std::unique_ptr<SliderAttachment> subOscGainAttachment;
    std::unique_ptr<SliderAttachment> noiseGainAttachment;
    
    // Pulse Width
    
    // ADSR
    juce::Slider ampAttackSlider;
    juce::Slider ampDecaySlider;
    juce::Slider ampSustainSlider;
    juce::Slider ampReleaseSlider;

    std::unique_ptr<SliderAttachment> ampAttackAttachment;
    std::unique_ptr<SliderAttachment> ampDecayAttachment;
    std::unique_ptr<SliderAttachment> ampSustainAttachment;
    std::unique_ptr<SliderAttachment> ampReleaseAttachment;
    
    // Filter
    
    juce::Slider filAttackSlider;
    juce::Slider filDecaySlider;
    juce::Slider filSustainSlider;
    juce::Slider filReleaseSlider;

    std::unique_ptr<SliderAttachment> filAttackAttachment;
    std::unique_ptr<SliderAttachment> filDecayAttachment;
    std::unique_ptr<SliderAttachment> filSustainAttachment;
    std::unique_ptr<SliderAttachment> filReleaseAttachment;

    // Output Params
    
    juce::Slider preSatSlider;
    juce::Slider outputGainSlider;
    
    std::unique_ptr<SliderAttachment> preSatAttachment;
    std::unique_ptr<SliderAttachment> outputGainAttachment;
    
    SynthProjectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthProjectAudioProcessorEditor)
};
