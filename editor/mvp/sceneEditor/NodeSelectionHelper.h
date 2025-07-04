//  KwasarEngine is an SDK that can help you speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 10.04.25.
//

#ifndef NODESELECTIONHELPER_H
#define NODESELECTIONHELPER_H
#include <vector>

namespace ke {
class Node;
}

namespace ked {
class NodeSelectionHelper {
	std::vector<ke::Node*> selectedNodes{};
	sigc::signal<void(const std::vector<ke::Node*> &pNodes, bool pSelected)> onNodeSelectionChanged;
	ke::Node* editingNode{};
	sigc::signal<void(ke::Node* pNode)> onEditingNodeChanged;

public:
	void addNode(ke::Node* pNode) {
		selectedNodes.push_back(pNode);
		onNodeSelectionChanged(std::vector{pNode}, true);
		editingNode = pNode;
		onEditingNodeChanged(editingNode);
	}

	void addNodes(const std::vector<ke::Node*> &pNodes) {
		selectedNodes.insert(selectedNodes.cend(), pNodes.begin(), pNodes.end());
		onNodeSelectionChanged(pNodes, true);
	}

	void clearSelection() {
		onNodeSelectionChanged(selectedNodes, false);
		selectedNodes.clear();
		editingNode = nullptr;
		onEditingNodeChanged(editingNode);
	}

	[[nodiscard]] const std::vector<ke::Node*> &getSelectedNodes() const { return selectedNodes; }

	sigc::connection connectOnNodeSelectionChanged(
		const sigc::slot<void(const std::vector<ke::Node*> &pNodes, bool pSelected)> &pSlot) {
		return onNodeSelectionChanged.connect(pSlot);
	}

	[[nodiscard]] ke::Node* getEditingNode() const { return editingNode; }

	sigc::connection connectOnEditingNodeChanged(const sigc::slot<void(ke::Node* pNode)> &pSlot) {
		pSlot(editingNode);
		return onEditingNodeChanged.connect(pSlot);
	}
};
} // namespace ked

#endif //NODESELECTIONHELPER_H
