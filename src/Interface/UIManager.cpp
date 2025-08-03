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

    MenuItem item1;
    item1.title = "Menu 1";
    item1.children = {};
    item1.expanded = false;
    item1.onClick = [this]() {
        SDL_Color color = {255, 0, 0, 255};
        content->setColor(color);
    };

    MenuItem item2;
    item2.title = "Menu 2";
    item2.children = {};
    item2.expanded = false;
    item2.onClick = [this]() {
        SDL_Color color = {0, 255, 0, 255};
        content->setColor(color);
    };


    menu->addMenuItem(item1);
    menu->addMenuItem(item2);
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
