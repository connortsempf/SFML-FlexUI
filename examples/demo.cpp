#include "propsheet_demo.hpp"


int main() {

    // Window Creation //
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 16;
    sf::RenderWindow window(sf::VideoMode({320, 470}), "SFML-FlexUI", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(120);
    //////////////////////////////

    // Asset Loading //
    SFUI::SharedPointer<SFUI::Font> font = std::make_shared<SFUI::Font>();
    if (!font->openFromFile("assets/inconsolata.ttf")) {}
    SFUI::SharedPointer<SFUI::Texture> image = std::make_shared<SFUI::Texture>();
    if (!image->loadFromFile("assets/image.png")) {}
    ////////////////////////////////////////////////

    // UI Tree Creation //
    SFUI::UIRoot ui = SFUI::UIRoot();
    SFUI::Container rootContainer("rootContainer", rootContainerProp,
        SFUI::Label("headerLabel", SFUI::PropGroup::Label{
            .layout = { .width = "100%", .height = 40.0f },
            .labelStyle { .text = "SFML-FlexUI Demo", .font = font, .textSize = 30.0f, .textAlignHorizontal = "center", .textColor = "#FFFFFF" }}
        ),
        SFUI::Container("midContainer", midContainerProp,
            SFUI::TextField("textField", SFUI::PropGroup::TextField{
                .layout = { .width = "100%", .height = 70.0f, .margin = 5.0f },
                .style = { .borderWidth = 4.0f, .cornerRadius = 8.0f, .fillColor = "#FFFFFF", .borderColor = SFUI::Color(20, 20, 20) },
                .textFieldStyle = {
                    .textInset = SFUI::SubProp::Vector4dim{10.0f, 10.0f, 0.0f, 0.0f},
                    .placeholderText = "Type Here...",
                    .font = font,
                    .textSize = 20.0f,
                    .textColor = "#000000",
                    .placeholderTextColor = SFUI::Color(120, 120, 120),
                    .caretFillColor = SFUI::Color(79, 168, 255)
                }
            }),
            SFUI::Slider("slider", sliderProp)
        ),
        SFUI::ScrollContainer("functionButtonContainer", scrollContainerProp,
            SFUI::Button("scrollButton1", scrollButtonProp1,
                SFUI::Label("buttonLabel", SFUI::PropGroup::Label{
                    .layout = { .width = "100%", .height = "100%" },
                    .labelStyle = { .text = "Button Text", .font = font, .textSize = 20.0f, .textAlignHorizontal = "center", .textColor = SFUI::Color(255, 255, 255) }
                })
            ),
            SFUI::Toggle("scrollToggle1", SFUI::PropGroup::Toggle{
                .layout = { .alignPrimary = "center", .alignSecondary = "center", .width = "50%", .height = 70.0f, .margin = SFUI::SubProp::Vector4dim{1.0f, 1.0f, 0.0f, 0.0f} },
                .style = { .cornerRadius = 5.0f },
                .toggleStyle = {
                    .offFillColor = SFUI::Color(150, 150, 150), .onFillColor = SFUI::Color(203, 197, 138),
                    .hoveredOffFillColor = SFUI::Color(130, 130, 130), .hoveredOnFillColor = SFUI::Color(150, 146, 102),
                    .pressedOffFillColor = SFUI::Color(110, 110, 110), .pressedOnFillColor = SFUI::Color(125, 121, 85)
                }
            },
                SFUI::Label("toggleLabel", SFUI::PropGroup::Label{
                    .layout = { .width = "100%", .height = "100%" },
                    .labelStyle = { .text = "Button Text", .font = font, .textSize = 20.0f, .textAlignHorizontal = "center", .textColor = SFUI::Color(255, 255, 255) }
                })
            ),
            SFUI::Button("scrollButton2", scrollButtonProp2,
                SFUI::Graphic("buttonGraphic", SFUI::PropGroup::Graphic{
                    .layout = { .width = 40.0f, .height = 40.0f },
                    .graphicStyle = { .loadedGraphic = image, .graphicAlign = "fit" }
                })
            ),
            SFUI::Button("scrollButton3", scrollButtonProp1),
            SFUI::Toggle("scrollToggle2", scrollToggleProp),
            SFUI::Button("scrollButton4", scrollButtonProp2)
        )
    );
    ui.setRootComponent(std::make_unique<SFUI::Container>(std::move(rootContainer)));
    //////////////////////////////////////////////////////////////////////////////////


    // Main Event and Render Loop //
    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {
            
            // Handle UI Event Passing with Each Event //
            if (event.has_value()) ui.handleEvent(*event);
            ///////////////////////////////////////////////

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const sf::Event::KeyPressed* keyPressedEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressedEvent->code == sf::Keyboard::Key::Escape) window.close();
            }
            if (const sf::Event::Resized* resizedEvent = event->getIf<sf::Event::Resized>()) {
                sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resizedEvent->size));
                window.setView(sf::View(visibleArea));
            }
        }

        // Update the UI Every Frame AFTER Event Passing //
        ui.update({window.getSize().x, window.getSize().y});
        /////////////////////////////////////////////////////
        
        window.clear();

        // Draw UI to current Render Target Every Frame AFTER Updating //
        ui.draw(window, window);
        /////////////////////////

        window.display();
    }
}
