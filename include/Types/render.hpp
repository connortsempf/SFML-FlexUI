#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>




///////////////////////////////////////////
// Rendering Library (SFML) Type Aliases //
///////////////////////////////////////////

namespace SFUI {

    using FloatRect = sf::FloatRect;

    using Color = sf::Color;
    
    using Text = sf::Text;
    
    using Font = sf::Font;
    
    using Clock = sf::Clock;
    
    using Time = sf::Time;

    using Event = sf::Event;

    using RectangleShape = sf::RectangleShape;
    
    using CircleShape = sf::CircleShape;

    using Cursor = sf::Cursor;
    
    using RenderTarget = sf::RenderTarget;

    using RenderWindow = sf::RenderWindow;

    using Image = sf::Image;
    
    using Texture = sf::Texture;
    
    using Sprite = sf::Sprite;
    
    using RenderTexture = sf::RenderTexture;
    
    using PrimitiveType = sf::PrimitiveType;

    using VertexArray = sf::VertexArray;

    using Shader = sf::Shader;
}
