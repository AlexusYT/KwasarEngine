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
// Created by alexus on 31.01.24.
//

#include "KwasarEngine/resources/LoadedResources.h"

#include "KwasarEngine/utils/Logger.h"
#include "KwasarEngine/render/IRenderable.h"
#include "KwasarEngine/render/Initializable.h"
#include "KwasarEngine/resources/IResource.h"
#include "KwasarEngine/resources/shaders/builtin/PrefabProgram.h"

namespace ke {
LoadedResources::LoadedResources() {
	auto prefabProgram = PrefabProgram::getInstance();
	LoadedResources::addResource(prefabProgram->getResourceUri().string(), prefabProgram);
	LoadedResources::markResourceComplete(prefabProgram->getResourceUri().string());
}

std::shared_ptr<ILoadedResources> LoadedResources::create() {
	return std::shared_ptr<ILoadedResources>(new LoadedResources());
}

void LoadedResources::markResourceComplete(const std::string &pResourceUri) {
	std::lock_guard lock(mutex);
	auto iter = incompleteResources.find(pResourceUri);
	if (iter == incompleteResources.end()) return;
	iter->second->setIncomplete(false);
	if (!iter->second->isInited()) //
		resourcesToInit.emplace_back(iter->second);
	else
		initializedResources.emplace(pResourceUri, iter->second);
	incompleteResources.erase(iter);
}

void LoadedResources::addResource(const std::string &pResourceUri, const std::shared_ptr<IResource> &pResource) {
	std::lock_guard lock(mutex);
	resources.insert_or_assign(pResourceUri, pResource);
	if (pResource->isIncomplete()) incompleteResources.emplace(pResourceUri, pResource);
	else if (!pResource->isInited())
		resourcesToInit.emplace_back(pResource);
	else {
		initializedResources.emplace(pResourceUri, pResource);
		if (auto renderable = std::dynamic_pointer_cast<IRenderable>(pResource)) {
			renderables.emplace_back(renderable);
		}
	}
}

void LoadedResources::removeResource(const std::string &pResourceUri) {
	std::lock_guard lock(mutex);
	auto iter = resources.find(pResourceUri);
	if (iter == resources.end()) return;
	if (auto renderable = std::dynamic_pointer_cast<IRenderable>(iter->second)) erase(renderables, renderable);
	erase(resourcesToInit, iter->second);
	initializedResources.erase(pResourceUri);
	incompleteResources.erase(pResourceUri);
	resources.erase(iter);
}

bool LoadedResources::hasResource(const std::string &pResourceUri) {
	std::lock_guard lock(mutex);
	return resources.contains(pResourceUri);
}

void LoadedResources::render() {
	for (int i = 0, maxI = std::min(static_cast<int>(resourcesToInit.size()), 10); i < maxI; ++i) {
		auto resource = resourcesToInit.front();
		if (auto initializable = std::dynamic_pointer_cast<IInitializable>(resource)) {
			if (auto msg = initializable->initialize()) { Logger::error(msg); }
			resourcesToInit.pop_front();
			initializedResources.emplace(resource->getResourceUri().string(), resource);
			if (auto renderable = std::dynamic_pointer_cast<IRenderable>(resource)) {
				renderables.emplace_back(renderable);
			}
		}
	}

	for (const auto &renderable: renderables) { renderable->render(); }
}

void LoadedResources::uninit() {
	for (const auto &[name, resource]: resources) {
		if (auto initializable = std::dynamic_pointer_cast<IInitializable>(resource)) { initializable->uninitialize(); }
	}
}
} // namespace ke
