#pragma once

#ifndef GLARSES_APPLICATION_H
#define GLARSES_APPLICATION_H

#include "dependencies.h"
#include "util/mediator.h"
#include "os/window.h"
#include "core/system.h"

#include <mutex>
#include <vector>

namespace glarses {
	class Application:
			public System
	{
	public:
		Application();
        virtual ~Application();

        bool init() override;
		void update() override;

	private:
        std::vector<os::Window> m_Windows;
	};

    // an Application derives from System; only one Application may run on the Engine
	template <typename T>
	concept c_Application = std::derived_from<T, Application>;
}

#endif