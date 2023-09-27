//
// Created by Pascal Thomet on 26/09/2023.
//
#include "imgui_test_engine/imgui_te_engine.h"
#include "hello_imgui/runner_params.h"
#include "hello_imgui/internal/functional_utils.h"

namespace HelloImGui
{
    namespace TestEngineIntegration
    {
        ImGuiTestEngine *GImGuiTestEngine = nullptr;

        void _SetOptions()
        {
            ImGuiTestEngineIO& test_io = ImGuiTestEngine_GetIO(GImGuiTestEngine);
            test_io.ConfigVerboseLevel = ImGuiTestVerboseLevel_Info;
            test_io.ConfigVerboseLevelOnError = ImGuiTestVerboseLevel_Debug;
            test_io.ConfigRunSpeed = ImGuiTestRunSpeed_Cinematic; // Default to slowest mode in this demo

            // test_io.ScreenCaptureFunc = ImGuiApp_ScreenCaptureFunc;
            // test_io.ScreenCaptureUserData = (void*)app;

            // Optional: save test output in junit-compatible XML format.
            //test_io.ExportResultsFile = "./results.xml";
            //test_io.ExportResultsFormat = ImGuiTestEngineExportFormat_JUnitXml;
        }

        void Setup()
        {
            // Setup test engine
            GImGuiTestEngine = ImGuiTestEngine_CreateContext();

            _SetOptions();

            // Start test engine
            ImGuiTestEngine_Start(GImGuiTestEngine, ImGui::GetCurrentContext());
            ImGuiTestEngine_InstallDefaultCrashHandler();
        }

        void TearDown_ImGuiContextAlive()
        {
            ImGuiTestEngine_Stop(GImGuiTestEngine);
        }
        void TearDown_ImGuiContextDestroyed()
        {
            // IMPORTANT: we need to destroy the Dear ImGui context BEFORE the test engine context, so .ini data may be saved.
            ImGuiTestEngine_DestroyContext(GImGuiTestEngine);
        }

        void PostSwap()
        {
            // Call after your rendering. This is mostly to support screen/video capturing features.
            ImGuiTestEngine_PostSwap(GImGuiTestEngine);
        }
    } // namespace TestEngineIntegration


    void _AddTestEngineCallbacks(RunnerParams* runnerParams)
    {
        auto &testEngineCallbacks = runnerParams->callbacks._testEngineCallbacks;

        testEngineCallbacks.OnSetup = TestEngineIntegration::Setup;
        testEngineCallbacks.OnFrame_PostSwap = TestEngineIntegration::PostSwap;
        testEngineCallbacks.OnTearDown_ImGuiContextAlive = TestEngineIntegration::TearDown_ImGuiContextAlive;
        testEngineCallbacks.OnTearDown_ImGuiContextDestroyed = TestEngineIntegration::TearDown_ImGuiContextDestroyed;
    }

    ImGuiTestEngine* GetImGuiTestEngine()
    {
        return TestEngineIntegration::GImGuiTestEngine;
    }
}