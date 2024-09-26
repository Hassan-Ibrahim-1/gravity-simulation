#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "error_handler.hpp"
#include "input_handler.hpp"
#include "renderer.hpp"
#include "sim.hpp"
#include "utils.hpp"
#include "window.hpp"
#include "globals.hpp"
#include "settings.hpp"
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
    glfwSetMouseButtonCallback(window.data(), InputHandler::mouse_button_callback);

    // Enable vsync
    glfwSwapInterval(1);

    // imgui stuff
    const char* glsl_version = "#version 410";
    
    // imgui context
    ImGuiIO& io = Utils::create_imgui_context();
    InputHandler::io = &io;

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

    Sim sim;
    sim.init();

    glm::vec3 pos1(0);
    glm::vec3 pos2(0.1f);
    glm::vec3 color(1);

    while (!glfwWindowShouldClose(window.data())) {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        Globals::delta_time = delta_time;

        // NOTE: Have to call this for mouse updates
        InputHandler::process_input(window.data());

        if (glfwGetWindowAttrib(window.data(), GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        if (Settings::wireframe_enabled) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // start imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Line test");
        ImGui::DragFloat2("pos1", (float*)&pos1, 0.01f, -1.0f, 1.0f);
        ImGui::DragFloat2("pos2", (float*)&pos2, 0.01f, -1.0f, 1.0f);
        ImGui::ColorEdit3("color", (float*)&color);
        ImGui::End();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glm::mat4 view = camera.get_view_matrix();
        renderer.set_view_matrix(view);

        sim.run();

        /*Line line((Point(pos1, glm::vec4(color, 1))), Point(pos2, glm::vec4(color, 1)));*/
        /*renderer.draw_line(line);*/
        /**/
        /*Line line2(Point(glm::vec3(-0.2), glm::vec4(1)), Point(glm::vec3(-0.1, -0.1, 0), glm::vec4(1, 0, 0, 1)));*/
        /*renderer.draw_line(line2);*/

        renderer.render();

        // render imgui windows
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window.data());
        glfwPollEvents();
    }
    cleanup();

    return 0;
}

static void cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

