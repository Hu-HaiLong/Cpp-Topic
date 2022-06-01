#include <vector>
#include "Vec3.h"

class Octree {
public:
	Octree(const Vec3& CenterPoint, const Vec3& HalfSize)
		: CenterPoint(CenterPoint), HalfSize(HalfSize), Data(nullptr)
	{
		for (int i = 0; i < 8; i++)
		{
			Children[i] = NULL;
		}
	}

	~Octree()
	{
		for (int i = 0; i < 8; i++)
		{
			delete Children[i];
		}
	}

	void Insert(Vec3* Point)
	{
		if (IsLeafNode())
		{
			if (Data == nullptr)
			{
				Data = Point;
				return;
			}
			else
			{
				Vec3* OldPoint = Data;
				Data = nullptr;

				CreateChildren();

				int OldLoc = GetPointBoxIndex(*OldPoint);
				int Loc = GetPointBoxIndex(*Point);
				Children[OldLoc]->Insert(OldPoint);
				Children[Loc]->Insert(Point);
			}
		}
		else 
		{
			int Loc = GetPointBoxIndex(*Point);
			Children[Loc]->Insert(Point);
		}
	}
	
	void GetPointsInsideBox(const Vec3& InMin, const Vec3& InMax, std::vector<Vec3*>& Results)
	{
		if (IsLeafNode())
		{
			if (Data == nullptr)
			{
				return;
			}
			if (Data->X < InMin.X || Data->Y < InMin.Y || Data->Z < InMin.Z)
			{
				return;
			}
			if (Data->X > InMax.X || Data->Y > InMax.Y || Data->Z > InMax.Z)
			{
				return;
			}
			Results.push_back(Data);
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{
				Vec3 CurMin = Children[i]->CenterPoint - Children[i]->HalfSize;
				Vec3 CurMax = Children[i]->CenterPoint + Children[i]->HalfSize;
				
				if (CurMax.X < InMin.X || CurMax.Y < InMin.Y || CurMax.Z < InMin.Z)
				{
					continue;
				}

				if (CurMin.X > InMax.X || CurMin.Y > InMax.Y || CurMin.Z > InMax.Z)
				{
					continue;
				}

				Children[i]->GetPointsInsideBox(InMin, InMax, Results);
			}
		}
	}

private:
	// return  0 ~ 7
	int GetPointBoxIndex(const Vec3& Point) const
	{
		int Loc = 0;

		if (Point.X >= CenterPoint.X)
		{
			Loc |= 1;
		}

		if (Point.Y >= CenterPoint.Y)
		{
			Loc |= 2;
		}

		if (Point.Z >= CenterPoint.Z)
		{
			Loc |= 4;
		}

		return Loc;
	}

	bool IsLeafNode() const
	{
		return Children[0] == NULL;
	}

	void CreateChildren()
	{
		for (int i = 0; i < 8; i++)
		{
			Vec3 NewCenter = CenterPoint;
			NewCenter.X += HalfSize.X * ((i & 1) ? 0.5f : -0.5f);
			NewCenter.Y += HalfSize.Y * ((i & 2) ? 0.5f : -0.5f);
			NewCenter.Z += HalfSize.Z * ((i & 4) ? 0.5f : -0.5f);
			Children[i] = new Octree(NewCenter, HalfSize * 0.5f);
		}
	}

private:
	Vec3 CenterPoint;
	Vec3 HalfSize;
	Vec3* Data;

	Octree* Children[8];
};