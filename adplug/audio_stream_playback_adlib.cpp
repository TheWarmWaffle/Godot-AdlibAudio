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
#include <adplug/nemuopl.h>

using namespace std;

unsigned long write_buffer_size = BUFSIZE; // BUFSIZE for mono, BUFSIZE/2 for stereo.
short write_multiplier = 1; // 1 for mono, 2 for stero.
bool stereo = false;

void make_mono() {
	write_buffer_size = BUFSIZE;
	write_multiplier = 1;
	stereo = false;
}

void make_stereo() {
	write_buffer_size = BUFSIZE/2;
	write_multiplier = 2;
	stereo = true;
}

AudioStreamPlaybackAdlib::~AudioStreamPlaybackAdlib() {
}

void AudioStreamPlaybackAdlib::stop() {
	// active = false;
	base->reset();
}

void AudioStreamPlaybackAdlib::start(double p_from_pos) {
	Copl::ChipType copl_chip_type = static_cast<Copl::ChipType>(base->get_chipset());
	Copl *opl;
	short buf[BUFSIZE];
	unsigned long towrite, write;
	
	if (base->emulator == AudioStreamAdlib::NUKED) { // Opl3, Recommended.
		opl = new CNemuopl(RATE);
		make_stereo();
	}
	if (base->emulator == AudioStreamAdlib::ADPLUG) { // Opl2, Dual Opl2, Opl3.
		cout << "made it se\n";
		
		if (copl_chip_type == Copl::TYPE_DUAL_OPL2 || copl_chip_type == Copl::TYPE_OPL3) {
			CEmuopl *new_opl = new CEmuopl(RATE, BIT16, false); // Need to have a throwoway variable, so we can properly access settype().
			new_opl->settype(copl_chip_type);
			opl = new_opl;
			make_mono();
		}
		else {
			CEmuopl *new_opl = new CEmuopl(RATE, BIT16, false); // Need to have a throwoway variable, so we can properly access settype().
			new_opl->settype(copl_chip_type);
			opl = new_opl;
			make_mono();
		}
		
		cout << "A\n";
	}
	
	String GlobalFilePath = ProjectSettings::get_singleton()->globalize_path(base->file_path);
	const char *char_path = GlobalFilePath.utf8();
	CPlayer* playback = CAdPlug::factory(char_path, &*opl);
	if (!playback) {
		print_error("Can't load Adplug file!");
		stop();
		return;
	}
	cout << "Made it here\n";
	while(playback->update())
		for (towrite = RATE / playback->getrefresh(); towrite; towrite -= write) {
			write = (towrite > write_buffer_size ? write_buffer_size : towrite);
			opl->update(buf, write);
			for (unsigned int buf_index = 0; buf_index < write * write_multiplier; buf_index++) {
				playback_data.push_back(buf[buf_index]);
			}
		}
	cout << "making it here\n";
	reverse(playback_data.begin(), playback_data.end());
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
	if (stereo == true) {
		towrite = p_frames*2;
		if (playback_data_size <= p_frames*2) {
			towrite = playback_data_size;
		}
		for (int i = 0; i < towrite; i+=2) {
			float sample1 = float(playback_data[playback_data_size-1-i]) / 32767.0;
			float sample2 = float(playback_data[playback_data_size-1-(i+1)]) / 32767.0;
			p_buffer[i/2] = AudioFrame(sample1, sample2);
		}
		if (playback_data_size <= p_frames*2) {
			stop();
		}
		playback_data.resize(playback_data_size-towrite);
		return towrite/2;
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
		return towrite;
	}
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

