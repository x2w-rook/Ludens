#pragma once

#include "Core/RenderBase/Include/RPipeline.h"

namespace LD
{

/// base class for first-party graphics pipelines
class PrefabPipeline
{
public:
	PrefabPipeline() = default;
	PrefabPipeline(const PrefabPipeline&) = delete;
	virtual ~PrefabPipeline()
	{
		LD_DEBUG_ASSERT(!mHandle);
	}

	PrefabPipeline& operator=(const PrefabPipeline&) = delete;

	operator bool() const
	{
		return (bool)mHandle;
	}

	explicit operator RPipeline() const
	{
		LD_DEBUG_ASSERT(mHandle);
		return mHandle;
	}

	virtual RPipelineLayoutData GetLayoutData() const = 0;

protected:
	RPipeline mHandle;
};

} // namespace LD