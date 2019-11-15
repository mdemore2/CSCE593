#pragma once
#include <string>
#include <iostream>
#include <memory>

class Model
{
public:
   Model() = default;
   virtual ~Model() = default;
   Model( Model const& toCopy ) = default;
   Model( Model&& toMove ) = default;
   Model& operator=( Model const& toAssign ) = default;
   Model& operator=( Model&& toMove ) = default;

   virtual void draw( std::ostream& out ) const = 0;
   virtual std::unique_ptr<Model> clone() const = 0;
};

class FancyModel : public Model
{
public:
   FancyModel() : Model() {}
   virtual ~FancyModel() {}
   virtual void draw( std::ostream& out) const override
   {
      out << "Drawing a FancyModel...\n";      
   }

   virtual std::unique_ptr<Model> clone() const override 
   {
      auto f = std::make_unique<FancyModel>();//copy constructor
      //assign local members here or call copy ctor
      return std::move(f);
   }

};

class QuickModel : public Model
{
public:
   QuickModel() : Model() {}
   virtual ~QuickModel() {}
   virtual void draw( std::ostream& out ) const override
   {
      out << "Drawing a QuickModel...\n";      
   }

   virtual std::unique_ptr<Model> clone() const override 
   {
      auto f = std::make_unique<QuickModel>(); //copy constructor
      //assign local members here or call copy ctor
      return std::move(f);
   }
};

class WO
{
public:
   //If implementing any of the follow 5 methods, in general, you should implement all
   //5. Otherwise the default implementations for all 5 can be used.
   //1) Destructor
   //2) Copy constructor T::T( T const& toCopy )
   //3) Copy assignment operator T T::operator =( T const& toCopy )
   //4) Move constructor T::T( T&& toMove )
   //5) Move assignment operator T& T::operator=( T&& toMove );
   WO() = default;
   WO( WO const& toCopy )
   {
      std::cout << "Copy C'tor\n";
      if( this != &toCopy )
         *this = toCopy;
   }
   WO( WO&& toMove )
   {  //in move, we can "steal" the internal values from another object.
      std::cout << "Move C'tor\n";
      this->id = toMove.id;
      this->name = toMove.name;
      this->m = std::move( toMove.m );
      if( toMove.m == nullptr )
         std::cout << "NULL\n";
      
      toMove.id = -1;
      toMove.name = "";
      toMove.m = nullptr;
   }

   virtual ~WO() = default;

   WO& operator= ( WO const& toCopy )
   {
      if( this != &toCopy )
      {
         std::cout << "Copy Assign\n";
         this->name = toCopy.name;
         this->id = toCopy.id;
         if( toCopy.m )
            this->m = toCopy.m->clone();
      }
      return *this;      
   }

   WO& operator=( WO&& toMove )
   {
      //in move, we can "steal" the internal values from another object.
      if( this != &toMove )
      {
         std::cout << "Move Assign\n";
         this->id = toMove.id;
         this->name = toMove.name;
         this->m = std::move( toMove.m );
         if( toMove.m == nullptr )
         std::cout << "NULL\n";

         toMove.id = -1;
         toMove.name = "";
         toMove.m = nullptr;
      }
      return *this;
   }

   void setModel( std::unique_ptr<Model> m ){ this->m = std::move(m); }
   virtual void draw( std::ostream& out ) const
   { 
      out << "id: " << this->id << ", name: " << this->name << ", Model: ";
      if( this->m ) 
         this->m->draw( std::cout );
      else
         out << "No Model to draw...\n";
   }

   virtual std::unique_ptr<WO> clone() const
   {
      std::unique_ptr< WO > w = std::make_unique< WO >( *this );
      return w;
   }

   std::string name = "Base WO...";
   int id = 0;
   std::unique_ptr< Model > m = nullptr;

};