#include "engine.h"

// default subsystems
#include "../util/timing.h"

#include <stdexcept>
#include <fstream>

namespace {
    constexpr const char* k_SettingsFilename = "glarses.json";

    bool is_satisfied(
            glarses::System*                s,
            const std::vector<std::string>& already_initialized
    ) {
        using glarses::contains;

        for (const auto& dependency : s->get_dependencies())
            if (!contains(already_initialized, dependency))
                return false;

        return true;
    }
}

namespace glarses {
    Engine::Engine() {
        add<Clock>();
    }

    Engine::~Engine() {
        g_Log << "Engine shut down";
    }

    Engine& Engine::instance() {
        static Engine e;
        return e;
    }

    void Engine::start() {
        m_Running = true;

        // if the global logger only has 1 sink, it logs *only* to 'glarses.log'
        if (log::Logger::instance().getNumSinks() < 2)
            log::Logger::instance().add(log::makeStdOutSink());

        start_libraries();
        start_systems();

        while (m_Running) {
            for (auto& system : m_Systems)
                system->update();

            if (m_Application)
                m_Application->update();
        }

        stop_systems();
        stop_libraries();
    }

    void Engine::stop() {
        m_Running = false;
    }

    void Engine::start_libraries() {
    }

    void Engine::stop_libraries() {
    }

    void Engine::save_settings() {
        using namespace nlohmann;

        json settings;

        // traverse and consolidate settings from all systems and the current application

        if (m_Application) {
            json js;

            for (const auto& entry : m_Application->get_settings()) {
                auto fetch = entry.m_GetFn();

                if (fetch.size() != 1)
                    throw std::runtime_error("Currently only singular variables are supported");

                js.emplace(
                        fetch.begin().key(),
                        *fetch.begin()
                );
            }

            settings[m_Application->get_name()] = js;
        }

        // now the rest of the subsystems
        for (const auto& system : m_Systems) {
            json js;

            for (const auto& entry : system->get_settings()) {
                auto fetch = entry.m_GetFn();

                if (fetch.size() != 1)
                    throw std::runtime_error("Currently only singular variables are supported");

                js.emplace(
                        fetch.begin().key(),
                        *fetch.begin()
                );
            }

            settings[std::string(system->get_name())] = js;
        }

        std::ofstream out(k_SettingsFilename);
        if (!out.good())
            g_Log << "Failed to write to " << k_SettingsFilename;
        else {
            out << settings.dump(2);
            g_Log << "Saved settings";
        }
    }

    void Engine::load_settings() {
        using namespace nlohmann;
        using namespace std;

        ifstream in(k_SettingsFilename);
        if (in.good()) {
            json settings;
            in >> settings;

            for (const auto& ptr : m_Systems) {
                auto it = settings.find(std::string(ptr->get_name()));

                if (it != settings.end()) {
                    for (const auto& entry : ptr->get_settings())
                        entry.m_SetFn(*it);
                }
                else
                    g_Log << "No settings for subsystem " << ptr->get_name();
            }

            if (m_Application) {
                auto it = settings.find(m_Application->get_name());

                if (it != settings.end()) {
                    for (const auto& entry : m_Application->get_settings())
                        entry.m_SetFn(*it);
                }
                else
                    g_Log << "No application settings for " << m_Application->get_name();
            }
        }
        else
            g_Log << "No configuration found, using defaults for all systems";

        g_Log << "Settings loaded";
    }

    void Engine::start_systems() {
        // first try and load subsystem settings
        load_settings();

        // start subsystem initialization; keep track of (satisfied) dependencies and store the order
        {
            size_t current_pass_count      = 0; // number of systems initialized at the beginning of a pass
            size_t num_systems_initialized = 0;
            size_t num_systems             = m_Systems.size();

            auto it = std::begin(m_Systems);

            while (it != std::end(m_Systems)) {
                auto* current_system = it->get();
                const auto& sytem_name = current_system->get_name();

                bool already_initialized = contains(m_InitOrder, sytem_name);

                if (
                        (!already_initialized) &&
                        (is_satisfied(current_system, m_InitOrder))
                        ) {
                    current_system->m_Engine = this;

                    if (!current_system->init())
                        throw std::runtime_error("Failed to start subsystem");

                    m_InitOrder.push_back(sytem_name);

                    ++num_systems_initialized;
                }

                ++it;

                // if we reached the end, see if we're actually done (or that we've stalled)
                if (it == std::end(m_Systems)) {
                    it = std::begin(m_Systems);

                    if (num_systems_initialized == current_pass_count) {
                        // we've stalled, figure out which systems failed to initialize
                        g_Log << "Stalled during system initialization";

                        for (const auto& ptr : m_Systems)
                            if (!contains(m_InitOrder, sytem_name)) {
                                // try to be specific about what's missing
                                std::stringstream sstr;

                                sstr << "\tFailed to initialize: " << sytem_name << ", missing dependencies ";

                                for (const auto& dep : ptr->get_dependencies())
                                    if (!contains(m_InitOrder, dep))
                                        sstr << "'" << dep << "' ";

                                g_Log << sstr.str();
                            }

                        throw std::runtime_error("Stalled during system initialization");
                    }

                    if (num_systems_initialized == num_systems)
                        break; // all systems were initialized

                    current_pass_count = num_systems_initialized;
                }
            }
        }

        // finally, start the application
        if (m_Application) {
            m_Application->m_Engine = this;

            if (is_satisfied(m_Application.get(), m_InitOrder))
                m_Application->init();
            else
                throw std::runtime_error("Application dependencies were not satisfied");
        }
        else
            g_Log << "No application was set"; // perhaps some kind of diagnosic mode?
    }

    void Engine::stop_systems() {
        if (m_Application)
            m_Application->shutdown();

        if (!m_InitOrder.empty())
            for (auto it = std::rbegin(m_InitOrder); it != std::rend(m_InitOrder); ++it) {
                auto jt = find_if(
                        m_Systems,
                        [name = *it](const SystemPtr& ptr) {
                            return ptr->get_name() == name;
                        }
                );

                if (jt == std::end(m_Systems))
                    // somehow the system was initialized and later removed
                    g_Log << "Cannot shutdown system: " << *it;
                else {
                    (*jt)->shutdown();
                }
            }

        save_settings();
    }
}