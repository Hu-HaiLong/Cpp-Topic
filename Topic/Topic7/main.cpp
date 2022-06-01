#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>

#include "Octree.h"
#include "Stopwatch.h"

// Used for testing
std::vector<Vec3> Points;
Octree* Oct;
Vec3 Qmin, Qmax;

float rand11() // Random number between [-1,1]
{
	return -1.f + (2.f * rand()) * (1.f / RAND_MAX);
}

Vec3 randVec3() // Random vector with components in the range [-1,1]
{
	return Vec3(rand11(), rand11(), rand11());
}

// Determine if 'point' is within the bounding box [bmin, bmax]
bool naivePointInBox(const Vec3& point, const Vec3& bmin, const Vec3& bmax) {
	return
		point.X >= bmin.X &&
		point.Y >= bmin.Y &&
		point.Z >= bmin.Z &&
		point.X <= bmax.X &&
		point.Y <= bmax.Y &&
		point.Z <= bmax.Z;
}

void init() {
	// Create a new Octree centered at the origin
	// with physical dimension 2x2x2
	Oct = new Octree(Vec3(0, 0, 0), Vec3(1, 1, 1));

	// Create a bunch of random points
	const int nPoints = 1 * 2000 * 2000;
	Points.reserve(nPoints);
	for (int i = 0; i < nPoints; ++i) {
		Points.push_back(randVec3());
	}
	printf("Created %ld points\n", Points.size()); fflush(stdout);

	// Insert the points into the Octree
	for (int i = 0; i < nPoints; ++i) 
	{
		Oct->Insert(&Points[i]);
	}

	printf("Inserted points to Octree\n"); fflush(stdout);

	// Create a very small query box. The smaller this box is
	// the less work the Octree will need to do. This may seem
	// like it is exagerating the benefits, but often, we only
	// need to know very nearby objects.
	Qmin = Vec3(-.05, -.05, -.05);
	Qmax = Vec3(.05, .05, .05);

	// Remember: In the case where the query is relatively close
	// to the size of the whole Octree space, the Octree will
	// actually be a good bit slower than brute forcing every point!
}

// Query using brute-force
void testNaive() {
	double StartTime = WatchTime();

	std::vector<int> results;
	for (int i = 0; i < Points.size(); ++i) 
	{
		if (naivePointInBox(Points[i], Qmin, Qmax)) {
			results.push_back(i);
		}
	}

	double CostTime = WatchTime() - StartTime;
	printf("testNaive found %ld points in %.5f sec.\n", results.size(), CostTime);
}

// Query using Octree
void testOctree() {
	double StartTime = WatchTime();

	std::vector<Vec3*> Results;
	Oct->GetPointsInsideBox(Qmin, Qmax, Results);

	double CostTime = WatchTime() - StartTime;
	printf("testOctree found %ld points in %.5f sec.\n", Results.size(), CostTime);
}


int main(int argc, char** argv) {
	init();
	testNaive();
	testOctree();

	return 0;
}