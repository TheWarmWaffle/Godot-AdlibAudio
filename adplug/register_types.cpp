/* register_types.cpp */

#include "register_types.h"

#include "core/object/class_db.h"
#include "audio_stream_adlib.h"
#include "audio_stream_playback_adlib.h"
#include "adplug_loader.h"

static Ref<ResourceFormatLoaderAdplug> adplug_loader;

void initialize_adplug_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<AudioStreamAdlib>();
	ClassDB::register_class<AudioStreamPlaybackAdlib>();
	
	
	adplug_loader.instantiate();
	ResourceLoader::add_resource_format_loader(adplug_loader);
}

void uninitialize_adplug_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ResourceLoader::remove_resource_format_loader(adplug_loader);
	adplug_loader.unref();
}
