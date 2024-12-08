/*  audio_stream_playback_adlib.h */
#ifndef AUDIO_STREAM_PLAYBACK_ADLIB_H
#define AUDIO_STREAM_PLAYBACK_ADLIB_H

// #include "audiostream_adlib.h"
#include "servers/audio/audio_stream.h"

#include "core/io/file_access.h"

#include <iostream>
#include <vector>

// Adplug definitions:
#include <adplug/adplug.h>
#include <adplug/emuopl.h>

#define RATE	48000	// Output frequency in Hz
#define BIT16	true	// true when 16bit samples should be used
#define BUFSIZE	512		// Sound buffer size in samples


class AudioStreamPlaybackAdlib : public AudioStreamPlayback {
	GDCLASS(AudioStreamPlaybackAdlib, AudioStreamPlayback);
	friend class AudioStreamAdlib;
protected:
	static void _bind_methods();
public:
	virtual int mix(AudioFrame *p_buffer, float p_rate_scale, int p_frames);
	
	virtual void start(double p_from_pos = 0.0);
	virtual void stop();
	virtual bool is_playing() const;
	virtual int get_loop_count() const; // times it looped
	
	virtual double get_playback_position() const;
	virtual void seek(double p_time);
	~AudioStreamPlaybackAdlib();
private:
	Ref<AudioStreamAdlib> base;
	std::vector<short> playback_data;
	bool active;
};
#endif