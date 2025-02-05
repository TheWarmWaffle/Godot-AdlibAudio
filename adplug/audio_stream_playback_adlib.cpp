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

AudioStreamPlaybackAdlib::~AudioStreamPlaybackAdlib() {}

void AudioStreamPlaybackAdlib::stop() {
	active = false;
	towrite = 0;
}

void AudioStreamPlaybackAdlib::start(double p_from_pos) {
	Copl::ChipType copl_chip_type = static_cast<Copl::ChipType>(base->get_chipset());

	if (base->emulator == AudioStreamAdlib::NUKED) { // Opl3, Recommended.
		opl = new CNemuopl(RATE);
		stereo = true;
	}
	else { // elif (base->emulator == AudioStreamAdlib::ADPLUG) { // Opl2, Dual Opl2, Opl3.
		if (copl_chip_type == Copl::TYPE_DUAL_OPL2 || copl_chip_type == Copl::TYPE_OPL3) {
			CEmuopl *new_opl = new CEmuopl(RATE, BIT16, false); // Need to have a throwoway variable, so we can properly access settype().
			new_opl->settype(copl_chip_type);
			opl = new_opl;
			stereo = true;
		}
		else {
			CEmuopl *new_opl = new CEmuopl(RATE, BIT16, false); // Need to have a throwoway variable, so we can properly access settype().
			new_opl->settype(copl_chip_type);
			opl = new_opl;
			stereo = false;
		}
	}
	String GlobalFilePath = ProjectSettings::get_singleton()->globalize_path(base->file_path);
	const char *char_path = GlobalFilePath.utf8();
	playback = CAdPlug::factory(char_path, &*opl);
	if (!playback) {
		print_error("Can't load Adplug file!");
		stop();
		return;
	}
	playback->seek(unsigned long(p_from_pos*1000));
	towrite = RATE / playback->getrefresh();
	active = true;
}

void AudioStreamPlaybackAdlib::seek(double p_time) {
	if (p_time < 0) {
		p_time = 0;
	}
	playback->seek(unsigned long(p_time*1000));
	towrite = RATE / playback->getrefresh();
	// if (!playback->update()) {
		// stop();
	// }
}


int AudioStreamPlaybackAdlib::mix(AudioFrame *p_buffer, float p_rate_scale, int p_frames) {
	if (!active) {
		stop();
		return 0;
	}
	
	if (stereo == true) {
		short buf[STEREO_BUFSIZE];
		unsigned long write;
		write = BUFSIZE;
		opl->update(buf, write);
		
		for (unsigned int buf_index = 0; buf_index < STEREO_BUFSIZE; buf_index+=2) {
			float sample1 = float(buf[buf_index]) / BIT16_TO_FLOAT;
			float sample2 = float(buf[buf_index+1]) / BIT16_TO_FLOAT;
			p_buffer[buf_index/2] = AudioFrame(sample1, sample2);
		}
		
		if (towrite > BUFSIZE) {
			towrite -= write;
		}
		else {
			towrite = RATE / playback->getrefresh();
			if (!playback->update()) {
				stop();
				return 0;
			}
		}
		return p_frames;
	}
	else {
		short buf[BUFSIZE];
		opl->update(buf, BUFSIZE);
		
		for (unsigned int buf_index = 0; buf_index < BUFSIZE; buf_index++) {
			float sample = float(buf[buf_index]) / BIT16_TO_FLOAT;
			p_buffer[buf_index] = AudioFrame(sample, sample);
		}
		
		if (towrite > BUFSIZE) {
			towrite -= BUFSIZE;
		}
		else {
			towrite = RATE / playback->getrefresh();
			if (!playback->update()) {
				stop();
				return 0;
			}
		}
		return p_frames;
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

