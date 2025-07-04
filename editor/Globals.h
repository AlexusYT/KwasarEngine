//  KwasarEngine is an SDK that can help you speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 31.12.23.
//

#ifndef GLOBALS_H
#define GLOBALS_H

namespace ked {
class Globals {
	inline static std::filesystem::path logPath;
	inline static std::filesystem::path projectsPath;
	inline static std::filesystem::path configPath;
	inline static std::filesystem::path cachePath;

public:
	static void init();

	[[nodiscard]] static const std::filesystem::path &getLogPath() { return logPath; }

	static void setLogPath(const std::filesystem::path &pLogPath) { logPath = pLogPath; }

	[[nodiscard]] static const std::filesystem::path &getProjectsPath() { return projectsPath; }

	static void setProjectsPath(const std::filesystem::path &pProjectsPath) { projectsPath = pProjectsPath; }

	[[nodiscard]] static const std::filesystem::path &getConfigPath() { return configPath; }

	static void setConfigPath(const std::filesystem::path &pConfigPath) { configPath = pConfigPath; }

	[[nodiscard]] static const std::filesystem::path &getCachePath() { return cachePath; }

	static void setCachePath(const std::filesystem::path &pCachePath) { cachePath = pCachePath; }
};
}


#endif //GLOBALS_H
