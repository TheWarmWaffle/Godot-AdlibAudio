/* audiostream_mytone.cpp */
// #include "audio_stream_playback_adlib.h"
#include "audio_stream_adlib.h"
#include "core/string/print_string.h"

AudioStreamAdlib::AudioStreamAdlib() : mix_rate(44100), stereo(false), hz(639) {
}

void AudioStreamAdlib::set_file_path(String _filename) {
	filename = _filename;
	// print_line("AAA FILENAME: ", filename);
}

Ref<AudioStreamPlayback> AudioStreamAdlib::instantiate_playback() {
	print_line("Here");
	Ref<AudioStreamPlaybackAdlib> playback;
	print_line("Here1");
	playback.instantiate();
	print_line("Here2");
	playback->base = Ref<AudioStreamAdlib>(this);
	print_line("Here3");
	return playback;
}

String AudioStreamAdlib::get_stream_name() const {
	return "";
}

void AudioStreamAdlib::reset() {
	set_position(0);
}
void AudioStreamAdlib::set_position(uint64_t p) {
	pos = p;
}

void AudioStreamAdlib::set_loop(bool p_enable) {
	loop = p_enable;
}

bool AudioStreamAdlib::has_loop() const {
	return loop;
}

void AudioStreamAdlib::set_chipset(ChipType p_type) {
	chipset = p_type;
}

AudioStreamAdlib::ChipType AudioStreamAdlib::get_chipset() {
	return chipset;
}

void AudioStreamAdlib::_bind_methods() {
	ClassDB::bind_method(D_METHOD("reset"), &AudioStreamAdlib::reset);
	ClassDB::bind_method(D_METHOD("get_stream_name"), &AudioStreamAdlib::get_stream_name);
	
	ClassDB::bind_method(D_METHOD("set_loop", "enable"), &AudioStreamAdlib::set_loop);
	ClassDB::bind_method(D_METHOD("has_loop"), &AudioStreamAdlib::has_loop);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "loop"), "set_loop", "has_loop");

	ClassDB::bind_method(D_METHOD("set_chipset", "chip_type"), &AudioStreamAdlib::set_chipset);
	ClassDB::bind_method(D_METHOD("get_chipset"), &AudioStreamAdlib::get_chipset);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "chipset"), "set_chipset", "get_chipset");
	
	BIND_ENUM_CONSTANT(TYPE_OPL2);
	BIND_ENUM_CONSTANT(TYPE_DUAL_OPL2);
	BIND_ENUM_CONSTANT(TYPE_OPL3);
}
