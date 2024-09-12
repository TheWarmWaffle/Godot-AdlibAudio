/*  resource_importer_adplug.cpp  */

#include "resource_importer_adplug.h"
#include "audio_stream_adlib.h"
#include "core/io/file_access.h"
#include "core/io/resource_saver.h"
#include "scene/resources/texture.h"

#ifdef TOOLS_ENABLED
#include "editor/import/audio_stream_import_settings.h"
#endif

#include "core/string/print_string.h"
#include <array>

// #include <ogg/ogg.h>
// #include <vorbis/codec.h>

String ResourceImporterAdplug::get_importer_name() const {
	return "adplugstr";
}

String ResourceImporterAdplug::get_visible_name() const {
	return "adplugstr";
}

void ResourceImporterAdplug::get_recognized_extensions(List<String> *p_extensions) const {
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
		if (!p_extensions->find(new_ext[i])) {
			p_extensions->push_back(new_ext[i]);
		}
	}
}

String ResourceImporterAdplug::get_save_extension() const {
	return "adplugstr";
}

String ResourceImporterAdplug::get_resource_type() const {
	return "AudioStreamAdlib";
}

bool ResourceImporterAdplug::get_option_visibility(const String &p_path, const String &p_option, const HashMap<StringName, Variant> &p_options) const {
	return true;
}

int ResourceImporterAdplug::get_preset_count() const {
	return 0;
}

String ResourceImporterAdplug::get_preset_name(int p_idx) const {
	return String();
}

void ResourceImporterAdplug::get_import_options(const String &p_path, List<ImportOption> *r_options, int p_preset) const {
	r_options->push_back(ImportOption(PropertyInfo(Variant::BOOL, "loop"), false));
	r_options->push_back(ImportOption(PropertyInfo(Variant::INT, "chipset", PROPERTY_HINT_ENUM, "OPL2,DUAL OPL2,OPL3,AUTO"), 3));
	// r_options->push_back(ImportOption(PropertyInfo(Variant::FLOAT, "loop_offset"), 0));
	// r_options->push_back(ImportOption(PropertyInfo(Variant::FLOAT, "bpm", PROPERTY_HINT_RANGE, "0,400,0.01,or_greater"), 0));
	// r_options->push_back(ImportOption(PropertyInfo(Variant::INT, "beat_count", PROPERTY_HINT_RANGE, "0,512,or_greater"), 0));
	// r_options->push_back(ImportOption(PropertyInfo(Variant::INT, "bar_beats", PROPERTY_HINT_RANGE, "2,32,or_greater"), 4));
}

#ifdef TOOLS_ENABLED

bool ResourceImporterAdplug::has_advanced_options() const {
	return true;
}

void ResourceImporterAdplug::show_advanced_options(const String &p_path) {
	Ref<AudioStreamAdlib> adlib_stream;
	adlib_stream.instantiate();
	if (adlib_stream.is_valid()) {
		AudioStreamImportSettingsDialog::get_singleton()->edit(p_path, "adplugstr", adlib_stream);
	}
}
#endif

Error ResourceImporterAdplug::import(const String &p_source_file, const String &p_save_path, const HashMap<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files, Variant *r_metadata) {
	bool loop = p_options["loop"];
	int chipset = p_options["chipset"];
	// double loop_offset = p_options["loop_offset"];
	// double bpm = p_options["bpm"];
	// int beat_count = p_options["beat_count"];
	// int bar_beats = p_options["bar_beats"];

	// Ref<AudioStreamAdlib> adlib_stream = load_from_file(p_source_file);
	Ref<AudioStreamAdlib> adlib_stream;
	adlib_stream.instantiate();
	if (adlib_stream.is_null()) {
		return ERR_CANT_OPEN;
	}
	
	adlib_stream->set_loop(loop);
	adlib_stream->set_chipset(static_cast<AudioStreamAdlib::ChipType>(chipset));
	adlib_stream->set_file_path(String(p_source_file));
	print_line("Import: ", p_source_file);
	// adlib_stream->set_loop_offset(loop_offset);
	// adlib_stream->set_bpm(bpm);
	// adlib_stream->set_beat_count(beat_count);
	// adlib_stream->set_bar_beats(bar_beats);
	return ResourceSaver::save(adlib_stream, p_save_path + ".adplugstr");
}

ResourceImporterAdplug::ResourceImporterAdplug() {
}

// void ResourceImporterAdplug::_bind_methods() {
	// ClassDB::bind_static_method("ResourceImporterAdplug", D_METHOD("load_from_buffer", "buffer"), &ResourceImporterAdplug::load_from_buffer);
	// ClassDB::bind_static_method("ResourceImporterAdplug", D_METHOD("load_from_file", "path"), &ResourceImporterAdplug::load_from_file);
// }


// Ref<AudioStreamAdlib> ResourceImporterAdplug::load_from_file(const String &p_path) {
	// Vector<uint8_t> file_data = FileAccess::get_file_as_bytes(p_path);
	// ERR_FAIL_COND_V_MSG(file_data.is_empty(), Ref<AudioStreamAdlib>(), "Cannot open file '" + p_path + "'.");
	// return load_from_buffer(file_data);
// }
