struct Vec3
{
public:
    Vec3() { }
    Vec3(float InX, float InY, float InZ)
        :X(InX), Y(InY), Z(InZ)
    { }

    Vec3 operator+(const Vec3& V3) const
    {
        return Vec3(X + V3.X, Y + V3.Y, Z + V3.Z);
    }

    Vec3 operator-(const Vec3& V3) const
    {
        return Vec3(X - V3.X, Y - V3.Y, Z - V3.Z);
    }

    Vec3 operator*(float InF) const
    {
        return Vec3(X * InF, Y * InF, Z * InF);
    }

    Vec3& operator+=(const Vec3& V3)
    {
        X += V3.X;
        Y += V3.Y;
        Z += V3.Z;

        return *this;
    }

    Vec3& operator-=(const Vec3& V3)
    {
        X -= V3.X;
        Y -= V3.Y;
        Z -= V3.Z;

        return *this;
    }

    Vec3& operator*=(float InF)
    {
        X *= InF;
        Y *= InF;
        Z *= InF;

        return *this;
    }

public:
    float X = 0.f, Y = 0.f, Z = 0.f;
};