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
    gainSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(gainSlider);
    
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);
    
    setSize (400, 300);
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
    gainSlider.setBounds (getWidth() / 2 - 100, getHeight() / 2 - 50, 300, 200);
}
