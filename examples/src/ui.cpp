#include "ui.hpp"


// Static Global Props //
static SFUI::PropSet::Container rootContainerProps;
static SFUI::PropSet::Container secondContainerProps;
static SFUI::PropSet::Label headerLabelProps;
static SFUI::PropSet::Container midContainerProps;
static SFUI::PropSet::TextField textFieldProps;
static SFUI::PropSet::Slider sliderProps;
static SFUI::PropSet::ScrollContainer scrollContainerProps;
static SFUI::PropSet::Button scrollButtonProps1;
static SFUI::PropSet::Label scrollButtonLabelProps;
static SFUI::PropSet::Toggle scrollToggleProps;
static SFUI::PropSet::Label scrollToggleLabelProps;
static SFUI::PropSet::Graphic scrollToggleGraphicProps;
static SFUI::PropSet::Button scrollButtonProps2;



// Create the Props for the UI Components //
void buildProps(SFUI::SharedPointer<SFUI::Font> font, SFUI::SharedPointer<SFUI::Texture> texture) {

    // Root Container Props //
    rootContainerProps.layout.alignDirection = "vertical";
    rootContainerProps.layout.alignPrimary = "start";
    rootContainerProps.layout.alignSecondary = "center";
    rootContainerProps.layout.width = "100%";
    rootContainerProps.layout.height = "100%";
    rootContainerProps.layout.padding = 5.0f;
    rootContainerProps.style.fillColor = SFUI::Color(32, 32, 32);

    // Second Container Props //
    secondContainerProps.layout.width = "80%";
    secondContainerProps.layout.height = "80%";
    secondContainerProps.style.cornerRadius = 10.0f;
    secondContainerProps.style.fillColor = SFUI::Color(70, 70, 70);

    // Header Label Props //
    headerLabelProps.layout.width = "100%";
    headerLabelProps.layout.height = 40.0f;
    headerLabelProps.style.text = "SFML-FlexUI Demo";
    headerLabelProps.style.font = font;
    headerLabelProps.style.textSize = 30.0f;
    headerLabelProps.style.textAlignHorizontal = "center";
    headerLabelProps.style.textColor = "#FFFFFF";

    // Mid Container Props //
    midContainerProps.layout.alignDirection = "vertical";
    midContainerProps.layout.alignPrimary = "center";
    midContainerProps.layout.alignSecondary = "center";
    midContainerProps.layout.width = "100%";
    midContainerProps.layout.height = "35%";
    midContainerProps.layout.padding = 5.0f;
    midContainerProps.style.fillColor = SFUI::Vector3ui8{77, 74, 52};

    // Text Field Props //
    textFieldProps.layout.width = "100%";
    textFieldProps.layout.height = 70.0f;
    textFieldProps.layout.margin = 5.0f;
    textFieldProps.style.borderWidth = 4.0f;
    textFieldProps.style.cornerRadius = 8.0f;
    textFieldProps.style.fillColor = "#FFFFFF";
    textFieldProps.style.borderColor = SFUI::Color(150, 150, 150),
    textFieldProps.style.textInset = SFUI::Prop::Vector4dim{10.0f, 10.0f, 0.0f, 0.0f};
    textFieldProps.style.placeholderText = "Type Here...";
    textFieldProps.style.font = font;
    textFieldProps.style.textSize = 20.0f;
    textFieldProps.style.textColor = "#000000";
    textFieldProps.style.placeholderTextColor = SFUI::Color(120, 120, 120);
    textFieldProps.style.toolTipText = "Tooltip Text";
    textFieldProps.style.toolTipFont = font;
    textFieldProps.style.toolTipFillColor = SFUI::Color(20, 20, 20);
    textFieldProps.style.toolTipTextColor = SFUI::Color(255, 255, 255);
    textFieldProps.style.caretFillColor = SFUI::Color(79, 168, 255);

    // Slider Props //
    sliderProps.layout.width = "100%";
    sliderProps.layout.height = "40%";
    sliderProps.layout.margin = 5.0f;
    sliderProps.style.cornerRadius = 5.0f;
    sliderProps.style.thumbWidth = 14.0f;
    sliderProps.style.thumbHeight = 14.0f;
    sliderProps.style.fillColor = SFUI::Color(60, 60, 60);
    sliderProps.style.trackUnprogressedFillColor = SFUI::Color(150, 150, 150);
    sliderProps.style.trackProgressedFillColor = SFUI::Color(235, 83, 85, 150);
    sliderProps.style.thumbFillColor = SFUI::Color(235, 83, 85);
    sliderProps.state.value = 5.0f;
    sliderProps.state.minimumValue = 0.0f;
    sliderProps.state.maximumValue = 20.0f;
    sliderProps.state.step = 1.0f;

    // Scroll Container Props //
    scrollContainerProps.layout.alignDirection = "vertical";
    scrollContainerProps.layout.alignPrimary = "start";
    scrollContainerProps.layout.alignSecondary = "center";
    scrollContainerProps.layout.width = "100%";
    scrollContainerProps.layout.height = "auto";
    scrollContainerProps.layout.padding = SFUI::Prop::Vector4dim{3.0f, 3.0f, 0.0f, 0.0f};

    // Scroll Button 1 Props //
    scrollButtonProps1.layout.alignPrimary = "center";
    scrollButtonProps1.layout.alignSecondary = "center";
    scrollButtonProps1.layout.width = "100%";
    scrollButtonProps1.layout.height = 70.0f;
    // scrollButtonProps1.layout.margin = SFUI::Prop::Vector4dim{0.0f, 0.0f, 20.0f, 1.0f};
    scrollButtonProps1.style.cornerRadius = 5.0f;
    scrollButtonProps1.style.fillColor = SFUI::Color(50, 50, 50);
    scrollButtonProps1.style.hoveredFillColor = SFUI::Color(60, 60, 60);
    scrollButtonProps1.style.pressedFillColor = SFUI::Color(70, 70, 70);
    scrollButtonProps1.style.toolTipText = "Button Tool Tip";
    scrollButtonProps1.style.toolTipFont = font;
    scrollButtonProps1.style.toolTipFillColor = SFUI::Color(20, 20, 20);
    scrollButtonProps1.style.toolTipTextColor = SFUI::Color(255, 255, 255);
    scrollButtonProps1.style.toolTipCornerRadius = 5.0f;

    // Scroll Button Label Props //
    scrollButtonLabelProps.layout.width = "100%";
    scrollButtonLabelProps.layout.height = "100%";
    scrollButtonLabelProps.style.text = "Button Text";
    scrollButtonLabelProps.style.font = font;
    scrollButtonLabelProps.style.textSize = 20.0f;
    scrollButtonLabelProps.style.textAlignHorizontal = "center";
    scrollButtonLabelProps.style.textColor = SFUI::Color(255, 255, 255);

    // Scroll Toggle Props //
    scrollToggleProps.layout.alignPrimary = "center";
    scrollToggleProps.layout.alignSecondary = "center";
    scrollToggleProps.layout.width = "50%";
    scrollToggleProps.layout.height = 70.0f;
    scrollToggleProps.layout.margin = SFUI::Prop::Vector4dim{1.0f, 1.0f, 0.0f, 0.0f};
    scrollToggleProps.style.cornerRadius = 5.0f;
    scrollToggleProps.style.offFillColor = SFUI::Color(150, 150, 150);
    scrollToggleProps.style.onFillColor = SFUI::Color(203, 197, 138);
    scrollToggleProps.style.hoveredOffFillColor = SFUI::Color(130, 130, 130);
    scrollToggleProps.style.hoveredOnFillColor = SFUI::Color(150, 146, 102);
    scrollToggleProps.style.pressedOffFillColor = SFUI::Color(110, 110, 110);
    scrollToggleProps.style.pressedOnFillColor = SFUI::Color(125, 121, 85);

    // Scroll Toggle Label Props //
    scrollToggleLabelProps.layout.width = "100%";
    scrollToggleLabelProps.layout.height = "100%";
    scrollToggleLabelProps.style.text = "Button Text";
    scrollToggleLabelProps.style.font = font;
    scrollToggleLabelProps.style.textSize = 20.0f;
    scrollToggleLabelProps.style.textAlignHorizontal = "center";
    scrollToggleLabelProps.style.textColor = SFUI::Color(255, 255, 255);

    // Scroll Toggle Graphic Props //
    scrollToggleGraphicProps.layout.width = 40.0f;
    scrollToggleGraphicProps.layout.height = 40.0f;
    scrollToggleGraphicProps.style.loadedGraphic = texture;
    scrollToggleGraphicProps.style.graphicAlign = "fit";

    // Scroll Button 2 Props //
    scrollButtonProps2.layout.alignPrimary = "center";
    scrollButtonProps2.layout.alignSecondary = "center";
    scrollButtonProps2.layout.width = "100%";
    scrollButtonProps2.layout.height = 70.0f;
    scrollButtonProps2.layout.margin = SFUI::Prop::Vector4dim{1.0f, 1.0f, 0.0f, 0.0f};
    scrollButtonProps2.style.cornerRadius = 5.0f;
    scrollButtonProps2.style.fillColor = SFUI::Color(59, 59, 59);
    scrollButtonProps2.style.hoveredFillColor = SFUI::Color(50, 50, 50);
    scrollButtonProps2.style.pressedFillColor = SFUI::Color(40, 40, 40);
}


// Creat the UI Tree Structure //
void buildUI(SFUI::UIRoot& uiRoot, SFUI::SharedPointer<SFUI::Font> font, SFUI::SharedPointer<SFUI::Texture> texture) {

    buildProps(font, texture);

    // SFUI::Container rootContainer("rootContainer", rootContainerProps
    //     // ,SFUI::Button("button", scrollButtonProps1)
    //     ,SFUI::TextField("textField", textFieldProps)
    // );

    // UI Tree //
    SFUI::Container rootContainer("rootContainer", rootContainerProps,
        SFUI::Label("headerLabel", headerLabelProps),
        SFUI::Container("midContainer", midContainerProps,
            SFUI::TextField("textField", textFieldProps),
            SFUI::Slider("slider", sliderProps)
        ),
        SFUI::ScrollContainer("functionButtonContainer", scrollContainerProps,
            SFUI::Button("scrollButton1", scrollButtonProps1,
                SFUI::Label("buttonLabel", scrollButtonLabelProps)
            ),
            SFUI::Toggle("scrollToggle1", scrollToggleProps,
                SFUI::Label("toggleLabel", scrollToggleLabelProps)
            ),
            SFUI::Button("scrollButton2", scrollButtonProps2,
                SFUI::Graphic("buttonGraphic", scrollToggleGraphicProps)
            ),
            SFUI::Button("scrollButton3", scrollButtonProps1),
            SFUI::Toggle("scrollToggle2", scrollToggleProps),
            SFUI::Button("scrollButton4", scrollButtonProps2)
        )
    );
    uiRoot.setRootComponent(std::make_unique<SFUI::Container>(std::move(rootContainer)));
}
