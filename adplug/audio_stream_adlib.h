/* audiostream_mytone.h */

#ifndef AUDIOSTREAM_ADLIB_H
#define AUDIOSTREAM_ADLIB_H

#include "audio_stream_playback_adlib.h"
// #include "core/reference.h"
// #include "core/resource.h"
#include "servers/audio/audio_stream.h"

// For extensions
#include "core/io/resource.h"
#include "core/io/resource_loader.h"

class AudioStreamAdlib : public AudioStream {
	GDCLASS(AudioStreamAdlib, AudioStream);
	OBJ_SAVE_TYPE(AudioStream); // Saves derived classes with common type so they can be interchanged.
	friend class AudioStreamPlaybackAdlib;
public:
	enum ChipType {
		TYPE_OPL2 = 0,
		TYPE_DUAL_OPL2 = 1,
		TYPE_OPL3 = 2
	};
	void set_chipset(ChipType p_type);
	ChipType get_chipset();
	void set_loop(bool p_enable);
	virtual bool has_loop() const;
	
	void reset();
	void set_position(uint64_t pos);
	virtual Ref<AudioStreamPlayback> instantiate_playback();
	virtual String get_stream_name() const;
	void set_file_path(String _filename);
	virtual double get_length() const { return 0; } // if supported, otherwise return 0
	virtual bool can_be_sampled() const override {
		return false;
	}
	AudioStreamAdlib();
private:
	uint64_t pos;
	int mix_rate;
	int hz;
	bool loop = false;
	bool stereo = false;
	ChipType chipset = TYPE_OPL2;
	String filename;
protected:
	static void _bind_methods();
};
VARIANT_ENUM_CAST(AudioStreamAdlib::ChipType);

#endif
