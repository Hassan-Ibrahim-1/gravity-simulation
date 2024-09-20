#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "common.hpp"
#include "point.hpp"
#include "shader.hpp"

enum class DrawMode {
    FILL,
    LINE,
};

class Renderer {
public:
    struct Shaders {
        Shaders() = default;
        Shader point;
    } shaders;

    Renderer();
    ~Renderer();

    void draw_point(Point& point);
    void draw_point(glm::vec3 position, glm::vec4 color);

    void draw_rect(Rect& rect, DrawMode draw_mode = DrawMode::FILL);
    void draw_rect(float width, float height, glm::vec3 position, glm::vec4 color);

    void draw_circle(Circle& circle);
    void draw_circle(glm::vec3 position, float radius, glm::vec4 color);
    void init_vaos();

    void reload_shaders();

    // Actually render all draw calls
    void render();

private:
    std::vector<Point> _points;

    std::vector<Rect> _rects;
    std::vector<DrawMode> _rect_draw_modes;

    std::vector<Circle> _circles;
    /*std::vector<DrawMode> _circle_draw_modes;*/

    uint _points_vao;
    uint _points_vbo;

    uint _rects_vao;
    uint _rects_vbo;
    uint _rects_ebo;

    uint _circles_vao;
    uint _circles_vbo;

    static constexpr std::array<uint, 8> _rect_outline_indices = {
        0, 1,
        1, 3,
        0, 2,
        2, 3,
    };

    static constexpr std::array<float, 12> _rect_vertices = {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, // bottom left
       -0.5f, 0.5f, 0.0f // top left
    };

    static constexpr std::array<float, 6> _rect_indices = {
        0, 1, 3,
        1, 2, 3 
    };

    static constexpr float _point_vertex = 0.0f;

    glm::vec2 _circle_start = glm::vec2(0.0);
    std::vector<float> _circle_vertices;
    int _n_circle_segments = 150;

    void generate_circle_vertices();

    void push_point(float x, float y, float z);
    void push_point_color(glm::vec4& color);

    void init_vbos();
    // void update_vaos();
    // initializes shaders needed by the renderer
    void init_shaders();

    // assumes radius is 0.5 and that 0, 0, 0 is the starting position
};

