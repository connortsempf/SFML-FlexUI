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
SFUI::Void SFUI::Component::setParent(const SFUI::SharedPointer<Component>& newParent) {
    parent = newParent;
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Component::addChild(const SFUI::SharedPointer<SFUI::Component>& newChild) {
    newChild->setParent(this->shared_from_this());
    children.push_back(newChild);
    childrenComputedLayout.emplace_back(SFUI::ComputedProp::ChildLayout{
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
        newChildren[i]->setParent(this->shared_from_this());
        childrenComputedLayout.emplace_back(SFUI::ComputedProp::ChildLayout{
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
SFUI::Void SFUI::Component::updateChildFromParent(SFUI::ComputedProp::ChildLayout childComputedLayout) {
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
SFUI::Float SFUI::Component::getPadding() {
    return computedLayout.padding;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Float SFUI::Component::getMargin() {
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
        // Going to Do this Later -- String Parsing Code Needed //
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
        computedSubProp.x = computedIndividualSubProps[1];
        computedSubProp.y = computedIndividualSubProps[2];
        computedSubProp.z = computedIndividualSubProps[3];
        computedSubProp.w = computedIndividualSubProps[4];
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
    if (!parent.lock()) {
        SFUI::Float computedMargin = 0.0f;
    
        // If the Margin Layout Variant Holds an Explicit Width Float //
        if (std::holds_alternative<SFUI::Float>(layout.margin)) {
            computedMargin = std::get<SFUI::Float>(layout.margin);
        }
        // If the Margin Layout Variant Holds a String Percentage Input //
        else if (std::holds_alternative<SFUI::String>(layout.margin)) {
            SFUI::String marginString = std::get<SFUI::String>(layout.margin);
            if (marginString.size() > 1 && marginString.back() == '%') {
                marginString.pop_back();
                try {
                    size_t index = 0;
                    SFUI::Double tempMargin = std::stod(marginString, &index);
                    if (index == marginString.size()) {
                        SFUI::Float relativeMarginFactor = std::min(renderTargetSize.x, renderTargetSize.y);
                        computedMargin = relativeMarginFactor * std::clamp(static_cast<SFUI::Float>(tempMargin) / 100.0f, 0.0f, 1.0f);
                    }   else {
                        computedMargin = 0.0f;
                    }
                }   catch (...) {
                    computedMargin = 0.0f;
                }
            }   else {
                computedMargin = 0.0f;
            }
        }

        // Update the Computed Margin //
        computedLayout.margin = computedMargin;
    }
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Component::computeSize() {
    // If Root Component (No Parent), Use RenderTarget Dimensions for Layout Computation //
    if (!parent.lock()) {
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
        computedLayout.size = {computedSize.x - (computedLayout.margin * 2.0f), computedSize.y - (computedLayout.margin * 2.0f)};
    }
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Component::computePadding() {
    SFUI::Float computedPadding = 0.0f;

    // If the Padding Layout Variant Holds an Explicit Width Float //
    if (std::holds_alternative<SFUI::Float>(layout.padding)) {
        computedPadding = std::get<SFUI::Float>(layout.padding);
    }
    // If the Padding Layout Variant Holds a String Percentage Input //
    else if (std::holds_alternative<SFUI::String>(layout.padding)) {
        SFUI::String paddingString = std::get<SFUI::String>(layout.padding);
        if (paddingString.size() > 1 && paddingString.back() == '%') {
            paddingString.pop_back();
            try {
                size_t index = 0;
                SFUI::Double tempPadding = std::stod(paddingString, &index);
                if (index == paddingString.size()) {
                    SFUI::Float relativePaddingFactor = std::min(computedLayout.size.x, computedLayout.size.y);
                    computedPadding = relativePaddingFactor * std::clamp(static_cast<SFUI::Float>(tempPadding) / 100.0f, 0.0f, 0.5f);
                }   else {
                    computedPadding = 0.0f;
                }
            }   catch (...) {
                computedPadding = 0.0f;
            }
        }   else {
            computedPadding = 0.0f;
        }
    }

    // Update the Computed Padding Style //
    computedLayout.padding = computedPadding;
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Component::computePosition() {
    // If Root Component (No Parent), Use RenderTarget Dimensions for Layout Computation //
    if (!parent.lock()) {
        SFUI::Vector2i computedPosition = {0, 0};
        
        // If Explicit X-Position Input Given by User //
        if (layout.xPosition.isSet()) {
            SFUI::Int xPositionValue = layout.xPosition.value();
            computedPosition.x = xPositionValue;
        }
        // Place Root Component X-Position In Center of the Render Target X-Dimension //
        else {
            computedPosition.x = (renderTargetSize.x / 2.0f) - (computedLayout.size.x / 2.0f);
        }

        // If Explicit Y-Position Input Given by User //
        if (layout.yPosition.isSet()) {
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

    // if (componentID == "rootView") {
    //     std::cout <<
    //         "(" << (style.cornerRadiusTopLeft.isSet() ? "Set, " : "Not Set, ") <<
    //         "" << (style.cornerRadiusTopRight.isSet() ? "Set, " : "Not Set, ") <<
    //         "" << (style.cornerRadiusBottomLeft.isSet() ? "Set, " : "Not Set, ") <<
    //         "" << (style.cornerRadiusBottomRight.isSet() ? "Set)\n" : "Not Set)\n");
    // }

    computedStyle.cornerRadius = resolveUniQuadSubProp(computedLayout.size, style.cornerRadius);

    // if (componentID == "rootView") {
    //     std::cout << "(" << computedStyle.cornerRadius.x << ", " <<
    //         "" << computedStyle.cornerRadius.y << ", " <<
    //         "" << computedStyle.cornerRadius.z << ", " <<
    //         "" << computedStyle.cornerRadius.w << ")\n";
    // }
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
SFUI::Void SFUI::Component::computeGraphics() {
    backgroundRects.clear();
    backgroundArcs.clear();
    borderRects.clear();
    borderArcs.clear();

    backgroundRects.setPrimitiveType(SFUI::PrimitiveType::Triangles);
    backgroundArcs.setPrimitiveType(SFUI::PrimitiveType::TriangleStrip);
    borderRects.setPrimitiveType(SFUI::PrimitiveType::Triangles);
    borderArcs.setPrimitiveType(SFUI::PrimitiveType::TriangleStrip);

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

    computeBackgroundRectGeometry({left + leftInset, top + topInset}, {width - leftInset - rightInset, height - topInset - bottomInset});
    computeBackgroundRectGeometry({left, top + topLeft}, {leftInset, height - topLeft - bottomLeft});
    computeBackgroundRectGeometry({right - rightInset, top + topRight}, {rightInset, height - topRight - bottomRight});
    computeBackgroundRectGeometry({left + topLeft, top}, {width - topLeft - topRight, topInset});
    computeBackgroundRectGeometry({left + bottomLeft, bottom - bottomInset}, {width - bottomLeft - bottomRight, bottomInset});

    if (topLeft > 0.0f) computeBackgroundArcGeometry({left + topLeft, top + topLeft}, topLeft, 180, 270);
    if (topRight > 0.0f) computeBackgroundArcGeometry({right - topRight, top + topRight}, topRight, 270, 360);
    if (bottomRight > 0.0f) computeBackgroundArcGeometry({right - bottomRight, bottom - bottomRight}, bottomRight, 0, 90);
    if (bottomLeft > 0.0f) computeBackgroundArcGeometry({left + bottomLeft, bottom - bottomLeft}, bottomLeft, 90, 180);

    if (borderWidth <= 0.0f) return;

    computeBorderRectGeometry({left, top + topLeft}, {borderWidth, height - topLeft - bottomLeft});
    computeBorderRectGeometry({right - borderWidth, top + topRight}, {borderWidth, height - topRight - bottomRight});
    computeBorderRectGeometry({left + topLeft, top}, {width - topLeft - topRight, borderWidth});
    computeBorderRectGeometry({left + bottomLeft, bottom - borderWidth}, {width - bottomLeft - bottomRight, borderWidth});
    
    if (topLeft > 0.0f) computeBorderArcGeometry({left + topLeft, top + topLeft}, topLeft, topLeft >= borderWidth ? topLeft - borderWidth : 0.0f, 180, 270);
    if (topRight > 0.0f) computeBorderArcGeometry({right - topRight, top + topRight}, topRight, topRight >= borderWidth ? topRight - borderWidth : 0.0f, 270, 360);
    if (bottomRight > 0.0f) computeBorderArcGeometry({right - bottomRight, bottom - bottomRight}, bottomRight, bottomRight >= borderWidth ? bottomRight - borderWidth : 0.0f, 0, 90);
    if (bottomLeft > 0.0f) computeBorderArcGeometry({left + bottomLeft, bottom - bottomLeft}, bottomLeft, bottomLeft >= borderWidth ? bottomLeft - borderWidth : 0.0f, 90, 180);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Component::computeChildrenMargin() {
    // If Has Child Components, Must Calculate Their Margins for Them Based on Its Own Size //
    if (children.size() > 0) {
        for (int i = 0; i < children.size(); i++) {
            SFUI::Float computedMargin = 0.0f;
            SFUI::Component& childComponent = *children[i];

            // If the Margin Layout Variant Holds an Explicit Width Float //
            if (std::holds_alternative<SFUI::Float>(childComponent.layout.margin)) {
                computedMargin = std::get<SFUI::Float>(childComponent.layout.margin);
            }
            // If the Margin Layout Variant Holds a String Percentage Input //
            else if (std::holds_alternative<SFUI::String>(childComponent.layout.margin)) {
                SFUI::String marginString = std::get<SFUI::String>(childComponent.layout.margin);
                if (marginString.size() > 1 && marginString.back() == '%') {
                    marginString.pop_back();
                    try {
                        size_t index = 0;
                        SFUI::Double tempMargin = std::stod(marginString, &index);
                        if (index == marginString.size()) {
                            SFUI::Float relativeMarginFactor = std::min(computedLayout.size.x, computedLayout.size.y);
                            computedMargin = relativeMarginFactor * std::clamp(static_cast<SFUI::Float>(tempMargin) / 100.0f, 0.0f, 1.0f);
                        }   else {
                            computedMargin = 0.0f;
                        }
                    }   catch (...) {
                        computedMargin = 0.0f;
                    }
                }   else {
                    computedMargin = 0.0f;
                }
            }

            // Update the Computed Border Width Style //
            childrenComputedLayout[i].margin = computedMargin;
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
            sf::Vector2i availableSize(computedLayout.size.x - (computedLayout.padding * 2), computedLayout.size.y - (computedLayout.padding * 2));
    
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
            childrenComputedLayout[i].size = {computedSize.x - (childrenComputedLayout[i].margin * 2.0f), computedSize.y - (childrenComputedLayout[i].margin * 2.0f)};
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
                childTotalSize = childrenComputedLayout[i].size.y + (childrenComputedLayout[i].margin * 2.0f);
            else if (computedLayout.alignDirection == "horizontal")
                childTotalSize = childrenComputedLayout[i].size.x + (childrenComputedLayout[i].margin * 2.0f);
            componentSizeAndMargins.push_back(childTotalSize);
            totalComponentSizingAndMargins += childTotalSize;
        }

        // Compute Total Gap Size Available Between Children //
        if (computedLayout.alignDirection == "vertical")
            availableGapSize = computedLayout.size.y - (computedLayout.padding * 2.0f) - totalComponentSizingAndMargins;
        else if (computedLayout.alignDirection == "horizontal")
            availableGapSize = computedLayout.size.x - (computedLayout.padding * 2.0f) - totalComponentSizingAndMargins;

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
            if (childComponent.layout.xPosition.isSet()) {
                SFUI::Int xPositionValue = childComponent.layout.xPosition.value();
                computedPosition.x = xPositionValue;
            }
            // Place Component X-Position According to Alignment Direction and Axis Alignment //
            else {
                // Off-Axis Positioning //
                if (computedLayout.alignDirection == "vertical") {
                    if (computedLayout.alignSecondary == "start")
                        computedPosition.x = computedLayout.position.x + computedLayout.padding + childrenComputedLayout[i].margin;
                    else if (computedLayout.alignSecondary == "end")
                        computedPosition.x = computedLayout.position.x + computedLayout.size.x - computedLayout.padding - childrenComputedLayout[i].margin - childrenComputedLayout[i].size.x;
                    else if (computedLayout.alignSecondary == "center")
                        computedPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (childrenComputedLayout[i].size.x / 2.0f);
                }
                // On-Axis Positioning //
                else if (computedLayout.alignDirection == "horizontal") {
                    if (computedLayout.alignPrimary == "start") {
                        if (i == 0) computedPosition.x = computedLayout.position.x + computedLayout.padding + childrenComputedLayout[i].margin;
                        else computedPosition.x = childrenComputedLayout[i - 1].position.x + childrenComputedLayout[i - 1].size.x + childrenComputedLayout[i - 1].margin + childrenComputedLayout[i].margin;
                    }
                    else if (computedLayout.alignPrimary == "end") {
                        if (i == 0) computedPosition.x = computedLayout.position.x + computedLayout.size.x - computedLayout.padding - totalComponentSizingAndMargins + childrenComputedLayout[i].margin;
                        else computedPosition.x = childrenComputedLayout[i - 1].position.x + childrenComputedLayout[i - 1].size.x + childrenComputedLayout[i - 1].margin + childrenComputedLayout[i].margin;
                    }
                    else if (computedLayout.alignPrimary == "center") {
                        if (i == 0) computedPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (totalComponentSizingAndMargins / 2.0f) + childrenComputedLayout[i].margin;
                        else computedPosition.x = childrenComputedLayout[i - 1].position.x + childrenComputedLayout[i - 1].size.x + childrenComputedLayout[i - 1].margin + childrenComputedLayout[i].margin;
                    }
                    else if (computedLayout.alignPrimary == "space-between") {
                        if (children.size() == 1) computedPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (childrenComputedLayout[i].size.x / 2.0f);
                        else if (i == 0) computedPosition.x = computedLayout.position.x + computedLayout.padding + childrenComputedLayout[i].margin;
                        else if (i == children.size() - 1) computedPosition.x = computedLayout.position.x + computedLayout.size.x - computedLayout.padding - childrenComputedLayout[i].margin - childrenComputedLayout[i].size.x;
                        else computedPosition.x = childrenComputedLayout[i - 1].position.x + childrenComputedLayout[i - 1].size.x + childrenComputedLayout[i - 1].margin + gapSize + childrenComputedLayout[i].margin;
                    }
                    else if (computedLayout.alignPrimary == "space-around") {
                        if (children.size() == 1) computedPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (childrenComputedLayout[i].size.x / 2.0f);
                        else if (i == 0) computedPosition.x = computedLayout.position.x + computedLayout.padding + edgeGapSize + childrenComputedLayout[i].margin;
                        else if (i == children.size() - 1) computedPosition.x = computedLayout.position.x + computedLayout.size.x - computedLayout.padding - childrenComputedLayout[i].margin - childrenComputedLayout[i].size.x - edgeGapSize;
                        else computedPosition.x = childrenComputedLayout[i - 1].position.x + childrenComputedLayout[i - 1].size.x + childrenComputedLayout[i - 1].margin + interiorGapSize + childrenComputedLayout[i].margin;
                    }
                    else if (computedLayout.alignPrimary == "space-evenly") {
                        if (children.size() == 1) computedPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (childrenComputedLayout[i].size.x / 2.0f);
                        else if (i == 0) computedPosition.x = computedLayout.position.x + computedLayout.padding + gapSize + childrenComputedLayout[i].margin;
                        else computedPosition.x = childrenComputedLayout[i - 1].position.x + childrenComputedLayout[i - 1].size.x + childrenComputedLayout[i - 1].margin + gapSize + childrenComputedLayout[i].margin;
                    }
                }
            }

            // Y-Positions //
            // If Explicit Y-Position Input Given by User //
            if (childComponent.layout.yPosition.isSet()) {
                SFUI::Int yPositionValue = childComponent.layout.yPosition.value();
                computedPosition.y = yPositionValue;
            }
            // Place Root Component Y-Position In Center of the Render Target Y-Dimension //
            else {
                // Off-Axis Positioning //
                if (computedLayout.alignDirection == "horizontal") {
                    if (computedLayout.alignSecondary == "start")
                        computedPosition.y = computedLayout.position.y + computedLayout.padding;
                    else if (computedLayout.alignSecondary == "end")
                        computedPosition.y = computedLayout.position.y + computedLayout.size.y - computedLayout.padding - childrenComputedLayout[i].size.y;
                    else if (computedLayout.alignSecondary == "center")
                        computedPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (childrenComputedLayout[i].size.y / 2.0f);
                }
                // On-Axis Positioning //
                else if (computedLayout.alignDirection == "vertical") {
                    if (computedLayout.alignPrimary == "start") {
                        if (i == 0) computedPosition.y = computedLayout.position.y + computedLayout.padding + childrenComputedLayout[i].margin;
                        else computedPosition.y = childrenComputedLayout[i - 1].position.y + childrenComputedLayout[i - 1].size.y + childrenComputedLayout[i - 1].margin + childrenComputedLayout[i].margin;
                    }
                    else if (computedLayout.alignPrimary == "end") {
                        if (i == 0) computedPosition.y = computedLayout.position.y + computedLayout.size.y - computedLayout.padding - totalComponentSizingAndMargins + childrenComputedLayout[i].margin;
                        else computedPosition.y = childrenComputedLayout[i - 1].position.y + childrenComputedLayout[i - 1].size.y + childrenComputedLayout[i - 1].margin + childrenComputedLayout[i].margin;
                    }
                    else if (computedLayout.alignPrimary == "center") {
                        if (i == 0) computedPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (totalComponentSizingAndMargins / 2.0f) + childrenComputedLayout[i].margin;
                        else computedPosition.y = childrenComputedLayout[i - 1].position.y + childrenComputedLayout[i - 1].size.y + childrenComputedLayout[i - 1].margin + childrenComputedLayout[i].margin;
                    }
                    else if (computedLayout.alignPrimary == "space-between") {
                        if (children.size() == 1) computedPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (childrenComputedLayout[i].size.y / 2.0f);
                        else if (i == 0) computedPosition.y = computedLayout.position.y + computedLayout.padding + childrenComputedLayout[i].margin;
                        else if (i == children.size() - 1) computedPosition.y = computedLayout.position.y + computedLayout.size.y - computedLayout.padding - childrenComputedLayout[i].margin - childrenComputedLayout[i].size.y;
                        else computedPosition.y = childrenComputedLayout[i - 1].position.y + childrenComputedLayout[i - 1].size.y + childrenComputedLayout[i - 1].margin + gapSize + childrenComputedLayout[i].margin;
                    }
                    else if (computedLayout.alignPrimary == "space-around") {
                        if (children.size() == 1) computedPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (childrenComputedLayout[i].size.y / 2.0f);
                        else if (i == 0) computedPosition.y = computedLayout.position.y + computedLayout.padding + edgeGapSize + childrenComputedLayout[i].margin;
                        else if (i == children.size() - 1) computedPosition.y = computedLayout.position.y + computedLayout.size.y - computedLayout.padding - childrenComputedLayout[i].margin - childrenComputedLayout[i].size.y - edgeGapSize;
                        else computedPosition.y = childrenComputedLayout[i - 1].position.y + childrenComputedLayout[i - 1].size.y + childrenComputedLayout[i - 1].margin + interiorGapSize + childrenComputedLayout[i].margin;
                    }
                    else if (computedLayout.alignPrimary == "space-evenly") {
                        if (children.size() == 1) computedPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (childrenComputedLayout[i].size.y / 2.0f);
                        else if (i == 0) computedPosition.y = computedLayout.position.y + computedLayout.padding + gapSize + childrenComputedLayout[i].margin;
                        else computedPosition.y = childrenComputedLayout[i - 1].position.y + childrenComputedLayout[i - 1].size.y + childrenComputedLayout[i - 1].margin + gapSize + childrenComputedLayout[i].margin;
                    }
                }
            }            

            // Update the Computed Position //
            childrenComputedLayout[i].position = computedPosition;
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
 * @brief Compute a box (two triangles) and return the VertexArray.
 * 
 * @param position The top-left position of the box to draw at.
 * @param size The size of the box to draw.
 * @param color The fill/stroke color of the box.
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
 * @brief Generate a stroked arc as a triangle strip and return the VertexArray.
 * 
 * @param center The center of the arc to draw.
 * @param outerRadius The outer radius of the arc to draw.
 * @param startAngleDeg The starting angle of the arc to draw.
 * @param endAngleDeg The ending angle of the arc to draw.
 * @param color The color to use for the arc vertices.
 */
SFUI::Void SFUI::Component::computeBackgroundArcGeometry(SFUI::Vector2f center, SFUI::Float outerRadius, SFUI::Float startAngleDeg, SFUI::Float endAngleDeg) {
    SFUI::Float arcResolution = std::clamp(static_cast<SFUI::Int>(outerRadius * 0.25f), 4, 12);
    SFUI::Float startRadians = startAngleDeg * M_PI / 180.f;
    SFUI::Float endRadians = endAngleDeg * M_PI / 180.f;
    SFUI::Float angleStep = (endRadians - startRadians) / static_cast<SFUI::Float>(arcResolution);
    for (SFUI::Int i = 0; i <= arcResolution; ++i) {
        SFUI::Float angle = startRadians + i * angleStep;
        SFUI::Vector2f dir(std::cos(angle), std::sin(angle));
        SFUI::Vector2f outer = {center.x + dir.x * outerRadius, center.y + dir.y * outerRadius};
        SFUI::Vector2f inner = center;
        backgroundArcs.append({outer, computedStyle.fillColor});
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
 * @brief Generate a stroked arc as a triangle strip and return the VertexArray.
 * 
 * @param center The center of the arc to draw.
 * @param outerRadius The outer radius of the arc to draw.
 * @param innerRadius The inner radius of the arc to draw.
 * @param startAngleDeg The starting angle of the arc to draw.
 * @param endAngleDeg The ending angle of the arc to draw.
 */
SFUI::Void SFUI::Component::computeBorderArcGeometry(SFUI::Vector2f center, SFUI::Float outerRadius, SFUI::Float innerRadius, SFUI::Float startAngleDeg, SFUI::Float endAngleDeg) {
    SFUI::Float arcResolution = std::clamp(static_cast<SFUI::Int>(outerRadius * 0.25f), 4, 12);
    SFUI::Float startRadians = startAngleDeg * M_PI / 180.f;
    SFUI::Float endRadians = endAngleDeg * M_PI / 180.f;
    SFUI::Float angleStep = (endRadians - startRadians) / static_cast<SFUI::Float>(arcResolution);
    for (SFUI::Int i = 0; i <= arcResolution; ++i) {
        SFUI::Float angle = startRadians + i * angleStep;
        SFUI::Vector2f dir(std::cos(angle), std::sin(angle));
        SFUI::Vector2f outer = {center.x + dir.x * outerRadius, center.y + dir.y * outerRadius};
        SFUI::Vector2f inner = {center.x + dir.x * innerRadius, center.y + dir.y * innerRadius};
        borderArcs.append({outer, computedStyle.borderColor});
        borderArcs.append({inner, computedStyle.borderColor});
    }
}
 