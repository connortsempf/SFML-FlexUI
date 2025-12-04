# SFML FlexUI #
**A modern, flexible UI library for SFML written in C++20.**
SFML-FlexUI is a C++ library that brings flexible, reactive, and modern UI components to SFML applications.
It uses SFML behind the scenes for input event handling, drawing to windows, and more. It’s designed for developers
who want clean layouts, fully customizable widgets, and an intuitive API for building complex interfaces quickly.
Build rich, responsive interfaces with composable components, powerful layout options, and fully documented code.


---
<br>
<br>


## 🚀 Features ##
- **🎨 Flexible Layouts:** Vertical, horizontal, and mixed alignment with full control over padding, margin, and sizing.
- **🖼️ Rich Components:** Buttons, Labels, Toggles, Sliders, ScrollContainers, Graphics, TextFields, all fully customizable.
- **⚡ State & Style Separation:** Clean separation between styles, states, and behavior for each component.
- **🖥️ Event Handling:** Built-in support for SFML events and custom callbacks for interactive UIs.
- **🛡️ Type Safety & Aliases:** Extensive use of type aliases, smart pointers, and variants for robust and safe design.
- **📚 Fully Documented:** Doxygen-ready headers and source files for quick reference and easy learning.
- **⏱️ Real-time Updates:** Reactive UI with efficient update loops and responsive interactions.
- **🧩 Easy Integration:** Lightweight, header-only style API for seamless use in SFML projects.



---
<br>
<br>


## 🏗️ Architecture ##
### 🌳 UIRoot ##
| Element | Description |
|---------|-------------|
| `UIRoot` | The central root container that manages all components for a given component tree. Handles rendering, event propagation, and layout updates. Create several ui-root objects to contain your many component trees for your application. |

### 🎨 Components ##
| Component | Description |
|-----------|-------------|
| `Container` | Generic UI container supporting child components. |
| `ScrollContainer` | Scrollable container for overflowing content. |
| `Label` | Text labels with styling support. |
| `Graphic` | Render images with UI integration. |
| `Button` | Clickable buttons with styles, state, and behavior. |
| `Toggle` | On/Off switches with styling and state handling. |
| `TextField` | Editable text fields with caret and input handling. |
| `Slider` | Range sliders with custom intervals and callbacks. |

### 🧩 Prop System ##
| Prop | Description |
|------------|-------------|
| `Layout` | Defines component positioning, size, alignment, padding, and margin. |
| `Style` | Controls colors, borders, shadows, and visual styling of components. |
| `State` | Tracks interactive state. |
| `Behavior` | Configures callbacks and event responses for components. |


---
<br>
<br>


## 💻 Examples / Quick Start ##
Check out the [**examples/**](examples/) folder for a fully working demo. Here's a minimal snippet to create a UI tree with a `Container` holding a `Button` holding a `Label` (without any styling):

```cpp
#include "SFML-FlexUI.hpp"

int main() {
    // Create an SFML Window //
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML-FlexUI Demo");

    // Create the UI root //
    SFUI::UIRoot uiRoot({800, 600});

    // Create a Container, Button, and Label //
    SFUI::Container container("rootContainer");
    SFUI::Button button("button");
    SFUI::Button label("buttonText");

    // Construct the UI Tree //
    container.addChild(button);
    button.addChild(label)
    uiRoot.setRootComponent(std::make_unique<SFUI::Container>(std::move(container)));

    // Main loop
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {

            // Pass Input Events to the UI Tree //
            if (event.has_value()) uiRoot.handleEvent(*event);

            if (event->is<sf::Event::Closed>()) window.close();

            // Do Not Let SFML Scale and Resize Screen Elements by Default -- SFML-FlexUI Does that Itself //
            if (const sf::Event::Resized* resizedEvent = event->getIf<sf::Event::Resized>()) {
                sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resizedEvent->size));
                window.setView(sf::View(visibleArea));
            }
        }

        // Update the Components //
        uiRoot.update({800, 600});

        window.clear();

        // Draw the Components //
        uiRoot.draw(window, window);

        window.display();
    }
}
```


---
<br>
<br>


## 🛠️ Getting Started ##
Follow these steps to set up **SFML-FlexUI** in your project.

### 🔹 Prerequisites ###
- **C++20 or higher** compiler.
- **CMake 3.16+** for building the project.
- **Git** to clone the repository and to possibly fetch SFML.
- Optional: **SFML 3.0.2+** installed on your system or simply let CMake fetch it for you.

### 🔹 Clone the Repository ###
```bash
git clone https://github.com/connortsempf/SFML-FlexUI.git
cd SFML-FlexUI
```

### 🔹 Build the Project ###
```bash
mkdir build
cd build
cmake .. -DSFML_LOCAL_PATH="path/to/your/SFMLConfig.cmake" -DBUILD_EXAMPLES="ON/OFF"
## Ignore -DSFML_LOCAL_PATH="" if you want CMake to fetch it for you (longer build time) ##
cmake --build .
```

### 🔹 Use SFML-FlexUI ###
There are **two main ways** to integrate SFML-FlexUI into your project:
#### 1️⃣ Fetch Content or Manually Add as a Subdirectory in CMake ####
This is the preferred approach if you are building your project with CMake. Fetch from GitHub or include the library as a subdirectory, and it will automatically handle dependencies and linking.
```cmake
## Fetch Content ##
FetchContent_Declare(
    SFML-FlexUI
    GIT_REPOSITORY https://github.com/connortsempf/SFML-FlexUI.git
    GIT_TAG main
)
FetchContent_MakeAvailable(SFML-FlexUI)
target_link_libraries(YourProject PRIVATE sfml-flexui)

## Add Subdirectory ##
add_subdirectory(path/to/SFML-FlexUI)
target_link_libraries(YourProject PRIVATE sfml-flexui)
```
#### 2️⃣ Using the Precompiled Library ####
If you prefer not to add SFML-FlexUI as a subdirectory, you can simply copy the generated library files within `build/` into your project. This works for both static (`.lib` / `.a`) and dynamic (`.dll` / `.so`) builds.
##### Steps: #####
1. **Build SFML-FlexUI** as usual using CMake.
2. **Locate the generated library files** in your build output (e.g., `libsfml-flexui.a`, `sfml-flexui.lib`, or `sfml-flexui.dll` / `.so`).
3. **Copy the library files** into a folder in your project, such as `lib/` or `vendor/`.
4. **Copy the header files** into a folder in your project, such as `include/SFML-FlexUI`
5. **Link the library** in your CMake project:
```cmake
target_link_libraries(YourProject PRIVATE path/to/lib/sfml-flexui-lib-file)
```


---
<br>
<br>


## 📄 License & Attribution ##
SFML-FlexUI is licensed under the MIT License.
SFML-FlexUI links against [SFML (Simple and Fast Multimedia Library)](https://www.sfml-dev.org/), which is licensed under the [zlib/png license](https://www.sfml-dev.org/license.php).

You are free to use, modify, and distribute SFML-FlexUI in your projects, including commercially.
Please note that SFML is not authored by the SFML-FlexUI team; all credit for SFML goes to the SFML contributors.
