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
    if (rootComponent) {
        // Breadth-First Search Algorithm for UI Component Updating //
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
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::UIRoot::handleEvent(const SFUI::Event& event) {
    if (rootComponent) {
        if (rootComponent) {
            // Breadth-First Search Algorithm for UI Component Drawing //
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
    }
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::UIRoot::draw(SFUI::RenderTarget& renderTarget) {
    if (rootComponent) {
        if (rootComponent) {
            // Breadth-First Search Algorithm for UI Component Drawing //
            std::deque<SFUI::SharedPointer<SFUI::Component>> childrenQueue;
            childrenQueue.push_back(rootComponent);

            while (!childrenQueue.empty()) {
                SFUI::SharedPointer<SFUI::Component> currentChild = childrenQueue.front();
                childrenQueue.pop_front();

                SFUI::Vector<SFUI::SharedPointer<SFUI::Component>> currentChildChildren = currentChild->getChildren();
                for (const auto& currentChildChild : currentChildChildren) {
                    childrenQueue.push_back(currentChildChild);
                }
                currentChild->draw(renderTarget);
            }
        }
    }
}
