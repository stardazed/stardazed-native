// ------------------------------------------------------------------
// audio::AudioContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "audio/AudioContext.hpp"

#import <AudioUnit/AudioUnit.h>

namespace stardazed {
namespace audio {


static AudioUnit auUnit;


OSStatus auCallback(void *inRefCon,
					AudioUnitRenderActionFlags *ioActionFlags,
					const AudioTimeStamp *inTimeStamp,
					UInt32 inBusNumber,
					UInt32 inNumberFrames,
					AudioBufferList *ioData)
{
	SD_UNUSED_PARAM(ioActionFlags)
	SD_UNUSED_PARAM(inTimeStamp)
	SD_UNUSED_PARAM(inBusNumber)

	AudioContext* context = static_cast<AudioContext*>(inRefCon);
//	genAudio(*soundState, ioData->mBuffers[0], inNumberFrames);
	
	return noErr;
}

		
AudioContext::AudioContext(const AudioContextDescriptor& desc) {
	assert(auUnit == nullptr); // faux-singleton class
	
	// stereo interleaved linear PCM audio data at 44.1kHz in float format
	AudioStreamBasicDescription streamDesc {};
	streamDesc.mSampleRate = desc.sampleRate;
	streamDesc.mFormatID = kAudioFormatLinearPCM;
	streamDesc.mFormatFlags = kAudioFormatFlagsNativeFloatPacked;
	streamDesc.mChannelsPerFrame = 2;
	streamDesc.mFramesPerPacket  = 1;
	streamDesc.mBitsPerChannel   = sizeof(float) * 8;
	streamDesc.mBytesPerFrame    = sizeof(float) * streamDesc.mChannelsPerFrame;
	streamDesc.mBytesPerPacket   = streamDesc.mBytesPerFrame * streamDesc.mFramesPerPacket;
	
	OSStatus err;
	
	AudioComponentDescription acd;
	acd.componentType         = kAudioUnitType_Output;
	acd.componentSubType      = kAudioUnitSubType_DefaultOutput;
	acd.componentManufacturer = kAudioUnitManufacturer_Apple;
	
	AudioComponent outputComponent = AudioComponentFindNext(NULL, &acd);
	
	err = AudioComponentInstanceNew(outputComponent, &auUnit);
	if (! err) {
		err = AudioUnitInitialize(auUnit);
		
		err = AudioUnitSetProperty(auUnit,
								   kAudioUnitProperty_StreamFormat,
								   kAudioUnitScope_Input,
								   0,
								   &streamDesc,
								   sizeof(streamDesc));
		
		AURenderCallbackStruct cb;
		cb.inputProc       = &auCallback;
		cb.inputProcRefCon = this;
		
		err = AudioUnitSetProperty(auUnit,
								   kAudioUnitProperty_SetRenderCallback,
								   kAudioUnitScope_Global,
								   0,
								   &cb,
								   sizeof(cb));
	}
}


AudioContext::~AudioContext() {
	if (auUnit) {
		AudioOutputUnitStop(auUnit);
		AudioUnitUninitialize(auUnit);
		AudioComponentInstanceDispose(auUnit);
		
		auUnit = nullptr;
	}
}


void AudioContext::start() {
	if (auUnit)
		AudioOutputUnitStart(auUnit);
}


} // ns audio
} // ns stardazed
