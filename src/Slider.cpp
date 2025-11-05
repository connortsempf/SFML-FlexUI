#include <sfml-flexui-types.hpp>
#include <sfml-flexui-core.hpp>


////////////////////////////////////////
// SFML-FlexUI Slider Component Class //
////////////////////////////////////////


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Slider::Slider(const SFUI::String& componentID) :
    Component(componentID),
    sliderStyle(),
    sliderState(),
    sliderBehavior(),
    unprogressedTrack(componentID + "_UnprogressedTrack"),
    progressedTrack(componentID + "_ProgressedTrack"),
    thumb(componentID + "_Thumb")
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Slider::Slider(const SFUI::String& componentID, const SFUI::PropGroup::Slider& togglePropGroup) :
    Component(componentID, togglePropGroup.layout, togglePropGroup.style),
    sliderStyle(togglePropGroup.sliderStyle),
    sliderState(togglePropGroup.sliderState),
    sliderBehavior(togglePropGroup.sliderBehavior),
    unprogressedTrack(componentID + "_UnprogressedTrack"),
    progressedTrack(componentID + "_ProgressedTrack"),
    thumb(componentID + "_Thumb")
{}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Slider::handleEvent(const SFUI::Event& event) {
    thumb.handleEvent(event);

    // Mouse Moved Event Handling //
    if (const SFUI::Event::MouseMoved* mouseMovedEvent = event.getIf<SFUI::Event::MouseMoved>()) {
        const SFUI::Vector2i mousePosition = SFUI::Vector2i(mouseMovedEvent->position.x, mouseMovedEvent->position.y);
        
        // Track Handling //
        const SFUI::Bool trackIsHovered = trackHovered(mousePosition);
        if (!isTrackHovered && trackIsHovered && sliderBehavior.onTrackHoverIn) sliderBehavior.onTrackHoverIn(componentID);
        if (isTrackHovered && !trackIsHovered && sliderBehavior.onTrackHoverOut) sliderBehavior.onTrackHoverOut(componentID);
        isTrackHovered = trackIsHovered;

        // Thumb Handling //
        if (isSliding) handleThumbMove(mousePosition);
    }

    // Mouse Button Pressed Event //
    else if (const SFUI::Event::MouseButtonPressed* mouseButtonPressedEvent = event.getIf<SFUI::Event::MouseButtonPressed>()) {
        const SFUI::Vector2i mousePosition = SFUI::Vector2i(mouseButtonPressedEvent->position.x, mouseButtonPressedEvent->position.y);
        const sf::Mouse::Button mouseButton = mouseButtonPressedEvent->button;

        // Track Handling //
        if (isTrackHovered && !isThumbHovered) {
            if (mouseButton == sf::Mouse::Button::Left) {
                isTrackLeftPressed = true;
                if (sliderBehavior.onThumbLeftPressIn) sliderBehavior.onThumbLeftPressIn(componentID);
                handleThumbMove(mousePosition);
            }
            else if (mouseButton == sf::Mouse::Button::Right) {
                isTrackRightPressed = true;
                if (sliderBehavior.onThumbRightPressIn) sliderBehavior.onThumbRightPressIn(componentID);
            }
            else if (mouseButton == sf::Mouse::Button::Middle) {
                isTrackMiddlePressed = true;
                if (sliderBehavior.onThumbMiddlePressIn) sliderBehavior.onThumbMiddlePressIn(componentID);
            }
        }
    }

    // Mouse Button Released Event //
    else if (const SFUI::Event::MouseButtonReleased* mouseButtonReleasedEvent = event.getIf<SFUI::Event::MouseButtonReleased>()) {
        const sf::Mouse::Button mouseButton = mouseButtonReleasedEvent->button;

        // Track Handling //
        if (isTrackHovered && !isThumbHovered) {
            if (mouseButton == sf::Mouse::Button::Left) {
                if (isTrackLeftPressed && sliderBehavior.onThumbLeftPress) sliderBehavior.onThumbLeftPress(componentID);
            }
            else if (mouseButton == sf::Mouse::Button::Right) {
                if (isTrackRightPressed && sliderBehavior.onThumbRightPress) sliderBehavior.onThumbRightPress(componentID);
            }
            else if (mouseButton == sf::Mouse::Button::Middle) {
                if (isTrackMiddlePressed && sliderBehavior.onThumbMiddlePress) sliderBehavior.onThumbMiddlePress(componentID);
            }
        }
        isTrackLeftPressed = isTrackRightPressed = isTrackMiddlePressed = false;

        // Thumb Handling //
        if (isSliding && sliderBehavior.onSlidingEnd) sliderBehavior.onSlidingEnd(componentID, sliderState.value); 
        isSliding = false;
    }
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Slider::update(const SFUI::Vector2u renderTargetSize) {
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
    
    // Slider Specific Computation //
    computeTrackAlign();
    computeValueDynamics();
    computeThumbSize();
    computeTrackWidth();
    computeTrackLayout();
    computeDynamicColors();
    computeComposedComponents();
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Slider::draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    drawTarget.draw(shadowRects);
    drawTarget.draw(shadowArcs);
    drawTarget.draw(backgroundRects);
    drawTarget.draw(backgroundArcs);
    drawTarget.draw(borderRects);
    drawTarget.draw(borderArcs);
    unprogressedTrack.draw(drawTarget, window);
    progressedTrack.draw(drawTarget, window);
    thumb.draw(drawTarget, window);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Slider::computeTrackAlign() {
    SFUI::String tempTrackAlign = sliderStyle.trackAlign;
    std::transform(tempTrackAlign.begin(), tempTrackAlign.end(), tempTrackAlign.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    
    if (tempTrackAlign == "horizontal" || tempTrackAlign == "vertical")
        computedSliderStyle.trackAlign = tempTrackAlign;
    else
        computedSliderStyle.trackAlign = "horizontal";
}


/**
 * @brief .
 */
SFUI::Void SFUI::Slider::computeValueDynamics() {
    // Sanitize Minimum and Maximum Values //
    if (sliderState.minimumValue > sliderState.maximumValue) std::swap(sliderState.minimumValue, sliderState.maximumValue);
    if (sliderState.minimumValue == sliderState.maximumValue) {
        sliderState.minimumValue = 0.0f;
        sliderState.maximumValue = 100.0f;
    }

    SFUI::Float valueRange = sliderState.maximumValue - sliderState.minimumValue;
    SFUI::UnsignedInt numSteps = static_cast<SFUI::UnsignedInt>(std::floor(valueRange / sliderState.step));
    if (numSteps <= 0) numSteps = 1u;
    SFUI::Float distanceInterval = 0.0f;
    if (computedSliderStyle.trackAlign == "horizontal")
        distanceInterval = ((computedLayout.position.x + computedLayout.size.x) - computedLayout.position.x) / static_cast<SFUI::Float>(numSteps);
    else if (computedSliderStyle.trackAlign == "vertical")
        distanceInterval = ((computedLayout.position.y + computedLayout.size.y) - computedLayout.position.y) / static_cast<SFUI::Float>(numSteps);

    // Calculate Interval Values //
    intervalValues.clear();
    for (int i = 0; i < numSteps + 1; i++)
        intervalValues.push_back(sliderState.minimumValue + (i * sliderState.step));

    // Calculate Interval Positions //
    intervalPositions.clear();
    for (int i = 0; i < numSteps + 1; i++) {
        SFUI::Float newPosition = 0.0f;
        if (computedSliderStyle.trackAlign == "horizontal")
            newPosition = !sliderState.isInverted ? computedLayout.position.x + (i * distanceInterval) : computedLayout.position.x + computedLayout.size.x - (i * distanceInterval);
        else if (computedSliderStyle.trackAlign == "vertical")
            newPosition = !sliderState.isInverted ? computedLayout.position.y + computedLayout.size.y - (i * distanceInterval) : computedLayout.position.y + (i * distanceInterval);
        intervalPositions.push_back(newPosition);
    }

    // React to Programmatic Changes to Value //
    SFUI::Float potentialNewValue = std::clamp(sliderState.value, sliderState.minimumValue, sliderState.maximumValue);
    SFUI::UnsignedInt nearestIndex = 0u;
    SFUI::Float smallestDistance = std::numeric_limits<SFUI::Float>::max();
    for (SFUI::UnsignedInt i = 0; i < intervalValues.size(); ++i) {
        SFUI::Float distance = std::abs(intervalValues[i] - potentialNewValue);
        if (distance < smallestDistance) {
            smallestDistance = distance;
            nearestIndex = i;
        }
    }
    SFUI::Float snappedValue = intervalValues.at(nearestIndex);
    if (sliderState.value != snappedValue && sliderBehavior.onValueChange) {
        sliderBehavior.onValueChange(componentID, snappedValue);
    }
    sliderState.value = snappedValue;
    if (computedSliderStyle.trackAlign == "horizontal") {
        thumbUpdatePosition = {
            intervalPositions[nearestIndex] - (computedSliderStyle.thumbSize.x / 2.0f),
            computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedSliderStyle.thumbSize.y / 2.0f)
        };
    }
    else if (computedSliderStyle.trackAlign == "vertical") {
        thumbUpdatePosition = {
            computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedSliderStyle.thumbSize.x / 2.0f),
            intervalPositions[nearestIndex] - (computedSliderStyle.thumbSize.y / 2.0f)
        };
    }
}


/**
 * @brief .
 */
SFUI::Void SFUI::Slider::computeThumbSize() {
    SFUI::Vector2f computedThumbSize = {0.0f, 0.0f};
    
    // Thumb Width //
    // If the Width Layout Variant Holds an Explicit Width Float //
    if (std::holds_alternative<SFUI::Float>(sliderStyle.thumbWidth)) {
        computedThumbSize.x = std::get<SFUI::Float>(sliderStyle.thumbWidth);
    }
    // If the Width Variant Holds a String Percentage Input //
    else if (std::holds_alternative<SFUI::String>(sliderStyle.thumbWidth)) {
        SFUI::String thumbWidthString = std::get<SFUI::String>(sliderStyle.thumbWidth);
        if (thumbWidthString.size() > 1 && thumbWidthString.back() == '%') {
            thumbWidthString.pop_back();
            try {
                size_t index = 0;
                SFUI::Double tempThumbWidth = std::stod(thumbWidthString, &index);
                if (index == thumbWidthString.size()) {
                    SFUI::Float relativeTrackWidthFactor = std::min(computedLayout.size.x, computedLayout.size.y);
                    computedThumbSize.x = relativeTrackWidthFactor * std::clamp(static_cast<SFUI::Float>(tempThumbWidth) / 100.0f, 0.0f, 0.5f);
                }   else {
                    computedThumbSize.x = 12.0f;
                }
            }   catch (...) {
                computedThumbSize.x = 12.0f;
            }
        }   else {
            computedThumbSize.x = 12.0f;
        }
    }

    // Thumb Height //
    // If the Height Variant Holds an Explicit Width Float //
    if (std::holds_alternative<SFUI::Float>(sliderStyle.thumbWidth)) {
        computedThumbSize.y = std::get<SFUI::Float>(sliderStyle.thumbWidth);
    }
    // If the Height Variant Holds a String Percentage Input //
    else if (std::holds_alternative<SFUI::String>(sliderStyle.thumbWidth)) {
        SFUI::String thumbHeightString = std::get<SFUI::String>(sliderStyle.thumbWidth);
        if (thumbHeightString.size() > 1 && thumbHeightString.back() == '%') {
            thumbHeightString.pop_back();
            try {
                size_t index = 0;
                SFUI::Double tempThumbHeight = std::stod(thumbHeightString, &index);
                if (index == thumbHeightString.size()) {
                    SFUI::Float relativeTrackWidthFactor = std::min(computedLayout.size.x, computedLayout.size.y);
                    computedThumbSize.y = relativeTrackWidthFactor * std::clamp(static_cast<SFUI::Float>(tempThumbHeight) / 100.0f, 0.0f, 0.5f);
                }   else {
                    computedThumbSize.y = 12.0f;
                }
            }   catch (...) {
                computedThumbSize.y = 12.0f;
            }
        }   else {
            computedThumbSize.y = 12.0f;
        }
    }

    computedSliderStyle.thumbSize = computedThumbSize;
}


/**
 * @brief .
 */
SFUI::Void SFUI::Slider::computeTrackWidth() {
    SFUI::Float computedTrackWidth = 0.0f;
    
    // Master Track Width //
    // If the Track Width Layout Variant Holds an Explicit Width Float //
    if (std::holds_alternative<SFUI::Float>(sliderStyle.trackWidth)) {
        computedTrackWidth = std::get<SFUI::Float>(sliderStyle.trackWidth);
    }
    // If the Focus Width Style Variant Holds a String Percentage Input //
    else if (std::holds_alternative<SFUI::String>(sliderStyle.trackWidth)) {
        SFUI::String trackWidthString = std::get<SFUI::String>(sliderStyle.trackWidth);
        if (trackWidthString.size() > 1 && trackWidthString.back() == '%') {
            trackWidthString.pop_back();
            try {
                size_t index = 0;
                SFUI::Double tempTrackWidth = std::stod(trackWidthString, &index);
                if (index == trackWidthString.size()) {
                    SFUI::Float relativeTrackWidthFactor = std::min(computedLayout.size.x, computedLayout.size.y);
                    computedTrackWidth = relativeTrackWidthFactor * std::clamp(static_cast<SFUI::Float>(tempTrackWidth) / 100.0f, 0.0f, 0.5f);
                }   else {
                    computedTrackWidth = 7.0f;
                }
            }   catch (...) {
                computedTrackWidth = 7.0f;
            }
        }   else {
            computedTrackWidth = 7.0f;
        }
    }
    computedSliderStyle.trackWidth = computedSliderStyle.trackProgressedWidth = computedTrackWidth;
    
    // Progressed Track Width //
    // If the Track Width Layout Variant Holds an Explicit Width Float //
    if (!sliderStyle.trackProgressedWidth.has_value()) return;
    SFUI::SubProp::Dimension trackProgressedWidth = sliderStyle.trackProgressedWidth.value();
    if (std::holds_alternative<SFUI::Float>(trackProgressedWidth)) {
        computedTrackWidth = std::get<SFUI::Float>(trackProgressedWidth);
    }
    // If the Focus Width Style Variant Holds a String Percentage Input //
    else if (std::holds_alternative<SFUI::String>(trackProgressedWidth)) {
        SFUI::String trackWidthString = std::get<SFUI::String>(trackProgressedWidth);
        if (trackWidthString.size() > 1 && trackWidthString.back() == '%') {
            trackWidthString.pop_back();
            try {
                size_t index = 0;
                SFUI::Double tempTrackWidth = std::stod(trackWidthString, &index);
                if (index == trackWidthString.size()) {
                    SFUI::Float relativeTrackWidthFactor = std::min(computedLayout.size.x, computedLayout.size.y);
                    computedTrackWidth = relativeTrackWidthFactor * std::clamp(static_cast<SFUI::Float>(tempTrackWidth) / 100.0f, 0.0f, 0.5f);
                }   else {
                    computedTrackWidth = 7.0f;
                }
            }   catch (...) {
                computedTrackWidth = 7.0f;
            }
        }   else {
            computedTrackWidth = 7.0f;
        }
    }
    computedSliderStyle.trackProgressedWidth = computedTrackWidth;
}


/**
 * @brief .
 */
SFUI::Void SFUI::Slider::computeTrackLayout() {
    SFUI::Vector2i thumbPosition = {static_cast<SFUI::Int>(thumbUpdatePosition.x), static_cast<SFUI::Int>(thumbUpdatePosition.y)};
    SFUI::Vector2f thumbCenterPosition = {
        static_cast<SFUI::Float>(thumbPosition.x) + (computedSliderStyle.thumbSize.x / 2.0f),
        static_cast<SFUI::Float>(thumbPosition.y) + (computedSliderStyle.thumbSize.y / 2.0f)
    };

    if (computedSliderStyle.trackAlign == "horizontal") {
        unprogressedTrack.layout.width = computedLayout.size.x;
        unprogressedTrack.layout.height = computedSliderStyle.trackWidth;
        unprogressedTrack.layout.xPosition = computedLayout.position.x;
        unprogressedTrack.layout.yPosition = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedSliderStyle.trackWidth / 2.0f);
        progressedTrack.layout.height = computedSliderStyle.trackProgressedWidth;
        progressedTrack.layout.yPosition = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedSliderStyle.trackProgressedWidth / 2.0f);
        thumb.layout.yPosition = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedSliderStyle.thumbSize.y / 2.0f);

        if (!sliderState.isInverted) {
            progressedTrack.layout.xPosition = computedLayout.position.x;
            progressedTrack.layout.width = thumbCenterPosition.x - computedLayout.position.x;
        }
        else if (sliderState.isInverted) {
            progressedTrack.layout.xPosition = thumbCenterPosition.x + 1;
            progressedTrack.layout.width = (computedLayout.position.x + computedLayout.size.x) - thumbCenterPosition.x;
        }
    }
    else if (computedSliderStyle.trackAlign == "vertical") {
        unprogressedTrack.layout.width = computedSliderStyle.trackWidth;
        unprogressedTrack.layout.height = computedLayout.size.y;
        unprogressedTrack.layout.xPosition = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedSliderStyle.trackWidth / 2.0f);
        unprogressedTrack.layout.yPosition = computedLayout.position.y;
        progressedTrack.layout.width = computedSliderStyle.trackProgressedWidth;
        progressedTrack.layout.xPosition = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedSliderStyle.trackProgressedWidth / 2.0f);
        thumb.layout.xPosition = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedSliderStyle.thumbSize.x / 2.0f);

        if (!sliderState.isInverted) {
            progressedTrack.layout.yPosition = thumbCenterPosition.y + 1;
            progressedTrack.layout.height = (computedLayout.position.y + computedLayout.size.y) - thumbCenterPosition.y;
        }
        else if (sliderState.isInverted) {
            progressedTrack.layout.yPosition = computedLayout.position.y;
            progressedTrack.layout.height = thumbCenterPosition.y - computedLayout.position.y;
        }
    }
}


/**
 * @brief .
 */
SFUI::Void SFUI::Slider::computeDynamicColors() {
    unprogressedTrack.style.fillColor = sliderStyle.trackFillColor;
    unprogressedTrack.style.borderColor = sliderStyle.trackBorderColor;
    progressedTrack.style.fillColor = sliderStyle.trackFillColor;
    progressedTrack.style.borderColor = sliderStyle.trackBorderColor;
    thumb.style.fillColor = sliderStyle.thumbFillColor;
    thumb.style.borderColor = sliderStyle.thumbBorderColor;

    if (sliderStyle.trackUnprogressedFillColor.has_value())
        unprogressedTrack.style.fillColor = sliderStyle.trackUnprogressedFillColor.value();
    if (sliderStyle.trackUnprogressedBorderColor.has_value())
        unprogressedTrack.style.borderColor = sliderStyle.trackUnprogressedBorderColor.value();
    if (sliderStyle.trackProgressedFillColor.has_value())
        progressedTrack.style.fillColor = sliderStyle.trackProgressedFillColor.value();
    if (sliderStyle.trackProgressedBorderColor.has_value())
        progressedTrack.style.borderColor = sliderStyle.trackProgressedBorderColor.value();
    
    if (sliderState.isDisabled) {
        if (sliderStyle.trackUnprogressedDisabledFillColor.has_value())
            unprogressedTrack.style.fillColor = sliderStyle.trackUnprogressedDisabledFillColor.value();
        if (sliderStyle.trackUnprogressedDisabledBorderColor.has_value())
            unprogressedTrack.style.borderColor = sliderStyle.trackUnprogressedDisabledBorderColor.value();
        if (sliderStyle.trackProgressedDisabledFillColor.has_value())
            progressedTrack.style.fillColor = sliderStyle.trackProgressedDisabledFillColor.value();
        if (sliderStyle.trackProgressedDisabledBorderColor.has_value())
            progressedTrack.style.borderColor = sliderStyle.trackProgressedDisabledBorderColor.value();
        if (sliderStyle.thumbDisabledFillColor.has_value())
            thumb.style.fillColor = sliderStyle.thumbDisabledFillColor.value();
        if (sliderStyle.thumbDisabledBorderColor.has_value())
            thumb.style.borderColor = sliderStyle.thumbDisabledBorderColor.value();
    }
    else if (isTrackLeftPressed || isTrackRightPressed || isTrackMiddlePressed || isThumbLeftPressed || isThumbRightPressed || isThumbMiddlePressed) {
        if (sliderStyle.trackUnprogressedPressedFillColor.has_value())
            unprogressedTrack.style.fillColor = sliderStyle.trackUnprogressedPressedFillColor.value();
        if (sliderStyle.trackUnprogressedPressedBorderColor.has_value())
            unprogressedTrack.style.borderColor = sliderStyle.trackUnprogressedPressedBorderColor.value();
        if (sliderStyle.trackProgressedPressedFillColor.has_value())
            progressedTrack.style.fillColor = sliderStyle.trackProgressedPressedFillColor.value();
        if (sliderStyle.trackProgressedPressedBorderColor.has_value())
            progressedTrack.style.borderColor = sliderStyle.trackProgressedPressedBorderColor.value();
        if (sliderStyle.thumbPressedFillColor.has_value())
            thumb.style.fillColor = sliderStyle.thumbPressedFillColor.value();
        if (sliderStyle.thumbPressedBorderColor.has_value())
            thumb.style.borderColor = sliderStyle.thumbPressedBorderColor.value();
    }
    else if (isTrackHovered || isThumbHovered) {
        if (sliderStyle.trackUnprogressedHoveredFillColor.has_value())
            unprogressedTrack.style.fillColor = sliderStyle.trackUnprogressedHoveredFillColor.value();
        if (sliderStyle.trackUnprogressedHoveredBorderColor.has_value())
            unprogressedTrack.style.borderColor = sliderStyle.trackUnprogressedHoveredBorderColor.value();
        if (sliderStyle.trackProgressedHoveredFillColor.has_value())
            progressedTrack.style.fillColor = sliderStyle.trackProgressedHoveredFillColor.value();
        if (sliderStyle.trackProgressedHoveredBorderColor.has_value())
            progressedTrack.style.borderColor = sliderStyle.trackProgressedHoveredBorderColor.value();
        if (sliderStyle.thumbHoveredFillColor.has_value())
            thumb.style.fillColor = sliderStyle.thumbHoveredFillColor.value();
        if (sliderStyle.thumbHoveredBorderColor.has_value())
            thumb.style.borderColor = sliderStyle.thumbHoveredBorderColor.value();
    }
}


/**
 * @brief .
 */
SFUI::Void SFUI::Slider::computeComposedComponents() {
    unprogressedTrack.style.cornerRadius = sliderStyle.trackCornerRadius;
    unprogressedTrack.style.shadowOffset = sliderStyle.shadowOffset;
    unprogressedTrack.style.shadowRadius = sliderStyle.shadowRadius;
    unprogressedTrack.style.shadowFillColor = sliderStyle.shadowFillColor;
    unprogressedTrack.update(renderTargetSize);
    
    progressedTrack.style.cornerRadius = sliderStyle.trackCornerRadius;
    progressedTrack.update(renderTargetSize);

    thumb.layout.width = computedSliderStyle.thumbSize.x;
    thumb.layout.height = computedSliderStyle.thumbSize.y;
    thumb.layout.xPosition = thumbUpdatePosition.x;
    thumb.layout.yPosition = thumbUpdatePosition.y;
    thumb.style.cornerRadius = sliderStyle.thumbCornerRadius;
    thumb.buttonStyle.focusWidth = sliderStyle.focusWidth;
    thumb.buttonStyle.focusOffset = sliderStyle.focusOffset;
    thumb.buttonStyle.focusCornerRadius = sliderStyle.focusCornerRadius;
    thumb.buttonStyle.focusFillColor = sliderStyle.focusFillColor;
    thumb.buttonStyle.toolTipPadding = sliderStyle.toolTipPadding;
    thumb.buttonStyle.toolTipCornerRadius = sliderStyle.toolTipCornerRadius;
    thumb.buttonStyle.toolTipText = sliderStyle.toolTipText;
    thumb.buttonStyle.toolTipFont = sliderStyle.toolTipFont;
    thumb.buttonStyle.toolTipTextSize = sliderStyle.toolTipTextSize;
    thumb.buttonStyle.toolTipFillColor = sliderStyle.toolTipFillColor;
    thumb.buttonStyle.toolTipTextColor = sliderStyle.toolTipTextColor;
    thumb.buttonState.isDisabled = sliderState.isDisabled;
    thumb.buttonState.isFocused = sliderState.isFocused;
    thumb.buttonBehavior.onEnable = [this](const SFUI::String& componentID) {
        if (sliderBehavior.onEnable) sliderBehavior.onEnable(componentID);    
    };
    thumb.buttonBehavior.onDisable = [this](const SFUI::String& componentID) {
        if (sliderBehavior.onDisable) sliderBehavior.onDisable(componentID);
    };
    thumb.buttonBehavior.onFocus = [this](const SFUI::String& componentID) {
        if (sliderBehavior.onFocus) sliderBehavior.onFocus(componentID);
    };
    thumb.buttonBehavior.onBlur = [this](const SFUI::String& componentID) {
        if (sliderBehavior.onBlur) sliderBehavior.onBlur(componentID);
    };
    thumb.buttonBehavior.onHoverIn = [this](const SFUI::String& componentID) {
        if (sliderBehavior.onThumbHoverIn) sliderBehavior.onThumbHoverIn(componentID);
        isThumbHovered = true;
    };
    thumb.buttonBehavior.onHoverOut = [this](const SFUI::String& componentID) {
        if (sliderBehavior.onThumbHoverOut) sliderBehavior.onThumbHoverOut(componentID);
        isThumbHovered = false;
    };
    thumb.buttonBehavior.onLeftPressIn = [this](const SFUI::String& componentID) {
        if (sliderBehavior.onThumbLeftPressIn) sliderBehavior.onThumbLeftPressIn(componentID);
        if (sliderBehavior.onSlidingStart) sliderBehavior.onSlidingStart(componentID, sliderState.value);
        isThumbLeftPressed = true;
        isSliding = true;
    };
    thumb.buttonBehavior.onLeftPress = [this](const SFUI::String& componentID) {
        if (sliderBehavior.onThumbLeftPress) sliderBehavior.onThumbLeftPress(componentID);
        if (sliderBehavior.onSlidingEnd) sliderBehavior.onSlidingEnd(componentID, sliderState.value);
        isThumbLeftPressed = false;
        isSliding = false;
    };
    thumb.buttonBehavior.onRightPressIn = [this](const SFUI::String& componentID) {
        if (sliderBehavior.onThumbRightPressIn) sliderBehavior.onThumbRightPressIn(componentID);
    };
    thumb.buttonBehavior.onRightPress = [this](const SFUI::String& componentID) {
        if (sliderBehavior.onThumbRightPress) sliderBehavior.onThumbRightPress(componentID);
    };
    thumb.buttonBehavior.onMiddlePressIn = [this](const SFUI::String& componentID) {
        if (sliderBehavior.onThumbMiddlePressIn) sliderBehavior.onThumbMiddlePressIn(componentID);
    };
    thumb.buttonBehavior.onMiddlePress = [this](const SFUI::String& componentID) {
        if (sliderBehavior.onThumbMiddlePress) sliderBehavior.onThumbMiddlePress(componentID);
    };

    thumb.update(renderTargetSize);
}


/**
 * @brief .
 * 
 * @param .
 * 
 * @return .
 */
SFUI::Bool SFUI::Slider::trackHovered(SFUI::Vector2i mousePosition) {
    return (
        mousePosition.x >= unprogressedTrack.getPosition().x && mousePosition.x <= (unprogressedTrack.getPosition().x + unprogressedTrack.getSize().x) &&
        mousePosition.y >= unprogressedTrack.getPosition().y && mousePosition.y <= (unprogressedTrack.getPosition().y + unprogressedTrack.getSize().y)
    );
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Slider::handleThumbMove(SFUI::Vector2i mousePosition) {
    SFUI::Float oldValue = sliderState.value;

    // Track Thumb Moving and New Value //
    SFUI::Float mouseClampDirection = 0.0f;
    if (computedSliderStyle.trackAlign == "horizontal")
        mouseClampDirection = mousePosition.x;
    else if (computedSliderStyle.trackAlign == "vertical")
        mouseClampDirection = mousePosition.y;
    
    SFUI::Float clampedMouse = 0.0f;
    if (computedSliderStyle.trackAlign == "horizontal") {
        if (!sliderState.isInverted)
            clampedMouse = std::clamp(mouseClampDirection, intervalPositions.front(), intervalPositions.back());
        else if (sliderState.isInverted)
            clampedMouse = std::clamp(mouseClampDirection, intervalPositions.back(), intervalPositions.front());
    }
    else if (computedSliderStyle.trackAlign == "vertical") {
        if (!sliderState.isInverted)
            clampedMouse = std::clamp(mouseClampDirection, intervalPositions.back(), intervalPositions.front());
        else if (sliderState.isInverted)
            clampedMouse = std::clamp(mouseClampDirection, intervalPositions.front(), intervalPositions.back());
    }
    

    
    // Find Nearest Interval Index //
    SFUI::UnsignedInt nearestIndex = 0u;
    SFUI::Float smallestDistance = std::numeric_limits<SFUI::Float>::max();

    for (SFUI::UnsignedInt i = 0; i < intervalPositions.size(); ++i) {
        SFUI::Float distance = std::abs(intervalPositions[i] - clampedMouse);
        if (distance < smallestDistance) {
            smallestDistance = distance;
            nearestIndex = i;
        }
    }

    SFUI::Float newValue = intervalValues.at(nearestIndex);
    if (oldValue != newValue && sliderBehavior.onValueChange) sliderBehavior.onValueChange(componentID, newValue); 

    // Update Slider Value and Thumb Position //
    sliderState.value = newValue;
    if (computedSliderStyle.trackAlign == "horizontal") {
        thumbUpdatePosition = {
            intervalPositions[nearestIndex] - (computedSliderStyle.thumbSize.x / 2.0f),
            computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedSliderStyle.thumbSize.y / 2.0f)
        };
    }
    else if (computedSliderStyle.trackAlign == "vertical") {
        thumbUpdatePosition = {
            computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedSliderStyle.thumbSize.x / 2.0f),
            intervalPositions[nearestIndex] - (computedSliderStyle.thumbSize.y / 2.0f)
        };
    }
}
