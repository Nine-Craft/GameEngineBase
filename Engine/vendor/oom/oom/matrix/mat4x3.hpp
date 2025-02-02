#pragma once

#include "../vector/vec3.hpp"
#include "../vector/vec4.hpp"
#include <limits>
#include <cstddef>

namespace oom
{
    template<typename T>
    struct mat<4, 3, T>
    {
        using col_type          = vec<3, T>;
        using row_type          = vec<4, T>;
        using type              = mat<4, 3, T>;
        using transpose_type    = mat<3, 4, T>;
        using value_type        = T;

    private:
        col_type value[4];

    public:
        // -- Accesses --

        using length_type = std::size_t;
        static constexpr length_type size() { return 4; }

        col_type & operator[](length_type i);
        constexpr col_type const& operator[](length_type i) const;

        // -- Constructors --

        constexpr mat() = default;
        constexpr mat(mat<4, 3, T> const& m);

        explicit constexpr mat(T const& x);
        constexpr mat(
            T const& x0, T const& y0, T const& z0,
            T const& x1, T const& y1, T const& z1,
            T const& x2, T const& y2, T const& z2,
            T const& x3, T const& y3, T const& z3);
        constexpr mat(
            col_type const& v0,
            col_type const& v1,
            col_type const& v2,
            col_type const& v3);

        // -- Conversions --

        template<
            typename X1, typename Y1, typename Z1,
            typename X2, typename Y2, typename Z2,
            typename X3, typename Y3, typename Z3,
            typename X4, typename Y4, typename Z4>
        constexpr mat(
            X1 const& x1, Y1 const& y1, Z1 const& z1,
            X2 const& x2, Y2 const& y2, Z2 const& z2,
            X3 const& x3, Y3 const& y3, Z3 const& z3,
            X4 const& x4, Y4 const& y4, Z4 const& z4);

        template<typename V1, typename V2, typename V3, typename V4>
        constexpr mat(
            vec<3, V1> const& v1,
            vec<3, V2> const& v2,
            vec<3, V3> const& v3,
            vec<3, V4> const& v4);

        // -- Matrix conversions --

        template<typename U>
        OOM_EXPLICIT constexpr mat(mat<4, 3, U> const& m);

        OOM_EXPLICIT constexpr mat(mat<2, 2, T> const& x);
        OOM_EXPLICIT constexpr mat(mat<3, 3, T> const& x);
        OOM_EXPLICIT constexpr mat(mat<4, 4, T> const& x);
        OOM_EXPLICIT constexpr mat(mat<2, 3, T> const& x);
        OOM_EXPLICIT constexpr mat(mat<3, 2, T> const& x);
        OOM_EXPLICIT constexpr mat(mat<2, 4, T> const& x);
        OOM_EXPLICIT constexpr mat(mat<4, 2, T> const& x);
        OOM_EXPLICIT constexpr mat(mat<3, 4, T> const& x);

        // -- Unary arithmetic operators --

        template<typename U>
        mat<4, 3, T> & operator=(mat<4, 3, U> const& m);
        template<typename U>
        mat<4, 3, T> & operator+=(U s);
        template<typename U>
        mat<4, 3, T> & operator+=(mat<4, 3, U> const& m);
        template<typename U>
        mat<4, 3, T> & operator-=(U s);
        template<typename U>
        mat<4, 3, T> & operator-=(mat<4, 3, U> const& m);
        template<typename U>
        mat<4, 3, T> & operator*=(U s);
        template<typename U>
        mat<4, 3, T> & operator/=(U s);

        // -- Increment and decrement operators --

        mat<4, 3, T>& operator++();
        mat<4, 3, T>& operator--();
        mat<4, 3, T> operator++(int);
        mat<4, 3, T> operator--(int);

        // -- Helper Functions --

        static constexpr mat<4, 3, T> zero() { return mat<4, 3, T>{0}; }

        void SetToZero() { *this = zero(); }

        mat<3, 4, T> Transposed() const { return transposed(*this); }
    };

    // -- Unary operators --

    template<typename T>
    mat<4, 3, T> operator+(mat<4, 3, T> const& m);

    template<typename T>
    mat<4, 3, T> operator-(mat<4, 3, T> const& m);

    // -- Binary operators --

    template<typename T>
    mat<4, 3, T> operator+(mat<4, 3, T> const& m, T const& s);

    template<typename T>
    mat<4, 3, T> operator+(mat<4, 3, T> const& m1, mat<4, 3, T> const& m2);

    template<typename T>
    mat<4, 3, T> operator-(mat<4, 3, T> const& m, T const& s);

    template<typename T>
    mat<4, 3, T> operator-(mat<4, 3, T> const& m1, mat<4, 3, T> const& m2);

    template<typename T>
    mat<4, 3, T> operator*(mat<4, 3, T> const& m, T const& s);

    template<typename T>
    mat<4, 3, T> operator*(T const& s, mat<4, 3, T> const& m);

    template<typename T>
    typename mat<4, 3, T>::col_type operator*(mat<4, 3, T> const& m, typename mat<4, 3, T>::row_type const& v);

    template<typename T>
    typename mat<4, 3, T>::row_type operator*(typename mat<4, 3, T>::col_type const& v, mat<4, 3, T> const& m);

    template<typename T>
    mat<2, 3, T> operator*(mat<4, 3, T> const& m1, mat<2, 4, T> const& m2);

    template<typename T>
    mat<3, 3, T> operator*(mat<4, 3, T> const& m1,	mat<3, 4, T> const& m2);

    template<typename T>
    mat<4, 3, T> operator*(mat<4, 3, T> const& m1, mat<4, 4, T> const& m2);

    template<typename T>
    mat<4, 3, T> operator/(mat<4, 3, T> const& m, T const& s);

    template<typename T>
    mat<4, 3, T> operator/(T const& s, mat<4, 3, T> const& m);

    // -- Boolean operators --

    template<typename T>
    bool operator==(mat<4, 3, T> const& m1, mat<4, 3, T> const& m2);

    template<typename T>
    bool operator!=(mat<4, 3, T> const& m1, mat<4, 3, T> const& m2);
}//namespace oom

#include "mat4x3_impl.hpp"
