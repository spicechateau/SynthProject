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
    setSize (800, 400);
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    
    // General Params
    
    // Osc
    osc1SelectorAttachment = std::make_unique<ComboBoxAttachment>(audioProcessor.apvts, "OSC1TYPE", osc1Selector);
    osc2SelectorAttachment = std::make_unique<ComboBoxAttachment>(audioProcessor.apvts, "OSC2TYPE", osc1Selector);
    
    osc1GainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "OSC1GAIN", osc1GainSlider);
    osc2GainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "OSC2GAIN", osc2GainSlider);
    subOscGainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SUBOSCGAIN", subOscGainSlider);
    noiseGainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "NOISEGAIN", noiseGainSlider);
    
    setSliderParams (osc1GainSlider);
    setSliderParams (osc2GainSlider);
    setSliderParams (subOscGainSlider);
    setSliderParams (noiseGainSlider);
    
    // Pulse Width
    
    // ADSR
    ampAttackAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "AMPATTACK", ampAttackSlider);
    ampDecayAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "AMPDECAY", ampDecaySlider);
    ampSustainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "AMPSUSTAIN", ampSustainSlider);
    ampReleaseAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "AMPRELEASE", ampReleaseSlider);

    setSliderParams (ampAttackSlider);
    setSliderParams (ampDecaySlider);
    setSliderParams (ampSustainSlider);
    setSliderParams (ampReleaseSlider);
    
    // Filter
    filAttackAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "FILATTACK", filAttackSlider);
    filDecayAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "FILDECAY", filDecaySlider);
    filSustainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "FILSUSTAIN", filSustainSlider);
    filReleaseAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "FILRELEASE", filReleaseSlider);

    setSliderParams (filAttackSlider);
    setSliderParams (filDecaySlider);
    setSliderParams (filSustainSlider);
    setSliderParams (filReleaseSlider);
    
    // Output
    preSatAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "PRESATGAIN", preSatSlider);
    outputGainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "OUTPUTGAIN", outputGainSlider);
    
    setSliderParams (preSatSlider);
    setSliderParams (outputGainSlider);
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
    const auto Padding = 15;
    const auto SliderWidth = 30;
    const auto SliderHeight = 175;
    const auto SliderStartX = 30;
    const auto SliderStartY = 200;
    
    // General Params
    
    // Osc
    osc1GainSlider.setBounds (SliderStartX, SliderStartY, SliderWidth, SliderHeight);
    osc2GainSlider.setBounds (osc1GainSlider.getRight() + Padding, SliderStartY, SliderWidth, SliderHeight);
    subOscGainSlider.setBounds (osc2GainSlider.getRight() + Padding, SliderStartY, SliderWidth, SliderHeight);
    noiseGainSlider.setBounds (subOscGainSlider.getRight() + Padding, SliderStartY, SliderWidth, SliderHeight);
    
    // Filter ADSR
    filAttackSlider.setBounds (noiseGainSlider.getRight() + Padding + 40, SliderStartY, SliderWidth, SliderHeight);
    filDecaySlider.setBounds (filAttackSlider.getRight() + Padding, SliderStartY, SliderWidth, SliderHeight);
    filSustainSlider.setBounds (filDecaySlider.getRight() + Padding, SliderStartY, SliderWidth, SliderHeight);
    filReleaseSlider.setBounds (filSustainSlider.getRight() + Padding, SliderStartY, SliderWidth, SliderHeight);
    
    // Amp ADSR
    ampAttackSlider.setBounds (filReleaseSlider.getRight() + Padding + 40, SliderStartY, SliderWidth, SliderHeight);
    ampDecaySlider.setBounds (ampAttackSlider.getRight() + Padding, SliderStartY, SliderWidth, SliderHeight);
    ampSustainSlider.setBounds (ampDecaySlider.getRight() + Padding, SliderStartY, SliderWidth, SliderHeight);
    ampReleaseSlider.setBounds (ampSustainSlider.getRight() + Padding, SliderStartY, SliderWidth, SliderHeight);
    
    // Output
    preSatSlider.setBounds (ampReleaseSlider.getRight() + 38, SliderStartY, SliderWidth, SliderHeight);
    outputGainSlider.setBounds (preSatSlider.getRight() + 37, SliderStartY, SliderWidth, SliderHeight);
    
}

//void SynthProjectAudioProcessorEditor::setComboBoxParams (juce::ComboBox& comboBox)
//{
//    comboBox.setEditableText(false);
//    comboBox.
//}

void SynthProjectAudioProcessorEditor::setSliderParams (juce::Slider& slider)
{
    slider.setSliderStyle (juce::Slider::SliderStyle::LinearVertical);
    slider.hideTextBox (true);
    addAndMakeVisible (slider);
}
