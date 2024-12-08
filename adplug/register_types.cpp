/* register_types.cpp */

// #include "register_types.h"

// #include "core/object/class_db.h"
// #include "audio_stream_adlib.h"
// #include "audio_stream_playback_adlib.h"
// #include "adplug_loader.h"

// static Ref<ResourceFormatLoaderAdplug> adplug_loader;

// void initialize_adplug_module(ModuleInitializationLevel p_level) {
	// if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		// return;
	// }
	// ClassDB::register_class<AudioStreamAdlib>();
	// ClassDB::register_class<AudioStreamPlaybackAdlib>();
	
	
	// adplug_loader.instantiate();
	// ResourceLoader::add_resource_format_loader(adplug_loader);
// }

// void uninitialize_adplug_module(ModuleInitializationLevel p_level) {
	// if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		// return;
	// }
	
	// ResourceLoader::remove_resource_format_loader(adplug_loader);
	// adplug_loader.unref();
// }



#include "register_types.h"

#include "audio_stream_adlib.h"

#ifdef TOOLS_ENABLED
#include "resource_importer_adplug.h"
#endif

void initialize_adplug_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

#ifdef TOOLS_ENABLED
	if (Engine::get_singleton()->is_editor_hint()) {
		Ref<ResourceImporterAdplug> adplug_importer;
		adplug_importer.instantiate();
		ResourceFormatImporter::get_singleton()->add_importer(adplug_importer);
	}

	ClassDB::APIType prev_api = ClassDB::get_current_api();
	ClassDB::set_current_api(ClassDB::API_EDITOR);

	// Required to document import options in the class reference.
	GDREGISTER_CLASS(ResourceImporterAdplug);

	ClassDB::set_current_api(prev_api);
#endif

	GDREGISTER_CLASS(AudioStreamAdlib);
	GDREGISTER_CLASS(AudioStreamPlaybackAdlib);
}

void uninitialize_adplug_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}
