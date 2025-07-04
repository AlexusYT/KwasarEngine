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
// Created by alexus on 12.08.24.
//

#ifndef IVIEWPROJECTEXPLORER_H
#define IVIEWPROJECTEXPLORER_H

#include "mvp/IView.h"

namespace ked {
class IPresenterProjectExplorer;
class ProjectExplorerElement;

class IViewProjectExplorer : public IView {
public:
	virtual void setElements(const std::vector<std::shared_ptr<ProjectExplorerElement>> &pArray) = 0;

	[[nodiscard]] virtual IPresenterProjectExplorer* getPresenter() const = 0;

	virtual void setPresenter(IPresenterProjectExplorer* pPresenter) = 0;
};
} // namespace ked

#endif //IVIEWPROJECTEXPLORER_H
