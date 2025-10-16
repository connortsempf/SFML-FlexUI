#include <sfml-flexui.hpp>




////////////////////////////////////////
// SFML-FlexUI Button Component Class //
////////////////////////////////////////




/**
 * @brief .
 * 
 * @param .
 */
SFUI::Button::Button(const SFUI::String& componentID) : Component(componentID) {}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Button::Button(const SFUI::String& componentID, const Style& style) : Component(componentID, style) {}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Button::Button(const SFUI::String& componentID, const Layout& layout) : Component(componentID, layout) {}


/**
 * @brief .
 * 
 * @param .
 * @param .
 * @param .
 */
SFUI::Button::Button(const SFUI::String& componentID, const Style& style, const Layout& layout) : Component(componentID, style, layout) {}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Button::update(const SFUI::Vector2u parentComponentSize) {
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
SFUI::Void SFUI::Button::handleEvent(const SFUI::Event& event) {

}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Button::draw(SFUI::RenderTarget& renderTarget) {
    renderTarget.draw(backgroundRects);
    renderTarget.draw(backgroundArcs);
    renderTarget.draw(borderRects);
    renderTarget.draw(borderArcs);
}