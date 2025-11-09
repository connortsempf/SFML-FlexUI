#include "Base/Component.hpp"




////////////////////////////////////////
// SFML-FlexUI Parent Component Class //
////////////////////////////////////////


/**
 * @brief Constructor for Component.
 * 
 * @param componentID The unique identifier for the component.
 */
SFUI::Component::Component(SFUI::String componentID) :
    componentID(std::move(componentID))
{}


/**
 * @brief Constructor for Component.
 * 
 * @param componentID The unique identifier for the component.
 * @param layout The layout properties for the component.
 * @param style The style properties for the component.
 * @param children The child components for the component.
 */
SFUI::Component::Component(SFUI::String componentID, SFUI::Prop::Layout::Component layout, SFUI::Prop::Style::Component style) :
    componentID(std::move(componentID)),
    layout(std::move(layout)),
    style(std::move(style))
{}


/**
 * @brief Set a new parent component for this component.
 * 
 * @param newParent The new parent component.
 */
SFUI::Void SFUI::Component::setParent(SFUI::Component* newParent) {
    parent = newParent;
}


/**
 * @brief Add a child component to this component.
 * 
 * @param newChild The new child component to add.
 */
SFUI::Void SFUI::Component::addChild(const SFUI::SharedPointer<SFUI::Component>& newChild) {
    newChild->setParent(this);
    children.push_back(newChild);
    computedChildrenLayout.emplace_back(SFUI::Component::ComputedChildLayout{
        {0.f, 0.f},     // size
        {0, 0},         // position
        0.f             // margin
    });
}


/**
 * @brief Add multiple child components to this component.
 * 
 * @param newChildren The new child components to add.
 */
SFUI::Void SFUI::Component::addChildren(const SFUI::Vector<SFUI::SharedPointer<SFUI::Component>>& newChildren) {
    children.insert(children.end(), newChildren.begin(), newChildren.end());
    for (int i = 0; i < newChildren.size(); i++) {
        newChildren[i]->setParent(this);
        computedChildrenLayout.emplace_back(SFUI::Component::ComputedChildLayout{
            {0.f, 0.f},     // size
            {0, 0},         // position
            0.f             // margin
        });
    }
}


/**
 * @brief Update this component from its parent's parent component's own layout computation.
 * 
 * @param childComputedLayout The computed layout information computed from the parent component.
 */
SFUI::Void SFUI::Component::updateChildFromParent(SFUI::Component::ComputedChildLayout computedChildLayout) {
    computedLayout.size = computedChildLayout.size;
    computedLayout.position = computedChildLayout.position;
    computedLayout.margin = computedChildLayout.margin;
}


/**
 * @brief Get the child components of this component.
 * 
 * @return The vector of child components.
 */
const SFUI::Vector<SFUI::SharedPointer<SFUI::Component>>& SFUI::Component::getChildren() const {
    return children;
}


/**
 * @brief Get the final computed size of this component.
 * 
 * @return The size of the component.
 */
const SFUI::Vector2f& SFUI::Component::getSize() const {
    return computedLayout.size;
}


/**
 * @brief Get the final computed position of this component.
 * 
 * @return The position of the component.
 */
const SFUI::Vector2i& SFUI::Component::getPosition() const {
    return computedLayout.position;
}


/**
 * @brief Get the final computed padding of this component.
 * 
 * @return The padding of the component.
 */
const SFUI::Vector4f& SFUI::Component::getPadding() const {
    return computedLayout.padding;
}


/**
 * @brief Get the final computed margin of this component.
 * 
 * @return The margin of the component.
 */
const SFUI::Vector4f& SFUI::Component::getMargin() const {
    return computedLayout.margin;
}


/**
 * @brief Check if the mouse is hovering over this component.
 * 
 * @param mousePosition The current mouse position.
 * 
 * @return True if the mouse is hovering over the component, false otherwise.
 */
SFUI::Bool SFUI::Component::isMouseHovered(const SFUI::Vector2i& mousePosition) {
    SFUI::Vector2f mousePos(mousePosition.x, mousePosition.y);
    SFUI::FloatRect cornerBounds = borderRects.getBounds();
    SFUI::FloatRect edgeBounds = backgroundRects.getBounds();
    return (cornerBounds.contains(mousePos) || edgeBounds.contains(mousePos));
}


/**
 * @brief Resolve a Color Sub Property into an actual SFML Color.
 * 
 * @param color The color sub property to resolve.
 * 
 * @return The resolved SFML Color.
 */
SFUI::Color SFUI::Component::resolveColorSubProp(const SFUI::SubProp::Color& color) {
    SFUI::Color resolvedFillColor;

    // If Input Fill Color is a 3-Channel Unsigned Byte //
    if (std::holds_alternative<SFUI::Vector3ui8>(color)) {
        SFUI::Vector3ui8 fillColor = std::get<SFUI::Vector3ui8>(color);
        resolvedFillColor = SFUI::Color(fillColor.x, fillColor.y, fillColor.z, 255);
    }
    // If Input Fill Color is a 4-Channel Unsigned Byte //
    else if (std::holds_alternative<SFUI::Vector4ui8>(color)) {
        SFUI::Vector4ui8 fillColor = std::get<SFUI::Vector4ui8>(color);
        resolvedFillColor = SFUI::Color(fillColor.x, fillColor.y, fillColor.z, fillColor.w);
    }
    // If Input Fill Color is a Hex String //
    else if (std::holds_alternative<SFUI::String>(color)) {
        SFUI::String fillColorHexString = std::get<SFUI::String>(color);
        
        if (!fillColorHexString.empty() && fillColorHexString[0] == '#') {
            fillColorHexString.erase(0, 1);
            try {
                // RRGGBB Oct-String //
                if (fillColorHexString.length() == 6) {
                    unsigned int r = std::stoul(fillColorHexString.substr(0, 2), nullptr, 16);
                    unsigned int g = std::stoul(fillColorHexString.substr(2, 2), nullptr, 16);
                    unsigned int b = std::stoul(fillColorHexString.substr(4, 2), nullptr, 16);
                    resolvedFillColor = SFUI::Color(r, g, b, 255);
                }
                // RRGGBBAA Hex-String //
                else if (fillColorHexString.length() == 8) {
                    unsigned int r = std::stoul(fillColorHexString.substr(0, 2), nullptr, 16);
                    unsigned int g = std::stoul(fillColorHexString.substr(2, 2), nullptr, 16);
                    unsigned int b = std::stoul(fillColorHexString.substr(4, 2), nullptr, 16);
                    unsigned int a = std::stoul(fillColorHexString.substr(6, 2), nullptr, 16);
                    resolvedFillColor = SFUI::Color(r, g, b, a);
                }
            }   catch (...) {
                resolvedFillColor = SFUI::Color(0, 0, 0, 255);
            }
        }
    }
    // If Input Fill Color is Already Given as a SFML::Color Type //
    else if (std::holds_alternative<SFUI::Color>(color)) {
        resolvedFillColor = std::get<SFUI::Color>(color);
    }

    return resolvedFillColor;
}


/**
 * @brief Resolve a UniQuad Sub Property into a Vector4f.
 * 
 * @param size The size of the component for percentage calculations.
 * @param subProp The UniQuad sub property to resolve.
 * 
 * @return The resolved sub-prop as a Vector4f.
 */
SFUI::Vector4f SFUI::Component::resolveUniQuadSubProp(const SFUI::Vector2f& size, const SFUI::SubProp::UniQuad& subProp) {
    SFUI::Vector4f computedSubProp = {0.0f, 0.0f, 0.0f, 0.0f};
    SFUI::Float subPropSizeFactor = std::min(size.x, size.y);

    // If the Sub Prop Holds the Master Value //
    if (std::holds_alternative<SFUI::SubProp::Dimension>(subProp)) {
        SFUI::SubProp::Dimension masterSubProp = std::get<SFUI::SubProp::Dimension>(subProp);
        SFUI::Float computedMasterSubProp = 0.0f;
        if (std::holds_alternative<SFUI::Float>(masterSubProp))
            computedMasterSubProp = std::get<SFUI::Float>(masterSubProp);
        else if (std::holds_alternative<SFUI::String>(masterSubProp)) {
            SFUI::String masterSubPropString = std::get<SFUI::String>(masterSubProp);
            if (masterSubPropString.size() > 1 && masterSubPropString.back() == '%') {
                masterSubPropString.pop_back();
                try {
                    size_t index = 0;
                    SFUI::Double tempSubProp = std::stod(masterSubPropString, &index);
                    if (index == masterSubPropString.size())
                        computedMasterSubProp = subPropSizeFactor * std::clamp(static_cast<SFUI::Float>(tempSubProp) / 100.0f, 0.0f, 0.5f);
                }   catch (...) {}
            }
        }
        computedSubProp.x = computedSubProp.y = computedSubProp.z = computedSubProp.w = computedMasterSubProp;
    }

    // If the Sub Prop Holds the Invidiual Inputs Value //
    else if (std::holds_alternative<SFUI::SubProp::Vector4dim>(subProp)) {
        SFUI::SubProp::Vector4dim subProps = std::get<SFUI::SubProp::Vector4dim>(subProp);
        SFUI::Array<SFUI::SubProp::Dimension, 4> individualSubProps = {subProps.x, subProps.y, subProps.z, subProps.w};
        SFUI::Array<SFUI::Float, 4> computedIndividualSubProps = {0.0f, 0.0f, 0.0f, 0.0f};
        for (int i = 0; i < 4; i++) {
            if (std::holds_alternative<SFUI::Float>(individualSubProps[i]))
                computedIndividualSubProps[i] = std::get<SFUI::Float>(individualSubProps[i]);
            else if (std::holds_alternative<SFUI::String>(individualSubProps[i])) {
                SFUI::String individualSubPropString = std::get<SFUI::String>(individualSubProps[i]);
                if (individualSubPropString.size() > 1 && individualSubPropString.back() == '%') {
                    individualSubPropString.pop_back();
                    try {
                        size_t index = 0;
                        SFUI::Double tempSubProp = std::stod(individualSubPropString, &index);
                        if (index == individualSubPropString.size())
                            computedIndividualSubProps[i] = subPropSizeFactor * std::clamp(static_cast<SFUI::Float>(tempSubProp) / 100.0f, 0.0f, 0.5f);
                    }   catch (...) {}
                }
            };
        }
        computedSubProp.x = computedIndividualSubProps[0];
        computedSubProp.y = computedIndividualSubProps[1];
        computedSubProp.z = computedIndividualSubProps[2];
        computedSubProp.w = computedIndividualSubProps[3];
    }

    return computedSubProp;
}


/**
 * @brief Compute the final alignment properties for this component.
 */
SFUI::Void SFUI::Component::computeAlignment() {
    // Children Layout Axis //
    SFUI::String& alignDirection = layout.alignDirection;
    std::transform(alignDirection.begin(), alignDirection.end(), alignDirection.begin(), ::tolower);
    if (alignDirection == "horizontal") computedLayout.alignDirection = SFUI::Component::AlignDirection::Horizontal;
    else computedLayout.alignDirection = SFUI::Component::AlignDirection::Vertical;
    
    // Primary (On-Axis) Children Alignment //
    SFUI::String& alignPrimary = layout.alignPrimary;
    std::transform(alignPrimary.begin(), alignPrimary.end(), alignPrimary.begin(), ::tolower);
    if (alignPrimary == "end") computedLayout.alignPrimary = SFUI::Component::AlignPrimary::End;
    else if (alignPrimary == "center") computedLayout.alignPrimary = SFUI::Component::AlignPrimary::Center;
    else if (alignPrimary == "space-between") computedLayout.alignPrimary = SFUI::Component::AlignPrimary::SpaceBetween;
    else if (alignPrimary == "space-around") computedLayout.alignPrimary = SFUI::Component::AlignPrimary::SpaceAround;
    else if (alignPrimary == "space-evenly") computedLayout.alignPrimary = SFUI::Component::AlignPrimary::SpaceEvenly;
    else computedLayout.alignPrimary = SFUI::Component::AlignPrimary::Start;

    // Secondary (Off-Axis) Children Alignment //
    SFUI::String& alignSecondary = layout.alignSecondary;
    std::transform(alignSecondary.begin(), alignSecondary.end(), alignSecondary.begin(), ::tolower);
    if (alignSecondary == "end") computedLayout.alignSecondary = SFUI::Component::AlignSecondary::End;
    else if (alignSecondary == "center") computedLayout.alignSecondary = SFUI::Component::AlignSecondary::Center;
    else computedLayout.alignSecondary = SFUI::Component::AlignSecondary::Start;
}


/**
 * @brief Compute the margin, size, position, and padding of this component.
 */
SFUI::Void SFUI::Component::computeLayoutBox() {
    // If Root Component (No Parent), Calculate Your Own Layout Box Using RenderTarget Dimensions as Parent Bounds //
    if (!parent) {

        // Layout Box Margin //
        computedLayout.margin = resolveUniQuadSubProp(
            SFUI::Vector2f{static_cast<SFUI::Float>(renderTargetSize.x), static_cast<SFUI::Float>(renderTargetSize.y)},
            layout.padding
        );

        // Layout Box Size //
        computedLayout.size.x = 0.0f;
        computedLayout.size.y = 0.0f;
        if (std::holds_alternative<SFUI::Float>(layout.width))
            computedLayout.size.x = std::get<SFUI::Float>(layout.width);
        else if (std::holds_alternative<SFUI::String>(layout.width)) {
            SFUI::String widthString = std::get<SFUI::String>(layout.width);
            if (widthString.size() > 1 && widthString.back() == '%') {
                widthString.pop_back();
                try {
                    SFUI::Size index = 0;
                    SFUI::Double tempWidth = std::stod(widthString, &index);
                    if (index == widthString.size())
                        computedLayout.size.x = renderTargetSize.x * std::clamp(static_cast<SFUI::Float>(tempWidth) / 100.0f, 0.0f, 1.0f);
                }   catch (...) {}
            }
        }
        if (std::holds_alternative<SFUI::Float>(layout.height))
            computedLayout.size.y = std::get<SFUI::Float>(layout.height);
        else if (std::holds_alternative<SFUI::String>(layout.height)) {
            SFUI::String heightString = std::get<SFUI::String>(layout.height);
            if (heightString.size() > 1 && heightString.back() == '%') {
                heightString.pop_back();
                try {
                    SFUI::Size index = 0;
                    SFUI::Double tempHeight = std::stod(heightString, &index);
                    if (index == heightString.size())
                        computedLayout.size.y = renderTargetSize.y * std::clamp(static_cast<SFUI::Float>(tempHeight) / 100.0f, 0.0f, 1.0f);
                }   catch (...) {}
            }
        }
        computedLayout.size.x -= (computedLayout.margin.x + computedLayout.margin.y);
        computedLayout.size.y -= (computedLayout.margin.z + computedLayout.margin.w);

        // Layout Box Position //
        computedLayout.position.x = 0.0f;
        computedLayout.position.y = 0.0f;
        if (layout.xPosition.has_value()) computedLayout.position.x = layout.xPosition.value();
        else computedLayout.position.x = (renderTargetSize.x / 2.0f) - (computedLayout.size.x / 2.0f);
        if (layout.yPosition.has_value()) computedLayout.position.y = layout.yPosition.value();
        else computedLayout.position.y = (renderTargetSize.y / 2.0f) - (computedLayout.size.y / 2.0f);
    }

    // Layout Box Padding //
    computedLayout.padding = resolveUniQuadSubProp(computedLayout.size, layout.padding);
}


/**
 * @brief Compute the style properties for this component.
 */
SFUI::Void SFUI::Component::computeStyles() {
    // Border Width //
    SFUI::Float computedBorderWidth = 0.0f;
    if (std::holds_alternative<SFUI::Float>(style.borderWidth))
        computedBorderWidth = std::get<SFUI::Float>(style.borderWidth);
    else if (std::holds_alternative<SFUI::String>(style.borderWidth)) {
        SFUI::String borderWidthString = std::get<SFUI::String>(style.borderWidth);
        if (borderWidthString.size() > 1 && borderWidthString.back() == '%') {
            borderWidthString.pop_back();
            try {
                size_t index = 0;
                SFUI::Double tempBorderWidth = std::stod(borderWidthString, &index);
                if (index == borderWidthString.size()) {
                    SFUI::Float relativeBorderWidthFactor = std::min(computedLayout.size.x, computedLayout.size.y);
                    computedBorderWidth = relativeBorderWidthFactor * std::clamp(static_cast<SFUI::Float>(tempBorderWidth) / 100.0f, 0.0f, 0.5f);
                }
            }   catch (...) {}
        }
    }
    computedStyle.borderWidth = computedBorderWidth;

    // Corner Radius //
    computedStyle.cornerRadius = resolveUniQuadSubProp(computedLayout.size, style.cornerRadius);
}


/**
 * @brief Compute the color properties for this component.
 */
SFUI::Void SFUI::Component::computeColors() {
    computedStyle.fillColor = resolveColorSubProp(style.fillColor);
    computedStyle.borderColor = resolveColorSubProp(style.borderColor);
}


/**
 * @brief Compute the shadow properties for this component.
 */
SFUI::Void SFUI::Component::computeShadows() {
    computedStyle.shadowOffset = style.shadowOffset;
    computedStyle.shadowRadius = std::clamp(style.shadowRadius, 1.0f, 20.0f);
    computedStyle.shadowFillColor = resolveColorSubProp(style.shadowFillColor);
}


/**
 * @brief Compute the graphical geometry for this component.
 */
SFUI::Void SFUI::Component::computeGraphics() {
    backgroundRects.clear();
    backgroundArcs.clear();
    borderRects.clear();
    borderArcs.clear();
    shadowRects.clear();
    shadowArcs.clear();

    backgroundRects.setPrimitiveType(SFUI::PrimitiveType::Triangles);
    backgroundArcs.setPrimitiveType(SFUI::PrimitiveType::Triangles);
    borderRects.setPrimitiveType(SFUI::PrimitiveType::Triangles);
    borderArcs.setPrimitiveType(SFUI::PrimitiveType::Triangles);
    shadowRects.setPrimitiveType(SFUI::PrimitiveType::Triangles);
    shadowArcs.setPrimitiveType(SFUI::PrimitiveType::Triangles);

    SFUI::Float width = computedLayout.size.x;
    SFUI::Float height = computedLayout.size.y;
    SFUI::Float left = computedLayout.position.x;
    SFUI::Float right = computedLayout.position.x + computedLayout.size.x;
    SFUI::Float top = computedLayout.position.y;
    SFUI::Float bottom = computedLayout.position.y + computedLayout.size.y;
    SFUI::Float topLeft = computedStyle.cornerRadius.x;
    SFUI::Float topRight = computedStyle.cornerRadius.y;
    SFUI::Float bottomLeft = computedStyle.cornerRadius.z;
    SFUI::Float bottomRight = computedStyle.cornerRadius.w;
    SFUI::Float leftInset = topLeft > bottomLeft ? topLeft : bottomLeft;
    SFUI::Float rightInset = topRight > bottomRight ? topRight : bottomRight;
    SFUI::Float topInset = topLeft > topRight ? topLeft : topRight;
    SFUI::Float bottomInset = bottomLeft > bottomRight ? bottomLeft : bottomRight;
    SFUI::Float borderWidth = computedStyle.borderWidth;
    SFUI::Float shadowXOffset = computedStyle.shadowOffset.x;
    SFUI::Float shadowYOffset = computedStyle.shadowOffset.y;
    SFUI::Float shadowRadius = computedStyle.shadowRadius;

    computeBackgroundRectGeometry({left + leftInset, top + topInset}, {width - leftInset - rightInset, height - topInset - bottomInset});
    computeBackgroundRectGeometry({left, top + topLeft}, {leftInset, height - topLeft - bottomLeft});
    computeBackgroundRectGeometry({right - rightInset, top + topRight}, {rightInset, height - topRight - bottomRight});
    computeBackgroundRectGeometry({left + topLeft, top}, {width - topLeft - topRight, topInset});
    computeBackgroundRectGeometry({left + bottomLeft, bottom - bottomInset}, {width - bottomLeft - bottomRight, bottomInset});

    if (topLeft > 0.0f) computeBackgroundArcGeometry({left + topLeft, top + topLeft}, topLeft, 180, 270);
    if (topRight > 0.0f) computeBackgroundArcGeometry({right - topRight, top + topRight}, topRight, 270, 360);
    if (bottomRight > 0.0f) computeBackgroundArcGeometry({right - bottomRight, bottom - bottomRight}, bottomRight, 0, 90);
    if (bottomLeft > 0.0f) computeBackgroundArcGeometry({left + bottomLeft, bottom - bottomLeft}, bottomLeft, 90, 180);

    if (borderWidth > 0.0f)  {
        computeBorderRectGeometry({left, top + topLeft}, {borderWidth, height - topLeft - bottomLeft});
        computeBorderRectGeometry({right - borderWidth, top + topRight}, {borderWidth, height - topRight - bottomRight});
        computeBorderRectGeometry({left + topLeft, top}, {width - topLeft - topRight, borderWidth});
        computeBorderRectGeometry({left + bottomLeft, bottom - borderWidth}, {width - bottomLeft - bottomRight, borderWidth});
        
        if (topLeft > 0.0f) computeBorderArcGeometry({left + topLeft, top + topLeft}, topLeft, topLeft >= borderWidth ? topLeft - borderWidth : 0.0f, 180, 270);
        if (topRight > 0.0f) computeBorderArcGeometry({right - topRight, top + topRight}, topRight, topRight >= borderWidth ? topRight - borderWidth : 0.0f, 270, 360);
        if (bottomRight > 0.0f) computeBorderArcGeometry({right - bottomRight, bottom - bottomRight}, bottomRight, bottomRight >= borderWidth ? bottomRight - borderWidth : 0.0f, 0, 90);
        if (bottomLeft > 0.0f) computeBorderArcGeometry({left + bottomLeft, bottom - bottomLeft}, bottomLeft, bottomLeft >= borderWidth ? bottomLeft - borderWidth : 0.0f, 90, 180);
    }

    if (static_cast<int>(computedStyle.shadowFillColor.a) > 0) {
        SFUI::Float shadowLayers = shadowRadius;
        SFUI::Vector2f startingSize = computedLayout.size;
        SFUI::Vector2f startingPos = {computedLayout.position.x + shadowXOffset, computedLayout.position.y + shadowYOffset};
        SFUI::Vector4f startingRadii = computedStyle.cornerRadius;
        // SFUI::Float perLayerSizeInset = std::clamp(shadowLayers / 2.5f, 2.0f, 10.0f);
        SFUI::Float perLayerSizeInset = 4.0f;
        SFUI::Float perLayerPosInset = perLayerSizeInset / 2;

        for (int i = 0; i < shadowLayers; i++) {
            SFUI::Color layerColor = computedStyle.shadowFillColor;
            SFUI::Float alphaFactor = static_cast<float>(i) / (shadowLayers - 1);
            SFUI::Float layerAlpha = computedStyle.shadowFillColor.a * (1.0f - alphaFactor * alphaFactor);
            layerColor.a = static_cast<std::uint8_t>(layerAlpha);

            SFUI::Float width = startingSize.x - (perLayerSizeInset * i);
            SFUI::Float height = startingSize.y - (perLayerSizeInset * i);
            SFUI::Float left = startingPos.x + (perLayerPosInset * i);
            SFUI::Float right = startingPos.x + (perLayerPosInset * i) + width;
            SFUI::Float top = startingPos.y + (perLayerPosInset * i);
            SFUI::Float bottom = startingPos.y + (perLayerPosInset * i) + height;
            SFUI::Float topLeft = std::max(0.0f, startingRadii.x - (perLayerSizeInset * i));
            SFUI::Float topRight = std::max(0.0f, startingRadii.y - (perLayerSizeInset * i));
            SFUI::Float bottomLeft = std::max(0.0f, startingRadii.z - (perLayerSizeInset * i));
            SFUI::Float bottomRight = std::max(0.0f, startingRadii.w - (perLayerSizeInset * i));
            SFUI::Float leftInset = topLeft > bottomLeft ? topLeft : bottomLeft;
            SFUI::Float rightInset = topRight > bottomRight ? topRight : bottomRight;
            SFUI::Float topInset = topLeft > topRight ? topLeft : topRight;
            SFUI::Float bottomInset = bottomLeft > bottomRight ? bottomLeft : bottomRight;

            computeShadowRectGeometry({left + leftInset, top + topInset}, {width - leftInset - rightInset, height - topInset - bottomInset}, layerColor);
            computeShadowRectGeometry({left, top + topLeft}, {leftInset, height - topLeft - bottomLeft}, layerColor);
            computeShadowRectGeometry({right - rightInset, top + topRight}, {rightInset, height - topRight - bottomRight}, layerColor);
            computeShadowRectGeometry({left + topLeft, top}, {width - topLeft - topRight, topInset}, layerColor);
            computeShadowRectGeometry({left + bottomLeft, bottom - bottomInset}, {width - bottomLeft - bottomRight, bottomInset}, layerColor);
        
            if (topLeft > 0.0f) computeShadowArcGeometry({left + topLeft, top + topLeft}, topLeft, 180, 270, layerColor);
            if (topRight > 0.0f) computeShadowArcGeometry({right - topRight, top + topRight}, topRight, 270, 360, layerColor);
            if (bottomRight > 0.0f) computeShadowArcGeometry({right - bottomRight, bottom - bottomRight}, bottomRight, 0, 90, layerColor);
            if (bottomLeft > 0.0f) computeShadowArcGeometry({left + bottomLeft, bottom - bottomLeft}, bottomLeft, 90, 180, layerColor);
        }
    }
}


/**
 * @brief Compute the magrgin, size, and position for all child components.
 */
SFUI::Void SFUI::Component::computeChildrenLayoutBox() {
    // If Has Child Components, Must Calculate Their Margins for Them Based on Its Own Size //
    if (children.size() > 0) {

        // Children Layout Box Margins //
        for (int i = 0; i < children.size(); i++) {
            SFUI::Component& childComponent = *children[i];
            computedChildrenLayout[i].margin = resolveUniQuadSubProp(computedLayout.size, childComponent.layout.margin);
        }


        // Children Layout Box Size //
        for (int i = 0; i < children.size(); i++) {
            SFUI::Vector2f computedSize = {0.0f, 0.0f};
            SFUI::Component& childComponent = *children[i];
            sf::Vector2i availableSize(
                computedLayout.size.x - (computedLayout.padding.x + computedLayout.padding.y),
                computedLayout.size.y - (computedLayout.padding.z + computedLayout.padding.w)
            );
            // Width //
            if (std::holds_alternative<SFUI::Float>(childComponent.layout.width))
                computedSize.x = std::get<SFUI::Float>(childComponent.layout.width);
            else if (std::holds_alternative<SFUI::String>(childComponent.layout.width)) {
                SFUI::String widthString = std::get<SFUI::String>(childComponent.layout.width);
                if (widthString.size() > 1 && widthString.back() == '%') {
                    widthString.pop_back();
                    try {
                        size_t index = 0;
                        SFUI::Double tempWidth = std::stod(widthString, &index);
                        if (index == widthString.size())
                            computedSize.x = availableSize.x * std::clamp(static_cast<SFUI::Float>(tempWidth) / 100.0f, 0.0f, 1.0f);
                    }   catch (...) {}
                }
            }
            // Height //
            if (std::holds_alternative<SFUI::Float>(childComponent.layout.height))
                computedSize.y = std::get<SFUI::Float>(childComponent.layout.height);
            else if (std::holds_alternative<SFUI::String>(childComponent.layout.height)) {
                SFUI::String heightString = std::get<SFUI::String>(childComponent.layout.height);
                if (heightString.size() > 1 && heightString.back() == '%') {
                    heightString.pop_back();
                    try {
                        size_t index = 0;
                        SFUI::Double tempHeight = std::stod(heightString, &index);
                        if (index == heightString.size())
                            computedSize.y = availableSize.y * (static_cast<SFUI::Float>(tempHeight) / 100.0f);
                    }   catch (...) {}
                }
            }
            computedChildrenLayout[i].size = {computedSize.x, computedSize.y};
        }


        // Children Layout Box Position //
        // Calculate Each Component's Sizing and Margins and Total Sizing with Margins Combined //
        SFUI::Vector<SFUI::Float> componentSizeAndMargins;
        SFUI::Float totalComponentSizingAndMargins = 0.0f;
        SFUI::Float availableGapSize = 0.0f;
        SFUI::Float gapSize = 0.0f;
        SFUI::Float interiorGapSize = 0.0f;
        SFUI::Float edgeGapSize = 0.0f;
        for (int i = 0; i < children.size(); i++) {
            SFUI::Float childTotalSize = 0.0f;
            if (computedLayout.alignDirection == SFUI::Component::AlignDirection::Vertical)
                childTotalSize = computedChildrenLayout[i].size.y + (computedChildrenLayout[i].margin.z + computedChildrenLayout[i].margin.w);
            else if (computedLayout.alignDirection == SFUI::Component::AlignDirection::Horizontal)
                childTotalSize = computedChildrenLayout[i].size.x + (computedChildrenLayout[i].margin.x + computedChildrenLayout[i].margin.y);
            componentSizeAndMargins.push_back(childTotalSize);
            totalComponentSizingAndMargins += childTotalSize;
        }

        // Compute Total Gap Size Available Between Children //
        if (computedLayout.alignDirection == SFUI::Component::AlignDirection::Vertical)
            availableGapSize = computedLayout.size.y - (computedLayout.padding.z + computedLayout.padding.w) - totalComponentSizingAndMargins;
        else if (computedLayout.alignDirection == SFUI::Component::AlignDirection::Horizontal)
            availableGapSize = computedLayout.size.x - (computedLayout.padding.x + computedLayout.padding.y) - totalComponentSizingAndMargins;

        // Compute the Gap Sizes Between Children //
        if (computedLayout.alignPrimary== SFUI::Component::AlignPrimary::SpaceBetween)
            gapSize = availableGapSize / (children.size() - 1);
        else if (computedLayout.alignPrimary== SFUI::Component::AlignPrimary::SpaceAround) {
            edgeGapSize = availableGapSize / (2.0f * children.size());
            interiorGapSize = availableGapSize / children.size();
        }
        else if (computedLayout.alignPrimary== SFUI::Component::AlignPrimary::SpaceEvenly)
            gapSize = availableGapSize / (children.size() + 1);

        // Compute Children Positions Iteratively //
        for (int i = 0; i < children.size(); i++) {
            SFUI::Vector2i computedPosition = {0, 0};
            SFUI::Component& childComponent = *children[i];
            // X-Positions //
            if (childComponent.layout.xPosition.has_value()) {
                SFUI::Int xPositionValue = childComponent.layout.xPosition.value();
                computedPosition.x = xPositionValue;
            }   else {
                // Off-Axis Positioning //
                if (computedLayout.alignDirection == SFUI::Component::AlignDirection::Vertical) {
                    if (computedLayout.alignSecondary == SFUI::Component::AlignSecondary::Start)
                        computedPosition.x = computedLayout.position.x + computedLayout.padding.x + computedChildrenLayout[i].margin.x;
                    else if (computedLayout.alignSecondary == SFUI::Component::AlignSecondary::End)
                        computedPosition.x = computedLayout.position.x + computedLayout.size.x - computedLayout.padding.y - computedChildrenLayout[i].margin.y - computedChildrenLayout[i].size.x;
                    else if (computedLayout.alignSecondary == SFUI::Component::AlignSecondary::Center)
                        computedPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedChildrenLayout[i].size.x / 2.0f);
                }
                // On-Axis Positioning //
                else if (computedLayout.alignDirection == SFUI::Component::AlignDirection::Horizontal) {
                    if (computedLayout.alignPrimary == SFUI::Component::AlignPrimary::Start) {
                        if (i == 0) computedPosition.x = computedLayout.position.x + computedLayout.padding.x + computedChildrenLayout[i].margin.x;
                        else computedPosition.x = computedChildrenLayout[i - 1].position.x + computedChildrenLayout[i - 1].size.x + computedChildrenLayout[i - 1].margin.y + computedChildrenLayout[i].margin.x;
                    }
                    else if (computedLayout.alignPrimary == SFUI::Component::AlignPrimary::End) {
                        if (i == 0) computedPosition.x = computedLayout.position.x + computedLayout.size.x - computedLayout.padding.y - totalComponentSizingAndMargins + computedChildrenLayout[i].margin.x;
                        else computedPosition.x = computedChildrenLayout[i - 1].position.x + computedChildrenLayout[i - 1].size.x + computedChildrenLayout[i - 1].margin.y + computedChildrenLayout[i].margin.x;
                    }
                    else if (computedLayout.alignPrimary == SFUI::Component::AlignPrimary::Center) {
                        if (i == 0) computedPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (totalComponentSizingAndMargins / 2.0f) + computedChildrenLayout[i].margin.x;
                        else computedPosition.x = computedChildrenLayout[i - 1].position.x + computedChildrenLayout[i - 1].size.x + computedChildrenLayout[i - 1].margin.y + computedChildrenLayout[i].margin.x;
                    }
                    else if (computedLayout.alignPrimary== SFUI::Component::AlignPrimary::SpaceBetween) {
                        if (children.size() == 1) computedPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedChildrenLayout[i].size.x / 2.0f);
                        else if (i == 0) computedPosition.x = computedLayout.position.x + computedLayout.padding.x + computedChildrenLayout[i].margin.x;
                        else if (i == children.size() - 1) computedPosition.x = computedLayout.position.x + computedLayout.size.x - computedLayout.padding.y - computedChildrenLayout[i].margin.y - computedChildrenLayout[i].size.x;
                        else computedPosition.x = computedChildrenLayout[i - 1].position.x + computedChildrenLayout[i - 1].size.x + computedChildrenLayout[i - 1].margin.y + gapSize + computedChildrenLayout[i].margin.x;
                    }
                    else if (computedLayout.alignPrimary== SFUI::Component::AlignPrimary::SpaceAround) {
                        if (children.size() == 1) computedPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedChildrenLayout[i].size.x / 2.0f);
                        else if (i == 0) computedPosition.x = computedLayout.position.x + computedLayout.padding.x + edgeGapSize + computedChildrenLayout[i].margin.x;
                        else if (i == children.size() - 1) computedPosition.x = computedLayout.position.x + computedLayout.size.x - computedLayout.padding.y - computedChildrenLayout[i].margin.y - computedChildrenLayout[i].size.x - edgeGapSize;
                        else computedPosition.x = computedChildrenLayout[i - 1].position.x + computedChildrenLayout[i - 1].size.x + computedChildrenLayout[i - 1].margin.y + interiorGapSize + computedChildrenLayout[i].margin.x;
                    }
                    else if (computedLayout.alignPrimary== SFUI::Component::AlignPrimary::SpaceEvenly) {
                        if (children.size() == 1) computedPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedChildrenLayout[i].size.x / 2.0f);
                        else if (i == 0) computedPosition.x = computedLayout.position.x + computedLayout.padding.x + gapSize + computedChildrenLayout[i].margin.x;
                        else computedPosition.x = computedChildrenLayout[i - 1].position.x + computedChildrenLayout[i - 1].size.x + computedChildrenLayout[i - 1].margin.y + gapSize + computedChildrenLayout[i].margin.x;
                    }
                }
            }

            // Y-Positions //
            if (childComponent.layout.yPosition.has_value()) {
                SFUI::Int yPositionValue = childComponent.layout.yPosition.value();
                computedPosition.y = yPositionValue;
            }   else {
                // Off-Axis Positioning //
                if (computedLayout.alignDirection == SFUI::Component::AlignDirection::Horizontal) {
                    if (computedLayout.alignSecondary == SFUI::Component::AlignSecondary::Start)
                        computedPosition.y = computedLayout.position.y + computedLayout.padding.z;
                    else if (computedLayout.alignSecondary == SFUI::Component::AlignSecondary::End)
                        computedPosition.y = computedLayout.position.y + computedLayout.size.y - computedLayout.padding.w - computedChildrenLayout[i].size.y;
                    else if (computedLayout.alignSecondary == SFUI::Component::AlignSecondary::Center)
                        computedPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedChildrenLayout[i].size.y / 2.0f);
                }
                // On-Axis Positioning //
                else if (computedLayout.alignDirection == SFUI::Component::AlignDirection::Vertical) {
                    if (computedLayout.alignPrimary == SFUI::Component::AlignPrimary::Start) {
                        if (i == 0) computedPosition.y = computedLayout.position.y + computedLayout.padding.z + computedChildrenLayout[i].margin.z;
                        else computedPosition.y = computedChildrenLayout[i - 1].position.y + computedChildrenLayout[i - 1].size.y + computedChildrenLayout[i - 1].margin.w + computedChildrenLayout[i].margin.z;
                    }
                    else if (computedLayout.alignPrimary == SFUI::Component::AlignPrimary::End) {
                        if (i == 0) computedPosition.y = computedLayout.position.y + computedLayout.size.y - computedLayout.padding.w - totalComponentSizingAndMargins + computedChildrenLayout[i].margin.z;
                        else computedPosition.y = computedChildrenLayout[i - 1].position.y + computedChildrenLayout[i - 1].size.y + computedChildrenLayout[i - 1].margin.w + computedChildrenLayout[i].margin.z;
                    }
                    else if (computedLayout.alignPrimary == SFUI::Component::AlignPrimary::Center) {
                        if (i == 0) computedPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (totalComponentSizingAndMargins / 2.0f) + computedChildrenLayout[i].margin.z;
                        else computedPosition.y = computedChildrenLayout[i - 1].position.y + computedChildrenLayout[i - 1].size.y + computedChildrenLayout[i - 1].margin.w + computedChildrenLayout[i].margin.z;
                    }
                    else if (computedLayout.alignPrimary== SFUI::Component::AlignPrimary::SpaceBetween) {
                        if (children.size() == 1) computedPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedChildrenLayout[i].size.y / 2.0f);
                        else if (i == 0) computedPosition.y = computedLayout.position.y + computedLayout.padding.z + computedChildrenLayout[i].margin.z;
                        else if (i == children.size() - 1) computedPosition.y = computedLayout.position.y + computedLayout.size.y - computedLayout.padding.w - computedChildrenLayout[i].margin.w - computedChildrenLayout[i].size.y;
                        else computedPosition.y = computedChildrenLayout[i - 1].position.y + computedChildrenLayout[i - 1].size.y + computedChildrenLayout[i - 1].margin.w + gapSize + computedChildrenLayout[i].margin.z;
                    }
                    else if (computedLayout.alignPrimary== SFUI::Component::AlignPrimary::SpaceAround) {
                        if (children.size() == 1) computedPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedChildrenLayout[i].size.y / 2.0f);
                        else if (i == 0) computedPosition.y = computedLayout.position.y + computedLayout.padding.z + edgeGapSize + computedChildrenLayout[i].margin.z;
                        else if (i == children.size() - 1) computedPosition.y = computedLayout.position.y + computedLayout.size.y - computedLayout.padding.w - computedChildrenLayout[i].margin.w - computedChildrenLayout[i].size.y - edgeGapSize;
                        else computedPosition.y = computedChildrenLayout[i - 1].position.y + computedChildrenLayout[i - 1].size.y + computedChildrenLayout[i - 1].margin.w + interiorGapSize + computedChildrenLayout[i].margin.z;
                    }
                    else if (computedLayout.alignPrimary== SFUI::Component::AlignPrimary::SpaceEvenly) {
                        if (children.size() == 1) computedPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedChildrenLayout[i].size.y / 2.0f);
                        else if (i == 0) computedPosition.y = computedLayout.position.y + computedLayout.padding.z + gapSize + computedChildrenLayout[i].margin.z;
                        else computedPosition.y = computedChildrenLayout[i - 1].position.y + computedChildrenLayout[i - 1].size.y + computedChildrenLayout[i - 1].margin.w + gapSize + computedChildrenLayout[i].margin.z;
                    }
                }
            }            
            computedChildrenLayout[i].position = computedPosition;
        }
    }
}


/**
 * @brief Update all child components with their computed layout box properties.
 */
SFUI::Void SFUI::Component::updateChildren() {
    for (int i = 0; i < children.size(); i++) {
        children[i]->updateChildFromParent(computedChildrenLayout[i]);
    }
}


/**
 * @brief Compute a box (two triangles).
 * 
 * @param position The top-left position of the box to draw at.
 * @param size The size of the box to draw.
 */
SFUI::Void SFUI::Component::computeBackgroundRectGeometry(SFUI::Vector2f position, SFUI::Vector2f size) {
    SFUI::Vector2f topLeft = position;
    SFUI::Vector2f topRight = {position.x + size.x, position.y};
    SFUI::Vector2f bottomLeft = {position.x, position.y + size.y};
    SFUI::Vector2f bottomRight = {position.x + size.x, position.y + size.y};
    backgroundRects.append({topLeft, computedStyle.fillColor});
    backgroundRects.append({topRight, computedStyle.fillColor});
    backgroundRects.append({bottomRight, computedStyle.fillColor});
    backgroundRects.append({topLeft, computedStyle.fillColor});
    backgroundRects.append({bottomRight, computedStyle.fillColor});
    backgroundRects.append({bottomLeft, computedStyle.fillColor});
}


/**
 * @brief Compute a circular arc (triangle fan) .
 * 
 * @param center The center position of the arc.
 * @param outerRadius The outer radius of the arc.
 * @param startAngleDeg The starting angle of the arc in degrees.
 * @param endAngleDeg The ending angle of the arc in degrees.
 */
SFUI::Void SFUI::Component::computeBackgroundArcGeometry(SFUI::Vector2f center, SFUI::Float outerRadius, SFUI::Float startAngleDeg, SFUI::Float endAngleDeg) {
    SFUI::Float arcResolution = std::clamp(static_cast<SFUI::Int>(outerRadius * 0.25f), 4, 12);
    SFUI::Float startRadians = startAngleDeg * M_PI / 180.f;
    SFUI::Float endRadians = endAngleDeg * M_PI / 180.f;
    SFUI::Float angleStep = (endRadians - startRadians) / static_cast<SFUI::Float>(arcResolution);

    for (SFUI::Int i = 0; i < arcResolution; ++i) {
        SFUI::Float angle0 = startRadians + i * angleStep;
        SFUI::Float angle1 = startRadians + (i + 1) * angleStep;
        SFUI::Vector2f outer0(center.x + std::cos(angle0) * outerRadius, center.y + std::sin(angle0) * outerRadius);
        SFUI::Vector2f outer1(center.x + std::cos(angle1) * outerRadius, center.y + std::sin(angle1) * outerRadius);
        SFUI::Vector2f inner(center);
        backgroundArcs.append({outer0, computedStyle.fillColor});
        backgroundArcs.append({outer1, computedStyle.fillColor});
        backgroundArcs.append({inner, computedStyle.fillColor});
    }
}


/**
 * @brief Compute a box (two triangles).
 * 
 * @param position The top-left position of the box to draw at.
 * @param size The size of the box to draw.
 */
SFUI::Void SFUI::Component::computeBorderRectGeometry(SFUI::Vector2f position, SFUI::Vector2f size) {
    SFUI::Vector2f topLeft = position;
    SFUI::Vector2f topRight = {position.x + size.x, position.y};
    SFUI::Vector2f bottomLeft = {position.x, position.y + size.y};
    SFUI::Vector2f bottomRight = {position.x + size.x, position.y + size.y};
    borderRects.append({topLeft, computedStyle.borderColor});
    borderRects.append({topRight, computedStyle.borderColor});
    borderRects.append({bottomRight, computedStyle.borderColor});
    borderRects.append({topLeft, computedStyle.borderColor});
    borderRects.append({bottomRight, computedStyle.borderColor});
    borderRects.append({bottomLeft, computedStyle.borderColor});
}


/**
 * @brief Compute a circular arc (triangle strip).
 * 
 * @param center The center position of the arc.
 * @param outerRadius The outer radius of the arc.
 * @param innerRadius The inner radius of the arc.
 * @param startAngleDeg The starting angle of the arc in degrees.
 * @param endAngleDeg The ending angle of the arc in degrees.
 */
SFUI::Void SFUI::Component::computeBorderArcGeometry(SFUI::Vector2f center, SFUI::Float outerRadius, SFUI::Float innerRadius, SFUI::Float startAngleDeg, SFUI::Float endAngleDeg) {
    SFUI::Float arcResolution = std::clamp(static_cast<SFUI::Int>(outerRadius * 0.25f), 4, 12);
    SFUI::Float startRadians = startAngleDeg * M_PI / 180.f;
    SFUI::Float endRadians = endAngleDeg * M_PI / 180.f;
    SFUI::Float angleStep = (endRadians - startRadians) / static_cast<SFUI::Float>(arcResolution);

    for (SFUI::Int i = 0; i < arcResolution; ++i) {
        SFUI::Float angle0 = startRadians + i * angleStep;
        SFUI::Float angle1 = startRadians + (i + 1) * angleStep;
        SFUI::Vector2f outer0(center.x + std::cos(angle0) * outerRadius, center.y + std::sin(angle0) * outerRadius);
        SFUI::Vector2f outer1(center.x + std::cos(angle1) * outerRadius, center.y + std::sin(angle1) * outerRadius);
        SFUI::Vector2f inner0(center.x + std::cos(angle0) * innerRadius, center.y + std::sin(angle0) * innerRadius);
        SFUI::Vector2f inner1(center.x + std::cos(angle1) * innerRadius, center.y + std::sin(angle1) * innerRadius);
        borderArcs.append({outer0, computedStyle.borderColor});
        borderArcs.append({outer1, computedStyle.borderColor});
        borderArcs.append({inner0, computedStyle.borderColor});
        borderArcs.append({inner0, computedStyle.borderColor});
        borderArcs.append({outer1, computedStyle.borderColor});
        borderArcs.append({inner1, computedStyle.borderColor});
    }
}


/**
 * @brief Compute a shadow box (two triangles).
 * 
 * @param position The top-left position of the box to draw at.
 * @param size The size of the box to draw.
 * @param modifiedShadowColor The modified shadow color for this layer.
 */
SFUI::Void SFUI::Component::computeShadowRectGeometry(SFUI::Vector2f position, SFUI::Vector2f size, SFUI::Color modifiedShadowColor) {
    SFUI::Vector2f topLeft = position;
    SFUI::Vector2f topRight = {position.x + size.x, position.y};
    SFUI::Vector2f bottomLeft = {position.x, position.y + size.y};
    SFUI::Vector2f bottomRight = {position.x + size.x, position.y + size.y};
    shadowRects.append({topLeft, modifiedShadowColor});
    shadowRects.append({topRight, modifiedShadowColor});
    shadowRects.append({bottomRight, modifiedShadowColor});
    shadowRects.append({topLeft, modifiedShadowColor});
    shadowRects.append({bottomRight, modifiedShadowColor});
    shadowRects.append({bottomLeft, modifiedShadowColor});
}


/**
 * @brief Compute a shadow circular arc (triangle fan).
 * 
 * @param center The center position of the arc.
 * @param outerRadius The outer radius of the arc.
 * @param startAngleDeg The starting angle of the arc in degrees.
 * @param endAngleDeg The ending angle of the arc in degrees.
 * @param modifiedShadowColor The modified shadow color for this layer.
 */
SFUI::Void SFUI::Component::computeShadowArcGeometry(SFUI::Vector2f center, SFUI::Float outerRadius, SFUI::Float startAngleDeg, SFUI::Float endAngleDeg, SFUI::Color modifiedShadowColor) {
    SFUI::Float arcResolution = std::clamp(static_cast<SFUI::Int>(outerRadius * 0.25f), 4, 12);
    SFUI::Float startRadians = startAngleDeg * M_PI / 180.f;
    SFUI::Float endRadians = endAngleDeg * M_PI / 180.f;
    SFUI::Float angleStep = (endRadians - startRadians) / static_cast<SFUI::Float>(arcResolution);

    for (SFUI::Int i = 0; i < arcResolution; ++i) {
        SFUI::Float angle0 = startRadians + i * angleStep;
        SFUI::Float angle1 = startRadians + (i + 1) * angleStep;
        SFUI::Vector2f outer0(center.x + std::cos(angle0) * outerRadius, center.y + std::sin(angle0) * outerRadius);
        SFUI::Vector2f outer1(center.x + std::cos(angle1) * outerRadius, center.y + std::sin(angle1) * outerRadius);
        SFUI::Vector2f inner(center);
        shadowArcs.append({outer0, modifiedShadowColor});
        shadowArcs.append({outer1, modifiedShadowColor});
        shadowArcs.append({inner, modifiedShadowColor});
    }
}
