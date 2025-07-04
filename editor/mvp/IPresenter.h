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
// Created by alexus on 24.01.24.
//

#ifndef IPRESENTER_H
#define IPRESENTER_H

namespace ked {
class IView;

class IPresenter {
public:
	virtual ~IPresenter() = default;

	virtual void run() = 0;

	virtual void stop() = 0;

	virtual void addView(const std::shared_ptr<IView> &pNewView);

	virtual void removeView(const std::shared_ptr<IView> &pOldView);

	virtual std::string getTypeName() = 0;
};

inline void IPresenter::addView(const std::shared_ptr<IView> & /*pNewView*/) {}

inline void IPresenter::removeView(const std::shared_ptr<IView> & /*pOldView*/) {}
} // namespace ked


#endif //IPRESENTER_H
