//
// Created by hugo on 8/14/24.
//

#ifndef GAME_HPP
#define GAME_HPP

#include <SDL.h>

#include <vulkan/vulkan.h>

#include <chrono>
#include <memory>
#include <unordered_map>

#include "vk_types.hpp"

class IScene;

class Game {
public:
    Game();

    ~Game();

    void run();

    [[nodiscard]] bool is_running() const;

    uint32_t add_scene(const std::shared_ptr<IScene> &scene);

    void remove_scene(uint32_t id);

    void load_scene(uint32_t id);

    void quit();

    int get_width() const;

    int get_height() const;

    std::pair<int, int> get_window_size() const;

private:
    SDL_Window *window_;
    SDL_Renderer *renderer_;

    int frame_number_{0};

    VkInstance vk_instance_;
    VkPhysicalDevice vk_physical_device_;
    VkDevice vk_device_;
    VkSurfaceKHR vk_surface_;
    VkDebugUtilsMessengerEXT vk_debug_messenger_;

    VkSwapchainKHR vk_swapchain_;
    VkFormat vk_swapchain_image_format_;

    std::vector<VkImage> vk_swapchain_images_;
    std::vector<VkImageView> vk_swapchain_image_views_;
    VkExtent2D vk_swapchain_extent_;

    FrameData _frames[FRAME_OVERLAP];

    FrameData &get_current_frame() { return _frames[frame_number_ % FRAME_OVERLAP]; };

    VkQueue _graphicsQueue;
    uint32_t _graphicsQueueFamily;

    std::unordered_map<uint32_t, std::shared_ptr<IScene> > scenes;
    std::shared_ptr<IScene> current_scene_ = nullptr;
    uint32_t inserted_scene_id_ = 0;

    std::chrono::time_point<std::chrono::steady_clock> last_time_;

    bool m_running = false;

    void create_swapchain(uint32_t width, uint32_t height);

    void init_vulkan();

    void init_swapchain();

    void init_commands();

    void init_sync_structures() {
    }

    void cleanup_vulkan();

    void cleanup_swapchain();

    void poll_events();

    void handle_event(const SDL_Event &e);

    void key_pressed(const SDL_KeyboardEvent &e) const;

    void update();

    void render();
};


#endif //GAME_HPP
