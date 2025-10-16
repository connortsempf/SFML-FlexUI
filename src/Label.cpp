#include <sfml-flexui.hpp>




///////////////////////////////////////
// SFML-FlexUI Label Component Class //
///////////////////////////////////////



// Static Variables //
SFUI::Float SFUI::Label::VERTICAL_CENTER_OFFSET_FACTOR = 0.65f;
SFUI::Float SFUI::Label::VERTICAL_BOTTOM_OFFSET_FACTOR = 1.22f;


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Label::Label(const SFUI::String& componentID) :
    Component(componentID),
    labelStyle(),
    textObject(SFUI::Text(*(labelStyle.font), labelStyle.text, labelStyle.textSize))
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Label::Label(const SFUI::String& componentID, const Style& style) :
    Component(componentID, style),
    labelStyle(),
    textObject(SFUI::Text(*(labelStyle.font), labelStyle.text, labelStyle.textSize))
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Label::Label(const SFUI::String& componentID, const LabelStyle& labelStyle) :
    Component(componentID),
    labelStyle(labelStyle),
    textObject(SFUI::Text(*(labelStyle.font), labelStyle.text, labelStyle.textSize))
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Label::Label(const SFUI::String& componentID, const Layout& layout) :
    Component(componentID, layout),
    labelStyle(),
    textObject(SFUI::Text(*(labelStyle.font), labelStyle.text, labelStyle.textSize))
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 * @param .
 */
SFUI::Label::Label(const SFUI::String& componentID, const Style& style, const LabelStyle& labelStyle, const Layout& layout) :
    Component(componentID, style, layout),
    labelStyle(labelStyle),
    textObject(SFUI::Text(*(labelStyle.font), labelStyle.text, labelStyle.textSize))
{}


/**
 * @brief .
 */
SFUI::Void SFUI::Label::computeTextSize() {
    if (labelStyle.textSize > 0.0f)
        computedLabelStyle.textSize = labelStyle.textSize;
    else
        computedLabelStyle.textSize = 12.0f;
}


/**
 * @brief .
 */
SFUI::Void SFUI::Label::computeTextAlignHorizontal() {
    SFUI::String tempAlignHorizontal = labelStyle.textAlignHorizontal;
    std::transform(tempAlignHorizontal.begin(), tempAlignHorizontal.end(), tempAlignHorizontal.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempAlignHorizontal == "left" || tempAlignHorizontal == "center" || tempAlignHorizontal == "right")
        computedLabelStyle.textAlignHorizontal = tempAlignHorizontal;
    else
        computedLabelStyle.textAlignHorizontal = "center";
}


/**
 * @brief .
 */
SFUI::Void SFUI::Label::computeTextAlignVertical() {
    SFUI::String tempAlignVertical = labelStyle.textAlignVertical;
    std::transform(tempAlignVertical.begin(), tempAlignVertical.end(), tempAlignVertical.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempAlignVertical == "top" || tempAlignVertical == "center" || tempAlignVertical == "bottom")
        computedLabelStyle.textAlignVertical = tempAlignVertical;
    else
        computedLabelStyle.textAlignVertical = "center";
}


/**
 * @brief .
 */
SFUI::Void SFUI::Label::computeTextColor() {
    SFUI::Color computedTextColor;

    // If Input Text Color is a 3-Channel Unsigned Byte //
    if (std::holds_alternative<SFUI::Vector3ui8>(labelStyle.textColor)) {
        SFUI::Vector3ui8 textColor = std::get<SFUI::Vector3ui8>(labelStyle.textColor);
        computedTextColor = SFUI::Color(textColor.x, textColor.y, textColor.z);
    }
    // If Input Text Color is a 4-Channel Unsigned Byte //
    else if (std::holds_alternative<SFUI::Vector4ui8>(labelStyle.textColor)) {
        SFUI::Vector4ui8 textColor = std::get<SFUI::Vector4ui8>(labelStyle.textColor);
        computedTextColor = SFUI::Color(textColor.x, textColor.y, textColor.z, textColor.w);
    }
    // If Input Text Color is a Hex String //
    else if (std::holds_alternative<SFUI::String>(labelStyle.textColor)) {
        // Going to Do this Later -- Lots of String Parsing Code Needed //
    }
    // If Input Text Color is Already Given as a SFML::Color Type //
    else if (std::holds_alternative<SFUI::Color>(labelStyle.textColor)) {
        SFUI::Color textColor = std::get<SFUI::Color>(labelStyle.textColor);
        computedTextColor = textColor;
    }

    computedLabelStyle.textColor = computedTextColor;

    DEBUG_PRINT(
        componentID <<
        " Computed Text Color = " <<
        static_cast<int>(computedLabelStyle.textColor.r) << ", " <<
        static_cast<int>(computedLabelStyle.textColor.g) << ", " <<
        static_cast<int>(computedLabelStyle.textColor.b) << ", " <<
        static_cast<int>(computedLabelStyle.textColor.a)
    );
}


/**
 * @brief .
 */
SFUI::Void SFUI::Label::computeText() {
    if (!labelStyle.font) return;

    // Text Object //
    textObject.setFont(*labelStyle.font);
    textObject.setString(labelStyle.text);
    textObject.setCharacterSize(computedLabelStyle.textSize);
    textObject.setFillColor(computedLabelStyle.textColor);
    
    SFUI::Vector2f textPosition;
    // Horizontal Text Position //
    if (computedLabelStyle.textAlignHorizontal == "left")
        textPosition.x = computedLayout.position.x + computedLayout.padding;
    else if (computedLabelStyle.textAlignHorizontal == "center")
        textPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (textObject.getLocalBounds().size.x / 2.0f);
    else if (computedLabelStyle.textAlignHorizontal == "right")
        textPosition.x = computedLayout.position.x + computedLayout.size.x - textObject.getLocalBounds().size.x - computedLayout.padding;
    // Vertical Text Position //
    if (computedLabelStyle.textAlignVertical == "top")
        textPosition.y = computedLayout.position.y + computedLayout.padding;
    else if (computedLabelStyle.textAlignVertical == "center")
        textPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedLabelStyle.textSize * VERTICAL_CENTER_OFFSET_FACTOR);
    else if (computedLabelStyle.textAlignVertical == "bottom")
        textPosition.y = computedLayout.position.y + computedLayout.size.y - (computedLabelStyle.textSize * VERTICAL_BOTTOM_OFFSET_FACTOR) - computedLayout.padding;

    textObject.setPosition(textPosition);
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Label::update(const SFUI::Vector2u parentComponentSize) {
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

    computeTextSize();
    computeTextAlignHorizontal();
    computeTextAlignVertical();
    computeTextColor();
    computeText();

    std::cout << "Label Here\n";
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Label::handleEvent(const SFUI::Event& event) {

}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Label::draw(SFUI::RenderTarget& renderTarget) {
    renderTarget.draw(backgroundRects);
    renderTarget.draw(backgroundArcs);
    renderTarget.draw(borderRects);
    renderTarget.draw(borderArcs);
    renderTarget.draw(textObject);
}
