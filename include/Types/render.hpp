/**
 * @file render.hpp
 * @brief Provides SFML type aliases used in SFUI for rendering and UI objects.
 * @author Connor Sempf
 * @date 2025-11-15
 * @version 1.0
 *
 * This file maps SFML classes like RenderTarget, Sprite, Texture,
 * VertexArray, Clock, and Color to SFUI types to simplify usage
 * and enable type consistency across the library.
 */


#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>


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
