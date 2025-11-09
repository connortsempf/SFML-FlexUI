#include "components/Container.hpp"


///////////////////////////////
// Container Component Class //
///////////////////////////////


/**
 * @brief The constructor for Container.
 * 
 * @param componentID The unique identifier for the container component.
 */
SFUI::Container::Container(SFUI::String componentID) :
    Component(std::move(componentID))
{}


/**
 * @brief The constructor for Container.
 * 
 * @param componentID The unique identifier for the container component.
 * @param containerPropGroup The property group for the container component.
 */
SFUI::Container::Container(SFUI::String componentID, SFUI::PropGroup::Container containerPropGroup) :
    Component(std::move(componentID), std::move(containerPropGroup.layout), std::move(containerPropGroup.style))
{}


/**
 * @brief Handle input events for the container.
 * 
 * @param event The input event to handle.
 */
SFUI::Void SFUI::Container::handleEvent(const SFUI::Event& event) {}


/**
 * @brief Recalculate the properties of the container.
 * 
 * @param renderTargetSize The size of the render target.
 */
SFUI::Void SFUI::Container::update(const SFUI::Vector2u renderTargetSize) {
    if (
        this->renderTargetSize != renderTargetSize ||
        layout != dirtyLayout ||
        style != dirtyStyle
    ) {
        this->renderTargetSize = renderTargetSize;
        computeAlignment();
        computeLayoutBox();
        computeStyles();
        computeColors();
        computeShadows();
        computeGraphics();
        computeChildrenLayoutBox();
        updateChildren();
    }
    this->renderTargetSize = renderTargetSize;
    dirtyLayout = layout;
    dirtyStyle = style;
}


/**
 * @brief Draw the container and its contents.
 * 
 * @param drawTarget The render target to draw to.
 * @param window The render window associated with the render target.
 */
SFUI::Void SFUI::Container::draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    drawTarget.draw(shadowRects);
    drawTarget.draw(shadowArcs);
    drawTarget.draw(backgroundRects);
    drawTarget.draw(backgroundArcs);
    drawTarget.draw(borderRects);
    drawTarget.draw(borderArcs);
}
