#include <sfml-flexui-types.hpp>
#include <sfml-flexui-core.hpp>


////////////////////////////////////////
// SFML-FlexUI Parent Component Class //
////////////////////////////////////////
    

/**
 * @brief .
 * 
 * @param .
 */
SFUI::Component::Component(const SFUI::String& componentID) :
    componentID(componentID),
    layout(),
    style()
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Component::Component(const SFUI::String& componentID, const SFUI::Prop::Layout::Component& layout, const SFUI::Prop::Style::Component& style) :
    componentID(componentID),
    layout(layout),
    style(style)
{}


/**
 * @brief .
 */
SFUI::Void SFUI::Component::setParent(SFUI::Component* newParent) {
    parent = newParent;
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Component::addChild(const SFUI::SharedPointer<SFUI::Component>& newChild) {
    newChild->setParent(this);
    children.push_back(newChild);
    childrenComputedLayout.emplace_back(SFUI::ComputedProp::Layout::ComponentChild{
        {0.f, 0.f},     // size
        {0, 0},         // position
        0.f             // margin
    });
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Component::addChildren(const SFUI::Vector<SFUI::SharedPointer<SFUI::Component>>& newChildren) {
    children.insert(children.end(), newChildren.begin(), newChildren.end());
    for (int i = 0; i < newChildren.size(); i++) {
        newChildren[i]->setParent(this);
        childrenComputedLayout.emplace_back(SFUI::ComputedProp::Layout::ComponentChild{
            {0.f, 0.f},     // size
            {0, 0},         // position
            0.f             // margin
        });
    }
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Component::updateChildFromParent(SFUI::ComputedProp::Layout::ComponentChild childComputedLayout) {
    computedLayout.size = childComputedLayout.size;
    computedLayout.position = childComputedLayout.position;
    computedLayout.margin = childComputedLayout.margin;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Vector<SFUI::SharedPointer<SFUI::Component>> SFUI::Component::getChildren() const {
    return children;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::String SFUI::Component::getAlignDirection() {
    return computedLayout.alignDirection;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::String SFUI::Component::getAlignPrimary() {
    return computedLayout.alignPrimary;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::String SFUI::Component::getAlignSecondary() {
    return computedLayout.alignSecondary;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Vector2f SFUI::Component::getSize() {
    return computedLayout.size;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Vector2i SFUI::Component::getPosition() {
    return computedLayout.position;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Vector4f SFUI::Component::getPadding() {
    return computedLayout.padding;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Vector4f SFUI::Component::getMargin() {
    return computedLayout.margin;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Float SFUI::Component::getBorderWidth() {
    return computedStyle.borderWidth;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Vector4f SFUI::Component::getCornerRadius() {
    return computedStyle.cornerRadius;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::Component::getFillColor() {
    return computedStyle.fillColor;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::Component::getBorderColor() {
    return computedStyle.borderColor;
}


/**
 * @brief .
 * 
 * @param .
 * 
 * @return .
 */
SFUI::Bool SFUI::Component::isMouseHovered(const SFUI::Vector2i& mousePosition) {
    SFUI::Vector2f mousePos(mousePosition.x, mousePosition.y);
    SFUI::FloatRect cornerBounds = borderRects.getBounds();
    SFUI::FloatRect edgeBounds = backgroundRects.getBounds();
    return (cornerBounds.contains(mousePos) || edgeBounds.contains(mousePos));
}


/**
 * @brief .
 * 
 * @param .
 * 
 * @return .
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
 * @brief .
 * 
 * @param .
 * @param .
 * @param .
 * @param .
 * @param .
 * 
 * @return .
 */
SFUI::Vector4f SFUI::Component::resolveUniQuadSubProp(SFUI::Vector2f size, SFUI::SubProp::UniQuad subProp) {
    SFUI::Vector4f computedSubProp = {0.0f, 0.0f, 0.0f, 0.0f};
    SFUI::Float subPropSizeFactor = std::min(size.x, size.y);

    // If the Sub Prop Holds the Master Value //
    if (std::holds_alternative<SFUI::SubProp::Dimension>(subProp)) {
        SFUI::SubProp::Dimension masterSubProp = std::get<SFUI::SubProp::Dimension>(subProp);
        SFUI::Float computedMasterSubProp = 0.0f;

        // If the Master Sub Prop Holds and Explicit Float Value //
        if (std::holds_alternative<SFUI::Float>(masterSubProp)) {
            computedMasterSubProp = std::get<SFUI::Float>(masterSubProp);
        }
        // If the Master Sub Prop Holds a String Percentage Value //
        else if (std::holds_alternative<SFUI::String>(masterSubProp)) {
            SFUI::String masterSubPropString = std::get<SFUI::String>(masterSubProp);
            if (masterSubPropString.size() > 1 && masterSubPropString.back() == '%') {
                masterSubPropString.pop_back();
                try {
                    size_t index = 0;
                    SFUI::Double tempSubProp = std::stod(masterSubPropString, &index);
                    if (index == masterSubPropString.size()) {
                        computedMasterSubProp = subPropSizeFactor * std::clamp(static_cast<SFUI::Float>(tempSubProp) / 100.0f, 0.0f, 0.5f);
                    }   else {
                        computedMasterSubProp = 0.0f;
                    }
                }   catch (...) {
                    computedMasterSubProp = 0.0f;
                }
            }   else {
                computedMasterSubProp = 0.0f;
            }
        }
        computedSubProp.x = computedSubProp.y = computedSubProp.z = computedSubProp.w = computedMasterSubProp;
    }

    // If the Sub Prop Holds the Invidiual Inputs Value //
    if (std::holds_alternative<SFUI::SubProp::Vector4dim>(subProp)) {
        SFUI::SubProp::Vector4dim subProps = std::get<SFUI::SubProp::Vector4dim>(subProp);
        SFUI::Array<SFUI::SubProp::Dimension, 4> individualSubProps = {subProps.x, subProps.y, subProps.z, subProps.w};
        SFUI::Array<SFUI::Float, 4> computedIndividualSubProps = {0.0f, 0.0f, 0.0f, 0.0f};

        for (int i = 0; i < 4; i++) {
            // If the Master Sub Prop Holds and Explicit Float Value //
            if (std::holds_alternative<SFUI::Float>(individualSubProps[i])) {
                computedIndividualSubProps[i] = std::get<SFUI::Float>(individualSubProps[i]);
            }
            // If the Master Sub Prop Holds a String Percentage Value //
            else if (std::holds_alternative<SFUI::String>(individualSubProps[i])) {
                SFUI::String individualSubPropString = std::get<SFUI::String>(individualSubProps[i]);
                if (individualSubPropString.size() > 1 && individualSubPropString.back() == '%') {
                    individualSubPropString.pop_back();
                    try {
                        size_t index = 0;
                        SFUI::Double tempSubProp = std::stod(individualSubPropString, &index);
                        if (index == individualSubPropString.size()) {
                            computedIndividualSubProps[i] = subPropSizeFactor * std::clamp(static_cast<SFUI::Float>(tempSubProp) / 100.0f, 0.0f, 0.5f);
                        }   else {
                            computedIndividualSubProps[i] = 0.0f;
                        }
                    }   catch (...) {
                        computedIndividualSubProps[i] = 0.0f;
                    }
                }   else {
                    computedIndividualSubProps[i] = 0.0f;
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
 * @brief .
 */
SFUI::Void SFUI::Component::computeAlignDirection() {
    SFUI::String tempAlignDirection = layout.alignDirection;
    std::transform(tempAlignDirection.begin(), tempAlignDirection.end(), tempAlignDirection.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempAlignDirection == "vertical" || tempAlignDirection == "horizontal") {
        computedLayout.alignDirection = tempAlignDirection;
    }   else {
        computedLayout.alignDirection = "vertical";
    }
}


/**
 * @brief .
 */
SFUI::Void SFUI::Component::computeAlignPrimary() {
    SFUI::String tempAlignPrimary = layout.alignPrimary;
    std::transform(tempAlignPrimary.begin(), tempAlignPrimary.end(), tempAlignPrimary.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempAlignPrimary == "start" ||
        tempAlignPrimary == "end" ||
        tempAlignPrimary == "center" ||
        tempAlignPrimary == "space-between" ||
        tempAlignPrimary == "space-around" ||
        tempAlignPrimary == "space-evenly") {
        computedLayout.alignPrimary = tempAlignPrimary;
    }   else {
        computedLayout.alignPrimary = "start";
    }
}


/**
 * @brief .
 */
SFUI::Void SFUI::Component::computeAlignSecondary() {
    SFUI::String tempAlignSecondary = layout.alignSecondary;
    std::transform(tempAlignSecondary.begin(), tempAlignSecondary.end(), tempAlignSecondary.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempAlignSecondary == "start" ||
        tempAlignSecondary == "end" ||
        tempAlignSecondary == "center") {
        computedLayout.alignSecondary = tempAlignSecondary;
    }   else {
        computedLayout.alignSecondary = "start";
    }
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Component::computeMargin() {
    // If Root Component (No Parent), Must Calculate Margin for Itself //
    if (!parent) {
        computedLayout.margin = resolveUniQuadSubProp(
            SFUI::Vector2f{static_cast<SFUI::Float>(renderTargetSize.x), static_cast<SFUI::Float>(renderTargetSize.y)},
            layout.padding
        );
    }
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Component::computeSize() {
    // If Root Component (No Parent), Use RenderTarget Dimensions for Layout Computation //
    if (!parent) {
        SFUI::Vector2f computedSize = {0.0f, 0.0f};
    
        // Only Need to Consider Explicit Width/Height Values Since Children Determine Flex Changes //
        sf::Vector2i availableSize(renderTargetSize.x, renderTargetSize.y);

        // Obtain Width //
        // If Explicit Float Width Given By User //
        if (std::holds_alternative<SFUI::Float>(layout.width)) {
            computedSize.x = std::get<SFUI::Float>(layout.width);
        }
        // If String Input Width Given By User //
        else if (std::holds_alternative<SFUI::String>(layout.width)) {
            SFUI::String widthString = std::get<SFUI::String>(layout.width);
            if (widthString.size() > 1 && widthString.back() == '%') {
                widthString.pop_back();
                try {
                    size_t index = 0;
                    SFUI::Double tempWidth = std::stod(widthString, &index);
                    if (index == widthString.size()) {
                        computedSize.x = availableSize.x * std::clamp(static_cast<SFUI::Float>(tempWidth) / 100.0f, 0.0f, 1.0f);
                    }   else {
                        computedSize.x = 0.0f;
                    }
                }   catch (...) {
                    computedSize.x = 0.0f;
                }
            }   else {
                computedSize.x = 0.0f;
            }
        }

        // Obtain Height //
        // If Explicit Float Height Given By User //
        if (std::holds_alternative<SFUI::Float>(layout.height)) {
            computedSize.y = std::get<SFUI::Float>(layout.height);
        }
        // If String Input Height Given By User //
        else if (std::holds_alternative<SFUI::String>(layout.height)) {
            SFUI::String heightString = std::get<SFUI::String>(layout.height);
            if (heightString.size() > 1 && heightString.back() == '%') {
                heightString.pop_back();
                try {
                    size_t index = 0;
                    SFUI::Double tempHeight = std::stod(heightString, &index);
                    if (index == heightString.size()) {
                        computedSize.y = availableSize.y * std::clamp(static_cast<SFUI::Float>(tempHeight) / 100.0f, 0.0f, 1.0f);
                    }   else {
                        computedSize.y = 0.0f;
                    }
                }   catch (...) {
                    computedSize.y = 0.0f;
                }
            }   else {
                computedSize.y = 0.0f;
            }
        }

        // Update the Computed Size //
        computedLayout.size = {
            computedSize.x - (computedLayout.margin.x + computedLayout.margin.y),
            computedSize.y - (computedLayout.margin.z + computedLayout.margin.w)
        };
    }
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Component::computePadding() {
    computedLayout.padding = resolveUniQuadSubProp(computedLayout.size, layout.padding);
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Component::computePosition() {
    // If Root Component (No Parent), Use RenderTarget Dimensions for Layout Computation //
    if (!parent) {
        SFUI::Vector2i computedPosition = {0, 0};
        
        // If Explicit X-Position Input Given by User //
        if (layout.xPosition.has_value()) {
            SFUI::Int xPositionValue = layout.xPosition.value();
            computedPosition.x = xPositionValue;
        }
        // Place Root Component X-Position In Center of the Render Target X-Dimension //
        else {
            computedPosition.x = (renderTargetSize.x / 2.0f) - (computedLayout.size.x / 2.0f);
        }

        // If Explicit Y-Position Input Given by User //
        if (layout.yPosition.has_value()) {
            SFUI::Int yPositionValue = layout.yPosition.value();
            computedPosition.y = yPositionValue;
        }
        // Place Root Component Y-Position In Center of the Render Target Y-Dimension //
        else {
            computedPosition.y = (renderTargetSize.y / 2.0f) - (computedLayout.size.y / 2.0f);
        }            

        // Update the Computed Position //
        computedLayout.position = computedPosition;
    }
}


/**
 * @brief .
 */
SFUI::Void SFUI::Component::computeBorderWidth() {
    SFUI::Float computedBorderWidth = 0.0f;
    
    // If the Border Width Style Variant Holds an Explicit Width Float //
    if (std::holds_alternative<SFUI::Float>(style.borderWidth)) {
        computedBorderWidth = std::get<SFUI::Float>(style.borderWidth);
    }
    // If the Border Width Style Variant Holds a String Percentage Input //
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
                }   else {
                    computedBorderWidth = 0.0f;
                }
            }   catch (...) {
                computedBorderWidth = 0.0f;
            }
        }   else {
            computedBorderWidth = 0.0f;
        }
    }

    // Update the Computed Border Width Style //
    computedStyle.borderWidth = computedBorderWidth;
}


/**
 * @brief .
 */
SFUI::Void SFUI::Component::computeCornerRadius() {
    computedStyle.cornerRadius = resolveUniQuadSubProp(computedLayout.size, style.cornerRadius);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Component::computeFillColor() {
    computedStyle.fillColor = resolveColorSubProp(style.fillColor);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Component::computeBorderColor() {
    computedStyle.borderColor = resolveColorSubProp(style.borderColor);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Component::computeShadow() {
    computedStyle.shadowOffset = style.shadowOffset;
    computedStyle.shadowRadius = std::clamp(style.shadowRadius, 1.0f, 20.0f);
    computedStyle.shadowFillColor = resolveColorSubProp(style.shadowFillColor);
}


/**
 * @brief .
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
 * @brief .
 */
SFUI::Void SFUI::Component::computeChildrenMargin() {
    // If Has Child Components, Must Calculate Their Margins for Them Based on Its Own Size //
    if (children.size() > 0) {
        for (int i = 0; i < children.size(); i++) {
            SFUI::Component& childComponent = *children[i];
            childrenComputedLayout[i].margin = resolveUniQuadSubProp(computedLayout.size, childComponent.layout.margin);
        }
    }
}


/**
 * @brief .
 */
SFUI::Void SFUI::Component::computeChildrenSize() {
    // If Has Child Components, Must Calculate Their Sizes for Them Based on Its Own Size //
    if (children.size() > 0) {
        for (int i = 0; i < children.size(); i++) {
            SFUI::Vector2f computedSize = {0.0f, 0.0f};
            SFUI::Component& childComponent = *children[i];
            sf::Vector2i availableSize(
                computedLayout.size.x - (computedLayout.padding.x + computedLayout.padding.y),
                computedLayout.size.y - (computedLayout.padding.z + computedLayout.padding.w)
            );
    
            // Obtain Width //
            // If Explicit Float Width Given By User //
            if (std::holds_alternative<SFUI::Float>(childComponent.layout.width)) {
                computedSize.x = std::get<SFUI::Float>(childComponent.layout.width);
            }
            // If String Input Width Given By User //
            else if (std::holds_alternative<SFUI::String>(childComponent.layout.width)) {
                SFUI::String widthString = std::get<SFUI::String>(childComponent.layout.width);
                if (widthString.size() > 1 && widthString.back() == '%') {
                    widthString.pop_back();
                    try {
                        size_t index = 0;
                        SFUI::Double tempWidth = std::stod(widthString, &index);
                        if (index == widthString.size()) {
                            computedSize.x = availableSize.x * std::clamp(static_cast<SFUI::Float>(tempWidth) / 100.0f, 0.0f, 1.0f);
                        }   else {
                            computedSize.x = 0.0f;
                        }
                    }   catch (...) {
                        computedSize.x = 0.0f;
                    }
                }   else {
                    computedSize.x = 0.0f;
                }
            }
    
            // Obtain Height //
            // If Explicit Float Height Given By User //
            if (std::holds_alternative<SFUI::Float>(childComponent.layout.height)) {
                computedSize.y = std::get<SFUI::Float>(childComponent.layout.height);
            }
            // If String Input Height Given By User //
            else if (std::holds_alternative<SFUI::String>(childComponent.layout.height)) {
                SFUI::String heightString = std::get<SFUI::String>(childComponent.layout.height);
                if (heightString.size() > 1 && heightString.back() == '%') {
                    heightString.pop_back();
                    try {
                        size_t index = 0;
                        SFUI::Double tempHeight = std::stod(heightString, &index);
                        if (index == heightString.size()) {
                            computedSize.y = availableSize.y * (static_cast<SFUI::Float>(tempHeight) / 100.0f);
                        }   else {
                            computedSize.y = 0.0f;
                        }
                    }   catch (...) {
                        computedSize.y = 0.0f;
                    }
                }   else {
                    computedSize.y = 0.0f;
                }
            }
    
            // Update the Computed Size //
            childrenComputedLayout[i].size = {
                computedSize.x - (childrenComputedLayout[i].margin.x + childrenComputedLayout[i].margin.y),
                computedSize.y - (childrenComputedLayout[i].margin.z + childrenComputedLayout[i].margin.w)
            };
        }
    }
}


/**
 * @brief .
 */
SFUI::Void SFUI::Component::computeChildrenPosition() {
    // If Child Components, Calculate Children Positions for Them //
    if (children.size() > 0) {

        // Calculate Each Component's Sizing and Margins and Total Sizing with Margins Combined //
        SFUI::Vector<SFUI::Float> componentSizeAndMargins;
        SFUI::Float totalComponentSizingAndMargins = 0.0f;
        SFUI::Float availableGapSize = 0.0f;
        SFUI::Float gapSize = 0.0f;
        SFUI::Float interiorGapSize = 0.0f;
        SFUI::Float edgeGapSize = 0.0f;
        for (int i = 0; i < children.size(); i++) {
            SFUI::Float childTotalSize = 0.0f;
            if (computedLayout.alignDirection == "vertical")
                childTotalSize = childrenComputedLayout[i].size.y + (childrenComputedLayout[i].margin.z + childrenComputedLayout[i].margin.w);
            else if (computedLayout.alignDirection == "horizontal")
                childTotalSize = childrenComputedLayout[i].size.x + (childrenComputedLayout[i].margin.x + childrenComputedLayout[i].margin.y);
            componentSizeAndMargins.push_back(childTotalSize);
            totalComponentSizingAndMargins += childTotalSize;
        }

        // Compute Total Gap Size Available Between Children //
        if (computedLayout.alignDirection == "vertical")
            availableGapSize = computedLayout.size.y - (computedLayout.padding.z + computedLayout.padding.w) - totalComponentSizingAndMargins;
        else if (computedLayout.alignDirection == "horizontal")
            availableGapSize = computedLayout.size.x - (computedLayout.padding.x + computedLayout.padding.y) - totalComponentSizingAndMargins;

        // Compute the Gap Sizes Between Children //
        if (children.size() > 1) {
            if (computedLayout.alignPrimary == "space-between") {
                gapSize = availableGapSize / (children.size() - 1);
            }
            else if (computedLayout.alignPrimary == "space-around") {
                edgeGapSize = availableGapSize / (2.0f * children.size());
                interiorGapSize = availableGapSize / children.size();
            }
            else if (computedLayout.alignPrimary == "space-evenly") {
                gapSize = availableGapSize / (children.size() + 1);
            }
        }

        // Compute Children Positions Iteratively //
        for (int i = 0; i < children.size(); i++) {
            SFUI::Vector2i computedPosition = {0, 0};
            SFUI::Component& childComponent = *children[i];
            
            // X-Positions //
            // If Explicit X-Position Input Given by User //
            if (childComponent.layout.xPosition.has_value()) {
                SFUI::Int xPositionValue = childComponent.layout.xPosition.value();
                computedPosition.x = xPositionValue;
            }
            // Place Component X-Position According to Alignment Direction and Axis Alignment //
            else {
                // Off-Axis Positioning //
                if (computedLayout.alignDirection == "vertical") {
                    if (computedLayout.alignSecondary == "start")
                        computedPosition.x = computedLayout.position.x + computedLayout.padding.x + childrenComputedLayout[i].margin.x;
                    else if (computedLayout.alignSecondary == "end")
                        computedPosition.x = computedLayout.position.x + computedLayout.size.x - computedLayout.padding.y - childrenComputedLayout[i].margin.y - childrenComputedLayout[i].size.x;
                    else if (computedLayout.alignSecondary == "center")
                        computedPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (childrenComputedLayout[i].size.x / 2.0f);
                }
                // On-Axis Positioning //
                else if (computedLayout.alignDirection == "horizontal") {
                    if (computedLayout.alignPrimary == "start") {
                        if (i == 0) computedPosition.x = computedLayout.position.x + computedLayout.padding.x + childrenComputedLayout[i].margin.x;
                        else computedPosition.x = childrenComputedLayout[i - 1].position.x + childrenComputedLayout[i - 1].size.x + childrenComputedLayout[i - 1].margin.y + childrenComputedLayout[i].margin.x;
                    }
                    else if (computedLayout.alignPrimary == "end") {
                        if (i == 0) computedPosition.x = computedLayout.position.x + computedLayout.size.x - computedLayout.padding.y - totalComponentSizingAndMargins + childrenComputedLayout[i].margin.x;
                        else computedPosition.x = childrenComputedLayout[i - 1].position.x + childrenComputedLayout[i - 1].size.x + childrenComputedLayout[i - 1].margin.y + childrenComputedLayout[i].margin.x;
                    }
                    else if (computedLayout.alignPrimary == "center") {
                        if (i == 0) computedPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (totalComponentSizingAndMargins / 2.0f) + childrenComputedLayout[i].margin.x;
                        else computedPosition.x = childrenComputedLayout[i - 1].position.x + childrenComputedLayout[i - 1].size.x + childrenComputedLayout[i - 1].margin.y + childrenComputedLayout[i].margin.x;
                    }
                    else if (computedLayout.alignPrimary == "space-between") {
                        if (children.size() == 1) computedPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (childrenComputedLayout[i].size.x / 2.0f);
                        else if (i == 0) computedPosition.x = computedLayout.position.x + computedLayout.padding.x + childrenComputedLayout[i].margin.x;
                        else if (i == children.size() - 1) computedPosition.x = computedLayout.position.x + computedLayout.size.x - computedLayout.padding.y - childrenComputedLayout[i].margin.y - childrenComputedLayout[i].size.x;
                        else computedPosition.x = childrenComputedLayout[i - 1].position.x + childrenComputedLayout[i - 1].size.x + childrenComputedLayout[i - 1].margin.y + gapSize + childrenComputedLayout[i].margin.x;
                    }
                    else if (computedLayout.alignPrimary == "space-around") {
                        if (children.size() == 1) computedPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (childrenComputedLayout[i].size.x / 2.0f);
                        else if (i == 0) computedPosition.x = computedLayout.position.x + computedLayout.padding.x + edgeGapSize + childrenComputedLayout[i].margin.x;
                        else if (i == children.size() - 1) computedPosition.x = computedLayout.position.x + computedLayout.size.x - computedLayout.padding.y - childrenComputedLayout[i].margin.y - childrenComputedLayout[i].size.x - edgeGapSize;
                        else computedPosition.x = childrenComputedLayout[i - 1].position.x + childrenComputedLayout[i - 1].size.x + childrenComputedLayout[i - 1].margin.y + interiorGapSize + childrenComputedLayout[i].margin.x;
                    }
                    else if (computedLayout.alignPrimary == "space-evenly") {
                        if (children.size() == 1) computedPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (childrenComputedLayout[i].size.x / 2.0f);
                        else if (i == 0) computedPosition.x = computedLayout.position.x + computedLayout.padding.x + gapSize + childrenComputedLayout[i].margin.x;
                        else computedPosition.x = childrenComputedLayout[i - 1].position.x + childrenComputedLayout[i - 1].size.x + childrenComputedLayout[i - 1].margin.y + gapSize + childrenComputedLayout[i].margin.x;
                    }
                }
            }

            // Y-Positions //
            // If Explicit Y-Position Input Given by User //
            if (childComponent.layout.yPosition.has_value()) {
                SFUI::Int yPositionValue = childComponent.layout.yPosition.value();
                computedPosition.y = yPositionValue;
            }
            // Place Root Component Y-Position In Center of the Render Target Y-Dimension //
            else {
                // Off-Axis Positioning //
                if (computedLayout.alignDirection == "horizontal") {
                    if (computedLayout.alignSecondary == "start")
                        computedPosition.y = computedLayout.position.y + computedLayout.padding.z;
                    else if (computedLayout.alignSecondary == "end")
                        computedPosition.y = computedLayout.position.y + computedLayout.size.y - computedLayout.padding.w - childrenComputedLayout[i].size.y;
                    else if (computedLayout.alignSecondary == "center")
                        computedPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (childrenComputedLayout[i].size.y / 2.0f);
                }
                // On-Axis Positioning //
                else if (computedLayout.alignDirection == "vertical") {
                    if (computedLayout.alignPrimary == "start") {
                        if (i == 0) computedPosition.y = computedLayout.position.y + computedLayout.padding.z + childrenComputedLayout[i].margin.z;
                        else computedPosition.y = childrenComputedLayout[i - 1].position.y + childrenComputedLayout[i - 1].size.y + childrenComputedLayout[i - 1].margin.w + childrenComputedLayout[i].margin.z;
                    }
                    else if (computedLayout.alignPrimary == "end") {
                        if (i == 0) computedPosition.y = computedLayout.position.y + computedLayout.size.y - computedLayout.padding.w - totalComponentSizingAndMargins + childrenComputedLayout[i].margin.z;
                        else computedPosition.y = childrenComputedLayout[i - 1].position.y + childrenComputedLayout[i - 1].size.y + childrenComputedLayout[i - 1].margin.w + childrenComputedLayout[i].margin.z;
                    }
                    else if (computedLayout.alignPrimary == "center") {
                        if (i == 0) computedPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (totalComponentSizingAndMargins / 2.0f) + childrenComputedLayout[i].margin.z;
                        else computedPosition.y = childrenComputedLayout[i - 1].position.y + childrenComputedLayout[i - 1].size.y + childrenComputedLayout[i - 1].margin.w + childrenComputedLayout[i].margin.z;
                    }
                    else if (computedLayout.alignPrimary == "space-between") {
                        if (children.size() == 1) computedPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (childrenComputedLayout[i].size.y / 2.0f);
                        else if (i == 0) computedPosition.y = computedLayout.position.y + computedLayout.padding.z + childrenComputedLayout[i].margin.z;
                        else if (i == children.size() - 1) computedPosition.y = computedLayout.position.y + computedLayout.size.y - computedLayout.padding.w - childrenComputedLayout[i].margin.w - childrenComputedLayout[i].size.y;
                        else computedPosition.y = childrenComputedLayout[i - 1].position.y + childrenComputedLayout[i - 1].size.y + childrenComputedLayout[i - 1].margin.w + gapSize + childrenComputedLayout[i].margin.z;
                    }
                    else if (computedLayout.alignPrimary == "space-around") {
                        if (children.size() == 1) computedPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (childrenComputedLayout[i].size.y / 2.0f);
                        else if (i == 0) computedPosition.y = computedLayout.position.y + computedLayout.padding.z + edgeGapSize + childrenComputedLayout[i].margin.z;
                        else if (i == children.size() - 1) computedPosition.y = computedLayout.position.y + computedLayout.size.y - computedLayout.padding.w - childrenComputedLayout[i].margin.w - childrenComputedLayout[i].size.y - edgeGapSize;
                        else computedPosition.y = childrenComputedLayout[i - 1].position.y + childrenComputedLayout[i - 1].size.y + childrenComputedLayout[i - 1].margin.w + interiorGapSize + childrenComputedLayout[i].margin.z;
                    }
                    else if (computedLayout.alignPrimary == "space-evenly") {
                        if (children.size() == 1) computedPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (childrenComputedLayout[i].size.y / 2.0f);
                        else if (i == 0) computedPosition.y = computedLayout.position.y + computedLayout.padding.z + gapSize + childrenComputedLayout[i].margin.z;
                        else computedPosition.y = childrenComputedLayout[i - 1].position.y + childrenComputedLayout[i - 1].size.y + childrenComputedLayout[i - 1].margin.w + gapSize + childrenComputedLayout[i].margin.z;
                    }
                }
            }            

            // Update the Computed Position //
            childrenComputedLayout[i].position = computedPosition;

            // if (childComponent.componentID == "slider") {
            //     std::cout << "Children Computed Position: " << computedPosition.x << ", " << computedPosition.y << "\n";
            // }
        }
    }
}


/**
 * @brief .
 */
SFUI::Void SFUI::Component::updateChildren() {
    for (int i = 0; i < children.size(); i++) {
        children[i]->updateChildFromParent(childrenComputedLayout[i]);
    }
}


/**
 * @brief .
 * 
 * @param .
 * @param .
 * @param .
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
 * @brief .
 * 
 * @param .
 * @param .
 * @param .
 * @param .
 * @param .
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
 * @brief Compute a box (two triangles) and return the VertexArray.
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
 * @brief .
 * 
 * @param .
 * @param .
 * @param .
 * @param .
 * @param .
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
 * @brief .
 * 
 * @param .
 * @param .
 * @param .
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
 * @brief .
 * 
 * @param .
 * @param .
 * @param .
 * @param .
 * @param .
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
