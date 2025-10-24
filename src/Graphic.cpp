#include <sfml-flexui.hpp>


/////////////////////////////////////////
// SFML-FlexUI Graphic Component Class //
/////////////////////////////////////////


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Graphic::Graphic(const SFUI::String& componentID) :
    Component(componentID),
    graphicStyle(),
    graphicBehavior(),
    graphic(sf::Sprite(graphicSource))
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Graphic::Graphic(const SFUI::String& componentID, const SFUI::PropGroup::Graphic& graphicPropGroup) :
    Component(componentID, graphicPropGroup.layout, graphicPropGroup.style),
    graphicStyle(graphicPropGroup.graphicStyle),
    graphicBehavior(graphicPropGroup.graphicBehavior),
    graphic(sf::Sprite(graphicSource))
{}


/**
 * @brief .
 */
SFUI::Void SFUI::Graphic::computeGraphicSource() {
    // Check if Pre-Loaded Texture was Provided //
    if (graphicStyle.loadedGraphic.has_value()) {
        loadType = SFUI::Graphic::LoadType::PRE_LOAD;
        if (loadState == SFUI::Graphic::LoadState::PRE_LOAD_UNLOADED) {
            graphic = SFUI::Sprite(*graphicStyle.loadedGraphic.value());
            computedGraphicStyle.originalTextureSize = {graphic.getGlobalBounds().size.x, graphic.getGlobalBounds().size.y};
            loadState = SFUI::Graphic::LoadState::PRE_LOAD_LOADED;
        }
        return;
    }

    // Check if Non-Loaded Texture Path was Given //
    if (graphicStyle.graphicPath.has_value() && graphicStyle.graphicPath.value() != "") {
        SFUI::Graphic::LoadType::SELF_LOAD;
        if (graphicStyle.graphicPath.value() != computedGraphicStyle.graphicPath) {
            loadState = SFUI::Graphic::LoadState::SELF_LOAD_UNLOADED;
            computedGraphicStyle.graphicPath = graphicStyle.graphicPath.value();
            if (!graphicSource.loadFromFile(computedGraphicStyle.graphicPath)) {
                if (graphicBehavior.onLoadError) graphicBehavior.onLoadError(componentID);
            }   else {
                if (graphicBehavior.onLoad) graphicBehavior.onLoad(componentID);
                graphic = SFUI::Sprite(graphicSource);
                computedGraphicStyle.originalTextureSize = {graphic.getGlobalBounds().size.x, graphic.getGlobalBounds().size.y};
            }
        }
    }
}


/**
 * @brief .
 */
SFUI::Void SFUI::Graphic::computeGraphicAlign() {
    SFUI::String tempAlign = graphicStyle.graphicAlign;
    std::transform(tempAlign.begin(), tempAlign.end(), tempAlign.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempAlign == "fit" || tempAlign == "fill" || tempAlign == "stretch" || tempAlign == "center")
        computedGraphicStyle.graphicAlign = tempAlign;
    else
        computedLayout.alignSecondary = "fit";
}



/**
 * @brief .
 */
SFUI::Void SFUI::Graphic::computeGraphic() {
    SFUI::Vector2f computedScale = {0.0f, 0.0f};
    SFUI::Float scaleX = computedLayout.size.x / computedGraphicStyle.originalTextureSize.x;
    SFUI::Float scaleY = computedLayout.size.y / computedGraphicStyle.originalTextureSize.y;

    if (computedGraphicStyle.graphicAlign == "fit") {
        SFUI::Float fitScale = std::min(scaleX, scaleY);
        computedScale = {fitScale, fitScale};
    }
    else if (computedGraphicStyle.graphicAlign == "fill") {
        SFUI::Float fillScale = std::max(scaleX, scaleY);
        computedScale = {fillScale, fillScale};
    }
    else if (computedGraphicStyle.graphicAlign == "stretch") {
        computedScale = {scaleX, scaleY};
    }
    else if (computedGraphicStyle.graphicAlign == "center") {
        computedScale = {1.0f, 1.0f};        
    }

    graphic.setScale(computedScale);
    graphic.setPosition({
        computedLayout.position.x + (computedLayout.size.x / 2.0f) - (graphic.getGlobalBounds().size.x / 2.0f),
        computedLayout.position.y + (computedLayout.size.y / 2.0f) - (graphic.getGlobalBounds().size.y / 2.0f)
    });
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Graphic::update(const SFUI::Vector2u renderTargetSize) {
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

    // Graphic Specific Computation //
    computeGraphicSource();
    computeGraphicAlign();
    computeGraphic();
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Graphic::handleEvent(const SFUI::Event& event) {}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Graphic::draw(SFUI::RenderTarget& renderTarget) {
    renderTarget.draw(backgroundRects);
    renderTarget.draw(backgroundArcs);
    renderTarget.draw(borderRects);
    renderTarget.draw(borderArcs);
    
    // Image Clipping for Fill and Center ALignments //
    if (computedGraphicStyle.graphicAlign == "fill" || computedGraphicStyle.graphicAlign == "center") {
        
        // View Method //
        sf::View previousView = renderTarget.getView();
        sf::View clipView(sf::FloatRect({0.f, 0.f}, {computedLayout.size.x, computedLayout.size.y}));
        float rtWidth = static_cast<float>(renderTarget.getSize().x);
        float rtHeight = static_cast<float>(renderTarget.getSize().y);
        clipView.setViewport(sf::FloatRect(
            {computedLayout.position.x / rtWidth, computedLayout.position.y / rtHeight},
            {computedLayout.size.x / rtWidth, computedLayout.size.y / rtHeight}
        ));
        renderTarget.setView(clipView);
        sf::Vector2f oldPosition = graphic.getPosition();
        graphic.setPosition({
            (computedLayout.size.x / 2.0f) - (graphic.getGlobalBounds().size.x / 2.0f),
            (computedLayout.size.y / 2.0f) - (graphic.getGlobalBounds().size.y / 2.0f)
        });
        renderTarget.draw(graphic);
        graphic.setPosition(oldPosition);
        renderTarget.setView(previousView);
    }   else {
        renderTarget.draw(graphic);
    }
}
