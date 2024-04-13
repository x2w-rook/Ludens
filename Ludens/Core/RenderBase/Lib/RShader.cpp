#include <iostream>
#include <vector>
#include <set>
#include <glslang/Include/Common.h>
#include <glslang/Include/ResourceLimits.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <spirv_cross/spirv_glsl.hpp>
#include "Core/RenderBase/Include/RShader.h"
#include "Core/RenderBase/Lib/RBase.h"
#include "Core/Header/Include/Error.h"
#include "Core/Header/Include/Types.h"
#include "Core/OS/Include/Exit.h"

namespace LD {

    // Glslang Version 11.8.0,  glslang/OSDependent/Web/glslang.js.cpp
    // TBuiltInResource does not provide default values, so we have to steal from somewhere.
    static const TBuiltInResource sDefaultTBuiltInResource = {
        /* .MaxLights = */ 32,
        /* .MaxClipPlanes = */ 6,
        /* .MaxTextureUnits = */ 32,
        /* .MaxTextureCoords = */ 32,
        /* .MaxVertexAttribs = */ 64,
        /* .MaxVertexUniformComponents = */ 4096,
        /* .MaxVaryingFloats = */ 64,
        /* .MaxVertexTextureImageUnits = */ 32,
        /* .MaxCombinedTextureImageUnits = */ 80,
        /* .MaxTextureImageUnits = */ 32,
        /* .MaxFragmentUniformComponents = */ 4096,
        /* .MaxDrawBuffers = */ 32,
        /* .MaxVertexUniformVectors = */ 128,
        /* .MaxVaryingVectors = */ 8,
        /* .MaxFragmentUniformVectors = */ 16,
        /* .MaxVertexOutputVectors = */ 16,
        /* .MaxFragmentInputVectors = */ 15,
        /* .MinProgramTexelOffset = */ -8,
        /* .MaxProgramTexelOffset = */ 7,
        /* .MaxClipDistances = */ 8,
        /* .MaxComputeWorkGroupCountX = */ 65535,
        /* .MaxComputeWorkGroupCountY = */ 65535,
        /* .MaxComputeWorkGroupCountZ = */ 65535,
        /* .MaxComputeWorkGroupSizeX = */ 1024,
        /* .MaxComputeWorkGroupSizeY = */ 1024,
        /* .MaxComputeWorkGroupSizeZ = */ 64,
        /* .MaxComputeUniformComponents = */ 1024,
        /* .MaxComputeTextureImageUnits = */ 16,
        /* .MaxComputeImageUniforms = */ 8,
        /* .MaxComputeAtomicCounters = */ 8,
        /* .MaxComputeAtomicCounterBuffers = */ 1,
        /* .MaxVaryingComponents = */ 60,
        /* .MaxVertexOutputComponents = */ 64,
        /* .MaxGeometryInputComponents = */ 64,
        /* .MaxGeometryOutputComponents = */ 128,
        /* .MaxFragmentInputComponents = */ 128,
        /* .MaxImageUnits = */ 8,
        /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
        /* .MaxCombinedShaderOutputResources = */ 8,
        /* .MaxImageSamples = */ 0,
        /* .MaxVertexImageUniforms = */ 0,
        /* .MaxTessControlImageUniforms = */ 0,
        /* .MaxTessEvaluationImageUniforms = */ 0,
        /* .MaxGeometryImageUniforms = */ 0,
        /* .MaxFragmentImageUniforms = */ 8,
        /* .MaxCombinedImageUniforms = */ 8,
        /* .MaxGeometryTextureImageUnits = */ 16,
        /* .MaxGeometryOutputVertices = */ 256,
        /* .MaxGeometryTotalOutputComponents = */ 1024,
        /* .MaxGeometryUniformComponents = */ 1024,
        /* .MaxGeometryVaryingComponents = */ 64,
        /* .MaxTessControlInputComponents = */ 128,
        /* .MaxTessControlOutputComponents = */ 128,
        /* .MaxTessControlTextureImageUnits = */ 16,
        /* .MaxTessControlUniformComponents = */ 1024,
        /* .MaxTessControlTotalOutputComponents = */ 4096,
        /* .MaxTessEvaluationInputComponents = */ 128,
        /* .MaxTessEvaluationOutputComponents = */ 128,
        /* .MaxTessEvaluationTextureImageUnits = */ 16,
        /* .MaxTessEvaluationUniformComponents = */ 1024,
        /* .MaxTessPatchComponents = */ 120,
        /* .MaxPatchVertices = */ 32,
        /* .MaxTessGenLevel = */ 64,
        /* .MaxViewports = */ 16,
        /* .MaxVertexAtomicCounters = */ 0,
        /* .MaxTessControlAtomicCounters = */ 0,
        /* .MaxTessEvaluationAtomicCounters = */ 0,
        /* .MaxGeometryAtomicCounters = */ 0,
        /* .MaxFragmentAtomicCounters = */ 8,
        /* .MaxCombinedAtomicCounters = */ 8,
        /* .MaxAtomicCounterBindings = */ 1,
        /* .MaxVertexAtomicCounterBuffers = */ 0,
        /* .MaxTessControlAtomicCounterBuffers = */ 0,
        /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
        /* .MaxGeometryAtomicCounterBuffers = */ 0,
        /* .MaxFragmentAtomicCounterBuffers = */ 1,
        /* .MaxCombinedAtomicCounterBuffers = */ 1,
        /* .MaxAtomicCounterBufferSize = */ 16384,
        /* .MaxTransformFeedbackBuffers = */ 4,
        /* .MaxTransformFeedbackInterleavedComponents = */ 64,
        /* .MaxCullDistances = */ 8,
        /* .MaxCombinedClipAndCullDistances = */ 8,
        /* .MaxSamples = */ 4,
        /* .maxMeshOutputVerticesNV = */ 256,
        /* .maxMeshOutputPrimitivesNV = */ 512,
        /* .maxMeshWorkGroupSizeX_NV = */ 32,
        /* .maxMeshWorkGroupSizeY_NV = */ 1,
        /* .maxMeshWorkGroupSizeZ_NV = */ 1,
        /* .maxTaskWorkGroupSizeX_NV = */ 32,
        /* .maxTaskWorkGroupSizeY_NV = */ 1,
        /* .maxTaskWorkGroupSizeZ_NV = */ 1,
        /* .maxMeshViewCountNV = */ 4,
        /* .maxDualSourceDrawBuffersEXT = */ 1,

        /* .limits = */ {
            /* .nonInductiveForLoops = */ 1,
            /* .whileLoops = */ 1,
            /* .doWhileLoops = */ 1,
            /* .generalUniformIndexing = */ 1,
            /* .generalAttributeMatrixVectorIndexing = */ 1,
            /* .generalVaryingIndexing = */ 1,
            /* .generalSamplerIndexing = */ 1,
            /* .generalVariableIndexing = */ 1,
            /* .generalConstantMatrixVectorIndexing = */ 1,
        }
    };

    struct Qualifier
    {
        u32 Group;
        u32 Binding;

        bool operator==(const Qualifier& other) const
        {
            return Group == other.Group && Binding == other.Binding;
        }

        bool operator<(const Qualifier& other) const
        {
            if (Group != other.Group)
                return Group < other.Group;

            return Binding < other.Binding;
        }
    };

    RShaderCompiler::RShaderCompiler(RBackend backend)
        : mTargetBackend(backend)
    {
        static bool sIsFirstInstance = true;

        if (sIsFirstInstance)
        {
            sIsFirstInstance = false;

            glslang::InitializeProcess();

            AtExit([]() { glslang::FinalizeProcess(); });
        }
    }

	RShaderCompiler::~RShaderCompiler()
	{
	}

    void RShaderCompiler::Compile(const RPipelineLayout& layout, RShaderType type, const std::string& glsl, Vector<u32>& spirv)
    {
        std::string input_glsl(glsl);
        std::string preamble;

        if (mTargetBackend == RBackend::OpenGL)
        {
            PatchOpenGL(layout, type, input_glsl);
            preamble = "#define LD_OPENGL\n";

            // dirty debug dump
            printf("=== Patched GLSL Begin === \n");
            printf("%s\n", input_glsl.c_str());
            printf("=== Patched GLSL End === \n");
        }
        else if (mTargetBackend == RBackend::Vulkan)
            preamble = "#define LD_VULKAN\n";

        GlslangCompile(mTargetBackend, type, input_glsl, preamble, spirv);
    }

    void RShaderCompiler::Compile(const RPipelineLayout& layout, RShaderType type, const std::string& glsl, Vector<u8>& spirv)
    {
        Vector<u32> spirv_u32;
        Compile(layout, type, glsl, spirv_u32);

        spirv.Resize(spirv_u32.Size() * 4);
        for (size_t i = 0; i < spirv_u32.Size(); i++)
        {
            spirv[4 * i + 0] = spirv_u32[i] & 0xFF;
            spirv[4 * i + 1] = (spirv_u32[i] >> 8) & 0xFF;
            spirv[4 * i + 2] = (spirv_u32[i] >> 16) & 0xFF;
            spirv[4 * i + 3] = (spirv_u32[i] >> 24) & 0xFF;
        }
    }

    void RShaderCompiler::GlslangShaderType(RShaderType type, EShLanguage* planguage)
    {
        EShLanguage language;

        switch (type)
        {
        case RShaderType::VertexShader:
            language = EShLangVertex;
            break;
        case RShaderType::FragmentShader:
            language = EShLangFragment;
            break;
        default:
            LD_DEBUG_UNREACHABLE;
        }

        if (planguage)
            *planguage = language;
    }

    void RShaderCompiler::GlslangBackend(RBackend backend, glslang::EShClient* pclient, glslang::EShTargetClientVersion* pversion)
    {
        glslang::EShClient client;
        glslang::EShTargetClientVersion version;

        switch (backend)
        {
        case RBackend::OpenGL:
            client = glslang::EShClientOpenGL;
            version = glslang::EShTargetOpenGL_450;
            break;
        case RBackend::Vulkan:
            client = glslang::EShClientVulkan;
            version = glslang::EShTargetVulkan_1_3;
            break;
        }

        if (pclient)
            *pclient = client;
        if (*pversion)
            *pversion = version;
    }

    void RShaderCompiler::GlslangCompile(RBackend backend, RShaderType type, const std::string& glsl, const std::string& glslPreamble, Vector<u32>& spirv)
    {
        EShLanguage shaderType;
        glslang::EShClient client;
        glslang::EShTargetClientVersion clientVersion;

        GlslangShaderType(type, &shaderType);
        GlslangBackend(backend, &client, &clientVersion);

        std::string preamble = "#define group set\n" + glslPreamble;
        glslang::TShader shader(shaderType);
        {
            const char* glslSource = glsl.c_str();
            shader.setStrings(&glslSource, 1);
            shader.setEntryPoint("main");
            shader.setPreamble(preamble.c_str());
            shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);
            shader.setEnvClient(client, clientVersion);

            bool result = shader.parse(&sDefaultTBuiltInResource, 100, false, EShMsgDefault);
            const char* log;
            
            if ((log = shader.getInfoLog()) && strlen(log) > 0)
                printf("%s\n", log);

            if ((log = shader.getInfoDebugLog()) && strlen(log) > 0)
                printf("%s\n", log);

            LD_DEBUG_ASSERT(result);
        }

        glslang::TProgram program{};
        {
            program.addShader(&shader);

            bool result = program.link(EShMsgDefault);
            LD_DEBUG_ASSERT(result);
        }

        // NOTE: glslang API uses std::vector<unsigned int> for spirv, but unsigned int may not be 32-bit words on all platforms
        std::vector<unsigned int> spirv_ui;
        glslang::TIntermediate* ir = program.getIntermediate(shaderType);
        glslang::SpvOptions options{};
        glslang::GlslangToSpv(*ir, spirv_ui, &options);

        spirv.Resize(spirv_ui.size());
        for (size_t i = 0; i < spirv_ui.size(); i++)
        {
            spirv[i] = (u32)spirv_ui[i];
        }
    }

    void RShaderCompiler::PatchOpenGL(const RPipelineLayout& layout, RShaderType type, std::string& glsl)
    {
        // converts RenderBase canonical GLSL into OpenGL compatible GLSL
        // we first compile Vulkan GLSL to SPIRV, then patch and reflect SPIRV back to OpenGL GLSL

        size_t groupCount = layout.GroupLayouts.Size();
        u32 textureUnitCtr = 0;
        u32 uboBaseCtr = 0;
        std::map<Qualifier, u32> textureUnitRemap;
        std::map<Qualifier, u32> uboBaseRemap;
        
        for (size_t groupIdx = 0; groupIdx < groupCount; groupIdx++)
        {
            LD_DEBUG_ASSERT((bool)layout.GroupLayouts[groupIdx]);

            RBindingGroupLayoutBase& groupLayout = Unwrap(layout.GroupLayouts[groupIdx]);
            size_t bindingCount = groupLayout.GetBindingCount();
            
            for (size_t bindingIdx = 0; bindingIdx < bindingCount; bindingIdx++)
            {
                const RBindingInfo& bindingInfo = groupLayout.Bindings[bindingIdx];

                switch (bindingInfo.Type)
                {
                case RBindingType::Texture:
                    textureUnitRemap[{(u32)groupIdx, (u32)bindingIdx}] = textureUnitCtr++;
                    break;
                case RBindingType::UniformBuffer:
                    uboBaseRemap[{(u32)groupIdx, (u32)bindingIdx}] = uboBaseCtr++;
                    break;
                }
            }
        }

        // even though ludens source GLSL is using the Vulkan dialect, we will still reconstruct
        // OpenGL GLSL later, so we respect the LD_OPENGL directives in the source GLSL
        std::string preamble = "#define LD_OPENGL\n";

        Vector<u32> spirv;
        GlslangCompile(RBackend::Vulkan, type, glsl, preamble, spirv);

        // TODO: validate shader reflection result with the pipeline layout, find any conflicts
        try
        {
            spirv_cross::CompilerGLSL compiler(spirv.Data(), spirv.Size());
            spirv_cross::ShaderResources resources = compiler.get_shader_resources();

            for (auto& uniform_buffer : resources.uniform_buffers)
            {
                u32 id = uniform_buffer.id;
                u32 group = compiler.get_decoration(id, spv::DecorationDescriptorSet);
                u32 binding = compiler.get_decoration(id, spv::DecorationBinding);
                compiler.unset_decoration(id, spv::DecorationDescriptorSet);
                compiler.unset_decoration(id, spv::DecorationBinding);

                u32 uboBase = uboBaseRemap[{group, binding}];
                compiler.set_decoration(id, spv::DecorationBinding, uboBase);
            }

            for (auto& sampled_image : resources.sampled_images)
            {
                u32 id = sampled_image.id;
                u32 group = compiler.get_decoration(id, spv::DecorationDescriptorSet);
                u32 binding = compiler.get_decoration(id, spv::DecorationBinding);
                compiler.unset_decoration(id, spv::DecorationDescriptorSet);
                compiler.unset_decoration(id, spv::DecorationBinding);

                LD_DEBUG_ASSERT(textureUnitRemap.find({ group, binding }) != textureUnitRemap.end());
                u32 textureUnit = textureUnitRemap[{ group, binding }];
                compiler.set_decoration(id, spv::DecorationBinding, textureUnit);
            }

            // output OpenGL compatible GLSL
            glsl = compiler.compile();
        }
        catch (spirv_cross::CompilerError error)
        {
            std::cout << error.what() << std::endl;
        };
    }

    RShaderCache::~RShaderCache()
    {
        LD_DEBUG_ASSERT(!mHasStartup && "RShaders not destroyed yet");
    }

    void RShaderCache::Startup(const RShaderCacheInfo& info)
    {
        LD_DEBUG_ASSERT((bool)info.Device && "invalid device");

        mDevice = info.Device;
        mCacheDirectory = info.CacheDirectory;
        mHasStartup = true;
    }

    void RShaderCache::Cleanup()
    {
        mHasStartup = false;

        for (auto& kv : mCacheInfo)
        {
            RResult result = mDevice.DeleteShader(kv.second.Shader);
            LD_DEBUG_ASSERT(result.Type == RResultType::Ok);
        }
    }

    RResult RShaderCache::GetOrCreateShader(const RPipelineLayout& layout, RShaderType type, const std::string& glsl, std::string name, RShader& shader)
    {
        name = (mDevice.GetBackend() == RBackend::Vulkan ? "vk_" : "gl_") + name;
        size_t sourceHash = std::hash<std::string>{}(glsl);
        std::string sourceHashStr = std::to_string(sourceHash);
        std::string basePath { mCacheDirectory.ToString() + "/" + name };
        Path spirvPath { basePath + ".spv"};
        Path hashPath  { basePath + ".txt" };
        Vector<u8> spirv;
        RResult result;
        File file;

        if (!File::Exists(mCacheDirectory))
        {
            FileSystem fs;
            fs.CreateDirectories(mCacheDirectory);
        }

        // load from memory
        if (mCacheInfo.find(name) != mCacheInfo.end())
        {
            CacheInfo& ci = mCacheInfo[name];

            if (ci.ShaderType == type && ci.SourceHash == sourceHash)
                shader = ci.Shader;

            return {};
        }

        bool spirvFromDisk = false;

        if (File::Exists(spirvPath) && File::Exists(hashPath))
        {
            std::string diskHashStr;

            file.Open(hashPath, FileMode::Read);
            diskHashStr.resize(file.Size());
            memcpy(diskHashStr.data(), file.Data(), file.Size());
            file.Close();

            if (diskHashStr == sourceHashStr)
            {
                file.Open(spirvPath, FileMode::Read);
                spirv.Resize(file.Size());
                memcpy(spirv.Data(), file.Data(), file.Size());
                file.Close();
         
                spirvFromDisk = true;
            }
        }

        if (!spirvFromDisk) // compile glsl for target backend and save spirv to disk
        {
            RShaderCompiler compiler(mDevice.GetBackend());
            compiler.Compile(layout, type, glsl, spirv);

            File file;
            file.Open(spirvPath, FileMode::Write);
            file.Write(spirv.Data(), spirv.Size());
            file.Close();
            file.Open(hashPath, FileMode::Write);
            file.Write((const u8*)sourceHashStr.data(), sourceHashStr.size());
            file.Close();
        }

        RShaderInfo info{};
        info.SourceType = RShaderSourceType::SPIRV;
        info.Type = type;
        info.Data = spirv.Data();
        info.Size = spirv.Size();
        result = mDevice.CreateShader(shader, info);

        if (result.Type == RResultType::Ok)
        {
            mCacheInfo[name].ShaderType = type;
            mCacheInfo[name].Shader = shader;
            mCacheInfo[name].SourceHash = sourceHash;
        }

        return result;
    }

} // namespace LD