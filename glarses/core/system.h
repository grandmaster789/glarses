#ifndef GLARSES_CORE_SYSTEM_H
#define GLARSES_CORE_SYSTEM_H

#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../dependencies.h"
#include "../util/mediator.h"

namespace glarses {
    class Application;

    /*
    *	Current Subsystem features:
    *		Engine-managed initialization/shutdown including inter-system dependency management
    *		Engine-managed updating of running subsystems
    *		Per-subsystem settings via combined 'glarses.json'
    *
    *	Considerations:
    *		Per-system logs? currently everything uses a global log though
    *       Dependencies are currently string-based, which is not ideal
    */

    class System {
    private:
        struct JsonProperties {
            std::string                           m_VariableName;
            Function<nlohmann::json()>            m_GetFn;
            Function<void(const nlohmann::json&)> m_SetFn;
        };

    public:
        friend class Engine;
        friend class Settings;

        using Dependencies = std::vector<std::string>; // based on System names (which must be unique)
        using Settings     = std::vector<JsonProperties>;

        explicit System(std::string unique_system_name);
        virtual ~System() = default;

        // no-copy, move-only semantics
        System             (const System&) = delete;
        System& operator = (const System&) = delete;
        System             (System&&) noexcept = default;
        System& operator = (System&&) noexcept = default;

        virtual bool init();     // returns false on failure
        virtual void update();   // update tick
        virtual void shutdown(); // prepare for cleanup - finalize settings to be serialized etc

        [[nodiscard]] const std::string& get_name() const;

        [[nodiscard]] const Dependencies& get_dependencies() const;
        [[nodiscard]] const Settings&     get_settings()     const;

        friend std::ostream& operator << (std::ostream& os, const System& s);

    protected:
        void add_dependency(const std::string& system_name);

        template <typename T>
        void register_setting(
                const std::string& json_key,
                T*                 member_variable
        );

        class Engine* m_Engine = nullptr;

    private:
        std::string  m_Name;
        Dependencies m_Dependencies;
        Settings     m_Settings;
    };

    // a System derives from System but is not an Application
    template <typename T>
    concept c_System =
        std::derived_from<T, System> &&
        !std::derived_from<T, Application>;
}

#include "system.inl"

#endif