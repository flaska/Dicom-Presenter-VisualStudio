#include "dpDebug/dpDebug.h"

void debug::Log::init(){
	remove("log.txt");
}

void debug::Log::write(string message){
	file.open("log.txt",fstream::app);
	file << message << "\n";
	file.close();
}