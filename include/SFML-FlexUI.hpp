/**
 * @file SFML-FlexUI.hpp
 * @brief Main SFUI include file that aggregates core types, base classes, and components.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.2
 *
 * This file includes all primary SFUI headers, providing a single
 * entry point for using the library. It covers:
 *   - Core type aliases and utility structures (base, render, vector, subprop, prop, propgroup)
 *   - Base classes (Component, UIRoot)
 *   - UI components (Container, ScrollContainer, Label, Button, Toggle, Graphic, TextField, Slider)
 *
 * Including this file gives access to the full SFUI framework in one import.
 */


#pragma once


// Types //
#include "Types/base.hpp"
#include "Types/render.hpp"
#include "Types/vector.hpp"
#include "Types/prop.hpp"
#include "Types/propgroup.hpp"
#include "Types/propset.hpp"
#include "Types/computedprop.hpp"


// Base //
#include "Base/UIRoot.hpp"
#include "Base/Component.hpp"


// Components //
#include "Components/Container.hpp"
#include "Components/ScrollContainer.hpp"
#include "Components/Label.hpp"
#include "Components/Graphic.hpp"
#include "Components/Button.hpp"
#include "Components/Toggle.hpp"
#include "Components/TextField.hpp"
#include "Components/Slider.hpp"
