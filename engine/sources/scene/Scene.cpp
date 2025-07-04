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
// Created by alexus on 02.02.24.
//

#include "KwasarEngine/scene/Scene.h"

#include <epoxy/gl.h>

#include "KwasarEngine/extensions/Extension.h"
#include "KwasarEngine/extensions/ExtensionProperty.h"
#include "KwasarEngine/extensions/cameras/ICamera.h"
#include "KwasarEngine/light/ILightInstance.h"
#include "KwasarEngine/light/LightSources.h"
#include "KwasarEngine/prefabs/Prefab.h"
#include "KwasarEngine/prefabs/PrefabInstance.h"
#include "KwasarEngine/prefabs/elements/MeshPrefabElement.h"
#include "KwasarEngine/prefabs/elements/PrefabElementInstance.h"
#include "KwasarEngine/resources/LoadedResources.h"
#include "KwasarEngine/resources/materials/ColorComponent.h"
#include "KwasarEngine/resources/materials/IMaterialResource.h"
#include "KwasarEngine/resources/materials/MaterialResource.h"
#include "KwasarEngine/resources/models/Model3DResource.h"
#include "KwasarEngine/resources/shaders/builtin/PrefabProgram.h"
#include "KwasarEngine/scene/objects/ISceneObject.h"
#include "KwasarEngine/utils/Logger.h"

namespace ke {
Scene::Scene()
	: programBuffer(std::make_shared<ProgramWideShaderBuffer>()), lightSources(LightSources::create()) {
	prefab = Prefab::create();
}

Scene::~Scene() {
	Scene::deinitScene();
	objects.clear();
}

std::shared_ptr<Scene> Scene::create() { return std::shared_ptr<Scene>(new Scene()); }

void Scene::setViewProjMatrix(const glm::mat4 &pViewProjMatrix) const {
	programBuffer->setViewProjMatrix(pViewProjMatrix);
}

void Scene::onResourceLoadingError(const std::string & /*pRequest*/, const ke::ReportMessagePtr &pError) {
	ke::Logger::error(pError);
}

void Scene::switchCamera(ICamera* pNewCamera) {
	currentCamera = pNewCamera;
	static sigc::connection connection;
	if (!connection.empty()) connection.disconnect();
	connection = pNewCamera->getOnMatrixChanged().connect(sigc::mem_fun(*this, &Scene::setViewProjMatrix));
	pNewCamera->updateMatrix();
	pNewCamera->getPosition().connectEvent(
		[this](const glm::vec3 &pPosition) { programBuffer->setCameraPos(pPosition); });
}

void Scene::beforeRender() {
	//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

ke::ReportMessagePtr Scene::initScene() {
	if (inited) return nullptr;
	for (const auto &object: objects) { if (auto msg = object->init()) return msg; }
	resourceExecutor->loadResourceAsync("Test.enmodel",
										[this](const std::shared_ptr<ResourceLoadResult> & /*pResult*/) {
											/*if (!pResult->isReady()) return;
		auto model = std::dynamic_pointer_cast<Model3DResource>(pResult->getResource());
		if (!model) return;
		for (const auto &modelObject: model->getModelObjects()) {
			auto element = MeshPrefabElement::create(modelObject.second);
			prefab->addElement(element);
		}

		instance2->visible = false;
		auto element = prefab->getElement("CustomPistol9mm");
		auto &transform = instance->getElement(element->getUuid())->getLocalTransform();
		transform->translate(0.0f, 0.1f, 0);
		auto element1 = prefab->getElement("CustomPistol9mm.007");
		element1->visible = false;

		auto elementInst1 = instance2->getElement(element->getUuid());
		elementInst1->visible = false;
		auto elementInst = instance2->getElement(element1->getUuid());
		auto transform1 = elementInst->getLocalTransform();
		transform1->translate(0.0f, 0.1f, 0);
		auto element2 = prefab->getElement("CustomPistol9mm.008");
		auto mat = MaterialResource::create();
		mat->setAlbedo(ColorComponent::create(1, 0, 0));
		element2->material = mat;*/
											/*auto &material = elementInst->material;
		MaterialResource::create();
		material->setAlbedo(ColorComponent::create(1, 0, 1));
		material.notifyChanged();*/
										});
	resourceExecutor->loadResourceAsync("test.enmat", [this](const std::shared_ptr<ResourceLoadResult> &pResult) {
		if (!pResult->isReady()) return;
		prefab->material = std::dynamic_pointer_cast<IMaterialResource>(
			pResult->getResource());
	});

	prefab->setShaderProgram(PrefabProgram::getInstance());
	instance = PrefabInstance::create();
	prefab->addInstance(instance);
	instance->getLocalTransform()->translate(glm::vec3(0.1f, 0.0f, 0.0f));
	instance2 = PrefabInstance::create();
	prefab->addInstance(instance2);

	prefab->initialize();


	inited = true;
	glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, false);
	glDebugMessageCallback(
		[](GLenum pSource, GLenum pType, GLuint pId, GLenum pSeverity, GLsizei pLength, const GLchar* pMessage,
		   const void* /*pUserParam*/) {
			//if (pId == 131076) return;
			if (pId == 131185) return;
			//if (pSeverity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
			auto msg = ReportMessage::create();
			msg->setTitle("OpenGL error");
			msg->setMessage(std::string(pMessage, static_cast<std::string::size_type>(pLength)));
			std::string sourceStr;
			switch (pSource) {
				case GL_DEBUG_SOURCE_API: sourceStr = "Calls to the OpenGL API (GL_DEBUG_SOURCE_API)";
					break;
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
					sourceStr = "Calls to a window-system API (GL_DEBUG_SOURCE_WINDOW_SYSTEM)";
					break;
				case GL_DEBUG_SOURCE_SHADER_COMPILER:
					sourceStr = "A compiler for a shading language (GL_DEBUG_SOURCE_SHADER_COMPILER)";
					break;
				case GL_DEBUG_SOURCE_THIRD_PARTY:
					sourceStr = "An application associated with OpenGL (GL_DEBUG_SOURCE_THIRD_PARTY)";
					break;
				case GL_DEBUG_SOURCE_APPLICATION:
					sourceStr = "Generated by the user of this application (GL_DEBUG_SOURCE_APPLICATION)";
					break;
				case GL_DEBUG_SOURCE_OTHER: sourceStr = "Some other source (GL_DEBUG_SOURCE_OTHER)";
					break;
				default: sourceStr = "Unknown";
			}

			msg->addInfoLine("Error source: {}: {}", sourceStr, pSource);
			std::string typeString;
			switch (pType) {
				case GL_DEBUG_TYPE_ERROR: typeString = "An error, typically from the API ()";
					break;
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
					typeString = "Some behavior marked deprecated has been used (GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)";
					break;
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
					typeString = "Something has invoked undefined behavior (GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)";
					break;
				case GL_DEBUG_TYPE_PORTABILITY:
					typeString = "Some functionality the user relies upon is not portable (GL_DEBUG_TYPE_PORTABILITY)";
					break;
				case GL_DEBUG_TYPE_PERFORMANCE:
					typeString = "Code has triggered possible performance issues (GL_DEBUG_TYPE_PERFORMANCE)";
					break;
				case GL_DEBUG_TYPE_MARKER: typeString = "Command stream annotation (GL_DEBUG_TYPE_MARKER)";
					break;
				case GL_DEBUG_TYPE_PUSH_GROUP: typeString = "Group pushing (GL_DEBUG_TYPE_PUSH_GROUP)";
					break;
				case GL_DEBUG_TYPE_POP_GROUP: typeString = "Group popping (GL_DEBUG_TYPE_POP_GROUP)";
					break;
				case GL_DEBUG_TYPE_OTHER: typeString = "Some other type (GL_DEBUG_TYPE_OTHER)";
					break;
				default: typeString = "Unknown";
			}
			msg->addInfoLine("Error type: {}: {}", typeString, pType);
			msg->addInfoLine("Error id: {}", pId);
			std::string severityStr;
			switch (pSeverity) {
				case GL_DEBUG_SEVERITY_HIGH:
					severityStr = "All OpenGL Errors, shader compilation/linking errors, or highly-dangerous "
						"undefined behavior (GL_DEBUG_SEVERITY_HIGH)";
					break;
				case GL_DEBUG_SEVERITY_MEDIUM:
					severityStr = "Major performance warnings, shader compilation/linking warnings, or the use of "
						"deprecated functionality (GL_DEBUG_SEVERITY_MEDIUM)";
					break;
				case GL_DEBUG_SEVERITY_LOW:
					severityStr = "Redundant state change performance warning, or unimportant undefined behavior "
						"(GL_DEBUG_SEVERITY_LOW)";
					break;
				case GL_DEBUG_SEVERITY_NOTIFICATION:
					severityStr = "Anything that isn't an error or performance issue (GL_DEBUG_SEVERITY_NOTIFICATION)";
					break;
				default: severityStr = "Unknown";
			}
			msg->addInfoLine("Error severity: {}: {}", severityStr, pSeverity);
			Logger::error(msg);
		},
		nullptr);
	return nullptr;
}

void Scene::deinitScene() {
	for (const auto &object: objects) { object->deinit(); }
	resourceExecutor->getResources()->uninit();
}

void Scene::addObject(const std::shared_ptr<ISceneObject> &pObject) {
	pObject->setScene(this);

	if (inited) {
		if (auto msg = pObject->init()) {
			Logger::warn("Object shouldn't report about errors. Ignoring");
			Logger::error(msg);
		}
	}
	pObject->connectOnExtensionAdded([this](const std::shared_ptr<Extension> &pExtension) {
		if (auto lightInstance = std::dynamic_pointer_cast<ILightInstance>(pExtension)) {
			lightSources->addLightInstance(lightInstance);
		}
	});
	pObject->connectOnExtensionRemoved([this](const std::shared_ptr<Extension> &pExtension) {
		if (auto lightInstance = std::dynamic_pointer_cast<ILightInstance>(pExtension)) {
			lightSources->removeLightInstance(lightInstance);
		}
	});
	objects.emplace_back(pObject);
	onObjectAddedSignal(pObject.get());
}

void Scene::removeObject(ISceneObject* pObjectToRemove) {
	erase_if(objects, [&pObjectToRemove](const std::shared_ptr<ISceneObject> &pObject) {
		return pObject.get() == pObjectToRemove;
	});
	onObjectRemovedSignal(pObjectToRemove);
}

void Scene::render() {
	beforeRender();
	programBuffer->update();
	programBuffer->bindBufferBase(0);
	lightSources->updateSsbo();
	lightSources->getLightSsbo()->bindBufferBase(3);
	resourceExecutor->getResources()->render();
	std::dynamic_pointer_cast<IRenderable>(prefab)->render();
	for (const auto &object: objects) { object->render(); }
	afterRender();
}

void Scene::resize(const int pWidth, const int pHeight) {
	glViewport(0, 0, pWidth, pHeight);
	onWindowSizeChangedSignal(pWidth, pHeight);
	for (const auto &object: objects) { object->onWindowSizeChanged(pWidth, pHeight); }
}

void Scene::onCursorPosChanged(const double pX, const double pY) {
	for (const auto &object: objects) { object->onCursorPosChanged(pX, pY); }
}

void Scene::onKeyChanged(const KeyboardKey pKey, const bool pPressed, const ModifierKeys &pMods) {
	for (const auto &object: objects) { object->onKeyStateChanged(pKey, pPressed, pMods); }
}

void Scene::onMouseButtonStateChanged(const MouseButton pButton, const bool pPressed, const double pX,
									  const double pY) {
	for (const auto &object: objects) { object->onMouseButtonStateChanged(pButton, pPressed, pX, pY); }
}

bool Scene::notifyOnMouseScroll(double pDx, double pDy) {
	bool handled{};
	for (const auto &object: objects) handled = object->notifyOnMouseScroll(pDx, pDy) || handled;
	return handled;
}
} // namespace ke
