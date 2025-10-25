#include <sfml-flexui.hpp>




///////////////////////////////////////
// SFML-FlexUI Root UI Manager Class //
///////////////////////////////////////




/**
 * @brief .
 */
SFUI::UIRoot::UIRoot(const SFUI::SharedPointer<SFUI::Component>& rootComponent) :
    rootComponent(rootComponent)
{}


/**
 * @brief .
 */
SFUI::Void SFUI::UIRoot::setRootComponent(const SFUI::SharedPointer<SFUI::Component>& rootComponent) {
    this->rootComponent = rootComponent;
}


/**
 * @brief .
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
        currentChild->update(renderTargetSize);
    }
}


/**
 * @brief .
 * 
 * @param .
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
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::UIRoot::draw(SFUI::RenderTarget& renderTarget) {
    if (!rootComponent) return;
    glDisable(GL_SCISSOR_TEST);
    // Depth-First Recursive Traversal Algorithm for UI Component Drawing //
    drawRecursive(rootComponent, renderTarget);
}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Void SFUI::UIRoot::drawRecursive(SFUI::SharedPointer<SFUI::Component> component, SFUI::RenderTarget& renderTarget) {
    component->draw(renderTarget);

    // Save Clipping State for Containing Children within its Bounds and Padding //
    GLint parentClipping[4];
    GLboolean scissorWasEnabled = glIsEnabled(GL_SCISSOR_TEST);
    if (scissorWasEnabled) glGetIntegerv(GL_SCISSOR_BOX, parentClipping);
    SFUI::Vector2i componentPosition = component->getPosition();
    SFUI::Vector2f componentSize = component->getSize();
    SFUI::Float componentPadding = component->getPadding();
    GLint newClipping[4] = {
        static_cast<GLint>(componentPosition.x + componentPadding),
        static_cast<GLint>(renderTarget.getSize().y - (componentPosition.y + componentPadding) - (componentSize.y - componentPadding * 2.0f)),
        static_cast<GLint>(componentSize.x - (componentPadding * 2.0f)),
        static_cast<GLint>(componentSize.y - (componentPadding * 2.0f))
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
        drawRecursive(child, renderTarget);
    }

    // Restore Previous Clipping //
    if (scissorWasEnabled) {
        glScissor(parentClipping[0], parentClipping[1], parentClipping[2], parentClipping[3]);
    }   else {
        glDisable(GL_SCISSOR_TEST);
    }
}
