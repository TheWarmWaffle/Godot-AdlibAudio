#include "adplug_loader.h"
#include "audio_stream_adlib.h"
// #include "core/io/resource.h"
#include "core/string/print_string.h"
#include "core/config/project_settings.h"
#include <iostream>
#include <array>
Ref<Resource> ResourceFormatLoaderAdplug::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress, CacheMode p_cache_mode) {
	// AudioStreamAdlib *adlib_stream = memnew(AudioStreamAdlib);
	// print_line("I was here");
	Ref<AudioStreamAdlib> adlib_stream;
	adlib_stream.instantiate();
	if (r_error) {
		*r_error = OK;
	}
	// String GlobalPath = ProjectSettings::get_singleton()->globalize_path(p_path);
	// const char *char_path = GlobalPath.utf8();
	// print_line(char_path);
	print_line("AAA", p_path);
	adlib_stream->set_file_path(p_path);
	return adlib_stream;
}

void ResourceFormatLoaderAdplug::get_recognized_extensions(List<String> *r_extensions) const {
	// if (!r_extensions->find("rad")) {
		// r_extensions->push_back("rad");
	// }
	
	std::array<String, 42> new_ext = {
	"a2m",
	"adl",
	"agd",
	"amd",
	"bam",
	"bmf",
	"cff",
	"cmf",
	"d00",
	"dfm",
	"dmo",
	"dro",
	"dtm",
	"got",
	"ha2",
	"hsc",
	"hsp",
	"hsq",
	"imf",
	"ims",
	"ksm",
	"ksm",
	"laa",
	"lds",
	"m",
	"mad",
	// "mdi",
	// "mid",
	// "mkj",
	// "msc",
	// "mtk",
	// "mus",
	"rad",
	// "raw",
	// "rix",
	"rol",//Needs file ‘standard.bnk’ in the same directory as loaded file
	// "s3m",
	// "sa2",
	// "sat",
	// "sci",
	"sdb",
	"sng",
	"sop",
	"sqx",
	"vgm",//Supports OPL2, Dual OPL2 and OPL3 playback.
	"xad",
	"xms",
	"xsm"
	};
	for (int i = 0; i < new_ext.size(); i++) {
		if (!r_extensions->find(new_ext[i])) {
			r_extensions->push_back(new_ext[i]);
		}
	}
}


String ResourceFormatLoaderAdplug::get_resource_type(const String &p_path) const {
	return "AudioStreamAdlib";
}

bool ResourceFormatLoaderAdplug::handles_type(const String &p_type) const {
	return (p_type == "AudioStreamAdlib");
}