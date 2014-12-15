// ------------------------------------------------------------------
// audio::AudioContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_AUDIO_AUDIOCONTEXT_H
#define SD_AUDIO_AUDIOCONTEXT_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"

namespace stardazed {
namespace audio {


struct AudioContextDescriptor {
	float sampleRate;
};


class AudioContext {
	SD_NOCOPYORMOVE_CLASS(AudioContext)

public:
	AudioContext(const AudioContextDescriptor&);
	~AudioContext();
	
	void start();
};


} // ns audio
} // ns stardazed

#endif
