#pragma once
#include <string>
#include <optional>
#include <functional>
#include <variant>
#include <vector>
#include <array>
#include <unordered_map>
#include <memory>




///////////////////////////////
// Standard C++ Type Aliases //
///////////////////////////////

namespace SFUI {

    using Void = void;

    using Bool = bool;

    using UnsignedInt8 = uint8_t;

    using UnsignedInt32 = uint32_t;

    using UnsignedInt = unsigned int;

    using Int = int;

    using Float = float;

    using Double = double;

    using Size = std::size_t;

    using String = std::string;

    template <typename... T>
    using Optional = std::optional<T ...>;

    template <typename... T>
    using Function = std::function<T ...>;

    template <typename... T>
    using Variant = std::variant<T ...>;

    template <typename... T>
    using Vector = std::vector<T ...>;

    template <typename T, std::size_t N>
    using Array = std::array<T, N>;

    template <typename T, typename N>
    using UnorderedMap = std::unordered_map<T, N>;

    template <typename... T>
    using WeakPointer = std::weak_ptr<T ...>;

    template <typename... T>
    using SharedPointer = std::shared_ptr<T ...>;

    template <typename... T>
    using UniquePointer = std::unique_ptr<T ...>;
}
