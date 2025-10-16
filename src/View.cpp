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
SFUI::Void SFUI::View::update(const SFUI::Vector2u parentComponentSize) {
    this->parentComponentSize = parentComponentSize;
    DEBUG_PRINT("\n\nRender Target Size = (" << parentComponentSize.x << ", " << parentComponentSize.y << ")");

    DEBUG_PRINT(componentID << " Pre-Update Size = (" << computedLayout.size.x << ", " << computedLayout.size.y << ")");
    DEBUG_PRINT(componentID << " Pre-Update Position = (" << computedLayout.position.x << ", " << computedLayout.position.y << ")");
    DEBUG_PRINT(componentID << " Pre-Update Margin = (" << computedLayout.margin << ", " << computedLayout.margin << ")");
    DEBUG_PRINT(componentID << " Pre-Update Padding = (" << computedLayout.padding << ", " << computedLayout.padding << ")");

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