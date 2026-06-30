#pragma once
#include "raylib.h"
#include "world/World.hpp"
#include <unordered_map>
#include <string>

class Renderer {
public:
    Renderer();
    ~Renderer();

    void init();
    void update(const World &world);
    void draw(const World &world);

private:
    // camera
    Camera3D camera;

    // models
    Model tile;
    Model cloud;
    Model cyborg;
    Model diamond;
    Model food;
    Model egg;

    // fonts
    Font milkywayFont;
    Font mochibopFont;

    // animations
    ModelAnimation *anims = nullptr;
    int animCount = 0;
    int idleAnim = 0;
    float animTime = 0.0f;

    // clouds
    static const int MAX_CLOUDS = 200;
    int cloudCount = 0;
    Vector3 cloudPos[MAX_CLOUDS];
    float cloudSpeed[MAX_CLOUDS];

    // team colors
    std::unordered_map<std::string, Color> teamColors;

    // helpers
    void initCamera();
    void initModels();
    void initFonts();
    void initClouds();
    Color getTeamColor(const std::string &team);

    // drawing helpers
    void drawGrass(const World &world);
    void drawClouds();
    void drawPlayers(const World &world);
    void drawEggs(const World &world);
    void drawTileContents(const World &world);
    void drawIncantationGlow(const World &world);
    void drawTitle();
    void drawSubtitle();
};
