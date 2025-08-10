//
// Created by 二九 on 2025/8/3.
//

#include"MenuPanel.h"               // 包含对应头文件，提供类声明
#include<SDL_ttf.h>                 // SDL字体库

//初始化面板位置和尺寸, :后面为初始化类的成员
MenuPanel::MenuPanel(SDL_Renderer *renderer, int x, int y, int width, int height): renderer(renderer), x(x), y(y),
    width(width), height(height) {
    font = TTF_OpenFont("resource/ttf/NotoSansSC-Bold.ttf", 20);
    if (!font) {
        SDL_Log("Failed to load font:%s",TTF_GetError());
    }
    textColor = {255, 255, 255};
    needsLayout = true;
}

//添加顶级菜单项
void MenuPanel::addMenuItem(const MenuItem &item) {
    items.push_back(item);
    needsLayout = true;
}

// 处理SDL事件（鼠标点击事件）
void MenuPanel::handleEvent(const SDL_Event &event) {
    //判断是否是鼠标左键点击
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x; // 获取鼠标X坐标
        int mouseY = event.button.y; // 获取鼠标的Y坐标

        if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height) {
            // 保证当前的layout是可用的
            if (needsLayout) rebuildLayout();

            for (auto &entry: layoutEntries) {
                SDL_Rect rect = entry.rect;
                if (mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h) {
                    MenuItem *item = entry.item;
                    if (!item->children.empty()) {
                        item->expanded = !item->expanded; //切换展开
                    } else if (item->onClick) {
                        item->onClick();
                    }
                    needsLayout = true;
                    break;
                }
            }
        }
    }
}

//递归处理点击事件，检测点击的菜单项
bool MenuPanel::handleClick(MenuItem &item, int &offsetY, int depth, int mouseX, int mouseY) {
    int itemY = offsetY; //当前菜单项的垂直位置
    offsetY += itemHeight; // 每处理一个菜单项，下移高度

    // 判断点击是否在当前菜单项的范围内
    if (mouseY >= itemY && mouseY <= itemY + itemHeight) {
        // 如果有子菜单则展开/折叠
        if (!item.children.empty()) {
            item.expanded = !item.expanded;
        }
        // 如果没有子菜单，有回调函数则执行
        else if (item.onClick) {
            item.onClick();
        }
        return true; // 表示点击命中
    }

    //如果该菜单是展开的状态，则递归检查子菜单
    if (item.expanded) {
        for (auto &child: item.children) {
            if (handleClick(child, offsetY, depth + 1, mouseX, mouseY)) {
                return true;
            }
        }
    }
    return false;
}

//渲染整个菜单面板
void MenuPanel::render() {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // 深灰色
    SDL_Rect panelRect = {x, y, width, height}; //菜单矩形区域
    // 填充背景
    SDL_RenderFillRect(renderer, &panelRect);

    if (!font) return;

    //如果需要重新计算布局则重算
    if (needsLayout) rebuildLayout();


    //如果是展开状态，渲染子菜单；
    for (auto &entry: layoutEntries) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &entry.rect);
        SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, entry.item->title.c_str(), textColor);
        if (!textSurface) continue;
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture) {
            SDL_FreeSurface(textSurface);
            continue;
        }

        //目标矩形，每一个菜单的
        SDL_Rect titleRect = {
            entry.rect.x + paddingX,
            entry.rect.y + paddingY,
            entry.rect.w,
            entry.rect.h
        };
        if (titleRect.w > entry.rect.w - paddingX * 2) {
            titleRect.w = entry.rect.w - paddingX * 2;
        }

        SDL_RenderCopy(renderer, textTexture, nullptr, &titleRect);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
    }
}


//  重新计算布局（存储每个菜单项的rect和文本尺寸都存入layoutEntries)
void MenuPanel::rebuildLayout() {
    layoutEntries.clear();

    int offsetY = y;
    for (auto &item: items) {
        layoutItemRecursive(&const_cast<MenuItem &>(item), offsetY, 0);
    }
    needsLayout = false;
}

// 递归为每一项生成渲染的实体， 并在展开时继续处理子项
void MenuPanel::layoutItemRecursive(MenuItem *item, int &offsetY, int depth) {
    if (!font)return;
    int textW = 0, textH = 0;
    // 如果测量字体大小失败则给个默认大小
    if (TTF_SizeUTF8(font, item->title.c_str(), &textW, &textH) != 0) {
        textW = 100;
        textH = 20;
    }

    //  计算背景宽度与高度（ 宽度以文本+padding为基准， 但是不能超过面板宽度）
    int availWidth = std::max(0, width - depth * 40); //缩进后剩余宽度
    int bgW = std::min(availWidth, textW + paddingX * 2);
    int bgH = textH + paddingY * 2;

    SDL_Rect textRect = {x + depth * 20, offsetY, bgW, bgH};

    RenderEntry entry;
    entry.item = item;
    entry.rect = textRect;
    entry.depth = depth;
    entry.textW = textW;
    entry.textH = textH;
    layoutEntries.push_back(entry);
    offsetY += bgH;
    if (item->expanded) {
        for (auto &child: item->children) {
            layoutItemRecursive(&child, offsetY, depth + 1);
        }
    }
}


// 递归渲染菜单项
void MenuPanel::renderItem(const MenuItem &item, int &offsetY, int depth) {
    //字体读取失败则直接返回
    if (!font) return;
    // TTF_RenderText_Blended只适用于ASCII/Latin-1编码的字符
    // SDL_Surface *textSurface = TTF_RenderText_Blended(font, item.title.c_str(), textColor);

    //  加载字体
    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, item.title.c_str(), textColor);
    if (!textSurface) return;

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) return;

    int textW = 0, textH = 0;
    // 也只使用于ASCII/Latin-1编码的字符
    // TTF_SizeText(font, item.title.c_str(), &textW, &textH);
    TTF_SizeUTF8(font, item.title.c_str(), &textW, &textH);

    int paddingY = 10;
    int paddingX = 10;
    // 设置菜单项背景色
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect rect = {x + depth * 20, offsetY, width - depth * 40, itemHeight};
    //x位置随层级缩进（depth*20），表现为树形结构
    SDL_RenderFillRect(renderer, &rect);

    //  绘制文字
    SDL_Rect dstRect = {
        x + depth * 20 + paddingX,
        offsetY + paddingY,
        textW,
        textH
    };
    SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);


    offsetY += itemHeight; //渲染完一个菜单项， Y坐标下移
    // 如果是展开状态，则渲染子菜单
    if (item.expanded) {
        for (auto &child: item.children) {
            renderItem(child, offsetY, depth + 1);
        }
    }
}

//更新菜单（可用于菜单动画）
void MenuPanel::update(double delta) {
}

MenuPanel::~MenuPanel() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
}
