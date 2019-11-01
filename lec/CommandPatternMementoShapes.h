#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <memory>

//We will use the memento pattern as well as the command pattern
//to make a mock "paint" program that supports undo and redo

template< typename STATE >
class Memento
{public:
    STATE t;
    template< typename OBJ >
    void restore( OBJ& obj)
    { obj.fromStream( t ); }
    
    template< typename OBJ >
    void save( OBJ& obj )
    { obj.toStream( t ); }
};

class Shape
{
public:
    virtual ~Shape() = default;
    virtual std::ostream& toStream( std::ostream& os ) const
    {   os << color << " " << pos << " " << orient << " "; }
    virtual bool fromStream( std::istream& is )
    {
        is >> color >> pos >> orient;
        if( is ) return true;
        return false;
    }
    
    int color = 0;
    float pos = 0;
    float orient = 0;
};

class Circle : public Shape
{public:
    virtual ~Circle() = default;
    virtual std::ostream& toStream( std::ostream& os ) const override
    {
        os << "CIRCLE " << radius << " ";
        Shape::toStream( os );
        os << "\n";
        return os;
    }

    virtual bool fromStream( std::istream& is ) override
    {
        std::string type;
        is >> type;
        is >> radius;
        Shape::fromStream( is );
        if( is ) return true;
        return false;
    }
    float radius = 0;
};

class Triangle : public Shape
{public:
    virtual ~Triangle() = default;
    virtual std::ostream& toStream( std::ostream& os ) const override
    {
        os << "TRIANGLE " << base << " " << height << " ";
        Shape::toStream( os );
    }

    virtual bool fromStream( std::istream& is ) override
    {
        std::string type;
        is >> type;
        is >> base >> height;
        Shape::fromStream( is );
    }
    float base = 0;
    float height = 0;
};

class Cmd
{
public:
    virtual ~Cmd() = default;
    virtual void execute() = 0;
    virtual void unexecute() = 0;
};

class CmdSetShapePosition : public Cmd
{
public:
    CmdSetShapePosition( std::shared_ptr< Shape > shp, float newPos ) : shp(shp), newPos(newPos) { oldPos = shp->pos; }
    virtual void execute() override
    { shp->pos = newPos; }
    virtual void unexecute() override
    { shp->pos = oldPos; }
    std::shared_ptr< Shape > shp = nullptr;
    float newPos = 0;
    float oldPos = 0;
};

class CmdSetTriangleHeight : public Cmd
{
public:
    CmdSetTriangleHeight( std::shared_ptr< Triangle > tri, float newHeight ) : tri(tri), newHeight(newHeight) {  }
    virtual void execute() override
    { m.save( *tri );
      tri->height = newHeight; }
    virtual void unexecute() override
    { m.restore( *tri ); }
    std::shared_ptr< Triangle > tri = nullptr;
    float newHeight = 0;
    Memento<std::stringstream> m;
};