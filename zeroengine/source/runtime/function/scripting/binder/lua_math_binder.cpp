#include "runtime/function/scripting/binder/lua_math_binder.h"

namespace Zero {
    void LuaMathBinder::bindMaths(sol::state& p_luaState) {
        /* Vector2 */
        p_luaState.new_usertype<Vector2>(
            "Vector2",
            /* Constructors */
            sol::constructors<
                Vector2(),
                Vector2(float, float)>(),

            /* Operators */
            sol::meta_function::addition, [](const Vector2& lhs, const Vector2& rhs) -> Vector2 { return lhs + rhs; },
            sol::meta_function::subtraction, [](const Vector2& lhs, const Vector2& rhs) -> Vector2 { return lhs - rhs; },
            sol::meta_function::unary_minus, sol::resolve<Vector2() const>(&Vector2::operator-),
            sol::meta_function::multiplication, [](const Vector2& lhs, const Vector2& rhs) -> Vector2 { return lhs * rhs; },
            sol::meta_function::division, [](const Vector2& lhs, const Vector2& rhs) -> Vector2 { Vector2 tmp(lhs); return tmp / rhs; },
            sol::meta_function::to_string, [](const Vector2& target) { return "(" + std::to_string(target.x) + "," + std::to_string(target.y) + "}"; },

            /* Variables */
            "x", &Vector2::x,
            "y", &Vector2::y,

            /* Data */
            "One", []() { return Vector2::One; },
            "Zero", []() { return Vector2::Zero; },

            /* Methods */
            "Length", &Vector2::Length,
            "LengthSquared", &Vector2::LengthSquared,
            "Dot", &Vector2::Dot,
            "Normalize", [](Vector2& vec) -> void { vec.Normalize(); },
            "Lerp", [](const Vector2& v1, const Vector2& v2, float t) -> Vector2 { Vector2 ret; v1.Lerp(v1, v2, t, ret); return ret; },
            "SmoothStep", [](const Vector2& v1, const Vector2& v2, float t) -> Vector2 { Vector2 ret; v1.SmoothStep(v1, v2, t, ret); return ret; },
            "Distance", [](const Vector2& v1, const Vector2& v2) -> float { return Vector2::Distance(v1, v2); });

        /* Vector3 */
        p_luaState.new_usertype<Vector3>(
            "Vector3",
            /* Constructors */
            sol::constructors<
                Vector3(),
                Vector3(float, float, float)>(),

            /* Operators */
            sol::meta_function::addition, [](const Vector3& lhs, const Vector3& rhs) -> Vector3 { return lhs + rhs; },
            sol::meta_function::subtraction, [](const Vector3& lhs, const Vector3& rhs) -> Vector3 { return lhs - rhs; },
            sol::meta_function::unary_minus, sol::resolve<Vector3() const>(&Vector3::operator-),
            sol::meta_function::multiplication, [](const Vector3& lhs, const Vector3& rhs) -> Vector3 { return lhs * rhs; },
            sol::meta_function::division, [](const Vector3& lhs, const Vector3& rhs) -> Vector3 { Vector3 tmp(lhs); return tmp / rhs; },
            sol::meta_function::to_string, [](const Vector3& target) { return "(" + std::to_string(target.x) + "," + std::to_string(target.y) + "," + std::to_string(target.z) + "}"; },

            /* Variables */
            "x", &Vector3::x,
            "y", &Vector3::y,
            "z", &Vector3::z,

            /* Data */
            "One", []() { return Vector3::One; },
            "Zero", []() { return Vector3::Zero; },
            "Forward", []() { return Vector3::Forward; },
            "Up", []() { return Vector3::Up; },
            "Right", []() { return Vector3::Right; },
            "Backward", []() { return Vector3::Forward * -1; },
            "Down", []() { return Vector3::Up * -1; },
            "Left", []() { return Vector3::Right * -1; },

            /* Methods */
            "Length", &Vector3::Length,
            "LengthSquared", &Vector3::LengthSquared,
            "Dot", &Vector3::Dot,
            "Normalize", [](Vector3& vec) -> void { vec.Normalize(); },
            "Lerp", [](const Vector3& v1, const Vector3& v2, float t) -> Vector3 { Vector3 ret; v1.Lerp(v1, v2, t, ret); return ret; },
            "SmoothStep", [](const Vector3& v1, const Vector3& v2, float t) -> Vector3 { Vector3 ret; v1.SmoothStep(v1, v2, t, ret); return ret; },
            "Distance", [](const Vector3& v1, const Vector3& v2) -> float { return Vector3::Distance(v1, v2); });

        /* Vector4 */
        p_luaState.new_usertype<Vector4>(
            "Vector4",
            /* Constructors */
            sol::constructors<
                Vector4(),
                Vector4(float, float, float, float)>(),

            /* Operators */
            sol::meta_function::addition, [](const Vector4& lhs, const Vector4& rhs) -> Vector4 { return lhs + rhs; },
            sol::meta_function::subtraction, [](const Vector4& lhs, const Vector4& rhs) -> Vector4 { return lhs - rhs; },
            sol::meta_function::unary_minus, sol::resolve<Vector4() const>(&Vector4::operator-),
            sol::meta_function::multiplication, [](const Vector4& lhs, const Vector4& rhs) -> Vector4 { return lhs * rhs; },
            sol::meta_function::division, [](const Vector4& lhs, const Vector4& rhs) -> Vector4 { Vector4 tmp(lhs); return tmp / rhs; },
            sol::meta_function::to_string, [](const Vector4& target) { return "(" + std::to_string(target.x) + "," + std::to_string(target.y) + "," + std::to_string(target.z) + "," + std::to_string(target.w) + "}"; },

            /* Variables */
            "x", &Vector4::x,
            "y", &Vector4::y,
            "z", &Vector4::z,
            "w", &Vector4::w,

            /* Data */
            "One", []() { return Vector4::One; },
            "Zero", []() { return Vector4::Zero; },

            /* Methods */
            "Length", &Vector4::Length,
            "LengthSquared", &Vector4::LengthSquared,
            "Dot", &Vector4::Dot,
            "Normalize", [](Vector4& vec) -> void { vec.Normalize(); },
            "Lerp", [](const Vector4& v1, const Vector4& v2, float t) -> Vector4 { Vector4 ret; v1.Lerp(v1, v2, t, ret); return ret; },
            "SmoothStep", [](const Vector4& v1, const Vector4& v2, float t) -> Vector4 { Vector4 ret; v1.SmoothStep(v1, v2, t, ret); return ret; },
            "Distance", [](const Vector4& v1, const Vector4& v2) -> float { return Vector4::Distance(v1, v2); });

        /* Color */
        p_luaState.new_usertype<Color>(
            "Color",

            /* Constructors */
            sol::constructors<
                Color(),
                Color(float, float, float, float)>(),

            /* Operators */
            sol::meta_function::addition, [](const Color& lhs, const Color& rhs) -> Color { return lhs + rhs; },
            sol::meta_function::subtraction, [](const Color& lhs, const Color& rhs) -> Color { return lhs - rhs; },
            sol::meta_function::unary_minus, sol::resolve<Color() const>(&Color::operator-),
            sol::meta_function::multiplication, [](const Color& lhs, const Color& rhs) -> Color { return lhs * rhs; },
            sol::meta_function::division, [](const Color& lhs, const Color& rhs) -> Color { Color tmp(lhs); return tmp / rhs; },
            sol::meta_function::to_string, [](const Color& target) { return "(" + std::to_string(target.x) + "," + std::to_string(target.y) + "," + std::to_string(target.z) + "," + std::to_string(target.w) + "}"; },

            /* Variables */
            "r", &Color::x,
            "g", &Color::y,
            "b", &Color::z,
            "a", &Color::w,

            /* Methods */
            "AdjustContrast", [](Color& color, float contrast) { color.AdjustContrast(contrast); },
            "AdjustSaturation", [](Color& color, float sat) { color.AdjustSaturation(sat); },
            "Premultiply", [](const Color& c1, const Color& c2) -> Color { Color tmp(c2); c1.Premultiply(tmp); return tmp; },
            "Modulate", [](const Color& c1, const Color& c2) -> Color { Color tmp; c1.Modulate(c1, c2, tmp); return tmp; });

        /* Matrix(4x4) */
        p_luaState.new_usertype<Matrix>(
            "Matrix",

            /* Constructors */
            sol::constructors<
                Matrix(),
                Matrix(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float)>(),
            // /* Operators */
            sol::meta_function::addition, [](const Matrix& lhs, const Matrix& rhs) -> Matrix { return lhs + rhs; },
            sol::meta_function::subtraction, [](const Matrix& lhs, const Matrix& rhs) -> Matrix { return lhs - rhs; },
            sol::meta_function::unary_minus, sol::resolve<Matrix() const>(&Matrix::operator-),
            sol::meta_function::multiplication, [](const Matrix& lhs, const Matrix& rhs) -> Matrix { return Matrix(lhs * rhs); },
            sol::meta_function::division, [](const Matrix& lhs, const Matrix& rhs) -> Matrix { Matrix tmp(lhs); return tmp / rhs; },
            sol::meta_function::to_string, [](const Matrix& target) { return "Can't show matrix as string for now"; },

            /* Data */
            "Identity", []() { return Matrix::Identity; },

            /* Methods */
            "Get", [](Matrix& target, int row, int col) { return target(row, col); },
            "Set", [](Matrix& target, int row, int col, float value) { target(row, col) = value; },
            "GetRow", [](Matrix& target, int row) -> Vector4 { return Vector4(target(row, 0), target(row, 1), target(row, 2), target(row, 3)); },
            "GetCol", [](Matrix& target, int col) -> Vector4 { return Vector4(target(0, col), target(1, col), target(2, col), target(3, col)); },
            "Invert", [](const Matrix& mat) -> Matrix { return mat.Invert(); },
            "Transpose", [](const Matrix& mat) -> Matrix { return mat.Transpose(); },
            "Determinant", [](const Matrix& mat) -> float { return mat.Determinant(); },
            "Translate", [](Matrix& target, const Vector3& translation) { target.Translation(translation); },
            "Translation", [](const Matrix& target, const Vector3& translation) -> Matrix { return target.CreateTranslation(translation); },
            "RotationOnAxisX", [](const Matrix& target, float degree) -> Matrix { return target.CreateRotationX(DirectX::XMConvertToRadians(degree)); },
            "RotationOnAxisY", [](const Matrix& target, float degree) -> Matrix { return target.CreateRotationY(DirectX::XMConvertToRadians(degree)); },
            "RotationOnAxisZ", [](const Matrix& target, float degree) -> Matrix { return target.CreateRotationZ(DirectX::XMConvertToRadians(degree)); },
            "Scaling", [](const Matrix& target, const Vector3& scale) -> Matrix { return target.CreateScale(scale); });
    }
} // namespace Zero