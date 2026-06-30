#include "Renderer.hpp"
#include <cmath>
#include <iostream>

Renderer::Renderer() {}

Renderer::~Renderer() {
    UnloadModel(tile);
    UnloadModel(cloud);
    UnloadModel(cyborg);
    UnloadModel(diamond);
    UnloadModel(food);
    UnloadModel(egg);

    UnloadFont(milkywayFont);
    UnloadFont(mochibopFont);

    if (anims) UnloadModelAnimations(anims, animCount);
}

void Renderer::init() {
    InitWindow(1280, 720, "Zappy GUI");
    SetTargetFPS(60);

    initCamera();
    initModels();
    initFonts();
    initClouds();
}

void Renderer::initCamera() {
    camera.position = { 50.0f, 50.0f, 50.0f };
    camera.target   = { 0.0f, 12.0f, 0.0f };
    camera.up       = { 0.0f, 1.0f, 0.0f };
    camera.fovy     = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

void Renderer::initModels() {
    tile    = LoadModel("assets/grass/grass_tile.gltf");
    cloud   = LoadModel("assets/cloud/cloud.gltf");
    cyborg  = LoadModel("assets/cyborg/cyborg.gltf");
    diamond = LoadModel("assets/diamond/diamond.gltf");
    food    = LoadModel("assets/food/food.gltf");
    egg     = LoadModel("assets/egg/egg.gltf");

    anims = LoadModelAnimations("assets/cyborg/cyborg.gltf", &animCount);
    idleAnim = 3;
}

void Renderer::initFonts() {
    milkywayFont = LoadFontEx("assets/milkyway/Milkyway.ttf", 256, NULL, 0);
    SetTextureFilter(milkywayFont.texture, TEXTURE_FILTER_ANISOTROPIC_8X);

    mochibopFont = LoadFontEx("assets/mochibop/Mochibop-Demo.ttf", 256, NULL, 0);
    SetTextureFilter(mochibopFont.texture, TEXTURE_FILTER_ANISOTROPIC_8X);
}

void Renderer::initClouds() {
    cloudCount = 100;

    for (int i = 0; i < cloudCount; i++) {
        cloudPos[i].x = GetRandomValue(-200, 200);
        cloudPos[i].y = 15.0f + GetRandomValue(0, 5);
        cloudPos[i].z = GetRandomValue(0, 200);

        cloudSpeed[i] = 0.02f + (float)GetRandomValue(0, 5) * 0.005f;
    }
}


Color Renderer::getTeamColor(const std::string &team) {
    if (teamColors.count(team)) return teamColors[team];

    Color c = {
        (unsigned char)GetRandomValue(0, 255),
        (unsigned char)GetRandomValue(0, 255),
        (unsigned char)GetRandomValue(0, 255),
        255
    };

    teamColors[team] = c;
    return c;
}

void Renderer::update(const World &world) {
    UpdateCamera(&camera, CAMERA_FREE);

    animTime += GetFrameTime() * 20.0f;
    UpdateModelAnimation(cyborg, anims[idleAnim], animTime);

    for (int i = 0; i < cloudCount; i++) {
        cloudPos[i].x += cloudSpeed[i];
        if (cloudPos[i].x > 200)
            cloudPos[i].x = -200;
    }
}

void Renderer::draw(const World &world) {
    BeginDrawing();
    ClearBackground({52, 164, 235, 255});

    BeginMode3D(camera);

        drawGrass(world);
        drawClouds();
        drawIncantationGlow(world);
        drawPlayers(world);
        drawEggs(world);
        drawTileContents(world);

    EndMode3D();

    drawTitle();
    drawSubtitle();
    DrawFPS(10, 10);

    EndDrawing();
}

void Renderer::drawGrass(const World &world) {
    float spacing = 5.0f;

    for (int y = 0; y < world.height; y++) {
        for (int x = 0; x < world.width; x++) {
            Vector3 pos = {
                x * (1.0f + spacing),
                0.0f,
                y * (1.0f + spacing)
            };
            DrawModel(tile, pos, 1.0f, WHITE);
        }
    }
}

void Renderer::drawClouds() {
    for (int i = 0; i < cloudCount; i++)
        DrawModel(cloud, cloudPos[i], 0.15f, WHITE);
}

void Renderer::drawPlayers(const World &world) {
    float spacing = 5.0f;

    for (auto &kv : world.players) {
        const Player &p = kv.second;

        Vector3 pos = {
            p.x * (1.0f + spacing),
            0.0f,
            p.y * (1.0f + spacing)
        };

        Color teamColor = getTeamColor(p.team);

        float angle = 0.0f;
        switch (p.orientation) {
            case 1: angle = 0.0f;   break;   // N
            case 2: angle = 90.0f;  break;   // E
            case 3: angle = 180.0f; break;   // S
            case 4: angle = 270.0f; break;   // W
        }

        DrawModelEx(
            cyborg,
            pos,
            (Vector3){0.0f, 1.0f, 0.0f},
            angle,
            (Vector3){0.5f, 0.5f, 0.5f},
            teamColor
        );
    }
}


void Renderer::drawEggs(const World &world) {
    float spacing = 5.0f;

    for (auto &kv : world.eggs) {
        const Egg &e = kv.second;

        Vector3 pos = {
            e.x * (1.0f + spacing),
            1.8f,
            e.y * (1.0f + spacing)
        };

        Color c = WHITE;

        // find owner team color
        for (auto &pkv : world.players) {
            if (pkv.second.id == e.owner) {
                c = getTeamColor(pkv.second.team);
                break;
            }
        }

        DrawModel(egg, pos, 1.0f, c);
    }
}

void Renderer::drawTileContents(const World &world) {
    float spacing = 5.0f;

    for (int y = 0; y < world.height; y++) {
        for (int x = 0; x < world.width; x++) {
            const Tile &t = world.map[y][x];

            Vector3 base = {
                x * (1.0f + spacing),
                2.0f,
                y * (1.0f + spacing)
            };

            // gem offsets inside tile
            Vector3 offsets[7] = {
                {0.0f, 0.0f, 0.0f},
                {0.5f, 0.0f, 0.3f},
                {-0.4f, 0.0f, -0.2f},
                {0.2f, 0.0f, -0.5f},
                {-0.6f, 0.0f, 0.4f},
                {0.3f, 0.0f, 0.6f},
                {0.0f, 0.0f, -0.7f}
            };

            Color gemColors[7] = {
                RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, WHITE
            };

            for (int i = 0; i < 7; i++) {
                for (int count = 0; count < t.q[i]; count++) {
                    Vector3 pos = {
                        base.x + offsets[i].x,
                        base.y,
                        base.z + offsets[i].z
                    };
                    DrawModel(diamond, pos, 0.008f, gemColors[i]);
                }
            }

            // food
            if (t.q[0] > 0) {
                Vector3 pos = {
                    base.x + 0.2f,
                    base.y,
                    base.z - 0.3f
                };
                DrawModel(food, pos, 0.008f, WHITE);
            }
        }
    }
}

void Renderer::drawIncantationGlow(const World &world) {
    float spacing = 5.0f;

    for (const Incant &ic : world.incantations) {
        Vector3 pos = {
            ic.x * (1.0f + spacing),
            0.1f,
            ic.y * (1.0f + spacing)
        };

        DrawCube(pos, 6.0f, 0.2f, 6.0f, {255, 255, 255, 120});
    }
}

void Renderer::drawTitle() {
    DrawTextEx(milkywayFont, "Zappy", {10, 40}, 64, 0, {255, 160, 0, 255});
}

void Renderer::drawSubtitle() {
    DrawTextEx(mochibopFont, "GUI Renderer", {10, 110}, 32, 0, {255, 240, 120, 255});
}
