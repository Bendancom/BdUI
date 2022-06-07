#include "readonly.hpp"

namespace BdUI
{
    template <typename T>
    ReadOnly<T>::ReadOnly(const ReadOnly<T> &r)
    {
        if (!r.Isempty)
            Value = r.Value;
        Isempty = false;
    }
    template <typename T>
    void ReadOnly<T>::Set(T data)
    {
        if (Isempty)
        {
            if (set)
            {
                (*set)(Value, data);
                Isempty = false;
            }
            else
            {
                Value = data;
                Isempty = false;
            }
        }
    }
    template <typename T>
    T ReadOnly<T>::Get()
    {
        if (get)
            return (*get)(Value);
        else
            return Value;
    }
    template <typename T>
    ReadOnly<T>::operator T()
    {
        if (get)
            return (*get)(Value);
        else
            return Value;
    }
    template <typename T>
    T *ReadOnly<T>::operator->() const
    {
        return &Value;
    }
    template <typename T>
    bool ReadOnly<T>::empty()
    {
        return Isempty;
    }
    template <typename T>
    ReadOnly<T>::operator bool()
    {
        return Isempty;
    }
    template <typename T>
    ReadOnly<T> &ReadOnly<T>::operator=(T data)
    {
        if (Isempty)
        {
            if (set)
            {
                (*set)(Value, data);
                Isempty = false;
            }
            else
            {
                Value = data;
                Isempty = false;
            }
        }
        return *this;
    }
    template <typename T>
    ReadOnly<T> &ReadOnly<T>::operator=(const ReadOnly<T> &r)
    {
        if (Isempty && !r.Isempty)
        {
            if (set)
            {
                (*set)(Value, r.Value);
                Isempty = false;
            }
            else
            {
                Value = r.Value;
                Isempty = false;
            }
        }
        return *this;
    }
}