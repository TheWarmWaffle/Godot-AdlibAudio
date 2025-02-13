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

AudioStreamPlaybackAdlib::AudioStreamPlaybackAdlib() {
}

AudioStreamPlaybackAdlib::~AudioStreamPlaybackAdlib() {
	cout << "Playback Adlib deinit, active = " << playback << "\n";
	stop();
}

void AudioStreamPlaybackAdlib::stop() {
	cout << "Stop " << opl << "\n";
	if (adplug_buffer) {
		cout << "free buffer" << "\n";
		memdelete_arr(adplug_buffer);
	}
	adplug_buffer = nullptr;
	if (playback) {
		cout << "free playback" << "\n";
		delete playback;
		playback = nullptr;
	}
	if (opl) {
		cout << "free opl" << "\n";
		delete opl;
		opl = nullptr;
	}
	towrite = 0;
	active = false;
}

void AudioStreamPlaybackAdlib::start(double p_from_pos) {
	active = true;
	cout << "Playback Adlib start1" << "\n";
	Copl::ChipType copl_chip_type = static_cast<Copl::ChipType>(base->get_chipset());
	if (adplug_buffer) {
		print_error("Already Allocated buffer, Please report");
	}
	if (opl) {
		print_error("Already Allocated opl, Please report");
	}
	if (playback) {
		print_error("Already Allocated playback, Please report");
	}
	if (base->emulator == AudioStreamAdlib::NUKED) { // Opl3, Recommended.
		opl = new CNemuopl(RATE);
		stereo = true;
		adplug_buffer = memnew_arr(short, BUFSIZE);
	}
	else { // elif (base->emulator == AudioStreamAdlib::ADPLUG) { // Opl2, Dual Opl2, Opl3.
		if (copl_chip_type == Copl::TYPE_DUAL_OPL2 || copl_chip_type == Copl::TYPE_OPL3) {
			CEmuopl *new_opl = new CEmuopl(RATE, BIT16, false); // Need to have a throwoway variable, so we can properly access settype().
			new_opl->settype(copl_chip_type);
			opl = new_opl;
			new_opl = NULL;
			stereo = true;
			adplug_buffer = memnew_arr(short, STEREO_BUFSIZE);
		}
		else {
			CEmuopl *new_opl = new CEmuopl(RATE, BIT16, false); // Need to have a throwoway variable, so we can properly access settype().
			new_opl->settype(copl_chip_type);
			opl = new_opl;
			new_opl = NULL;
			stereo = false;
			adplug_buffer = memnew_arr(short, BUFSIZE);
		}
	}
	if (!adplug_buffer) {
		cout << "Buffer failed to be allocated" << adplug_buffer << "\n";
	}
	if (!opl) {
		cout << "Opl failed to be allocated" << opl << "\n";
	}
	if (base->emulator == AudioStreamAdlib::NUKED) { // Opl3, Recommended.
		opl = new CNemuopl(RATE);
		stereo = true;
		adplug_buffer = (short *)memalloc(STEREO_BUFSIZE);
	}
	
	String GlobalFilePath = ProjectSettings::get_singleton()->globalize_path(base->file_path);
	const char *char_path = GlobalFilePath.utf8();
	playback = CAdPlug::factory(char_path, &*opl);
	if (!playback) {
		print_error("Can't load Adplug file! " + base->file_path);
		stop();
		return;
	}
	playback->seek(unsigned long(p_from_pos*1000));
	towrite = RATE / playback->getrefresh();
}

void AudioStreamPlaybackAdlib::seek(double p_time) {
	if (p_time < 0) {
		p_time = 0;
	}
	cout << "Seek";
	if (playback) {
		playback->seek(unsigned long(p_time*1000));
		towrite = RATE / playback->getrefresh();
	}
	// if (!playback->update()) {
		// stop();
	// }
}

int AudioStreamPlaybackAdlib::_process(AudioFrame *p_buffer, unsigned int p_frames, unsigned int p_buffer_offset) {
	unsigned int write = (towrite > p_frames ? p_frames : towrite);
	if (stereo == true) {
		unsigned int halfwrite = write/2; // It seems as though we can't use buffer sizes of 1024 with adplug, so instead we use 512 twice for stereo audio.
		opl->update(adplug_buffer, halfwrite);
		unsigned int buf_index = 0;
		for (buf_index = 0; buf_index < halfwrite; buf_index++) {
			float sample1 = float(adplug_buffer[(buf_index*2)]) / BIT16_TO_FLOAT;
			float sample2 = float(adplug_buffer[(buf_index*2)+1]) / BIT16_TO_FLOAT;
			p_buffer[p_buffer_offset + buf_index] = AudioFrame(sample1, sample2);
		}
		opl->update(adplug_buffer, halfwrite);
		for (buf_index = 0; buf_index < halfwrite; buf_index++) {
			float sample1 = float(adplug_buffer[(buf_index*2)]) / BIT16_TO_FLOAT;
			float sample2 = float(adplug_buffer[(buf_index*2)+1]) / BIT16_TO_FLOAT;
			p_buffer[p_buffer_offset + (buf_index+halfwrite)] = AudioFrame(sample1, sample2);
		}
	}
	else {
		opl->update(adplug_buffer, write);
		for (unsigned int buf_index = 0; buf_index < write; buf_index++) {
			float sample = float(adplug_buffer[buf_index]) / BIT16_TO_FLOAT;
			p_buffer[p_buffer_offset + buf_index] = AudioFrame(sample, sample);
		}
	}
	towrite -= write;
	if (towrite <= 0) { // When true we had to make the last buffer smaller, therefore we need to to update
		towrite = RATE / playback->getrefresh();
		if (!playback->update()) {
			stop();
			return write;
		}
		if (write < p_frames) { // if false there were no more p_frames left to process anyways, so don't.
			write += _process(p_buffer, p_frames - write, p_buffer_offset + write); // Finish the buffer since towrite ran out.
		}
	}
	
	return write;
}

int AudioStreamPlaybackAdlib::mix(AudioFrame *p_buffer, float p_rate_scale, int p_frames) {
	if (!adplug_buffer || !opl || !playback) {
		return 0;
	}
	unsigned long total_frames_processed = 0, left_over_towrite = p_frames % BUFSIZE;
	int write = ((unsigned int)p_frames - left_over_towrite) / BUFSIZE;
	int buffer_offset = 0;
	for (buffer_offset; buffer_offset < write; buffer_offset++) {
		unsigned long new_frames_processed = _process(p_buffer, BUFSIZE, BUFSIZE * buffer_offset);
		total_frames_processed += new_frames_processed;
		if (new_frames_processed < BUFSIZE) { // Song Ended.
			break;
		}
	}
	if (left_over_towrite != 0) {
		total_frames_processed += _process(p_buffer, left_over_towrite, buffer_offset);
	}
	return total_frames_processed;
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

