#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <fstream>
#include <filesystem>

std::ofstream open_log_file() {
	return std::ofstream(std::filesystem::current_path().string() + "/achievementhandler.log", std::ios::app);
}

void log(std::string message) {
	auto ofs = open_log_file();

	ofs << message << "\n";

	ofs.close();
}

void log_ccharpp(const char** message) {
	auto ofs = open_log_file();

	ofs << message << "\n";

	ofs.close();
}

void log_two(std::string var_name, std::string var_var) {
	auto ofs = open_log_file();

	ofs << var_name << ": " << var_var << "\n";

	ofs.close();
}

#endif