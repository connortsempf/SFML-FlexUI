#include "ui.hpp"


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
    SFUI::SharedPointer<SFUI::Texture> texture = std::make_shared<SFUI::Texture>();
    if (!texture->loadFromFile("assets/image.png")) {}
    //////////////////////////////////////////////////

    // UI Creation //
    SFUI::UIRoot ui = SFUI::UIRoot();
    buildUI(ui, font, texture);
    ///////////////////////////


    // Main Event and Render Loop //
    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {
            
            // Handle UI Event Passing with Each Event //
            if (event.has_value()) ui.handleEvent(*event);
            //////////////////////////////////////////////

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const sf::Event::KeyPressed* keyPressedEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressedEvent->code == sf::Keyboard::Key::Escape) window.close();
                if (keyPressedEvent->code == sf::Keyboard::Key::G) {
                    // SFUI::Container* rootContainer = ui.getComponent<SFUI::Container>("rootContainer");
                    // rootContainer->layout.height = "100%";
                    // rootContainer->style.cornerRadius = 0.0f;
                    // rootContainer->style.fillColor = SFUI::Color(0, 255, 0);
                    SFUI::Button* button = ui.getComponent<SFUI::Button>("button");
                    button->layout.width = 200.0f;
                };
                if (keyPressedEvent->code == sf::Keyboard::Key::B) {
                    // SFUI::Container* rootContainer = ui.getComponent<SFUI::Container>("rootContainer");
                    // rootContainer->layout.height = "50%";
                    // rootContainer->style.cornerRadius = 10.0f;
                    // rootContainer->style.fillColor = SFUI::Color(32, 32, 32);
                    SFUI::Button* button = ui.getComponent<SFUI::Button>("button");
                    button->layout.width = "80%";
                };
            }
            if (const sf::Event::Resized* resizedEvent = event->getIf<sf::Event::Resized>()) {
                sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resizedEvent->size));
                window.setView(sf::View(visibleArea));
            }
        }

        // Update the UI Every Frame AFTER Event Passing //
        ui.update({window.getSize().x, window.getSize().y});
        ////////////////////////////////////////////////////
        
        window.clear();

        // Draw UI to current Render Target Every Frame AFTER Updating //
        ui.draw(window, window);
        ////////////////////////

        window.display();
    }
}
