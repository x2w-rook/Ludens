#pragma once

#include <string>
#include <unordered_map>
#include <glslang/Public/ShaderLang.h>
#include "Core/OS/Include/UID.h"
#include "Core/IO/Include/FileSystem.h"
#include "Core/DSA/Include/Vector.h"
#include "Core/DSA/Include/View.h"
#include "Core/Header/Include/Types.h"
#include "Core/RenderBase/Include/RResult.h"
#include "Core/RenderBase/Include/RDevice.h"


namespace LD {

	struct RPipelineLayout;

	enum class RShaderSourceType
	{
		GLSL,
		SPIRV,
	};

	enum class RDataType
	{
		Float,
		Vec2,
		Vec3,
		Vec4,
	};

	struct RShaderInfo
	{
		const char* Name = nullptr;
		RShaderType Type;
		RShaderSourceType SourceType;
		const void* Data = nullptr;
		u32 Size = 0;
	};

	struct RShaderBase;
	struct RShaderGL;

	// shader handle and interface
	class RShader
	{
		friend struct RShaderBase;
		friend struct RShaderGL;
	public:
		using TBase = RShaderBase;

		inline operator bool() const { return mID != 0 && mShader != nullptr; }
		inline void Reset() { mID = 0; mShader = nullptr; }

		inline operator RShaderBase*() const { return mShader; }

	private:
		UID mID = 0;
		RShaderBase* mShader = nullptr;
	};

	// compiles RenderBase cananical GLSL to SPIRV that is compatible with target backend,
	// - OpenGL backend eventually passes to glShaderBinary
	// - Vulkan backend eventually passes to VkShaderModuleCreateInfo
	// the compiler also checks if the GLSL bindings are compatible with the
	// pipeline layout, detecting errors early.
	class RShaderCompiler
	{
	public:
		RShaderCompiler() = delete;
		RShaderCompiler(const RShaderCompiler&) = delete;
		~RShaderCompiler();

		explicit RShaderCompiler(RBackend targetBackend);

		RShaderCompiler& operator=(const RShaderCompiler&) = delete;

		void Compile(const RPipelineLayout& layout, RShaderType type, const std::string& glsl, Vector<u32>& spirv);
		void Compile(const RPipelineLayout& layout, RShaderType type, const std::string& glsl, Vector<u8>& spirv);

	private:
		void GlslangShaderType(RShaderType type, EShLanguage* language);
		void GlslangBackend(RBackend backend, glslang::EShClient* client, glslang::EShTargetClientVersion* version);
		void GlslangCompile(RBackend backend, RShaderType type, const std::string& glsl, const std::string& glslPreamble, Vector<u32>& spirv);
		void PatchOpenGL(const RPipelineLayout& layout, RShaderType type, std::string& glsl);

		RBackend mTargetBackend;
	};

	struct RShaderCacheInfo
	{
		RDevice Device;
		Path CacheDirectory;
	};

	// utility service to cache and retrieve spirv binaries on disk,
	// this class should have a shorter lifetime than RShaderCacheInfo::Device,
	// which is used to create and delete shaders.
	class RShaderCache
	{
	public:
		RShaderCache() = default;
		RShaderCache(const RShaderCache&) = delete;
		~RShaderCache();

		RShaderCache& operator=(const RShaderCache&) = delete;

		void Startup(const RShaderCacheInfo& info);
		void Cleanup();

		RResult GetOrCreateShader(const RPipelineLayout& layout, RShaderType type, const std::string& glsl, std::string name, RShader& shader);

	private:
		struct CacheInfo
		{
			RShaderType ShaderType;
			RShader Shader;
			size_t SourceHash;
		};

		std::unordered_map<std::string, CacheInfo> mCacheInfo;
		RDevice mDevice;
		Path mCacheDirectory;
		bool mHasStartup = false;
	};

} // namespace LD
