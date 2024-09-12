/* audio_stream_playback_adlib.cpp */

#include "audio_stream_playback_adlib.h"
#include "audio_stream_adlib.h"

#include "core/math/math_funcs.h"
#include "core/string/print_string.h"

#include "core/config/project_settings.h"
#include "servers/audio_server.h"

// Adplug definitions:
#include <adplug/adplug.h>
#include <adplug/emuopl.h>

using namespace std;


AudioStreamPlaybackAdlib::~AudioStreamPlaybackAdlib() {
}

void AudioStreamPlaybackAdlib::stop() {
	// active = false;
	base->reset();
}
void AudioStreamPlaybackAdlib::start(double p_from_pos) {
	CEmuopl opl(RATE, BIT16, base->stereo);
	opl.settype(static_cast<Copl::ChipType>(base->get_chipset()));
	String GlobalFilePath = ProjectSettings::get_singleton()->globalize_path(base->file_path);
	const char *char_path = GlobalFilePath.utf8();
	CPlayer* playback = CAdPlug::factory(char_path, &opl);
	if (!playback) {
		print_error("Can't load Adplug file");
		stop();
		return;
	}
	short buf[BUFSIZE];
	unsigned long towrite, write;
	while(playback->update())
		for (towrite = RATE / playback->getrefresh(); towrite; towrite -= write) {
			write = (towrite > BUFSIZE ? BUFSIZE : towrite);
			opl.update(buf, write);
			for (unsigned int buf_index = 0; buf_index < write; buf_index++) {
				playback_data.push_back(buf[buf_index]);
			}
		}
	reverse(playback_data.begin(), playback_data.end());
	// print_line(playback_data.size());
	active = true;
}
void AudioStreamPlaybackAdlib::seek(double p_time) {
	if (p_time < 0) {
		p_time = 0;
	}
	p_time = 0;
	// base->set_position(uint64_t(p_time * base->mix_rate) << MIX_FRAC_BITS);
}

int AudioStreamPlaybackAdlib::mix(AudioFrame *p_buffer, float p_rate_scale, int p_frames) {
	if (!active) {
		stop();
		return 0;
	}
	int towrite = p_frames;
	int playback_data_size = playback_data.size();
	if (base->stereo == true) {
		towrite = p_frames*2;
		if (playback_data_size <= p_frames*2) {
			towrite = playback_data_size;
		}
		for (int i = 0; i < towrite; i+=2) {
			float sample1 = float(playback_data[playback_data_size-1-i]) / 32767.0;
			float sample2 = float(playback_data[playback_data_size-1-(i+1)]) / 32767.0;
			p_buffer[i] = AudioFrame(sample1, sample2);
		}
		if (playback_data_size <= p_frames*2) {
			stop();
		}
		playback_data.resize(playback_data_size-towrite);
	}
	else {
		if (playback_data_size <= p_frames) {
			towrite = playback_data_size;
		}
		for (int i = 0; i < towrite; i++) {
			float sample = float(playback_data[playback_data_size-1-i]) / 32767.0;
			p_buffer[i] = AudioFrame(sample, sample);
		}
		if (playback_data_size <= p_frames) {
			stop();
		}
		playback_data.resize(playback_data_size-towrite);
	}
	return towrite;
}

int AudioStreamPlaybackAdlib::get_loop_count() const {
	return 0;
}
double AudioStreamPlaybackAdlib::get_playback_position() const {
	return 0.0;
}
// double AudioStreamPlaybackAdlib::get_length() const {
	// return 0.0;
// }
bool AudioStreamPlaybackAdlib::is_playing() const {
	return active;
}

void AudioStreamPlaybackAdlib::_bind_methods() {
	return;
}

