#pragma once

#include <string>
#include "Core/Header/Include/Types.h"
#include "Core/RenderBase/Include/RDevice.h"
#include "Core/DSA/Include/View.h"

namespace LD {
	

	struct RBindingInfo;
	struct RBindingGroupInfo;
	struct RBindingGroupLayoutInfo;
	struct RBindingGroupBase;
	struct RBindingGroupLayoutBase;
	class RBindingGroupLayout;
	class RBindingGroup;
	class RTexture;

	enum class RBindingType : u8
	{
		Texture = 0,
		UniformBuffer,
	};

	// Binding group handle and interface.
	// Describes a group of bindings that have similiar access frequencies in the shader.
	class RBindingGroup
	{
		friend struct RBindingGroupBase;
		friend struct RBindingGroupGL;
	public:
		using TBase = RBindingGroupBase;

		inline bool IsValid() const { return mID != 0 && mGroup != nullptr; }

		inline bool operator==(const RBindingGroup& other) const { return mID == other.mID; }
		inline bool operator!=(const RBindingGroup& other) const { return mID != other.mID; }

		inline operator RBindingGroupBase*() const { return mGroup; }

		RResult BindTexture(u32 binding, RTexture& textureH, int arrayIndex = 0);
		RResult BindUniformBuffer(u32 binding, RBuffer& bufferH);

	private:
		UID mID = 0;
		RBindingGroupBase* mGroup = nullptr;
		RBackend mBackend;
	};

	// Binding group layout handle and interface.
	// Used during binding group creation and pipeline creation.
	class RBindingGroupLayout
	{
		friend class RBindingGroupLayoutBase;
		friend class RBindingGroupLayoutGL;
	public:
		using TBase = RBindingGroupLayoutBase;

		inline bool IsValid() const { return mID != 0 && mGroupLayout != nullptr; }

		inline bool operator==(const RBindingGroupLayout& other) const { return mID == other.mID; }
		inline bool operator!=(const RBindingGroupLayout& other) const { return mID != other.mID; }

		inline operator RBindingGroupLayoutBase*() const { return mGroupLayout; }

	private:
		UID mID = 0;
		RBindingGroupLayoutBase* mGroupLayout = nullptr;
	};

	struct RBindingInfo
	{
		RBindingType Type;
		int Count = 1;
	};

	// Info to create a binding group.
	struct RBindingGroupInfo
	{
		RBindingGroupLayout Layout;
	};

	// Info to create a binding group layout
	struct RBindingGroupLayoutInfo
	{
		View<RBindingInfo> Bindings;
	};


} // namespace LD
