#pragma once

#ifndef GLARSES_APPLICATION_H
#define GLARSES_APPLICATION_H

#include "dependencies.h"
#include "util/mediator.h"
#include "os/window.h"

#include <mutex>
#include <vector>

namespace glarses {
	class Application {
	public:
		Application();
        virtual ~Application();

		void run();

	private:
        std::vector<os::Window> m_Windows;
	};
}

#endif