#pragma once

#include <cmath>
#include <iostream>
#include <string>

#define PI 3.141592653589793238462643383279

template <typename R> struct Const
{
        static R pi;
        static R tol;
};

template <typename R> bool IsZero(R x)
{
        return x>=-Const<R>::tol && x<=Const<R>::tol;
}

template <typename R> R Sign(R x)
{
        return x>=(R)0 ? (R)1 : (R)-1;
}

template <typename R> R Sign0(R x)
{
        if(x==(R)0)
                return (R)0;
        return x>(R)0 ? (R)1 : (R)-1;
}

template <typename R> R DegToRad(R deg)
{
        return deg*(R)PI/(R)180.0;
}

template <typename R> R RadToDeg(R rad)
{       
        return rad*(R)180.0/(R)PI;
}

template <typename R> R Sqr(R x)
{
        return x*x;
}

template <typename R> R Min(R a, R b)
{
        return a<b ? a : b;
}

template <typename R> R Max(R a, R b)
{
        return a>b ? a : b;
}

template<typename R> R Rect2CmpWH1(R x1, R y1, R x2, R y2)
{
        if(abs(x1-x2)>=(R)1 || abs(y1-y2)>=(R)1)
                return (R)0;
        return ((R)1-abs(x1-x2))*((R)1-abs(y1-y2));
}

template<typename R> R Ring(R x, R d)
{
        while(x<0)
                x+=d;
        while(x>=d)
                x-=d;
        return x;
}

// Vector2 typename and functions

template <typename R> struct Vector2
{
        union
        {
                struct { R x, y; };
                struct { R r, g; };
                struct { R h ,s; };
                struct { R e[2]; };
        };

        Vector2() {x=y=0;}
        Vector2(R x, R y) : x(x), y(y) {};

        R magnitude()
        {
                return sqrt(x*x+y*y);
        }

        R magnitude2()
        {
                return x*x+y*y;
        }
        
        template <typename T> void magnitude2T(T &out)
        {
        T _x = (T)x;
        T _y = (T)y;
        out = _x*_x+_y*_y;
    }
    
    R absSum()
    {
        return abs(x)+abs(y);
    }

        void normalize()
        {
                R m = magnitude();
                if(m<=Const<R>::tol)
                        x = y = 0;
                else
                {
                        x/=m;
                        y/=m;
                }
        }
        Vector2 normalized()
        {
                R m = magnitude();
                if(m<=Const<R>::tol)
                        return Vector2<R>(0,0);
                else
                        return Vector2<R>(x/m,y/m);
        }

        Vector2 rotated(R angle)
        {
                R c = cos(angle), s = sin(angle);
                return Vector2<R>(x*c-y*s,x*s+y*c);
        }

        void rotateLeft()
        {
                R z = y;
                y = x;
                x = -z;
        }

        Vector2 rotatedLeft()
        {
                return Vector2<R>(-y,x);
        }

        void rotateRight()
        {
                R z = y;
                y = -x;
                x = z;
        }

        void angle(R a)
        {
                x = cos(a);
                y = sin(a);
        }

        R angle()
        {
                return atan2(y,x);
        }

        //static Vector2<R> angle(R a)
        //{
        //      return Vector2<R>(cos(a),sin(a));
        //}

        template <typename T> void convert(Vector2<T> &out)
        {
                out.x = (T)x;
                out.y = (T)y;
        }

        Vector2<R>& operator+=(Vector2<R> u)
        {
                x+=u.x;
                y+=u.y;
                return *this;
        }

        Vector2<R>& operator-=(Vector2<R> u)
        {
                x-=u.x;
                y-=u.y;
                return *this;
        }

        Vector2<R>& operator*=(R s)
        {
                x*=s;
                y*=s;
                return *this;
        }

        Vector2<R>& operator/=(R s)
        {
                x/=s;
                y/=s;
                return *this;
        }

        Vector2<R> operator-(void)
        {
                return Vector2<R>(-x,-y);
        }
};

// IO

template <typename R> std::ostream& operator<< (std::ostream& o, Vector2<R> v)
{
        o << v.x << ' ' << v.y;
        return o;
}

template <typename R> std::ostream& operator+= (std::ostream& o, Vector2<R> v)
{
        o << "[" << v.x << ";" << v.y << "]";
        return o;
}

template <typename R> std::istream& operator>> (std::istream& i, Vector2<R> &v)
{
        i >> v.x >> v.y;
        return i;
}

template <typename R> bool operator== (Vector2<R> u, Vector2<R> v)
{
        return u.x==v.x && u.y==v.y;
}

template <typename R> bool operator!= (Vector2<R> u, Vector2<R> v)
{
        return u.x!=v.x || u.y!=v.y;
}

template <typename R> bool operator< (Vector2<R> u, Vector2<R> v)
{
        if(u.x==v.x)
                return u.y<v.y;
        return u.x<v.x;
}

template <typename R> Vector2<R> operator+(Vector2<R> u, Vector2<R> v)
{
        return Vector2<R>(u.x+v.x,u.y+v.y);
}

template <typename R> Vector2<R> operator-(Vector2<R> u, Vector2<R> v)
{
        return Vector2<R>(u.x-v.x,u.y-v.y);
}

// Vector2 dot product
template <typename R> R operator*(Vector2<R> u, Vector2<R> v)
{
        return (u.x*v.x + u.y*v.y);
}

template <typename R> R operator^ (Vector2<R> u, Vector2<R> v)
{
        return u.x*v.y-u.y*v.x;
}

template <typename R> Vector2<R> operator*(R s, Vector2<R> u)
{
        return Vector2<R>(u.x*s, u.y*s);
}

template <typename R> Vector2<R> operator*(Vector2<R> u, R s)
{
        return Vector2<R>(u.x*s, u.y*s);
}

template <typename R> Vector2<R> operator/(Vector2<R> u, R s)
{
        return Vector2<R>(u.x/s, u.y/s);
}

// Vector3 typename and functions

template <typename R> struct Vector3
{
        union
        {
                struct { R x, y, z; };
                
                struct { R r, g, b ; };
                struct { R h ,s , v; };
                struct { R e[3]; };
        };

        Vector3() {x=y=z=0;}
        Vector3(R x, R y, R z) : x(x), y(y), z(z) {};

        R magnitude()
        {
                return sqrt(x*x+y*y+z*z);
        }

        R magnitude2()
        {
                return x*x+y*y+z*z;
        }
        
        template <typename T> void magnitude2T(T &out)
        {
        T _x = (T)x;
        T _y = (T)y;
        T _z = (T)z;
        out = _x*_x+_y*_y+_z*_z;
    }
    
    R absSum()
    {
        return abs(x)+abs(y)+abs(z);
    }

        Vector3<R>& angles(R h, R v)
        {
                z = cos(v);
                x = cos(h)*z;
                y = sin(h)*z;
                z = sin(v);
                return *this;
        }

        void normalize()
        {
                R m = magnitude();
                if(m<=Const<R>::tol)
                        x = y = z = 0;
                else
                {
                        x/=m;
                        y/=m;
                        z/=m;
                }
        }
        Vector3<R> normalized()
        {
                R m = magnitude();
                if(m<=Const<R>::tol)
                        return Vector3<R>(0,0,0);
                else
                        return Vector3<R>(x/m,y/m,z/m);
        }

        template <typename T> void convert(Vector3<T> &out)
        {
                out.x = (T)x;
                out.y = (T)y;
                out.z = (T)z;
        }

        template <typename T> Vector3<T> convert()
        {
        	return Vector3<T>((T)x,(T)y,(T)z);
        }

        std::ostream& out(std::ostream &o)
        {
                o << '[' << x << ';' << y << ';' << z << ']';
                return o;
        }

        void mul(Vector3<R> v)
        {
                x*=v.x;
                y*=v.y;
                z*=v.z;
        }

        Vector3<R>& operator+=(Vector3<R> u)
        {
                x+=u.x;
                y+=u.y;
                z+=u.z;
                return *this;
        }

        Vector3<R>& operator-=(Vector3<R> u)
        {
                x-=u.x;
                y-=u.y;
                z-=u.z;
                return *this;
        }

        Vector3<R>& operator*=(R s)
        {
                x*=s;
                y*=s;
                z*=s;
                return *this;
        }

        Vector3<R>& operator/=(R s)
        {
                x/=s;
                y/=s;
                z/=s;
                return *this;
        }

        Vector3<R> operator-(void)
        {
                return Vector3<R>(-x,-y,-z);
        }
};

// IO

template <typename R> std::ostream& operator<< (std::ostream& o, Vector3<R> v)
{
        o << v.x << ' ' << v.y << ' ' << v.z;
        return o;
}

template <typename R> std::istream& operator>> (std::istream& i, Vector3<R> &v)
{
        i >> v.x >> v.y >> v.z;
        return i;
}

template <typename R> bool operator== (Vector3<R> u, Vector3<R> v)
{
        return u.x==v.x && u.y==v.y && u.z==v.z;
}

template <typename R> bool operator!= (Vector3<R> u, Vector3<R> v)
{
        return u.x!=v.x || u.y!=v.y || u.z!=v.z;
}

template <typename R> bool operator< (Vector3<R> u, Vector3<R> v)
{
        if(u.x==v.x)
        {
                if(u.y==v.y)
                        return u.z<v.z;
                return u.y<v.y;
        }
        return u.x<v.x;
}

template <typename R> Vector3<R> operator+(Vector3<R> u, Vector3<R> v)
{
        return Vector3<R>(u.x+v.x,u.y+v.y,u.z+v.z);
}

template <typename R> Vector3<R> operator-(Vector3<R> u, Vector3<R> v)
{
        return Vector3<R>(u.x-v.x,u.y-v.y,u.z-v.z);
}


// Vector3<R>cross product (u cross v)
template <typename R> Vector3<R> operator^(Vector3<R> u, Vector3<R> v)
{
        return Vector3<R>(      u.y*v.z - u.z*v.y,
                                        -u.x*v.z + u.z*v.x,
                                        u.x*v.y - u.y*v.x );
}
// Vector3<R>dot product
template <typename R> R operator*(Vector3<R> u, Vector3<R> v)
{
        return (u.x*v.x + u.y*v.y + u.z*v.z);
}

template <typename R> Vector3<R> operator*(R s, Vector3<R> u)
{
        return Vector3<R>(u.x*s, u.y*s, u.z*s);
}

template <typename R> Vector3<R> operator*(Vector3<R> u, R s)
{
        return Vector3<R>(u.x*s, u.y*s, u.z*s);
}

template <typename R> Vector3<R> operator/(Vector3<R> u, R s)
{
        return Vector3<R>(u.x/s, u.y/s, u.z/s);
}

// triple scalar product (u dot (v cross w))
/*template <typename R> R TripleScalarProduct(Vector3<R>u, Vector3<R>v, Vector3<R>w)
{
        return R(       (u.x * (v.y*w.z - v.z*w.y)) +
                                        (u.y * (-v.x*w.z + v.z*w.x)) +
                                        (u.z * (v.x*w.y - v.y*w.x)) );
        //u*(v^w);
}*/

// Vector4 typename and functions

template <typename R> struct Vector4
{
        union
        {
                struct { R x, y, z, w; };
                
                struct { R r, g, b, a; };
                struct { R e[4]; } ;
        };

        Vector4() {x=y=z=w=0;}
        Vector4(R x, R y, R z, R w) : x(x), y(y), z(z), w(w) {};

        R magnitude()
        {
                return sqrt(x*x+y*y+z*z+w*w);
        }

        R magnitude2()
        {
                return x*x+y*y+z*z+w*w;
        }
        
        template <typename T> void magnitude2T(T &out)
        {
        T _x = (T)x;
        T _y = (T)y;
        T _z = (T)z;
        T _w = (T)w;
        out = _x*_x+_y*_y+_z*_z+_w*_w;
    }
    
    R absSum()
    {
        return abs(x)+abs(y)+abs(z)+abs(w);
    }

        void normalize()
        {
                R m = magnitude();
                if(m<=Const<R>::tol)
                        x = y = z = w = 0;
                else
                {
                        x/=m;
                        y/=m;
                        z/=m;
                        w/=m;
                }
        }

        Vector4<R> normalized()
        {
                R m = magnitude();
                if(m<=Const<R>::tol)
                        return Vector4<R>(0,0,0,0);
                else
                        return Vector4<R>(x/m,y/m,z/m,w/m);
        }

        template <typename T> void convert(Vector4<T> &out)
        {
                out.x = (T)x;
                out.y = (T)y;
                out.z = (T)z;
                out.w = (T)w;
        }

        std::ostream& out(std::ostream &o)
        {
                o << '[' << x << ';' << y << ';' << z << ';' << w << ']';
                return o;
        }

        Vector4<R>& operator+=(Vector4<R> u)
        {
                x+=u.x;
                y+=u.y;
                z+=u.z;
                w+=u.w;
                return *this;
        }

        Vector4<R>& operator-=(Vector4<R> u)
        {
                x-=u.x;
                y-=u.y;
                z-=u.z;
                w-=u.w;
                return *this;
        }

        Vector4<R>& operator*=(R s)
        {
                x*=s;
                y*=s;
                z*=s;
                w*=s;
                return *this;
        }

        Vector4<R>& operator/=(R s)
        {
                x/=s;
                y/=s;
                z/=s;
                w/=s;
                return *this;
        }

        Vector4<R> operator-(void)
        {
                return Vector4<R>(-x,-y,-z,-w);
        }
};

// IO

template <typename R> std::ostream& operator<< (std::ostream& o, Vector4<R> v)
{
        o << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w;
        return o;
}

template <typename R> std::istream& operator>> (std::istream& i, Vector4<R> &v)
{
        i >> v.x >> v.y >> v.z >> v.w;
        return i;
}

template <typename R> bool operator== (Vector4<R> u, Vector4<R> v)
{
        return u.x==v.x && u.y==v.y && u.z==v.z && u.w==v.w;
}

template <typename R> bool operator!= (Vector4<R> u, Vector4<R> v)
{
        return u.x!=v.x || u.y!=v.y || u.z!=v.z || u.w!=v.w;
}

template <typename R> bool operator< (Vector4<R> u, Vector4<R> v)
{
        if(u.x==v.x)
        {
                if(u.y==v.y)
                {
                        if(u.z==v.z)
                                return u.w<v.w;
                        return u.z<v.z;
                }
                return u.y<v.y;
        }
        return u.x<v.x;
}

template <typename R> Vector4<R> operator+(Vector4<R> u, Vector4<R> v)
{
        return Vector3<R>(u.x+v.x,u.y+v.y,u.z+v.z,u.w+v.w);
}

template <typename R> Vector4<R> operator-(Vector4<R> u, Vector4<R> v)
{
        return Vector4<R>(u.x-v.x,u.y-v.y,u.z-v.z,u.w-v.w);
}

// Vector4 cross product (u cross v)
/*template <typename R> Vector3<R> operator^(Vector3<R> u, Vector3<R> v)
{
        return Vector3<R>(      u.y*v.z - u.z*v.y,
                                        -u.x*v.z + u.z*v.x,
                                        u.x*v.y - u.y*v.x );
}*/

// Vector4 dot product
template <typename R> R operator*(Vector4<R> u, Vector4<R> v)
{
        return (u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w);
}

template <typename R> Vector4<R> operator*(R s, Vector4<R> u)
{
        return Vector4<R>(u.x*s, u.y*s, u.z*s, u.w*s);
}

template <typename R> Vector4<R> operator*(Vector4<R> u, R s)
{
        return Vector4<R>(u.x*s, u.y*s, u.z*s, u.w*s);
}

template <typename R> Vector4<R> operator/(Vector4<R> u, R s)
{
        return Vector4<R>(u.x/s, u.y/s, u.z/s, u.w/s);
}

//------------------------------------------------------------------------//
// Matrix typename and matrix functions
//------------------------------------------------------------------------//

template <typename R> struct Matrix3x3
{
        // elements eij: i -> row, j -> column
        union
        {
        struct
        {
                R e11, e12, e13, e21, e22, e23, e31, e32, e33;
        };
        struct
        {
            R e[9];
        };
        };

        Matrix3x3()
        {
                e11 = 0;
                e12 = 0;
                e13 = 0;
                e21 = 0;
                e22 = 0;
                e23 = 0;
                e31 = 0;
                e32 = 0;
                e33 = 0;
        }

        Matrix3x3(      R r1c1, R r1c2, R r1c3, 
                                R r2c1, R r2c2, R r2c3, 
                                R r3c1, R r3c2, R r3c3 )
        {
                e11 = r1c1;
                e12 = r1c2;
                e13 = r1c3;
                e21 = r2c1;
                e22 = r2c2;
                e23 = r2c3;
                e31 = r3c1;
                e32 = r3c2;
                e33 = r3c3;
        }

        R det(void)
        {
                return  e11*e22*e33 - 
                                e11*e32*e23 + 
                                e21*e32*e13 - 
                                e21*e12*e33 + 
                                e31*e12*e23 - 
                                e31*e22*e13;    
        }

        Matrix3x3<R> transpose(void)
        {
                return Matrix3x3<R>(e11,e21,e31,e12,e22,e32,e13,e23,e33);
        }

        Matrix3x3<R> inverse(void)
        {
                R       d = e11*e22*e33 - 
                                        e11*e32*e23 + 
                                        e21*e32*e13 - 
                                        e21*e12*e33 + 
                                        e31*e12*e23 - 
                                        e31*e22*e13;

                if (d == 0) d = 1;

                return  Matrix3x3<R>(   (e22*e33-e23*e32)/d,
                                                        -(e12*e33-e13*e32)/d,
                                                        (e12*e23-e13*e22)/d,
                                                        -(e21*e33-e23*e31)/d,
                                                        (e11*e33-e13*e31)/d,
                                                        -(e11*e23-e13*e21)/d,
                                                        (e21*e32-e22*e31)/d,
                                                        -(e11*e32-e12*e31)/d,
                                                        (e11*e22-e12*e21)/d );  
        }

        Matrix3x3<R>& operator+=(Matrix3x3<R> m)
        {
                e11 += m.e11;
                e12 += m.e12;
                e13 += m.e13;
                e21 += m.e21;
                e22 += m.e22;
                e23 += m.e23;
                e31 += m.e31;
                e32 += m.e32;
                e33 += m.e33;
                return *this;
        }

        Matrix3x3<R>& operator-=(Matrix3x3<R> m)
        {
                e11 -= m.e11;
                e12 -= m.e12;
                e13 -= m.e13;
                e21 -= m.e21;
                e22 -= m.e22;
                e23 -= m.e23;
                e31 -= m.e31;
                e32 -= m.e32;
                e33 -= m.e33;
                return *this;
        }

        Matrix3x3<R>& operator*=(R s)
        {
                e11 *= s;
                e12 *= s;
                e13 *= s;
                e21 *= s;
                e22 *= s;
                e23 *= s;
                e31 *= s;
                e32 *= s;
                e33 *= s;
                return *this;
        }

        Matrix3x3<R>& operator/=(R s)
        {
                e11 /= s;
                e12 /= s;
                e13 /= s;
                e21 /= s;
                e22 /= s;
                e23 /= s;
                e31 /= s;
                e32 /= s;
                e33 /= s;
                return *this;
        }
};

template <typename R> Matrix3x3<R> operator+(Matrix3x3<R> m1, Matrix3x3<R> m2)
{
        return  Matrix3x3<R>(   m1.e11+m2.e11,
                                                m1.e12+m2.e12,
                                                m1.e13+m2.e13,
                                                m1.e21+m2.e21,
                                                m1.e22+m2.e22,
                                                m1.e23+m2.e23,
                                                m1.e31+m2.e31,
                                                m1.e32+m2.e32,
                                                m1.e33+m2.e33);
}

template <typename R> Matrix3x3<R> operator-(Matrix3x3<R> m1, Matrix3x3<R> m2)
{
        return  Matrix3x3<R>(   m1.e11-m2.e11,
                                                m1.e12-m2.e12,
                                                m1.e13-m2.e13,
                                                m1.e21-m2.e21,
                                                m1.e22-m2.e22,
                                                m1.e23-m2.e23,
                                                m1.e31-m2.e31,
                                                m1.e32-m2.e32,
                                                m1.e33-m2.e33);
}

template <typename R> Matrix3x3<R> operator/(Matrix3x3<R> m, R s)
{       
        return  Matrix3x3<R>(   m.e11/s,
                                                m.e12/s,
                                                m.e13/s,
                                                m.e21/s,
                                                m.e22/s,
                                                m.e23/s,
                                                m.e31/s,
                                                m.e32/s,
                                                m.e33/s);
}

template <typename R> Matrix3x3<R> operator*(Matrix3x3<R> m1, Matrix3x3<R> m2)
{
        return Matrix3x3<R>(    m1.e11*m2.e11 + m1.e12*m2.e21 + m1.e13*m2.e31,
                                                m1.e11*m2.e12 + m1.e12*m2.e22 + m1.e13*m2.e32,
                                                m1.e11*m2.e13 + m1.e12*m2.e23 + m1.e13*m2.e33,
                                                m1.e21*m2.e11 + m1.e22*m2.e21 + m1.e23*m2.e31,
                                                m1.e21*m2.e12 + m1.e22*m2.e22 + m1.e23*m2.e32,
                                                m1.e21*m2.e13 + m1.e22*m2.e23 + m1.e23*m2.e33,
                                                m1.e31*m2.e11 + m1.e32*m2.e21 + m1.e33*m2.e31,
                                                m1.e31*m2.e12 + m1.e32*m2.e22 + m1.e33*m2.e32,
                                                m1.e31*m2.e13 + m1.e32*m2.e23 + m1.e33*m2.e33 );
}

template <typename R> Matrix3x3<R> operator*(Matrix3x3<R> m, R s)
{
        return  Matrix3x3<R>(   m.e11*s,
                                                m.e12*s,
                                                m.e13*s,
                                                m.e21*s,
                                                m.e22*s,
                                                m.e23*s,
                                                m.e31*s,
                                                m.e32*s,
                                                m.e33*s);
}

template <typename R> Matrix3x3<R> operator*(R s, Matrix3x3<R> m)
{
        return  Matrix3x3<R>(   m.e11*s,
                                                m.e12*s,
                                                m.e13*s,
                                                m.e21*s,
                                                m.e22*s,
                                                m.e23*s,
                                                m.e31*s,
                                                m.e32*s,
                                                m.e33*s);
}

template <typename R> Vector3<R> operator*(Matrix3x3<R> m, Vector3<R>u)
{
        return Vector3<R>(      m.e11*u.x + m.e12*u.y + m.e13*u.z,
                                        m.e21*u.x + m.e22*u.y + m.e23*u.z,
                                        m.e31*u.x + m.e32*u.y + m.e33*u.z);                                     
}

template <typename R> Vector3<R> operator*(Vector3<R>u, Matrix3x3<R> m)
{
        return Vector3<R>(      u.x*m.e11 + u.y*m.e21 + u.z*m.e31,
                                        u.x*m.e12 + u.y*m.e22 + u.z*m.e32,
                                        u.x*m.e13 + u.y*m.e23 + u.z*m.e33);
}

template <typename R> struct Matrix4x4
{
        R m[16];

        Vector3<R> transform(Vector3<R> u)
        {
                R r[3];
                for ( int i = 0; i < 3; ++i )
                        r[i] = u.e[0] * m[i] + u.e[1] * m[4+i] + u.e[2] + m[8+i] + m[12+i];
                return Vector3<R>(r[0],r[1],r[2]);
        }

        Matrix4x4<R>& translate(Vector3<R> v)
        {
                //m[3] = v.x, m[7] = v.y, m[11] = v.z;
                
                //m[1] = m[2] = m[4] = m[6] = m[8] = m[9] = m[12] = m[13] = m[14] = 0.0;
                //m[0] = m[5] = m[10] = m[15] = 1.0;
                translate(v.x,v.y,v.z);
                return *this;
        }

        Matrix4x4<R>& translate(R x, R y, R z)
        {
                //m[3] = x, m[7] = y, m[11] = z;
                //m[1] = m[2] = m[4] = m[6] = m[8] = m[9] = m[12] = m[13] = m[14] = 0.0;
                //m[0] = m[5] = m[10] = m[15] = 1.0;
                m[0] = m[5] = m[10] = m[15] = 1.0;
                //-------
                m[1] = m[2] = m[4] = m[6] = m[8] = m[9] = m[3] = m[7] = m[11] = 0.0;
                m[12] = x;
                m[13] = y;
                m[14] = z;
                return *this;
        }

        Matrix4x4<R>& rotate(double angle, Vector3<R> axis)
        {
                axis.normalize();
                if(IsZero(axis.magnitude()))
                {
                        angle = 0.0;
                        axis.x = 1.0;
                        axis.y = 0.0;
                        axis.z = 0.0;
                }
                R s = sin(-angle), c = cos(angle);
                m[0] = axis.x*axis.x*(1.0-c)+c;
                m[1] = axis.x*axis.y*(1.0-c)-axis.z*s;
                m[2] = axis.x*axis.z*(1.0-c)+axis.y*s;
                m[4] = axis.y*axis.x*(1.0-c)+axis.z*s;
                m[5] = axis.y*axis.y*(1.0-c)+c;
                m[6] = axis.y*axis.z*(1.0-c)-axis.x*s;
                m[8] = axis.z*axis.x*(1.0-c)-axis.y*s;
                m[9] = axis.z*axis.y*(1.0-c)+axis.x*s;
                m[10] = axis.z*axis.z*(1.0-c)+c;
                m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0.0;
                m[15] = 1.0;
                return *this;
        }

        Matrix4x4<R> scale(Vector3<R> s)
        {
                m[0] = s.x, m[5] = s.y, m[10] = s.z;
                m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0;
                m[15] = 1.0;
                return *this;
        }

        Matrix4x4<R> scale(R x, R y, R z)
        {
                m[0] = x, m[5] = y, m[10] = z;
                m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0;
                m[15] = 1.0;
                return *this;
        }

        Matrix4x4<R> direction(Vector3<R> direction, Vector3<R> up)
        {
            Vector3<R> f = direction.normalized();
            up.normalize();

            Vector3<R> s = f^up;
            Vector3<R> u = s^f;

            m[0] = s.x;
            m[1] = u.x;
            m[2] = -f.x;
            m[4] = s.y;
            m[5] = u.y;
            m[6] = -f.y;
            m[8] = s.z;
            m[9] = u.z;
            m[10] = -f.z;
            m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0.0;
            m[15] = 1.0;

            return *this;
        }

        Matrix4x4<R> perspective(R fovy, R aspect, R zNear, R zFar)
        {
            R f = 1.0/tan((PI/360.0)*fovy);
            m[0] = f/aspect;
            m[5] = f;
            m[10] = (zNear+zFar)/(zNear-zFar);
            m[14] = (2.0*zNear*zFar)/(zNear-zFar);
            m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[12] = m[13] = m[15] = 0.0;
            m[11] = -1.0;
        }

        Matrix4x4<R> inversed()
        {
                Matrix4x4<R> inv;

                inv.m[0] = m[5]  * m[10] * m[15] - 
                m[5]  * m[11] * m[14] - 
                m[9]  * m[6]  * m[15] + 
                m[9]  * m[7]  * m[14] +
                m[13] * m[6]  * m[11] - 
                m[13] * m[7]  * m[10];

                inv.m[4] = -m[4]  * m[10] * m[15] + 
                m[4]  * m[11] * m[14] + 
                m[8]  * m[6]  * m[15] - 
                m[8]  * m[7]  * m[14] - 
                m[12] * m[6]  * m[11] + 
                m[12] * m[7]  * m[10];

                inv.m[8] = m[4]  * m[9] * m[15] - 
                m[4]  * m[11] * m[13] - 
                m[8]  * m[5] * m[15] + 
                m[8]  * m[7] * m[13] + 
                m[12] * m[5] * m[11] - 
                m[12] * m[7] * m[9];

                inv.m[12] = -m[4]  * m[9] * m[14] + 
                m[4]  * m[10] * m[13] +
                m[8]  * m[5] * m[14] - 
                m[8]  * m[6] * m[13] - 
                m[12] * m[5] * m[10] + 
                m[12] * m[6] * m[9];

                inv.m[1] = -m[1]  * m[10] * m[15] + 
                m[1]  * m[11] * m[14] + 
                m[9]  * m[2] * m[15] - 
                m[9]  * m[3] * m[14] - 
                m[13] * m[2] * m[11] + 
                m[13] * m[3] * m[10];

                inv.m[5] = m[0]  * m[10] * m[15] - 
                m[0]  * m[11] * m[14] - 
                m[8]  * m[2] * m[15] + 
                m[8]  * m[3] * m[14] + 
                m[12] * m[2] * m[11] - 
                m[12] * m[3] * m[10];

                inv.m[9] = -m[0]  * m[9] * m[15] + 
                m[0]  * m[11] * m[13] + 
                m[8]  * m[1] * m[15] - 
                m[8]  * m[3] * m[13] - 
                m[12] * m[1] * m[11] + 
                m[12] * m[3] * m[9];

                inv.m[13] = m[0]  * m[9] * m[14] - 
                m[0]  * m[10] * m[13] - 
                m[8]  * m[1] * m[14] + 
                m[8]  * m[2] * m[13] + 
                m[12] * m[1] * m[10] - 
                m[12] * m[2] * m[9];

                inv.m[2] = m[1]  * m[6] * m[15] - 
                m[1]  * m[7] * m[14] - 
                m[5]  * m[2] * m[15] + 
                m[5]  * m[3] * m[14] + 
                m[13] * m[2] * m[7] - 
                m[13] * m[3] * m[6];

                inv.m[6] = -m[0]  * m[6] * m[15] + 
                m[0]  * m[7] * m[14] + 
                m[4]  * m[2] * m[15] - 
                m[4]  * m[3] * m[14] - 
                m[12] * m[2] * m[7] + 
                m[12] * m[3] * m[6];

                inv.m[10] = m[0]  * m[5] * m[15] - 
                m[0]  * m[7] * m[13] - 
                m[4]  * m[1] * m[15] + 
                m[4]  * m[3] * m[13] + 
                m[12] * m[1] * m[7] - 
                m[12] * m[3] * m[5];

                inv.m[14] = -m[0]  * m[5] * m[14] + 
                m[0]  * m[6] * m[13] + 
                m[4]  * m[1] * m[14] - 
                m[4]  * m[2] * m[13] - 
                m[12] * m[1] * m[6] + 
                m[12] * m[2] * m[5];

                inv.m[3] = -m[1] * m[6] * m[11] + 
                m[1] * m[7] * m[10] + 
                m[5] * m[2] * m[11] - 
                m[5] * m[3] * m[10] - 
                m[9] * m[2] * m[7] + 
                m[9] * m[3] * m[6];

                inv.m[7] = m[0] * m[6] * m[11] - 
                m[0] * m[7] * m[10] - 
                m[4] * m[2] * m[11] + 
                m[4] * m[3] * m[10] + 
                m[8] * m[2] * m[7] - 
                m[8] * m[3] * m[6];

                inv.m[11] = -m[0] * m[5] * m[11] + 
                m[0] * m[7] * m[9] + 
                m[4] * m[1] * m[11] - 
                m[4] * m[3] * m[9] - 
                m[8] * m[1] * m[7] + 
                m[8] * m[3] * m[5];

                inv.m[15] = m[0] * m[5] * m[10] - 
                m[0] * m[6] * m[9] - 
                m[4] * m[1] * m[10] + 
                m[4] * m[2] * m[9] + 
                m[8] * m[1] * m[6] - 
                m[8] * m[2] * m[5];

                R det = m[0] * inv.m[0] + m[1] * inv.m[4] + m[2] * inv.m[8] + m[3] * inv.m[12];

                if (IsZero(det))
                                return inv;

                det = (R)1.0/det;

                for (int i = 0; i < 16; i++)
                        inv.m[i] *= det;

                return inv;
        }

        Matrix4x4<R>& ortho(R left, R right, R bottom, R top, R _near, R _far)
        {
                m[0] = 2.0/(right-left);
                m[5] = 2.0/(top-bottom);
                m[10] = 2.0/(_near-_far);
                m[12] = (right+left)/(left-right);
                m[13] = (top+bottom)/(bottom-top);
                m[14] = (_near+_far)/(_near-_far);
                m[1] = m[2] = m[4] = m[6] = m[8] = m[9] = m[3] = m[7] = m[11] = 0.0;
                m[15] = 1.0;
                return *this;
        }

        /*Matrix4x4<R>& operator=(Matrix4x4<R> o)
        {
                for(int i=0;i<16;i++)
                        m[i] = o.m[i];
        }*/

        /*Matrix4x4<R>& operator*=(Matrix4x4<R> b)
        {
                Matrix4x4<R> m, &a = *this;
                int i, j, k;
                for(i=0;i<4;i++)
                for(j=0;j<4;j++)
                for(k=0;k<4;k++)
                        m.m[4*j+i] += a.m[k+4*j]*b.m[i+4*k];
                a = m;
                return a;
        }*/

        template<typename T> Matrix4x4<T> convert()
        {
                Matrix4x4<T> t;
                for(int i=0;i<16;i++)
                        t.m[i] = (T)m[i];
                return t;
        }

        Matrix4x4(R a = 1)
        {
                m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0;
                m[0] = m[5] = m[10] = m[15] = a;
        }
};

template <typename R> Matrix4x4<R> operator*(Matrix4x4<R> a, Matrix4x4<R> b)
{
        Matrix4x4<R> m(0);
        int i, j, k;
        for(i=0;i<4;i++)
        for(j=0;j<4;j++)
        for(k=0;k<4;k++)
                m.m[j+4*i] += a.m[4*k+j]*b.m[4*i+k];
        return m;
}

//------------------------------------------------------------------------//
// Quaternion<R> typename and Quaternion<R> functions
//------------------------------------------------------------------------//

template <typename R> struct Quaternion
{
        R n;    // number (scalar) part
        Vector3<R> v;   // Vector3<R>part: v.x, v.y, v.z

        Quaternion()
        {
                n = 1.0;
                v.x = 0.0;
                v.y =  0.0;
                v.z = 0.0;
        }

        // cos(angle/2)+sin(angle/2)*axis

        Quaternion(R _n, R x, R y, R z)
        {
                n = _n;
                v.x = x;
                v.y = y;
                v.z = z;
        }

        void set(R _n, R x, R y, R z)
        {
                n = _n;
                v.x = x;
                v.y = y;
                v.z = z;
        }
        
        R magnitude(void)
        {
                return (R) sqrt(n*n + v.x*v.x + v.y*v.y + v.z*v.z);
        }
        
        void normalize()
        {
                R m = magnitude();
                if(m<=Const<R>::tol)
                        n = v.x = v.y = v.z = 0;
                else
                {
                        n/=m;
                        v.x/=m;
                        v.y/=m;
                        v.z/=m;
                }
        }

        Quaternion<R> normalized()
        {
                R m = magnitude();
                if(m<=Const<R>::tol)
                        return Quaternion<R>(0,0,0,0);
                else
                        return Quaternion<R>(n/m,v.x/m,v.y/m,v.z/m);
        }

        Vector3<R> getVector(void)
        {
                return Vector3<R>(v.x, v.y, v.z);
        }

        R getScalar(void)
        {
                return n;
        }

        Quaternion<R> operator+=(Quaternion<R> q)
        {
                n += q.n;
                v.x += q.v.x;
                v.y += q.v.y;
                v.z += q.v.z;
                return *this;
        }

        Quaternion<R> operator-=(Quaternion<R> q)
        {
                n -= q.n;
                v.x -= q.v.x;
                v.y -= q.v.y;
                v.z -= q.v.z;
                return *this;
        }

        Quaternion<R> operator*=(R s)
        {
                n *= s;
                v.x *= s;
                v.y *= s;
                v.z *= s;
                return *this;
        }

        Quaternion<R> operator/=(R s)
        {
                n /= s;
                v.x /= s;
                v.y /= s;
                v.z /= s;
                return *this;
        }

        Quaternion<R> operator~()
        {
                Quaternion<R> q;
                q.n = n;
                q.v = -v;
                return q;
        }
};

template <typename R> std::ostream& operator<< (std::ostream& o, Quaternion<R> q)
{
        o << q.n << ' ' << q.v.x << ' ' << q.v.y << ' ' << q.v.z;
        return o;
}

template <typename R> std::istream& operator>> (std::istream& i, Quaternion<R> &q)
{
        R s;
        i >> q.n >> q.v.x >> q.v.y >> q.v.z;
        if(q.n==0)
        {
                q.n = 1;
                q.v.x=q.v.y=q.v.z=0;
                return i;
        }
        q.n = q.n*(R)PI/(R)180.0;
        s = sin(q.n/2);
        q.n = cos(q.n/2);
        q.v.normalize();
        q.v*=s;
        return i;
}

/*template <typename R> Quaternion      Quaternion::operator~()
{
        return Quaternion<R>(n, -v.x, -v.y, -v.z);
}*/

template <typename R> Quaternion<R> operator+(Quaternion<R> q1, Quaternion<R> q2)
{
        Quaternion<R> q;
        q.n = q1.n + q2.n;
        q.v.x = q1.v.x + q2.v.x;
        q.v.y = q1.v.y + q2.v.y;
        q.v.z = q1.v.z + q2.v.z;
        return  q;
}

template <typename R> Quaternion<R> operator-(Quaternion<R> q1, Quaternion<R> q2)
{
        Quaternion<R> q;
        q.set(  q1.n - q2.n,
                                                        q1.v.x - q2.v.x,
                                                        q1.v.y - q2.v.y,
                                                        q1.v.z - q2.v.z);
        return  q;
}

template <typename R> Quaternion<R> operator*(Quaternion<R> q1, Quaternion<R> q2)
{
        Quaternion<R> q;
        q.set(  q1.n*q2.n - q1.v.x*q2.v.x - q1.v.y*q2.v.y - q1.v.z*q2.v.z,
                                                        q1.n*q2.v.x + q1.v.x*q2.n + q1.v.y*q2.v.z - q1.v.z*q2.v.y,
                                                        q1.n*q2.v.y + q1.v.y*q2.n + q1.v.z*q2.v.x - q1.v.x*q2.v.z,
                                                        q1.n*q2.v.z + q1.v.z*q2.n + q1.v.x*q2.v.y - q1.v.y*q2.v.x);     
        return  q;
}

template <typename R> Quaternion<R> operator*(Quaternion<R> q, R s)
{
        Quaternion<R> qr;
        qr.set(q.n*s, q.v.x*s, q.v.y*s, q.v.z*s);
        return qr;
}

template <typename R> Quaternion<R> operator*(R s, Quaternion<R> q)
{
        Quaternion<R> qr;
        qr.set(q.n*s, q.v.x*s, q.v.y*s, q.v.z*s);
        return qr;
}

template <typename R> Quaternion<R> operator*(Quaternion<R> q, Vector3<R> v)
{
        Quaternion<R> qr;
        qr.set( -(q.v.x*v.x + q.v.y*v.y + q.v.z*v.z),
                                                        q.n*v.x + q.v.y*v.z - q.v.z*v.y,
                                                        q.n*v.y + q.v.z*v.x - q.v.x*v.z,
                                                        q.n*v.z + q.v.x*v.y - q.v.y*v.x);
        return  qr;
}

template <typename R> Quaternion<R> operator*(Vector3<R> v, Quaternion<R> q)
{
        Quaternion<R> qr;
        qr.set( -(q.v.x*v.x + q.v.y*v.y + q.v.z*v.z),
                                                        q.n*v.x + q.v.z*v.y - q.v.y*v.z,
                                                        q.n*v.y + q.v.x*v.z - q.v.z*v.x,
                                                        q.n*v.z + q.v.y*v.x - q.v.x*v.y);
        return  qr;
}

template <typename R> Quaternion<R> operator/(Quaternion<R> q, R s)
{
        Quaternion<R> qr;
        qr.set(q.n/s, q.v.x/s, q.v.y/s, q.v.z/s);
        return  qr;
}

template <typename R> R QGetAngle(Quaternion<R> q)
{
        return  (R)(2.0*acos(q.n));
}

template <typename R> Vector3<R> QGetAxis(Quaternion<R> q)
{
        Vector3<R> v;
        R m;

        v = q.getVector();
        m = v.magnitude();
        
        if (m <= Const<R>::tol)
                return Vector3<R>(0,0,0);
        else
                return v/m;     
}

template <typename R> Quaternion<R> QRotate(Quaternion<R> q1, Quaternion<R> q2)
{
        return  q1*q2*(~q1);
}

template <typename R> Vector3<R> QVRotateOld(Quaternion<R> q, Vector3<R>v)
{
        Quaternion<R> t;

        t = q*v*(~q);

        return  t.getVector();
}

template <typename R> Vector3<R> QVRotate(Quaternion<R> q, Vector3<R> v)
{
    R n = q.n*q.n, a = q.v.x*q.v.x, b = q.v.y*q.v.y, c = q.v.z*q.v.z;
    return Vector3<R>(  v.x*(n+a-b-c)+2.0*(v.y*(q.v.x*q.v.y-q.v.z*q.n)+v.z*(q.v.x*q.v.z+q.v.y*q.n)),
                        v.y*(n-a+b-c)+2.0*(v.x*(q.v.x*q.v.y+q.v.z*q.n)+v.z*(q.v.y*q.v.z-q.v.x*q.n)),
                        v.z*(n-a-b+c)+2.0*(v.x*(q.v.x*q.v.z-q.v.y*q.n)+v.y*(q.v.y*q.v.z+q.v.x*q.n))
                        );
}

template <typename R> Quaternion<R> MakeQFromEulerAngles(R x, R y, R z)
{
        Quaternion<R> q;
        R       roll = DegToRad(x);
        R       pitch = DegToRad(y);
        R       yaw = DegToRad(z);
        
        R       cyaw, cpitch, croll, syaw, spitch, sroll;
        R       cyawcpitch, syawspitch, cyawspitch, syawcpitch;

        cyaw = cos((R)0.5 * yaw);
        cpitch = cos((R)0.5 * pitch);
        croll = cos((R)0.5 * roll);
        syaw = sin((R)0.5 * yaw);
        spitch = sin((R)0.5 * pitch);
        sroll = sin((R)0.5 * roll);

        cyawcpitch = cyaw*cpitch;
        syawspitch = syaw*spitch;
        cyawspitch = cyaw*spitch;
        syawcpitch = syaw*cpitch;

        q.n = (R) (cyawcpitch * croll + syawspitch * sroll);
        q.v.x = (R) (cyawcpitch * sroll - syawspitch * croll); 
        q.v.y = (R) (cyawspitch * croll + syawcpitch * sroll);
        q.v.z = (R) (syawcpitch * croll - cyawspitch * sroll);

        return q;
}

template <typename R> Vector3<R> MakeEulerAnglesFromQ(Quaternion<R> q)
{
        R       r11, r21, r31, r32, r33, r12, r13;
        R       q00, q11, q22, q33;
        R       tmp;
        Vector3<R> u;

        q00 = q.n * q.n;
        q11 = q.v.x * q.v.x;
        q22 = q.v.y * q.v.y;
        q33 = q.v.z * q.v.z;

        r11 = q00 + q11 - q22 - q33;
        r21 = 2 * (q.v.x*q.v.y + q.n*q.v.z);
        r31 = 2 * (q.v.x*q.v.z - q.n*q.v.y);
        r32 = 2 * (q.v.y*q.v.z + q.n*q.v.x);
        r33 = q00 - q11 - q22 + q33;

        tmp = fabs(r31);
        if(tmp > (R)0.99999)
        {
                r12 = 2 * (q.v.x*q.v.y - q.n*q.v.z);
                r13 = 2 * (q.v.x*q.v.z + q.n*q.v.y);

                u.x = RadToDeg(0.0f); //roll
                u.y = RadToDeg((R) (-(PI/2) * r31/tmp)); // pitch
                u.z = RadToDeg((R) atan2(-r12, -r31*r13)); // yaw
                return u;
        }

        u.x = RadToDeg((R) atan2(r32, r33)); // roll
        u.y = RadToDeg((R) asin(-r31));          // pitch
        u.z = RadToDeg((R) atan2(r21, r11)); // yaw
        return u;
        

}

template <typename R> Matrix3x3<R> MakeMatrixFromQuaternion(Quaternion<R> q)
{
        R       q00, q11, q22, q33;
        
        Matrix3x3<R>    m;

        q00 = q.n * q.n;
        q11 = q.v.x * q.v.x;
        q22 = q.v.y * q.v.y;
        q33 = q.v.z * q.v.z;

        m.e11 = q00 + q11 - q22 - q33;
        m.e21 = 2 * (q.v.x*q.v.y + q.n*q.v.z);
        m.e31 = 2 * (q.v.x*q.v.z - q.n*q.v.y);
        
        m.e12 = 2 * (q.v.x*q.v.y - q.n*q.v.z);
        m.e22 = q00 - q11 + q22 - q33;
        m.e32 = 2 * (q.v.y*q.v.z + q.n*q.v.x);
        
        m.e13 = 2 * (q.v.x*q.v.z + q.n*q.v.y);
        m.e23 = 2 * (q.v.y*q.v.z - q.n*q.v.x);
        m.e33 = q00 - q11 - q22 + q33;

        return m;
}

template <typename R> R PTPDist(Vector3<R> &p, Vector3<R> &pn, Vector3<R> &n)
{
        return (p-pn)*n;
}

template <typename R> R ptridis(Vector3<R> p, Vector3<R> *v)
{
        Vector3<R> n = (v[1]-v[0])^(v[2]-v[0]).normalized();
}

template <typename T, typename R> Vector3<T> V3Subtract(Vector3<R> a, Vector3<R> b)
{
        return Vector3<T>((T)(a.x-b.x),(T)(a.y-b.y),(T)(a.z-b.z));
}

template <typename T, typename R> Vector3<T> V3Add(Vector3<R> a, Vector3<R> b)
{
        return Vector3<T>((T)(a.x+b.x),(T)(a.y+b.y),(T)(a.z+b.z));
}

typedef Vector2<float> Vector2f;
typedef Vector3<float> Vector3f;
typedef Vector4<float> Vector4f;
typedef Matrix3x3<float> Matrix3x3f;
typedef Matrix4x4<float> Matrix4x4f;
typedef Quaternion<float> Quaternionf;

typedef Vector2<double> Vector2d;
typedef Vector3<double> Vector3d;
typedef Vector4<double> Vector4d;
typedef Matrix3x3<double> Matrix3x3d;
typedef Matrix4x4<double> Matrix4x4d;
typedef Quaternion<double> Quaterniond;

typedef Vector2<long double> Vector2ld;
typedef Vector3<long double> Vector3ld;
typedef Vector4<long double> Vector4ld;
typedef Matrix3x3<long double> Matrix3x3ld;
typedef Matrix4x4<long double> Matrix4x4ld;
typedef Quaternion<long double> Quaternionld;

typedef Vector2<int> Vector2i;
typedef Vector3<int> Vector3i;
typedef Vector4<int> Vector4i;
typedef Matrix3x3<int> Matrix3x3i;
typedef Matrix4x4<int> Matrix4x4i;

typedef Vector2<long long> Vector2li;
typedef Vector3<long long> Vector3li;
typedef Vector4<long long> Vector4li;
typedef Matrix3x3<long long> Matrix3x3li;
typedef Matrix4x4<long long> Matrix4x4li;

template <typename R> bool QuadraticEquation(R a, R b, R c, R &x1, R &x2)
{
        R delta = b*b-4*a*c;
        if(delta<0)
                return false;
        delta = sqrt(delta);
        x1 = (-b-delta)/(2*a);
        x2 = (-b+delta)/(2*a);
        return true;
}

template <typename R> R AngleDist(R a, R b)
{
        R d = abs(a-b);
        if(d>PI)
                return (R)2*PI-d;
        return d;
}

template <typename C> C Mod(C x, C m)
{
        if(m<=0)
                return 0;
        if(x<0)
                return m-((-x)%m)-1;
        return x%m;
}

template <typename R> struct Polynomial
{
    int n;
    R *v;
    
    Polynomial<R>& operator= (Polynomial<R> &p)
    {
        if(n!=p.n)
        {
            delete [] v;
            v = new R[p.n];
            n = p.n;
        }
        for(int i=0;i<n;i++)
            v[i] = p.v[i];
        return *this;
    }
    
    static void div(Polynomial<R> &p, Polynomial<R> &q, Polynomial<R> &w, Polynomial<R> &r)
    {
        int i = p.n-1, j = q.n-1, k;
        r = p;
        std::cout << "$1\n";
        while(i>=0 && IsZero(r.v[i])) i--;
        while(j>=0 && IsZero(q.v[j])) j--;
                std::cout << "$2\n";
        if(j<0 || i<j)
            return;
                    std::cout << "$3\n";
        while(i>=0)
        {
            w.v[i-j] = r.v[i]/q.v[j];
            for(k=0;k<r.n;k++)
                r.v[k] -= w.v[i-j]*q.v[k+i-j];
            r.v[i] = 0, i--;
            while(i>=0 && IsZero(r.v[i])) i--;
        }
    }
    
    R& operator[] (int i)
    {
        return v[i];
    }
    
    Polynomial(int _n)
    {
        n = _n;
        v = new R[n];
        for(int i=0;i<n;i++)
            v[i] = 0;
    }
    
    template<typename RC> Polynomial(Polynomial<RC> p)
    {
        n = p.n;
        v = new R[n];
        for(int i=0;i<n;i++)
            v[i] = p.v[i];
    }
    
    template<typename RC> Polynomial(Polynomial<RC> &p)
    {
        n = p.n;
        v = new R[n];
        for(int i=0;i<n;i++)
            v[i] = p.v[i];
    }
    
    Polynomial()
    {
        v = 0;
        n = 0;
    }

    ~Polynomial()
    {
        delete [] v;
        v = 0;
    }
};

template <typename R> Polynomial<R> operator+ (Polynomial<R> &a, Polynomial<R> &b)
{
    int i, n = max(a.n,b.n);
    Polynomial<R> p(n);
    for(i=0;i<n;i++)
    {
        if(i<a.n)
            p.v[i]+=a.v[i];
        if(i<b.n)
            p.v[i]+=b.v[i];
    }
    return p;
}

template <typename R> Polynomial<R> operator- (Polynomial<R> &a, Polynomial<R> &b)
{
    int i, n = max(a.n,b.n);
    Polynomial<R> p(n);
    for(i=0;i<n;i++)
    {
        if(i<a.n)
            p.v[i]+=a.v[i];
        if(i<b.n)
            p.v[i]-=b.v[i];
    }
    return p;
}

template <typename R> Polynomial<R> operator* (Polynomial<R> &a, Polynomial<R> &b)
{
    int i, j, n = a.n+b.n-1;
    Polynomial<R> p(n);
    for(i=0;i<a.n;i++)
    for(j=0;j<b.n;j++)
    {
        p.v[i+j] += a.v[i]*b.v[j];
    }
    return p;
}

template<typename R> std::istream& operator>> (std::istream &is, Polynomial<R> &p)
{
    for(int i=0;i<p.n;i++)
    {
        is >> p.v[i];
    }
    return is;
}

template<typename R> std::ostream& operator<< (std::ostream &os, Polynomial<R> p)
{
    for(int i=0;i<p.n;i++)
    {
        os << p.v[i] << (i<p.n-1 ? " " : "");
    }
    return os;
}

struct BigInt
{
    char *data;
    std::size_t size;

    template<typename I> I get()
    {
        I out = 0, p = 1;
        for(std::size_t i=0;i<size;i++)
            p+=((I)data[i]), p*=10;
        return out;
    }

    BigInt()
    {
        data = new char[2];
        data[0] = 0;
        data[1] = 0;
    }

    ~BigInt()
    {
        delete [] data;
        data = 0;
    }
};
