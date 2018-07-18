#pragma once

#include <vsg/core/Object.h>


namespace vsg
{
    class Data : public Object
    {
    public:

        Data() {};

        virtual std::size_t dataSize() const = 0;
        virtual void* dataPointer() = 0;
        virtual const void* dataPointer() const = 0;

    protected:
        virtual ~Data() {}

    };
}
