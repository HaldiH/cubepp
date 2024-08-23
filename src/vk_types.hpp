//
// Created by hugo on 8/20/24.
//

#pragma once

#define VK_CHECK(x) \
do { \
VkResult err = x; \
if (err) { \
fmt::print("Detected Vulkan error: {}", string_VkResult(err)); \
abort(); \
} \
} while (0)

struct FrameData {

    VkCommandPool _commandPool;
    VkCommandBuffer _mainCommandBuffer;
};

constexpr unsigned int FRAME_OVERLAP = 2;