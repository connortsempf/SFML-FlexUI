#include "base/UIRoot.hpp"

#include <chrono>


///////////////////////////
// Root UI Manager Class //
///////////////////////////


/**
 * @brief Constructor for UIRoot.
 * 
 * @param rootComponent The root component of the UI hierarchy.
 */
SFUI::UIRoot::UIRoot(const SFUI::SharedPointer<SFUI::Component>& rootComponent) :
    rootComponent(rootComponent)
{}


/**
 * @brief Sets the root component of the UI hierarchy.
 * 
 * @param rootComponent The new root component.
 */
SFUI::Void SFUI::UIRoot::setRootComponent(const SFUI::SharedPointer<SFUI::Component>& rootComponent) {
    this->rootComponent = rootComponent;
}


/**
 * @brief Handles input events by propagating to all children in the contained UI.
 * 
 * @param event The input event to handle.
 */
SFUI::Void SFUI::UIRoot::handleEvent(const SFUI::Event& event) {
    if (!rootComponent) return;
        
    // Breadth-First Traversal Algorithm for UI Component Drawing //
    std::deque<SFUI::SharedPointer<SFUI::Component>> childrenQueue;
    childrenQueue.push_back(rootComponent);

    while (!childrenQueue.empty()) {
        SFUI::SharedPointer<SFUI::Component> currentChild = childrenQueue.front();
        childrenQueue.pop_front();

        SFUI::Vector<SFUI::SharedPointer<SFUI::Component>> currentChildChildren = currentChild->getChildren();
        for (const auto& currentChildChild : currentChildChildren) {
            childrenQueue.push_back(currentChildChild);
        }
        currentChild->handleEvent(event);
    }
}


/**
 * @brief Updates all children in the contained UI.
 */
SFUI::Void SFUI::UIRoot::update(const SFUI::Vector2u renderTargetSize) {
    if (!rootComponent) return;

    // Breadth-First Traversal Algorithm for UI Component Updating //
    std::deque<SFUI::SharedPointer<SFUI::Component>> childrenQueue;
    childrenQueue.push_back(rootComponent);

    while (!childrenQueue.empty()) {
        SFUI::SharedPointer<SFUI::Component> currentChild = childrenQueue.front();
        childrenQueue.pop_front();

        SFUI::Vector<SFUI::SharedPointer<SFUI::Component>> currentChildChildren = currentChild->getChildren();
        for (const auto& currentChildChild : currentChildChildren) {
            childrenQueue.push_back(currentChildChild);
        }

        // auto start = std::chrono::high_resolution_clock::now();

        currentChild->update(renderTargetSize);

        // auto end = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double, std::micro> elapsed = end - start;
        // accumulator += elapsed.count();
        // if (count >= 100.0) {
        //     std::cout << "\rAverage Update Time: " << accumulator / count << " us";
        //     count = 0.0;
        //     accumulator = 0.0;
        // }   else count += 1.0;
    }
}


/**
 * @brief Draws all children in the contained UI.
 * 
 * @param drawTarget The render target to draw on.
 * @param window The render window associated with the render target.
 */
SFUI::Void SFUI::UIRoot::draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    if (!rootComponent) return;
    glDisable(GL_SCISSOR_TEST);
    // Depth-First Recursive Traversal Algorithm for UI Component Drawing //
    drawRecursive(rootComponent, drawTarget, window);
}


/**
 * @brief Recursive helper function to draw components and their children.
 * 
 * @param component The current component to draw.
 * @param drawTarget The render target to draw on.
 * @param window The render window associated with the render target.
 */
SFUI::Void SFUI::UIRoot::drawRecursive(SFUI::SharedPointer<SFUI::Component> component, SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
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
