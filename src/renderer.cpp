#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>

#include "renderer.hpp"
#include "transform.hpp"

Renderer::Renderer() {
    generate_circle_vertices();

    init_vbos();
    init_vaos();
    init_shaders();
}

Renderer::~Renderer() {
    glDeleteBuffers(1, &_points_vbo);
    glDeleteBuffers(1, &_rects_vbo);
    glDeleteVertexArrays(1, &_points_vao);
    glDeleteVertexArrays(1, &_rects_vao);
}

void Renderer::draw_point(Point& point) {
    _points.push_back(point);
}

void Renderer::draw_point(glm::vec3 position, glm::vec4 color) {
    _points.push_back(Point(position, color));
}

void Renderer::reload_shaders() {
    printf("Reloading shaders\n");
    shaders.point.reload();
}

void Renderer::draw_rect(Rect& rect, DrawMode draw_mode) {
    _rects.push_back(rect);
    _rect_draw_modes.push_back(draw_mode);
}

void Renderer::draw_circle(Circle& circle) {
    _circles.push_back(circle);
}

void Renderer::draw_circle(glm::vec3 position, float radius, glm::vec4 color) {
    _circles.push_back(Circle(position, radius, color));
}

void Renderer::render() {
    shaders.point.use();

    glBindVertexArray(_points_vao);

    for (Point& point : _points) {
        glm::mat4 model = point.transform.get_mat4();
        shaders.point.set_mat4("model", model);
        shaders.point.set_vec4("color", point.color);
        glDrawArrays(GL_POINTS, 0, _points.size());
    }

    glBindVertexArray(_rects_vao);
    for (size_t i = 0; i < _rects.size(); i++) {
        Rect& rect = _rects[i];
        DrawMode draw_mode = _rect_draw_modes[i];

        glm::mat4 model(1);
        model = glm::translate(model, rect.transform.position);
        model = glm::scale(model, rect.transform.scale);
        shaders.point.set_mat4("model", model);
        shaders.point.set_vec4("color", rect.color);

        if (draw_mode == DrawMode::FILL) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        else if (draw_mode == DrawMode::LINE) {
            glDrawArrays(GL_LINE_LOOP, 0, 4);
        }
    }

    glBindVertexArray(_circles_vao);
    for (Circle& circle : _circles) {
        glm::mat4 model = circle.transform.get_mat4();
        shaders.point.set_mat4("model", model);
        shaders.point.set_vec4("color", circle.color);
        glDrawArrays(GL_TRIANGLE_FAN, 0, _n_circle_segments + 2);
    }

    _points.clear();
    _rects.clear();
    _rect_draw_modes.clear();
    _circles.clear();
}

void Renderer::init_vbos() {
    glGenBuffers(1, &_points_vbo);
    glGenBuffers(1, &_rects_vbo);
    glGenBuffers(1, &_circles_vbo);
}

void Renderer::init_vaos() {
    // points
    glGenVertexArrays(1, &_points_vao);
    glBindVertexArray(_points_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float), &_point_vertex, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    // rects
    glGenVertexArrays(1, &_rects_vao);
    glBindVertexArray(_rects_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _rects_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _rect_vertices.size(), _rect_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &_rects_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rects_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * _rect_indices.size(), _rect_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    // circles
    glGenVertexArrays(1, &_circles_vao);
    glBindVertexArray(_circles_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _circles_vbo);
    /*printf("circle vertices size: %zu\n", _circle_vertices.size());*/
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _circle_vertices.size(), _circle_vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
}

void Renderer::init_shaders() {
    shaders.point.load("shaders/point_shader.vert", "shaders/point_shader.frag");
}

void Renderer::generate_circle_vertices() {
    static constexpr float two_pi = M_PI * 2;
    static constexpr float radius = 0.5f;

    /*_circle_vertices.push_back(_circle_start.x);*/
    /*_circle_vertices.push_back(_circle_start.y);*/

    for (size_t i = 0; i < _n_circle_segments; i++) {
        // x, y, z
        /*float angle = 2.0f * M_PI * i / _n_circle_segments;*/
        _circle_vertices.push_back(_circle_start.x + radius * cos((float) i / _n_circle_segments * two_pi));
        _circle_vertices.push_back(_circle_start.y + radius * sin((float) i / _n_circle_segments * two_pi));
        /*_circle_vertices.push_back(_circle_start.x + cos(angle) * radius);*/
        /*_circle_vertices.push_back(_circle_start.y + sin(angle) * radius);*/
        _circle_vertices.push_back(0);
    }
}

