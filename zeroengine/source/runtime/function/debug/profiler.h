#pragma once

#include <imgui.h>

#include "runtime/core/util/singleton.h"

namespace Zero {
    struct ProfileTaskResult {
        std::string name;
        long long   start{0};
        long long   end{0};
        long long   getLength() { return end - start; }
    };

    class StepTimer;

    class Profiler : public Singleton<Profiler> {
        friend class StepTimer;

    public:
        void render() {
            ImGui::Begin("Profiler");
            for (auto& ret : m_profile_tasks) {
                ImGui::Text("name: %s, cost time: %.3f ms", ret.name.c_str(), double(ret.getLength() / 1000.0));
            }
            ImGui::End();

            m_profile_tasks.clear();
        }

    private:
        std::vector<ProfileTaskResult> m_profile_tasks;
    };

    class StepTimer {
    public:
        StepTimer(const char* name) :
            m_name(name), m_stopped(false) {
            m_startTime_point = std::chrono::high_resolution_clock::now();
        }

        ~StepTimer() {
            if (!m_stopped)
                stop();
        }

        void stop() {
            auto endTime_point = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTime_point).time_since_epoch().count();
            long long end   = std::chrono::time_point_cast<std::chrono::microseconds>(endTime_point).time_since_epoch().count();

            Profiler::getInstance().m_profile_tasks.push_back({m_name, start, end});

            m_stopped = true;
        }

    private:
        const char*                                                 m_name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime_point;
        bool                                                        m_stopped;
    };
} // namespace Zero

#define ZE_PROFILE 0
#if ZE_PROFILE
#define ZE_PROFILE_SCOPE(name) ::Zero::StepTimer timer##__LINE__(name)
#define ZE_PROFILE_FUNCTION() ZE_PROFILE_SCOPE(__FUNCSIG__)
#define ZE_PROFILE_RENDER() ::Zero::Profiler::getInstance().render()
#else
#define ZE_PROFILE_SCOPE(name)
#define ZE_PROFILE_FUNCTION()
#define ZE_PROFILE_RENDER()
#endif
