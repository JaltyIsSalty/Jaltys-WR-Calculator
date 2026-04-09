#include "mainGUI.h"
#include "../WR/WRcalculator.h"

#include "Dependencies/imgui.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Dependencies/stb_image.h"
#include "Dependencies/imgui_impl_glfw.h"
#include "Dependencies/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
#include <Windows.h>


// ----------------------
// Settings namespace
// ----------------------
namespace Settings {
    std::string windowName = "Jalty's WR Calculator (1.0.0)";
}

enum class Page
{
    main,
    credits
};
// ----------------------
// UI namespace
// ----------------------
namespace UI
{
    // Defaulting to typical WR Robot values
    double Request_HP             = 0;
    double Request_greyResistance = Info::defaults::greyResistance;
    double Request_reflector      = Info::defaults::reflector;
    double Request_defensePoints  = Info::defaults::defensePoints;

    // Calculated results
    double EffectiveHP  = 0.0;
    double RealisticHP  = 0.0;
    
    //Page whatnots
    Page currentPage = Page::main;

    void ResetToDefaults()
    {
        Request_HP             = 0;
        Request_greyResistance = Info::defaults::greyResistance;
        Request_reflector      = Info::defaults::reflector;
        Request_defensePoints  = Info::defaults::defensePoints;

        EffectiveHP = 0.0;
        RealisticHP = 0.0;
    }

    void OnPressedCalculate()
    {
        robot userRobot(Request_HP, Request_greyResistance, Request_reflector, Request_defensePoints);
        EffectiveHP = userRobot.CalculateEffectiveHP();
        RealisticHP = userRobot.CalculateRealisticHP();
    }

    void OpenLink(const char* link)
    {
        ShellExecuteA(NULL, "open", link, NULL, NULL, SW_SHOWNORMAL);
    }
}


void SetWindowIcon(GLFWwindow* window)
{
    int width, height, channels;
    unsigned char* pixels = stbi_load("assets/icon.png", &width, &height, &channels, 4);

    if (pixels)
    {
        GLFWimage image;
        image.width = width;
        image.height = height;
        image.pixels = pixels;

        glfwSetWindowIcon(window, 1, &image);

        stbi_image_free(pixels);
    }
}


// Keep window global/static so Render() can access it if needed later
static GLFWwindow* g_Window = nullptr;


//--------------
// Render Logic |
//--------------
void Render()
{
    int display_w, display_h;
    glfwGetFramebufferSize(g_Window, &display_w, &display_h);

    // Fullscreen ImGui window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoBackground;

    ImGui::Begin(Settings::windowName.c_str(), nullptr, window_flags);

    if (UI::currentPage == Page::main)
    {
        // ---------- UI ----------
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::TextWrapped("Note: Effective HP assumes ideal conditions and instant healing of all repairable HP");
        ImGui::TextWrapped("Note: Realistic Effective HP attempts to factor in healing and ability restrictions for more accuracy");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 10));

        // Input fields
        ImGui::InputDouble("HP", &UI::Request_HP, 1.0, 10.0, "%.0f");
        ImGui::InputDouble("Grey Resistance %", &UI::Request_greyResistance, 1.0, 5.0, "%.1f");
        ImGui::InputDouble("Reflector %", &UI::Request_reflector, 1.0, 5.0, "%.1f");
        ImGui::InputDouble("Defense Points", &UI::Request_defensePoints, 1.0, 5.0, "%.1f");

        ImGui::Dummy(ImVec2(0, 10));

        // Buttons
        if (ImGui::Button("Calculate", ImVec2(150, 40)))
        {
            UI::OnPressedCalculate();
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset to Defaults", ImVec2(150, 40)))
        {
            UI::ResetToDefaults();
        }
        ImGui::SameLine();
        if (ImGui::Button("Credits", ImVec2(150, 40)))
        {
            UI::currentPage = Page::credits;
        }

        ImGui::Dummy(ImVec2(0, 20));

        // Display results
        if (UI::EffectiveHP > 0.0)
        {
            ImGui::Text("Robot Effective HP: %.2f", UI::EffectiveHP);
            ImGui::Text("Robot Realistic Effective HP: %.2f", UI::RealisticHP);
        }
    }
    //----------------
    // Credits Page
    //----------------
    else
    {
        // Close button
        if (ImGui::Button("X", ImVec2(25, 25)))
        {
            UI::currentPage = Page::main;
        }

        ImGui::Dummy(ImVec2(0, 10));

        // Title
        ImGui::TextWrapped("Credits");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 10));

        // ------------------------
        // Person 1
        // ------------------------
        ImGui::BeginChild("Person1", ImVec2(0, 120), true);

        ImGui::Text("Jalty");
        ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "Program Developer");

        ImGui::Dummy(ImVec2(0, 5));
        ImGui::TextWrapped("Developer of all code, UI, and calculation logic");

        ImGui::Dummy(ImVec2(0, 10));

        if (ImGui::Button("Visit Discord##p1"))
        {
            UI::OpenLink("https://discord.com/users/1215916981237317663");
        }

        ImGui::EndChild();

        ImGui::Dummy(ImVec2(0, 10));

        // ------------------------
        // Person 2
        // ------------------------
        ImGui::BeginChild("Person2", ImVec2(0, 120), true);

        ImGui::Text("Prince Moffle");
        ImGui::TextColored(ImVec4(0.6f, 0.9f, 0.4f, 1.0f), "Requested By / Brought To You By");

        ImGui::Dummy(ImVec2(0, 5));
        ImGui::TextWrapped("Provided the idea, testing, and direction for the calculator.");

        ImGui::Dummy(ImVec2(0, 10));
        
        if (ImGui::Button("Visit Discord##p2"))
        {
            UI::OpenLink("https://discord.com/users/347939921605951489");
        }

        ImGui::EndChild();
    }

    ImGui::End();
}

// ----------------------
// Main launcher
// ----------------------
void LaunchModMenu()
{
    if (!glfwInit())
        return;

    g_Window = glfwCreateWindow(800, 600, Settings::windowName.c_str(), nullptr, nullptr);
    if (!g_Window)
    {
        glfwTerminate();
        return;
    }

    SetWindowIcon(g_Window);
    glfwMakeContextCurrent(g_Window);
    glfwSwapInterval(1); // VSync

    // ImGui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = "\\assets\\imgui.ini";
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(0, 0);
    style.FramePadding = ImVec2(10, 6);
    style.ItemSpacing = ImVec2(8, 6);

    ImGui_ImplGlfw_InitForOpenGL(g_Window, true);
    ImGui_ImplOpenGL3_Init("#version 130");


    // Main loop
    while (!glfwWindowShouldClose(g_Window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render UI
        Render();

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(g_Window, &display_w, &display_h);

        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(g_Window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(g_Window);
    glfwTerminate();
    g_Window = nullptr;
}