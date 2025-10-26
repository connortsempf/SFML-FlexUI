#include <sfml-flexui-types.hpp>
#include <sfml-flexui-core.hpp>


///////////////////////////////////////////
// SFML-FlexUI Container Component Class //
///////////////////////////////////////////


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Container::Container(const SFUI::String& componentID) :
    Component(componentID)
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Container::Container(const SFUI::String& componentID, const SFUI::PropGroup::Container& containerPropGroup) :
    Component(componentID, containerPropGroup.layout, containerPropGroup.style)
{}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Container::update(const SFUI::Vector2u renderTargetSize) {
    this->renderTargetSize = renderTargetSize;

    computeAlignDirection();
    computeAlignPrimary();
    computeAlignSecondary();
    computeMargin();
    computeSize();
    computePadding();
    computePosition();
    computeBorderWidth();
    computeCornerRadius();
    computeFillColor();
    computeBorderColor();
    computeGraphics();
    computeChildrenMargin();
    computeChildrenSize();
    computeChildrenPosition();
    updateChildren();
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Container::handleEvent(const SFUI::Event& event) {}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Container::draw(SFUI::RenderTarget& renderTarget) {
    renderTarget.draw(backgroundRects);
    renderTarget.draw(backgroundArcs);
    renderTarget.draw(borderRects);
    renderTarget.draw(borderArcs);
}
