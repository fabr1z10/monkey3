#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>


#include <monkey3/math/geometry.h>

TEST_CASE("sanity check - math works")
{
	REQUIRE(1 + 1 == 2);
}

TEST_CASE("orient basic cases")
{
	REQUIRE(orient({0,0}, {1,0}, {0,1}) > 0);
	REQUIRE(orient({0,0}, {1,0}, {0,-1}) < 0);
	REQUIRE(orient({0,0}, {1,0}, {5,0}) == 0);  // collinear
	REQUIRE(orient({0, 0}, {1, 0}, {0, 1}) == Catch::Approx(1.f));
}

TEST_CASE("segment intersection") {
	REQUIRE(segmentsIntersect({0, 0}, {5, 0}, {2, 2}, {2, -2}) == true);
	REQUIRE(segmentsIntersect({0, 0}, {5, 0}, {6, 2}, {6, -2}) == false);
	REQUIRE(segmentsIntersect({0,0}, {2,2}, {0,2}, {2,0}) == true);
	REQUIRE_FALSE(segmentsIntersect({0,0}, {1,0}, {2,0}, {3,0}));
	// segments share endpoint
	REQUIRE(segmentsIntersect({0,0}, {1,0}, {1,0}, {2,1}));
	// T-junction
	REQUIRE(segmentsIntersect({0,0}, {4,0}, {2,0}, {2,1}));
	// collinear overlapping
	REQUIRE(segmentsIntersect({0,0}, {4,0}, {2,0}, {6,0}));
	// collinear contained
	REQUIRE(segmentsIntersect({0,0}, {10,0}, {3,0}, {5,0}));
	REQUIRE(segmentsIntersect({0,0}, {6, 3}, {2, 1}, {8, 4}));
	// collinear but disjoint
	REQUIRE_FALSE(segmentsIntersect({0,0}, {2,0}, {3,0}, {5,0}));
	// parallel not insersecting
	REQUIRE_FALSE(segmentsIntersect({0,0}, {4,0}, {0,1}, {4,1}));
	// degenerate point touching
	REQUIRE(segmentsIntersect({1,0}, {1,0}, {0,0}, {2,0}));
	// degenerate point not touching
	REQUIRE_FALSE(segmentsIntersect({5,0}, {5,0}, {0,0}, {2,0}));
}

TEST_CASE("point on segment"){
	REQUIRE(pointOnSegment({0,0}, {10,0}, {5,0}));
	REQUIRE(pointOnSegment({0,0}, {10,0}, {0,0}));			 // end-point A
	REQUIRE(pointOnSegment({0,0}, {10,0}, {10,0}));			 // end-point B
	REQUIRE_FALSE(pointOnSegment({0,0}, {10,0}, {15,0}));			 // collinear but beyond B
	REQUIRE_FALSE(pointOnSegment({0,0}, {10,0}, {-5,0}));			 // collinear but beyond A
	REQUIRE_FALSE(pointOnSegment({0,0}, {10,10}, {5,6})); 	 // inside AABB but not in seg
	REQUIRE(pointOnSegment({3,0}, {3,10}, {3,5}));			 // vertical segment
	REQUIRE(pointOnSegment({0,0}, {10,10}, {5,5}));			 // diagonal segment
	REQUIRE_FALSE(pointOnSegment({0,0}, {10,10}, {5,5.1f})); // diagonal - near but not on
	REQUIRE(pointOnSegment({10,0}, {0,0}, {5,0}));           // reversed segment

}

TEST_CASE("point_in_simple_polygon") {
	// Point clearly inside a square
	std::vector<glm::vec2> square = {{0,0}, {10,0}, {10,10}, {0,10}};
	std::vector<glm::vec2> concavePoly = {
			{0,0},
			{10,0},
			{10,10},
			{5,5},
			{0,10}
	};
	std::vector<glm::vec2> collinearPoints = {{0,0}, {5,0}, {10,0}, {10,10}, {0, 10}};
	REQUIRE(pointInSimplePolygon({5,5}, square));
	REQUIRE_FALSE(pointInSimplePolygon({15,5}, square));
	REQUIRE(pointInSimplePolygon({5,0}, square));					// point on edge
	REQUIRE(pointInSimplePolygon({0,0}, square));					// point on vertex
	REQUIRE(pointInSimplePolygon({3,5}, concavePoly));
	REQUIRE(pointInSimplePolygon({7,7}, concavePoly));
	REQUIRE_FALSE(pointInSimplePolygon({5, 8}, concavePoly));
	REQUIRE(pointInSimplePolygon({5.8, 5.2}, concavePoly));
	REQUIRE(pointInSimplePolygon({5,0}, collinearPoints));		// point on collinear edge
	// degenerate cases (polygon with 0, 1, or 2 points)
	REQUIRE_FALSE(pointInSimplePolygon({0, 0}, {}));
	REQUIRE(pointInSimplePolygon({3, 2}, {{3, 2}}));
	REQUIRE(pointInSimplePolygon({5, 0}, {{0, 0}, {10, 0}}));

}