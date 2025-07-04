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
// Created by alexus on 22.03.24.
//

#ifndef PROJECTIONCAMERAMOD_H
#define PROJECTIONCAMERAMOD_H
#include <glm/mat4x4.hpp>
#include <sigc++/signal.h>

#include "KwasarEngine/extensions/Extension.h"
#include "KwasarEngine/extensions/ExtensionProperty.h"
#include "IProjectionCameraMod.h"

namespace ke {
class PerspectiveProjectionCameraMod : public Extension, public IProjectionCameraMod {
protected:
	PerspectiveProjectionCameraMod();

public:
	ExtensionProperty<float> propertyAspect;
	ExtensionProperty<float> propertyFov;
	ExtensionProperty<float> propertyZNear;
	ExtensionProperty<float> propertyZFar;

	ExtensionProperty<glm::mat4> propertyProjMatrix;

	[[nodiscard]] sigc::signal<void(const glm::mat4 &)> &getOnProjMatrixChanged() override {
		return propertyProjMatrix.getEvent();
	}

	[[nodiscard]] const glm::mat4 &getProjMatrix() const override { return propertyProjMatrix; }

protected:
	virtual void projectionMatrixChanged(const glm::mat4 &pNewMatrix);

private:
	void updateProjMatrix();
};
} // namespace ke

#endif //PROJECTIONCAMERAMOD_H