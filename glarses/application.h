#pragma once

#ifndef GLARSES_APPLICATION_H
#define GLARSES_APPLICATION_H

#include "dependencies.h"
#include "util/mediator.h"
#include "t5/t5_manager.h"

#include <mutex>
#include <vector>

namespace glarses {
	class Application {
	public:
		Application();
        virtual ~Application();

		void run();

	private:

	};
}

#endif