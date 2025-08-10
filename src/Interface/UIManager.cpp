//
// Created by 二九 on 2025/8/3.
//
#include "UIManager.h"

UIManager::UIManager() {
    delete menu;
    delete content;
}

void UIManager::init(SDL_Renderer *renderer) {
    this->renderer = renderer;
    menu = new MenuPanel(renderer, 0, 0, 200, 720);
    content = new ContentPanel(renderer, 200, 0, 600, 720);

    //读取菜单配置文件
    std::ifstream file("resource/config/menu_config.json");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open menu file");
    }

    json config;
    file >> config;

    for (auto &itemData: config) {
        MenuItem item;
        item.title = itemData["title"].get<std::string>();
        item.expanded = itemData.value("expanded", false);

        auto colorArr = itemData["color"];
        SDL_Color color = {
            static_cast<Uint8>(colorArr[0]),
            static_cast<Uint8>(colorArr[1]),
            static_cast<Uint8>(colorArr[2]),
            255
        };

        item.onClick = [this,color]() {
            content->setColor(color);
        };

        menu->addMenuItem(item);
    }
}

void UIManager::handleEvent(const SDL_Event &event) {
    menu->handleEvent(event);
    content->handleEvent(event);
}

void UIManager::render() {
    menu->render();
    content->render();
}

void UIManager::update(double delta) {
    menu->update(delta);
    content->update();
}
