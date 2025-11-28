/**
 * @file Slider.cpp
 * @brief Implements the SFUI Slider component. 
 * @author Connor Sempf
 * @date 2025-11-15
 * @version 1.0
 *
 * This file contains the function definitions and internal logic for the
 * SFUI Slider component. It handles:
 *   - Construction and destruction
 *   - Event handling (dragging, value changes)
 *   - Rendering and style updates
 *
 * It works in conjunction with Slider.hpp to provide full functionality
 * of the Slider component.
 */


#include "components/Slider.hpp"


/**
 * @brief The constrctor for the slider component.
 * 
 * @param componentID The unique identifier for the slider component.
 */
SFUI::Slider::Slider(SFUI::String componentID) :
    Component(std::move(componentID)),
    unprogressedTrack(componentID + "_UnprogressedTrack"),
    progressedTrack(componentID + "_ProgressedTrack"),
    thumb(componentID + "_Thumb")
{}


/**
 * @brief Handle a user input event.
 * 
 * @param event The event to handle.
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
        
        if (mouseButton == sf::Mouse::Button::Left) sliderState.isFocused = false;

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

    // Key Pressed Event //
    if (const SFUI::Event::KeyPressed* keyPressedEvent = event.getIf<SFUI::Event::KeyPressed>()) {
        if (sliderState.isFocused) {
            SFUI::Float minValue = std::min(intervalValues.front(), intervalValues.back());
            SFUI::Float maxValue = std::max(intervalValues.front(), intervalValues.back());
            if (
                ((sliderStyle.trackAlign == "horizontal") &&
                ((keyPressedEvent->code == sf::Keyboard::Key::Left && !sliderState.isInverted) ||
                (keyPressedEvent->code == sf::Keyboard::Key::Right && sliderState.isInverted))) ||
                ((sliderStyle.trackAlign == "vertical") &&
                (keyPressedEvent->code == sf::Keyboard::Key::Down && !sliderState.isInverted) ||
                (keyPressedEvent->code == sf::Keyboard::Key::Up && sliderState.isInverted))) {   
                    sliderState.value = std::clamp(sliderState.value - sliderState.step, minValue, maxValue);
            }
            else if (
                ((sliderStyle.trackAlign == "horizontal") &&
                ((keyPressedEvent->code == sf::Keyboard::Key::Right && !sliderState.isInverted) ||
                (keyPressedEvent->code == sf::Keyboard::Key::Left && sliderState.isInverted))) ||
                (sliderStyle.trackAlign == "vertical") &&
                ((keyPressedEvent->code == sf::Keyboard::Key::Up && !sliderState.isInverted) ||
                (keyPressedEvent->code == sf::Keyboard::Key::Down && sliderState.isInverted))) {
                    sliderState.value = std::clamp(sliderState.value + sliderState.step, minValue, maxValue);        
            }
        }
    }
}


/**
 * @brief Recalculate the slider component's properties.
 * 
 * @param renderTargetSize The size of the render target the component is being drawn to.
 */
SFUI::Void SFUI::Slider::update(const SFUI::Vector2u renderTargetSize) {
    if (
        this->renderTargetSize != renderTargetSize ||
        layout != dirtyLayout ||
        style != dirtyStyle ||
        sliderStyle != dirtySliderStyle ||
        sliderState != dirtySliderState ||
        dirtyEvent
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
        computeTrackAlign();
        computeValueDynamics();
        computeDynamicColors();
        computeThumb();
        computeTracks();
    }
    this->renderTargetSize = renderTargetSize;
    dirtyLayout = layout;
    dirtyStyle = style;
    dirtySliderStyle = sliderStyle;
    dirtySliderState = sliderState;
}


/**
 * @brief Draw the slider component to a render target.
 * 
 * @param drawTarget The render target to draw to.
 * @param window The render window associated with the render target.
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
 * @brief Draw the component or inner components on an overlay layer on top of the main UI tree to the render target.
 * 
 * This is relevant for components that are actively animating and do not want their drawn geometry subject to
 * clipping by their parents' bounds. It is also useful for inner components like tooltips, context menus, modals,
 * and other special UI components. This meant to have a seperate second draw pass after the initial UI tree draw()
 * function calls to the components.
 * 
 * @param drawTarget Target to draw on.
 * @param window Window reference.
 */
SFUI::Void SFUI::Slider::drawOverlay(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    thumb.drawOverlay(drawTarget, window);
}


/**
 * @brief Compute the alignment of the tracks within its bounding box.
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
 * @brief Compute the value dynamics of the slider component.
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
        thumbUpdatePosition.x = intervalPositions[nearestIndex] - (computedSliderStyle.thumbSize.x / 2.0f);
        thumbUpdatePosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedSliderStyle.thumbSize.y / 2.0f);
    }
    else if (computedSliderStyle.trackAlign == "vertical") {
        thumbUpdatePosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedSliderStyle.thumbSize.x / 2.0f);
        thumbUpdatePosition.y = intervalPositions[nearestIndex] - (computedSliderStyle.thumbSize.y / 2.0f);
    }
}


/**
 * @brief Compute the dynamic colors of the slider component.
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
 * @brief Compute the thumb properties of the slider component.
 */
SFUI::Void SFUI::Slider::computeThumb() {
    // Thumb Size //
    SFUI::Vector2f computedThumbSize = {12.0f, 12.0f};
    if (std::holds_alternative<SFUI::Float>(sliderStyle.thumbWidth))
        computedThumbSize.x = std::get<SFUI::Float>(sliderStyle.thumbWidth);
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
                }
            }   catch (...) {}
        }
    }
    if (std::holds_alternative<SFUI::Float>(sliderStyle.thumbWidth))
        computedThumbSize.y = std::get<SFUI::Float>(sliderStyle.thumbWidth);
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
                }
            }   catch (...) {}
        }
    }
    computedSliderStyle.thumbSize = computedThumbSize;
    
    // Thumb Layout //
    thumb.layout.width = computedThumbSize.x;
    thumb.layout.height = computedThumbSize.y;
    if (computedSliderStyle.trackAlign == "horizontal") {
        thumb.layout.xPosition = thumbUpdatePosition.x;
        thumb.layout.yPosition = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedThumbSize.y / 2.0f);
    }
    else if (computedSliderStyle.trackAlign == "vertical") {
        thumb.layout.xPosition = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedThumbSize.x / 2.0f);
        thumb.layout.yPosition = thumbUpdatePosition.y;
    }
    
    // Thumb Style //
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

    // Thumb State //
    thumb.buttonState.isDisabled = sliderState.isDisabled;
    thumb.buttonState.isFocused = sliderState.isFocused;

    // Thumb Behavior //
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
        sliderState.isFocused = false;
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
    thumb.buttonBehavior.onKeyPress = [this](SFUI::String componentID, sf::Keyboard::Key pressedKey) {
        if (sliderBehavior.onKeyPress) sliderBehavior.onKeyPress(componentID, pressedKey);
    };

    // Update //
    thumb.update(renderTargetSize);
}


/**
 * @brief Compute the thumb properties of the slider component.
 */
SFUI::Void SFUI::Slider::computeTracks() {
    // Master Track Width //
    SFUI::Float computedTrackWidth = 7.0f;
    if (std::holds_alternative<SFUI::Float>(sliderStyle.trackWidth))
        computedTrackWidth = std::get<SFUI::Float>(sliderStyle.trackWidth);
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
                }
            }   catch (...) {}
        }
    }
    
    // Progressed Track Width //
    SFUI::Float computedProgressedTrackWidth = computedTrackWidth;
    if (sliderStyle.trackProgressedWidth.has_value()) {
        SFUI::SubProp::Dimension trackProgressedWidth = sliderStyle.trackProgressedWidth.value();
        if (std::holds_alternative<SFUI::Float>(trackProgressedWidth))
            computedTrackWidth = std::get<SFUI::Float>(trackProgressedWidth);
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
                    }
                }   catch (...) {}
            }
        }
    }

    // Track Size and Positions //
    SFUI::Vector2i thumbPosition = {static_cast<SFUI::Int>(thumbUpdatePosition.x), static_cast<SFUI::Int>(thumbUpdatePosition.y)};
    SFUI::Vector2f thumbCenterPosition = {
        static_cast<SFUI::Float>(thumbPosition.x) + (computedSliderStyle.thumbSize.x / 2.0f),
        static_cast<SFUI::Float>(thumbPosition.y) + (computedSliderStyle.thumbSize.y / 2.0f)
    };
    if (computedSliderStyle.trackAlign == "horizontal") {
        unprogressedTrack.layout.width = computedLayout.size.x;
        unprogressedTrack.layout.height = computedTrackWidth;
        unprogressedTrack.layout.xPosition = computedLayout.position.x;
        unprogressedTrack.layout.yPosition = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedTrackWidth / 2.0f);
        progressedTrack.layout.height = computedProgressedTrackWidth;
        progressedTrack.layout.yPosition = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedProgressedTrackWidth / 2.0f);
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
        unprogressedTrack.layout.width = computedTrackWidth;
        unprogressedTrack.layout.height = computedLayout.size.y;
        unprogressedTrack.layout.xPosition = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedTrackWidth / 2.0f);
        unprogressedTrack.layout.yPosition = computedLayout.position.y;
        progressedTrack.layout.width = computedProgressedTrackWidth;
        progressedTrack.layout.xPosition = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedProgressedTrackWidth / 2.0f);
        if (!sliderState.isInverted) {
            progressedTrack.layout.yPosition = thumbCenterPosition.y + 1;
            progressedTrack.layout.height = (computedLayout.position.y + computedLayout.size.y) - thumbCenterPosition.y;
        }
        else if (sliderState.isInverted) {
            progressedTrack.layout.yPosition = computedLayout.position.y;
            progressedTrack.layout.height = thumbCenterPosition.y - computedLayout.position.y;
        }
    }

    // Track Style //
    unprogressedTrack.style.cornerRadius = sliderStyle.trackCornerRadius;
    unprogressedTrack.style.shadowOffset = sliderStyle.shadowOffset;
    unprogressedTrack.style.shadowRadius = sliderStyle.shadowRadius;
    unprogressedTrack.style.shadowFillColor = sliderStyle.shadowFillColor;
    progressedTrack.style.cornerRadius = sliderStyle.trackCornerRadius;

    // Update //
    unprogressedTrack.update(renderTargetSize);
    progressedTrack.update(renderTargetSize);
}


/**
 * @brief Check if the track is being hovered by the mouse.
 * 
 * @param mousePosition The current mouse position.
 * 
 * @return True if the track is being hovered, false otherwise.
 */
SFUI::Bool SFUI::Slider::trackHovered(SFUI::Vector2i mousePosition) {
    return (
        mousePosition.x >= unprogressedTrack.getPosition().x && mousePosition.x <= (unprogressedTrack.getPosition().x + unprogressedTrack.getSize().x) &&
        mousePosition.y >= unprogressedTrack.getPosition().y && mousePosition.y <= (unprogressedTrack.getPosition().y + unprogressedTrack.getSize().y)
    );
}


/**
 * @brief Handle thumb movement and update the slider value accordingly.
 * 
 * @param mousePosition The current mouse position.
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
        thumbUpdatePosition.x = intervalPositions[nearestIndex] - (computedSliderStyle.thumbSize.x / 2.0f);
        thumbUpdatePosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedSliderStyle.thumbSize.y / 2.0f);
    }
    else if (computedSliderStyle.trackAlign == "vertical") {
        thumbUpdatePosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedSliderStyle.thumbSize.x / 2.0f);
        thumbUpdatePosition.y = intervalPositions[nearestIndex] - (computedSliderStyle.thumbSize.y / 2.0f);
    }
}
