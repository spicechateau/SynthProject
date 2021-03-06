/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SynthProjectAudioProcessor::SynthProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), false)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", createParameters())
#endif
{
    synth.addSound (new SynthSound());
    synth.addVoice (new SynthVoice());
}

SynthProjectAudioProcessor::~SynthProjectAudioProcessor()
{
}

//==============================================================================
const juce::String SynthProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void SynthProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SynthProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            // osc controls
            auto& osc1Type = *apvts.getRawParameterValue ("OSC1TYPE");
            auto& osc2Type = *apvts.getRawParameterValue ("OSC2TYPE");
            
            auto& osc1Gain = *apvts.getRawParameterValue ("OSC1GAIN");
            auto& osc2Gain = *apvts.getRawParameterValue ("OSC2GAIN");
            auto& subOscGain = *apvts.getRawParameterValue ("SUBOSCGAIN");
            auto& noiseGain = *apvts.getRawParameterValue ("NOISEGAIN");
            
            voice->updateGain(osc1Gain, osc2Gain, subOscGain, noiseGain);
            
            //
            
            // ADSR
            auto& attack = *apvts.getRawParameterValue ("AMPATTACK");
            auto& decay = *apvts.getRawParameterValue ("AMPDECAY");
            auto& sustain = *apvts.getRawParameterValue ("AMPSUSTAIN");
            auto& release = *apvts.getRawParameterValue ("AMPRELEASE");

            voice->updateADSR(attack, decay, sustain, release);
            
            // Filter
            
            // Output
        }
    }

    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool SynthProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynthProjectAudioProcessor::createEditor()
{
    return new SynthProjectAudioProcessorEditor (*this);
}

//==============================================================================
void SynthProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynthProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthProjectAudioProcessor();
}


juce::AudioProcessorValueTreeState::ParameterLayout SynthProjectAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // Gen Params
    params.push_back(std::make_unique<juce::AudioParameterFloat>("STABILITY", "Stability", juce::NormalisableRange<float> {0.0f, 10.0f }, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("STABILITYRATE", "Stability Rate", juce::StringArray {"Whole", "Half", "Quarter", "8th", "16th", "32nd"}, 3));

    // OSC Params
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1TYPE", "Oscillator 1 Type", juce::StringArray {"Sine", "Saw", "Square"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1GAIN", "Oscillator 1 Gain", juce::NormalisableRange<float> {0.0f, 1.0f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2TYPE", "Oscillator 2 Type", juce::StringArray {"Sine", "Saw", "Square"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2GAIN", "Oscillator 2 Gain", juce::NormalisableRange<float> {0.0f, 1.0f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUBOSCGAIN", "Suboscillator Gain", juce::NormalisableRange<float> {0.0f, 1.0f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("NOISEGAIN", "Noise Gain", juce::NormalisableRange<float> {0.0f, 1.0f }, 0.0f));
    
    // Pulse Width
    
    // Amp ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>("AMPATTACK", "Amp Attack", juce::NormalisableRange<float> {0.01f, 5.0f }, 0.01f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("AMPDECAY", "Amp Decay", juce::NormalisableRange<float> {0.01f, 5.0f }, 0.01f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("AMPSUSTAIN", "Amp Sustain", juce::NormalisableRange<float> {0.1f, 1.0f }, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("AMPRELEASE", "Amp Release", juce::NormalisableRange<float> {0.1f, 5.0f }, 0.1f));
    
    // Filter
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILATTACK", "Filter Attack", juce::NormalisableRange<float> {0.01f, 5.0f }, 0.01f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILDECAY", "Filter Decay", juce::NormalisableRange<float> {0.01f, 5.0f }, 0.01f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILSUSTAIN", "Filter Sustain", juce::NormalisableRange<float> {0.1f, 1.0f }, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILRELEASE", "Filter Release", juce::NormalisableRange<float> {0.1f, 5.0f }, 0.1f));
    
    // Output
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PRESATGAIN", "Pre-Saturation Gain", juce::NormalisableRange<float> {0.0f, 1.0f }, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("Output Gain", "Output Gain", juce::NormalisableRange<float> {0.0f, 1.0f }, 0.5f));
    
    return { params.begin(), params.end() };
}
