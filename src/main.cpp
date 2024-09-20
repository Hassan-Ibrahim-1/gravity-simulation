#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iterator>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "error_handler.hpp"
#include "input_handler.hpp"
#include "renderer.hpp"
#include "utils.hpp"
#include "window.hpp"
#include "globals.hpp"
#include "camera.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

static void cleanup();

int main() {
    Utils::init();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    #endif

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Bebe");

    glfwSetInputMode(window.data(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (ErrorHandler::had_error) {
        cleanup();
        return -1;
    }

    glfwSetKeyCallback(window.data(), InputHandler::key_callback);
    glfwSetCursorPosCallback(window.data(), InputHandler::mouse_movement_callback);
    /*glfwSetScrollCallback(window.get_window(), InputHandler::mouse_scroll_callback);*/

    // Enable vsync
    glfwSwapInterval(1);

    // imgui stuff
    const char* glsl_version = "#version 410";
    
    // imgui context
    ImGuiIO& io = Utils::create_imgui_context();
    Globals::io = &io;

    ImGui::StyleColorsDark();

    // setup renderer
    ImGui_ImplGlfw_InitForOpenGL(window.data(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    float delta_time = 0.0f;
    float last_frame = 0.0f;

    glEnable(GL_DEPTH_TEST);

    Renderer renderer;
    Globals::renderer = &renderer;

    Camera& camera = Globals::camera;
    camera = Camera(glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 position(0);
    glm::vec3 scale(1);
    glm::vec4 color(1);

    while (!glfwWindowShouldClose(window.data())) {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        if (glfwGetWindowAttrib(window.data(), GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        /*if (Settings::wireframe_enabled) {*/
        /*    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
        /*}*/
        /*else {*/
        /*    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
        /*}*/

        // start imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("config");
        /*ImGui::SetWindowSize("config", ImVec2(450, 230));*/

        ImGui::ColorEdit3("color", (float*)&color);
        ImGui::DragFloat2("position", (float*)&position, 0.01f, -1.0f, 1.0f);
        ImGui::DragFloat2("scale", (float*)&scale, 0.01f, -2.0f, 2.0f);
        /*ImGui::DragFloat2("circle start", (float*)&renderer._circle_start, 0.1f);*/
        /*ImGui::DragFloat("radius", (float*)&renderer.radius, 0.01f, 0, 10000);*/
        /*ImGui::DragInt("segments", (int*)&renderer._n_circle_segments, 1, 1, 100000);*/
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / Globals::io->Framerate, Globals::io->Framerate);

        ImGui::End();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        /*glClearColor(1, 1, 1, 1);*/
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glm::mat4 view = camera.get_view_matrix();
        renderer.shaders.point.use();
        renderer.shaders.point.set_mat4("view", view);

        /*Point point(position, color);*/
        /*point.transform.scale = scale;*/
        /*renderer.draw_point(point);*/
        Circle circle(Transform(position, scale), 0, color);
        renderer.draw_circle(circle);

        /*if (renderer.prev_segment != renderer._n_circle_segments) {*/
        /*    renderer.prev_segment = renderer._n_circle_segments;*/
        /*    renderer._circle_vertices.clear();*/
        /*    renderer.generate_circle_vertices();*/
        /*    renderer.init_vaos();*/
        /*}*/
        /**/
        /*if (renderer._circle_start != renderer.prev_circle_start) {*/
        /*    renderer.prev_circle_start = renderer._circle_start;*/
        /*    renderer._circle_vertices.clear();*/
        /*    renderer.generate_circle_vertices();*/
        /*    renderer.init_vaos();*/
        /*}*/
        /*if (renderer.radius != renderer.prev_radius) {*/
        /*    renderer.prev_radius = renderer.radius;*/
        /*    renderer._circle_vertices.clear();*/
        /*    renderer.generate_circle_vertices();*/
        /*    renderer.init_vaos();*/
        /*}*/

        renderer.render();

        // render imgui windows
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window.data());
        glfwPollEvents();
    }
    cleanup();
}

static void cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

