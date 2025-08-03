//
// Created by 二九 on 2025/8/3.
//

#ifndef CONTENTPANEL_H
#define CONTENTPANEL_H
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<string>
#include<vector>

// 内容面板： 负责右侧区域的绘制（动画帧，按钮，文本等）
class ContentPanel {
private:
    SDL_Renderer *renderer; //渲染器，用于绘制所有内容
    int x, y, width, height; //面板的位置与大小
    int currentFrame = 0; //当前动画帧索引
    std::vector<SDL_Texture *> frames; //动画帧纹理数组
    TTF_Font *font = nullptr;
    SDL_Rect buttonRect; //按钮矩形区域
    std::string buttonText = "Play Animation"; // 按钮文本
    bool isPlaying = false; // 判断动画是否正在播放
    double frameTime = 0.0; //积累时间，用于动画帧切换
    double frameDuration = 0.1; //每帧持续的时间（秒）
    SDL_Color bgColor;

public:
    ContentPanel(SDL_Renderer *renderer, int x, int y, int width, int height);

    void setColor(const SDL_Color &color);
    //加载动画帧
    void loadAnimationFrames(const std::vector<std::string> &framePaths);

    //处理输入事件（比如按钮点击）
    void handleEvent(const SDL_Event &event);

    //更新动画
    void update();

    // 绘制面板
    void render();

    // 析构函数，释放资源
    ~ContentPanel();
};

#endif //CONTENTPANEL_H
