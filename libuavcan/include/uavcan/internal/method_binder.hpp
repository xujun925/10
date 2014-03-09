/*
 * Copyright (C) 2014 Pavel Kirienko <pavel.kirienko@gmail.com>
 */

#pragma once

#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include <typeinfo>
#include <uavcan/internal/impl_constants.hpp>
#include <uavcan/internal/util.hpp>

namespace uavcan
{

template <typename ObjectPtr, typename MemFunPtr>
class MethodBinder
{
    ObjectPtr obj_;
    MemFunPtr fun_;

    void validateBeforeCall() const
    {
        if (!operator bool())
        {
#if UAVCAN_EXCEPTIONS
            throw std::runtime_error(typeid(*this).name());
#else
            assert(0);
            std::abort();
#endif
        }
    }

public:
    MethodBinder()
    : obj_()
    , fun_()
    { }

    MethodBinder(ObjectPtr o, MemFunPtr f)
    : obj_(o)
    , fun_(f)
    { }

    operator bool() const
    {
        return try_implicit_cast<bool>(obj_, true) && try_implicit_cast<bool>(fun_, true);
    }

    void operator()()
    {
        validateBeforeCall();
        (obj_->*fun_)();
    }

    template <typename Par1>
    void operator()(Par1 p1)
    {
        validateBeforeCall();
        (obj_->*fun_)(p1);
    }

    template <typename Par1, typename Par2>
    void operator()(Par1 p1, Par2 p2)
    {
        validateBeforeCall();
        (obj_->*fun_)(p1, p2);
    }
};

}