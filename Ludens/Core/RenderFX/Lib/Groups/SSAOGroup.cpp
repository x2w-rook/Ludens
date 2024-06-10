#include <random>
#include "Core/DSA/Include/Array.h"
#include "Core/DSA/Include/Vector.h"
#include "Core/RenderFX/Include/Groups/SSAOGroup.h"

namespace LD
{

static void GenerateSSAOKernelSamples(int numSamples, Vector<Vec4>& samples)
{
    std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
    std::default_random_engine generator;

    samples.Resize(numSamples);

    auto lerp = [](float from, float to, float f) { return from + f * (to - from); };

    for (int i = 0; i < numSamples; ++i)
    {
        Vec4 sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f,
                    randomFloats(generator), 0.0f);
        sample = sample.Normalized();

        float scale = (float)i / numSamples;
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample = sample * scale;
        samples[i] = sample;
    }
}

static void GenerateSSAONoiseTexture(int numVectors, Vector<u32>& pixels)
{
    std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
    std::default_random_engine generator;

    pixels.Resize(numVectors);

    for (int i = 0; i < numVectors; i++)
    {
        Vec2 noise(randomFloats(generator), randomFloats(generator));

        u32 pixel = 0;
        pixel |= static_cast<int>(noise.x * 255.0f);
        pixel |= (static_cast<int>(noise.y * 255.0f) << 8);
        pixels[i] = pixel;
    }
}

SSAOGroup::~SSAOGroup()
{
    LD_DEBUG_ASSERT(!mDevice);
}

void SSAOGroup::Startup(RDevice device, RBindingGroupLayout ssaoBGL)
{
    LD_DEBUG_ASSERT(device);
    mDevice = device;

    RBindingGroupInfo groupI;
    groupI.Layout = ssaoBGL;
    mDevice.CreateBindingGroup(mHandle, groupI);

    Vector<Vec4> samples;
    GenerateSSAOKernelSamples(64, samples);

    RBufferInfo bufferI;
    bufferI.Type = RBufferType::UniformBuffer;
    bufferI.MemoryUsage = RMemoryUsage::FrameDynamic;
    bufferI.Data = samples.Data();
    bufferI.Size = samples.ByteSize();
    mDevice.CreateBuffer(mKernelUBO, bufferI);
    mHandle.BindUniformBuffer(0, mKernelUBO);

    Vector<u32> pixels;
    GenerateSSAONoiseTexture(16, pixels);

    RTextureInfo noiseI;
    noiseI.Type = RTextureType::Texture2D;
    noiseI.Format = RTextureFormat::RGBA8;
    noiseI.Width = 4;
    noiseI.Height = 4;
    noiseI.Data = pixels.Data();
    noiseI.Size = pixels.ByteSize();
    noiseI.Sampler.AddressMode = RSamplerAddressMode::Repeat;
    mDevice.CreateTexture(mNoise, noiseI);
    mHandle.BindTexture(1, mNoise);
}

void SSAOGroup::Cleanup()
{
    mDevice.DeleteTexture(mNoise);
    mDevice.DeleteBuffer(mKernelUBO);
    mDevice.DeleteBindingGroup(mHandle);
    mDevice.ResetHandle();
}

RBindingGroupLayoutData SSAOGroup::GetLayoutData() const
{
    // kernel ubo
    RBindingInfo ubo;
    ubo.Count = 1;
    ubo.Type = RBindingType::UniformBuffer;

    // noise texture
    RBindingInfo texture;
    texture.Count = 1;
    texture.Type = RBindingType::Texture;

    return { ubo, texture };
}

RBindingGroupLayout SSAOGroup::CreateLayout(RDevice device)
{
    LD_DEBUG_ASSERT(device);

    RBindingGroupLayout ssaoBGL;

    Array<RBindingInfo, 2> bindings;
    bindings[0].Type = RBindingType::UniformBuffer;
    bindings[0].Count = 1;
    bindings[1].Type = RBindingType::Texture;
    bindings[1].Count = 1;

    RBindingGroupLayoutInfo ssaoBGLI;
    ssaoBGLI.Bindings = bindings.GetView();
    device.CreateBindingGroupLayout(ssaoBGL, ssaoBGLI);

    return ssaoBGL;
}

} // namespace LD