#include <Temporality/Vulkan/VulkanShader.hpp>

#include <Temporality/Log.hpp>
#include <Temporality/Vulkan/VulkanGraphicsDriver.hpp>

#include <fstream>

namespace Temporality::Vulkan {

TEMPORALITY_VULKAN_API
bool VulkanShader::LoadFromFiles(const std::vector<std::string>& filenames)
{
    BenchmarkStart();

    for (const auto& filename : filenames) {
        if (!LoadSPV(filename)) {
            if (!LoadSPV(filename + ".spv")) {
                LogError("Failed to load '%s'", filename);
                return false;
            }
        }
    }

    BenchmarkEnd("VulkanShader::LoadFromFiles");
    return true;
}

TEMPORALITY_VULKAN_API
bool VulkanShader::LoadSPV(const std::string& filename)
{
    VulkanGraphicsDriver * gfx = TEMPORALITY_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    const auto& assetPaths = GetAssetPaths();

    std::ifstream file;
    
    for (const auto& path : assetPaths) {
        const std::string& fullPath = path + "Shaders/" + filename;
        LogVerbose("Checking '%s'", fullPath);

        file.open(fullPath, std::ios::binary);
        if (file.is_open()) {
            break;
        }
    }

    if (!file.is_open()) {
        return false;
    }

    file.unsetf(std::ios::skipws);

    LogLoad("Loading SPIR-V shader '%s'", filename);

    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    VkShaderStageFlagBits type = GetVkShaderType(filename);
    if (type == VK_SHADER_STAGE_ALL) {
        LogError("Failed to determine shader type of '%s'", filename);
        return false;
    }

    VkShaderModuleCreateInfo shaderModuleCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .codeSize = data.size(),
        .pCode = reinterpret_cast<const uint32_t *>(data.data()),
    };

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(gfx->GetDevice(), &shaderModuleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        LogError("Failed to create shader module");
        return false;
    }

    VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = type,
        .module = shaderModule,
        .pName = "main",
        .pSpecializationInfo = nullptr,
    };

    _shaderStages.push_back(shaderStageCreateInfo);

    return true;
}

TEMPORALITY_VULKAN_API
VkShaderStageFlagBits VulkanShader::GetVkShaderType(const std::string& filename)
{
    std::string ext = GetExtension(filename);
    size_t pivot = filename.find_last_of('.');
    if (pivot == std::string::npos) {
        return VK_SHADER_STAGE_ALL; // Invalid
    }
    ext = GetExtension(filename.substr(0, pivot));

    if (ext == "vert" || ext == "vertex") {
        return VK_SHADER_STAGE_VERTEX_BIT;
    }
    else if (ext == "frag" || ext == "fragment") {
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    else if (ext == "tesc" || ext == "tesscontrol") {
        return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    }
    else if (ext == "tese" || ext == "tesseval") {
        return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    }
    else if (ext == "comp" || ext == "compute") {
        return VK_SHADER_STAGE_COMPUTE_BIT;
    }

    return VK_SHADER_STAGE_ALL; // Invalid
}

} // namespace Temporality::Vulkan