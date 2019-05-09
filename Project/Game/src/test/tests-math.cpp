#include <catch2/catch.hpp>
#include "tinymath.h"

TEST_CASE("1D line segments overlapping can be detected correctly",
          "[LineSeg1][overlap]") {
    Math::LineSeg1<double> l1 {1.0, 4.0};
    Math::LineSeg1<double> l2;

    SECTION("when they overlap") {
        l2.max = 2.0;
        l2.min = -1.0;

        REQUIRE(true == Math::DoLineSegOverlap(l1, l2));
        REQUIRE(true == Math::DoLineSegOverlap(l2, l1));
    }

    SECTION("when they overlap at one point") {
        l2.max = 1.0;
        l2.min = -1.0;

        REQUIRE(false == Math::DoLineSegOverlap(l1, l2));
        REQUIRE(false == Math::DoLineSegOverlap(l2, l1));
    }

    SECTION("when they overlap at the other point") {
        l2.min = 4.0;
        l2.max = 5.0;

        REQUIRE(false == Math::DoLineSegOverlap(l1, l2));
        REQUIRE(false == Math::DoLineSegOverlap(l2, l1));
    }

    SECTION("when one completely contains the other") {
        l2.min = 2.0;
        l2.max = 3.0;

        REQUIRE(true == Math::DoLineSegOverlap(l1, l2));
        REQUIRE(true == Math::DoLineSegOverlap(l2, l1));
    }

    SECTION("when one contains the other at a point") {
        l2.min = 1.0;
        l2.max = 3.0;

        REQUIRE(true == Math::DoLineSegOverlap(l1, l2));
        REQUIRE(true == Math::DoLineSegOverlap(l2, l1));
    }

    SECTION("when they do not overlap") {
        l2.min = -3.0;
        l2.max = 0.0;

        REQUIRE(false == Math::DoLineSegOverlap(l1, l2));
        REQUIRE(false == Math::DoLineSegOverlap(l2, l1));
    }
}

TEST_CASE("1D line segment containing can be detected", "[LineSeg1][contain]") {
    Math::LineSeg1<double> l1 {1.0, 4.0};
    Math::LineSeg1<double> l2;

    SECTION("when one completely contains another") {
        l2.min = 1.1;
        l2.max = 3.5;

        REQUIRE(true == Math::DoLineSegContain(l1, l2));
        REQUIRE(false == Math::DoLineSegContain(l2, l1));
    }

    SECTION("when one contains another at a point") {
        l2.min = 1.0;
        l2.max = 3.5;

        REQUIRE(true == Math::DoLineSegContain(l1, l2));
        REQUIRE(false == Math::DoLineSegContain(l2, l1));
    }

    SECTION("when one contains another at the other point") {
        l2.min = 1.1;
        l2.max = 4.0;

        REQUIRE(true == Math::DoLineSegContain(l1, l2));
        REQUIRE(false == Math::DoLineSegContain(l2, l1));
    }

    SECTION("when they do not overlap") {
        l2.min = 4.2;
        l2.max = 5.0;

        REQUIRE(false == Math::DoLineSegContain(l1, l2));
        REQUIRE(false == Math::DoLineSegContain(l2, l1));
    }
}
