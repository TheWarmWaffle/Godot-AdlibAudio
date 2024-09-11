#ifndef ADPLUG_LOADER_H
#define ADPLUG_LOADER_H
#include "audio_stream_adlib.h"
#include "core/io/resource_loader.h"
#include "core/io/resource.h"

class ResourceFormatLoaderAdplug : public ResourceFormatLoader {
	GDCLASS(ResourceFormatLoaderAdplug, ResourceFormatLoader);
public:
	virtual Ref<Resource> load(const String &p_path, const String &p_original_path = "", Error *r_error = nullptr, bool p_use_sub_threads = false, float *r_progress = nullptr, CacheMode p_cache_mode = CACHE_MODE_REUSE);
	virtual void get_recognized_extensions(List<String> *r_extensions) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;
};
#endif // ADPLUG_LOADER_H