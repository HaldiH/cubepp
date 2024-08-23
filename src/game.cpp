#include "game.hpp"
#include "scene.hpp"

#include <iostream>
#include <ostream>

#include <SDL_vulkan.h>
#include "VkBootstrap.h"

constexpr bool use_validation_layers = false;

Game::Game() {
    SDL_Init(SDL_INIT_VIDEO);

    window_ = SDL_CreateWindow(
        "cubepp",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN
    );
    if (!window_) {
        std::cerr << "Failed to create window" << std::endl;
    }

    init_vulkan();

    init_swapchain();

    init_commands();

    init_sync_structures();
}

Game::~Game() {
    cleanup_vulkan();
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Game::create_swapchain(uint32_t width, uint32_t height) {
    vkb::SwapchainBuilder swapchain_builder{vk_physical_device_, vk_device_, vk_surface_};

    vk_swapchain_image_format_ = VK_FORMAT_B8G8R8A8_UNORM;

    vkb::Swapchain vkb_swapchain = swapchain_builder
            //.use_default_format_selection()
            .set_desired_format(VkSurfaceFormatKHR{
                .format = vk_swapchain_image_format_, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
            })
            //use vsync present mode
            .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
            .set_desired_extent(width, height)
            .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            .build()
            .value();

    vk_swapchain_extent_ = vkb_swapchain.extent;
    //store swapchain and its related images
    vk_swapchain_ = vkb_swapchain.swapchain;
    vk_swapchain_images_ = vkb_swapchain.get_images().value();
    vk_swapchain_image_views_ = vkb_swapchain.get_image_views().value();
}

void Game::init_vulkan() {
    vkb::InstanceBuilder builder;

    auto inst_ret = builder.set_app_name("cubepp")
            .request_validation_layers(use_validation_layers)
            .use_default_debug_messenger()
            .require_api_version(1, 3, 0)
            .build();

    const auto vkb_instance = inst_ret.value();

    vk_instance_ = vkb_instance.instance;
    vk_debug_messenger_ = vkb_instance.debug_messenger;

    SDL_Vulkan_CreateSurface(window_, vk_instance_, &vk_surface_);

    VkPhysicalDeviceVulkan13Features features{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
    features.dynamicRendering = true;
    features.synchronization2 = true;

    //vulkan 1.2 features
    VkPhysicalDeviceVulkan12Features features12{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES};
    features12.bufferDeviceAddress = true;
    features12.descriptorIndexing = true;

    vkb::PhysicalDeviceSelector selector{vkb_instance};
    vkb::PhysicalDevice physical_device = selector
            .set_minimum_version(1, 3)
            .set_required_features_13(features)
            .set_required_features_12(features12)
            .set_surface(vk_surface_)
            .select()
            .value();

    vkb::DeviceBuilder device_builder{physical_device};
    vkb::Device vkb_device = device_builder.build().value();

    vk_device_ = vkb_device.device;
    vk_physical_device_ = physical_device.physical_device;

    // use vkbootstrap to get a Graphics queue
    _graphicsQueue = vkb_device.get_queue(vkb::QueueType::graphics).value();
    _graphicsQueueFamily = vkb_device.get_queue_index(vkb::QueueType::graphics).value();
}

void Game::init_swapchain() {
    const auto [width, height] = get_window_size();
    create_swapchain(width, height);
}

void Game::init_commands() {
    //create a command pool for commands submitted to the graphics queue.
    //we also want the pool to allow for resetting of individual command buffers
    VkCommandPoolCreateInfo commandPoolInfo = {};
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.pNext = nullptr;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolInfo.queueFamilyIndex = _graphicsQueueFamily;

    for (int i = 0; i < FRAME_OVERLAP; i++) {
        VK_CHECK(vkCreateCommandPool(vk_device_, &commandPoolInfo, nullptr, &_frames[i]._commandPool));

        // allocate the default command buffer that we will use for rendering
        VkCommandBufferAllocateInfo cmdAllocInfo = {};
        cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmdAllocInfo.pNext = nullptr;
        cmdAllocInfo.commandPool = _frames[i]._commandPool;
        cmdAllocInfo.commandBufferCount = 1;
        cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        VK_CHECK(vkAllocateCommandBuffers(vk_device_, &cmdAllocInfo, &_frames[i]._mainCommandBuffer));
    }
}

void Game::cleanup_vulkan() {
    cleanup_swapchain();

    vkDestroySurfaceKHR(vk_instance_, vk_surface_, nullptr);
    vkDestroyDevice(vk_device_, nullptr);

    vkb::destroy_debug_utils_messenger(vk_instance_, vk_debug_messenger_);
    vkDestroyInstance(vk_instance_, nullptr);
}

void Game::cleanup_swapchain() {
    vkDestroySwapchainKHR(vk_device_, vk_swapchain_, nullptr);
    for (const auto &image_view: vk_swapchain_image_views_) {
        vkDestroyImageView(vk_device_, image_view, nullptr);
    }
}

void Game::run() {
    m_running = true;
    last_time_ = std::chrono::steady_clock::now();
    while (true) {
        poll_events();
        if (!m_running)
            break;
        update();
        render();
    }

    quit();
}

bool Game::is_running() const {
    return m_running;
}

uint32_t Game::add_scene(const std::shared_ptr<IScene> &scene) {
    scene->game_ = this;
    const auto [id, scene_] = *scenes.insert(std::make_pair(inserted_scene_id_++, scene)).first;
    return id;
}

void Game::remove_scene(uint32_t id) {
    if (const auto it = scenes.find(id); it != scenes.end()) {
        const auto [_, scene] = *it;
        if (current_scene_ == scene) {
            current_scene_->on_scene_exit();
            current_scene_ = nullptr;
        }
        scene->on_unload();
        scenes.erase(it);
    }
}

void Game::load_scene(uint32_t id) {
    if (const auto it = scenes.find(id); it != scenes.end()) {
        if (current_scene_) {
            current_scene_->on_scene_exit();
            current_scene_->on_unload();
        }
        current_scene_ = it->second;
        current_scene_->on_load();
        current_scene_->on_scene_enter();
    }
}

void Game::poll_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        handle_event(event);
    }
}

void Game::handle_event(const SDL_Event &e) {
    switch (e.type) {
        case SDL_QUIT:
            m_running = false;
            break;
        case SDL_KEYDOWN:
            key_pressed(e.key);
            break;
    }
    if (current_scene_) {
        current_scene_->on_event(e);
    }
}

void Game::key_pressed(const SDL_KeyboardEvent &e) const {
    switch (e.keysym.sym) {
        case SDLK_F11:
            if (SDL_GetWindowFlags(window_) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
                SDL_SetWindowFullscreen(window_, 0);
                break;
            }
            SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
            break;
    }
}

void Game::update() {
    if (current_scene_) {
        const std::chrono::duration<float> delta = std::chrono::steady_clock::now() - last_time_;
        last_time_ = std::chrono::steady_clock::now();
        current_scene_->on_update(delta.count());
    }
}

void Game::render() {
    if (current_scene_) {
        current_scene_->on_render(renderer_);
    }
}

void Game::quit() {
    std::cout << "Quitting..." << std::endl;
    m_running = false;
    if (current_scene_) {
        current_scene_->on_scene_exit();
        current_scene_->on_unload();
        current_scene_ = nullptr;
    }
}

int Game::get_width() const {
    int width;
    SDL_GetWindowSize(window_, &width, nullptr);
    return width;
}

int Game::get_height() const {
    int height;
    SDL_GetWindowSize(window_, nullptr, &height);
    return height;
}

std::pair<int, int> Game::get_window_size() const {
    int width, height;
    SDL_GetWindowSize(window_, &width, &height);
    return std::make_pair(width, height);
}
