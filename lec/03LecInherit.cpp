#include "03LecInherit.h"
#include <memory>   //std::unique_ptr
#include <iostream> //std::cout
#include <vector>   //std::vector

//Difference betwee private, public, and protected inheritance
//https://stackoverflow.com/questions/860339/difference-between-private-public-and-protected-inheritance

//Using public vs protected vs private inheritance (generally use public, unless you know exactly what you're doing)
//https://stackoverflow.com/questions/374399/why-do-we-actually-need-private-or-protected-inheritance-in-c/374423#374423

//Let's not forget about virtual inheritance. This is usually used when combined with multiple inheritance.
//https://en.wikipedia.org/wiki/Virtual_inheritance


//------------------------------
Triangle::Triangle(float baseInCm, float heightInCm) : Shape()
{
   this->baseCm = baseInCm;
   this->heightCm = heightInCm;
   this->c.r = 0.75f; //triangles are only 75% red... for some reason
}

float Triangle::getAreaCmSq()
{
   float area = (this->baseCm * this->heightCm) / 2.0f;
   return area;
}

std::string Triangle::draw()
{
   this->requestDoIt();
   return std::string{"Drawing a Triangle... My Red Value is " + std::to_string(this->c.r) + "\n"};
}
//------------------------------

//------------------------------
Rectangle::Rectangle(float lengthInCm, float heightInCm)
{
   this->lengthCm = lengthInCm;
   this->heightCm = heightInCm;
}

float Rectangle::getAreaCmSq()
{
   float area = this->lengthCm * this->heightCm;
   return area;
}

std::string Rectangle::draw()
{
   return std::string{"Drawing a Rectangle... My Red Value is " + std::to_string(this->c.r) + "\n"};
}

//------------------------------

int main(int argc, char *argv[])
{
   std::cout << "Hello world... Let's make shapes...\n";

   Triangle *t = new Triangle(2.0f, 3.0f);
   Rectangle *r = new Rectangle(2.0f, 3.0f);
   auto tt = new Triangle{30.0f, 40.0f};
   auto rr = new Rectangle(11.0f, 4.0f);

   std::vector<Shape*> shapesRaw;
   shapesRaw.push_back(t);
   shapesRaw.push_back(r);
   shapesRaw.push_back(tt);
   shapesRaw.push_back(rr);

   for (auto &s : shapesRaw)
   {
      std::cout << "Area: " << s->getAreaCmSq() << ", Type is " << s->draw()
                << ", ";
      //s->requestDoIt();
      std::cout << "\n";
   }

   //Repeat above using smart pointers
   std::unique_ptr<Triangle> triA = std::make_unique<Triangle>(2.0f, 3.0f);
   std::unique_ptr<Rectangle> recA = std::unique_ptr<Rectangle>(new Rectangle(2.0f, 3.0f));
   auto triC = std::make_unique<Triangle>(30.0f, 40.0f);
   auto recB = std::make_unique<Rectangle>(11.0f, 4.0f);

   std::cout << "Let's look at a smart pointer...\n";
   std::vector<std::unique_ptr<Shape>> shapesSmart;
   shapesSmart.push_back(std::move(triA));
   shapesSmart.push_back(std::move(recA));
   shapesSmart.push_back(std::move(triC));
   shapesSmart.push_back(std::move(recB));

   for (auto &s : shapesSmart)
      std::cout << "Area: " << s->getAreaCmSq() << ", Type is " << s->draw() << "\n";

   return 0;
}