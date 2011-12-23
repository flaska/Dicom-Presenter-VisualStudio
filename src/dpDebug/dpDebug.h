#pragma once

#include <fstream>
#include <string>

using namespace std;
namespace debug {
	class Log {
		ofstream file;
	public:
		void init();
		void write (string message);
	};
}