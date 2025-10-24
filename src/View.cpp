#include <sfml-flexui.hpp>


//////////////////////////////////////
// SFML-FlexUI View Component Class //
//////////////////////////////////////


/**
 * @brief .
 * 
 * @param .
 */
SFUI::View::View(const SFUI::String& componentID) : Component(componentID) {}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::View::View(const SFUI::String& componentID, const SFUI::Prop::Layout& layout) : Component(componentID, layout) {}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::View::View(const SFUI::String& componentID, const SFUI::Prop::Style& style) : Component(componentID, style) {}


/**
 * @brief .
 * 
 * @param .
 * @param .
 * @param .
 */
SFUI::View::View(const SFUI::String& componentID, const SFUI::Prop::Layout& layout, const SFUI::Prop::Style& style) :
    Component(componentID, layout, style)
{}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::View::update(const SFUI::Vector2u renderTargetSize) {
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
SFUI::Void SFUI::View::handleEvent(const SFUI::Event& event) {

}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::View::draw(SFUI::RenderTarget& renderTarget) {
    renderTarget.draw(backgroundRects);
    renderTarget.draw(backgroundArcs);
    renderTarget.draw(borderRects);
    renderTarget.draw(borderArcs);
}