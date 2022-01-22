#pragma once

class ICollideable
{
public:
    virtual void pad(void) = 0;
    virtual const Vector& obb_mins() const = 0;
    virtual const Vector& obb_maxs() const = 0;
};
