#ifndef PLATFORMER_TINYMATH_H
#define PLATFORMER_TINYMATH_H

#include <SDL2pp/Rect.hh>
#include <random>
#include <string>
#include <vector>

//! A tiny math library for mathematical calculations
class Math {
   public:
    //! 1-dimension line segment
    template <typename T>  // requires a number type
    struct LineSeg1 {
        LineSeg1() {}
        LineSeg1(T mn, T mx) : min(mn), max(mx) {}

        T min;
        T max;
    };

    //! 2-dimension vector
    template <typename T>  // requires a number type
    struct Vec2 {
        Vec2() {}
        Vec2(T xx, T yy) : x(xx), y(yy) {}
        template <typename U>
        Vec2(Vec2<U>& other) : x((T)other.x), y((T)other.y) {}
        bool operator==(const Vec2& other) const {
            return x == other.x && y == other.y;
        }
        void Add(const Vec2<T>& other) {
            x += other.x;
            y += other.y;
        }
        std::string to_string() const {
            return "[vec2 {pos: (" + std::to_string(x) + ", " +
                   std::to_string(y) + ")}]";
        }

        T x;
        T y;
    };

    //! Rectangle
    template <typename T>  // requires a number type
    struct Rect {
        Rect() {}
        Rect(T xx, T yy, T ww, T hh) : x(xx), y(yy), w(ww), h(hh) {}
        Rect(std::vector<T> ints) {
            assert(ints.size() == 4);
            x = ints[0];
            y = ints[1];
            w = ints[2];
            h = ints[3];
        }

        SDL2pp::Rect toSDLRect() const {
            return SDL2pp::Rect(static_cast<int>(x), static_cast<int>(y),
                                static_cast<int>(w), static_cast<int>(h));
        }

        Vec2<T> Pos() { return {x, y}; }
        Vec2<T> PosTopRight() { return {x + w, y}; }
        Vec2<T> PosBottomLeft() { return {x, y + h}; }
        Vec2<T> PosBottomRight() { return {x + w, y + h}; }

        T x, y, w, h;
    };

    //! Determines if one 1D line segment contains another
    template <typename T>  // requires a number type
    static bool DoLineSegContain(const LineSeg1<T>& l1, const LineSeg1<T>& l2) {
        return l1.max >= l2.max && l1.min <= l2.min;
    }

    //! Determines if two 1D line segments overlap
    template <typename T>
    static bool DoLineSegOverlap(const LineSeg1<T>& l1, const LineSeg1<T>& l2) {
        return (l1.min < l2.max && l1.min > l2.min) ||
               (l1.max < l2.max && l1.max > l2.min) ||
               DoLineSegContain(l1, l2) || DoLineSegContain(l2, l1);
    }
    //! Determine if a rectangle contains a point
    template <typename T, typename U>
    static bool DoRectContainPoint(const Rect<T>& rect, const Vec2<U>& point) {
        return (point.x >= rect.x && point.x <= rect.x + rect.w &&
                point.y >= rect.y && point.y <= rect.y + rect.h);
    }

    //! Returns dot product of two Vec2 objects
    template <typename T>
    static double Dot(const Vec2<T>& vec_a, const Vec2<T>& vec_b) {
        return vec_a.x * vec_b.x + vec_a.y * vec_b.y;
    }

    //! Returns sum of two Vec2 objects
    template <typename T>
    static Vec2<T> Add(const Vec2<T>& vec_a, const Vec2<T>& vec_b) {
        return {vec_a.x + vec_b.x, vec_a.y + vec_b.y};
    }

    //! Scales a Vec2 object
    template <typename T>
    static Vec2<T> Scale(const Vec2<T>& vec, T scalar) {
        return {vec.x * scalar, vec.y * scalar};
    }

    //! Projects a Rect onto a Vec2
    template <typename T>
    static LineSeg1<T> ProjectRectToVec(const Rect<T>& poly,
                                        const Vec2<T>& vec) {
        std::vector<Vec2<T>> vertices {{poly.x, poly.y},
                                       {poly.x + poly.w, poly.y},
                                       {poly.x, poly.y + poly.h},
                                       {poly.x + poly.w, poly.y + poly.h}};
        T min = Dot(vec, vertices[0]);
        T max = min;
        for (int i = 1; i < vertices.size(); ++i) {
            T proj = Dot(vec, vertices[i]);
            if (proj < min) {
                min = proj;
            } else if (proj > max) {
                max = proj;
            }
        }
        return {min, max};
    }

    //! Returns a random floating-point number in range [0, 1)
    static double Random() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> dis(0.0, 1.0);

        return dis(gen);
    }

    static constexpr double kPi = 3.14159265358979323846;
};

#endif
