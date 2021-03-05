/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthProjectAudioProcessorEditor::SynthProjectAudioProcessorEditor (SynthProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (1000, 500);
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    
    // General Params
    
    // Osc
    osc1SelectorAttachment = std::make_unique<ComboBoxAttachment>(audioProcessor.apvts, "OSC1TYPE", osc1Selector);
    
    // ADSR
    attackAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DECAY", decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseSlider);

    setSliderParams (attackSlider);
    setSliderParams (decaySlider);
    setSliderParams (sustainSlider);
    setSliderParams (releaseSlider);
    
    // Output

}

SynthProjectAudioProcessorEditor::~SynthProjectAudioProcessorEditor()
{
}

//==============================================================================
void SynthProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void SynthProjectAudioProcessorEditor::resized()
{
    // General Params
    
    // Osc
    
    // ADSR
    const auto bounds = getLocalBounds().reduced (10);
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getWidth() / 4 - padding;
    const auto sliderStartX = 0;
    const auto sliderStartY = bounds.getHeight() / 2 - (sliderHeight / 2);

    attackSlider.setBounds (sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds (attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds (decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds (sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    
    // Output
}

void SynthProjectAudioProcessorEditor::setSliderParams (juce::Slider& slider)
{
    slider.setSliderStyle (juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible (slider);
}
