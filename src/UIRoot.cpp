/**
 * @file UIRoot.cpp
 * @brief Implements the SFUI UIRoot class for managing the UI hierarchy.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.0
 *
 * This file contains the function definitions and internal logic for the
 * SFUI UIRoot class. It handles:
 *   - Root-level event propagation
 *   - Layout computation for child components
 *   - Rendering of the UI tree
 *
 * It works in conjunction with UIRoot.hpp to provide full functionality
 * for managing UI hierarchies.
 */


#include "base/UIRoot.hpp"


/**
 * @brief Constructor for UIRoot.
 *
 * @param rootComponent The root component of the UI hierarchy.
 */
SFUI::UIRoot::UIRoot(SFUI::UniquePointer<SFUI::Component> rootComponent) :
    rootComponent(std::move(rootComponent))
{}


/**
 * @brief Sets the root component of the UI hierarchy.
 *
 * @param rootComponent The new root component.
 */
SFUI::Void SFUI::UIRoot::setRootComponent(SFUI::UniquePointer<SFUI::Component> rootComponent) {
    this->rootComponent = std::move(rootComponent);
}


/**
 * @brief Handles input events by propagating to all components contained in the UI.
 *
 * @param event The input event to handle.
 */
SFUI::Void SFUI::UIRoot::handleEvent(const SFUI::Event& event) {
    if (!rootComponent) return;

    // Breadth-First Traversal Algorithm for UI Component Drawing //
    std::deque<SFUI::Component*> childrenQueue;
    childrenQueue.push_back(rootComponent.get());

    while (!childrenQueue.empty()) {
        SFUI::Component* currentChild = childrenQueue.front();
        childrenQueue.pop_front();

        const SFUI::Vector<SFUI::UniquePointer<SFUI::Component>>& currentChildChildren = currentChild->getChildren();
        for (const auto& currentChildChild : currentChildChildren) {
            childrenQueue.push_back(currentChildChild.get());
        }
        currentChild->handleEvent(event);
    }
}


/**
 * @brief Updates all components contained in the UI.
 *
 * @param renderTargetSize The dimensions of the object to which the UI is rendering.
 */
SFUI::Void SFUI::UIRoot::update(const SFUI::Vector2u renderTargetSize) {
    if (!rootComponent) return;

    // Breadth-First Traversal Algorithm for UI Component Pre-Updating //
    std::deque<SFUI::Component*> childrenQueue;
    childrenQueue.push_back(rootComponent.get());

    while (!childrenQueue.empty()) {
        SFUI::Component* currentChild = childrenQueue.front();
        childrenQueue.pop_front();
        const SFUI::Vector<SFUI::UniquePointer<SFUI::Component>>& currentChildChildren = currentChild->getChildren();
        for (const auto& currentChildChild : currentChildChildren) {
            childrenQueue.push_back(currentChildChild.get());
        }
        currentChild->preUpdate();
    }

    // Breadth-First Traversal Algorithm for UI Component Updating //
    childrenQueue.clear();
    childrenQueue.push_back(rootComponent.get());

    while (!childrenQueue.empty()) {
        SFUI::Component* currentChild = childrenQueue.front();
        childrenQueue.pop_front();
        const SFUI::Vector<SFUI::UniquePointer<SFUI::Component>>& currentChildChildren = currentChild->getChildren();
        for (const auto& currentChildChild : currentChildChildren) {
            childrenQueue.push_back(currentChildChild.get());
        }
        currentChild->update(renderTargetSize);
    }
}


/**
 * @brief Draws all components contained in the UI.
 *
 * @param drawTarget The render target to draw on.
 * @param window The render window associated with the render target.
 */
SFUI::Void SFUI::UIRoot::draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    if (!rootComponent) return;

    // Depth-First Recursive Traversal Algorithm for UI Component Drawing //
    glDisable(GL_SCISSOR_TEST);
    drawRecursive(rootComponent, drawTarget, window);

    // Breadth-First Traversal Algorithm for UI Overlay Component Drawing //
    drawOverlay(drawTarget, window);
}


/**
 * @brief Recursive helper function to draw components and their children.
 *
 * @param component The current component to draw.
 * @param drawTarget The render target to draw on.
 * @param window The render window associated with the render target.
 */
SFUI::Void SFUI::UIRoot::drawRecursive(const SFUI::UniquePointer<SFUI::Component>& component, SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    component->draw(drawTarget, window);

    // Save Clipping State for Containing Children within its Bounds and Padding //
    GLint parentClipping[4];
    GLboolean scissorWasEnabled = glIsEnabled(GL_SCISSOR_TEST);
    if (scissorWasEnabled) glGetIntegerv(GL_SCISSOR_BOX, parentClipping);
    SFUI::Vector2i componentPosition = component->getPosition();
    SFUI::Vector2f componentSize = component->getSize();
    SFUI::Vector4f componentPadding = component->getPadding();
    GLint newClipping[4] = {
        static_cast<GLint>(componentPosition.x + componentPadding.x),
        static_cast<GLint>(drawTarget.getSize().y - (componentPosition.y + componentPadding.z) - (componentSize.y - componentPadding.z - componentPadding.w)),
        static_cast<GLint>(componentSize.x - (componentPadding.x + componentPadding.y)),
        static_cast<GLint>(componentSize.y - (componentPadding.z + componentPadding.w))
    };
    if (scissorWasEnabled) {
        GLint newRight = newClipping[0] + newClipping[2];
        GLint newBottom = newClipping[1] + newClipping[3];
        GLint parentRight = parentClipping[0] + parentClipping[2];
        GLint parentBottom = parentClipping[1] + parentClipping[3];
        newClipping[0] = std::max(newClipping[0], parentClipping[0]);
        newClipping[1] = std::max(newClipping[1], parentClipping[1]);
        newClipping[2] = std::min(newRight, parentRight) - newClipping[0];
        newClipping[3] = std::min(newBottom, parentBottom) - newClipping[1];
        newClipping[2] = std::max(0, newClipping[2]);
        newClipping[3] = std::max(0, newClipping[3]);
    };
    glEnable(GL_SCISSOR_TEST);
    glScissor(newClipping[0], newClipping[1], newClipping[2], newClipping[3]);

    // Recursive Draw Call //
    for (const auto& child : component->getChildren()) {
        drawRecursive(child, drawTarget, window);
    }

    // Restore Previous Clipping //
    if (scissorWasEnabled) {
        glScissor(parentClipping[0], parentClipping[1], parentClipping[2], parentClipping[3]);
    }   else {
        glDisable(GL_SCISSOR_TEST);
    }
}


/**
 * @brief Draw the overlay components of the UI to the render target.
 *
 * @param drawTarget Target to draw on.
 * @param window Window reference.
 */
SFUI::Void SFUI::UIRoot::drawOverlay(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    if (!rootComponent) return;

    // Breadth-First Traversal Algorithm for UI Overlay Component Drawing //
    std::deque<SFUI::Component*> childrenQueue;
    childrenQueue.push_back(rootComponent.get());

    while (!childrenQueue.empty()) {
        SFUI::Component* currentChild = childrenQueue.front();
        childrenQueue.pop_front();

        const SFUI::Vector<SFUI::UniquePointer<SFUI::Component>>& currentChildChildren = currentChild->getChildren();
        for (const auto& currentChildChild : currentChildChildren) {
            childrenQueue.push_back(currentChildChild.get());
        }

        currentChild->drawOverlay(drawTarget, window);
    }
}
