//
// Created by 二九 on 2025/8/3.
//

#include "ContentPanel.h"
#include<iostream>

ContentPanel::ContentPanel(SDL_Renderer *renderer, int x, int y, int width, int height): renderer(renderer), x(x), y(y),
    width(width), height(height) {
    bgColor = {100, 100, 100, 255};
}

void ContentPanel::setColor(const SDL_Color &color) {
    bgColor = color;
}

void ContentPanel::render() {
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &rect);
}

void ContentPanel::update() {
}

void ContentPanel::handleEvent(const SDL_Event &event) {
}

ContentPanel::~ContentPanel() {
}
