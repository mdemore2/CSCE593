#pragma once
#include <string>
#include <iostream>

struct Color //this is identical to using keyword "class" when all members and attributes "public:"
{
   float r = 0;
   float g = 0;
   float b = 0;
   float a = 0;
};

class Shape
{
 public:
   Shape() = default;
   virtual ~Shape() = default;
   virtual void requestDoIt() const { this->doIt(); }

   virtual float getAreaCmSq() const = 0;
   virtual std::string draw() const = 0;

 protected:
   //float r = 1.0f, g = 1.0f, b = 1.0f;
   //Instead of raw floats, let's encapsulate them into a color class to minimize coupling
   //and maximize cohesion
   Color c = {0,0,0,1}; //assigns values in order they are defined in struct/class

 private:
   int myVal = 3;
   void doIt() const { std::cout << "Shape did it\n"; }
};

class Triangle : public Shape //this is inheritance, Triangle inherits from Shape
{
 public:
   Triangle(float baseInCm, float heightInCm);
   virtual ~Triangle() = default;

   virtual void requestDoIt() const { this->doIt(); } //Use virtual dispatch to call  Shape::doIt().

   virtual float getAreaCmSq() const override;
   virtual std::string draw() const override;

 private:
   float baseCm = 0;
   float heightCm = 0;
   void doIt() const { std::cout << "Triangle did it\n"; }
};

class Rectangle : public Shape //Rectangle also inherits from Shape
{
 public:
   Rectangle(float lengthInCm, float heighInCm);
   virtual ~Rectangle() = default;

   virtual float getAreaCmSq() const override;
   virtual std::string draw() const override;

 private:
   float lengthCm = 0;
   float heightCm = 0;
   void doIt() const { std::cout << "Rectangle did it\n"; }
};