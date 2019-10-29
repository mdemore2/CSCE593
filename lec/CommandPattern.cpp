#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include <sstream>

using namespace std;

class Giant
{
  public:
    Giant() { m_id = s_next++; }
    void fee()
    {       
       m_fee += 3;
       toString( cout );
    }
    void unFee() { m_fee -= 3; }
    
    void phi()
    {
       m_phi += 4;
       toString( cout );
    }
    void unPhi() { m_phi -= 4; }
    
    void pho()
    {
       m_pho += 5;
       toString( cout );
    }
    void unPho() { m_pho -= 5; }

    void toString( std::ostream& o ) noexcept
    {
       o << "Giant[" << this->m_id << "]: fee,phi,pho = " << m_fee << "," << m_phi << "," << m_pho << "\t";
    }
    
  private:
    int m_id = 0;
    int m_fee = 0, m_phi = 0, m_pho = 0;
    static int s_next; //static class variable - no storage is provided here, must be placed in specific translation unit
};
int Giant::s_next = 0;

template< typename T >
class Command
{
   public:
   //typedef void(T::* Action)(); //Old typedef, prefer a using alias over the typedef keyword
   using Action = void(T::*)(); //ptr to a member in giant that returns void and takes () as input
    
   Command(){}
    
   Command( std::shared_ptr<T> object, Action doMethod, Action undoMethod )
   {
       m_object = object;
       m_do = doMethod;
       m_undo = undoMethod;
   }
    
   void execute()
   {
      T* g = m_object.get();
      (g->*m_do)();
   }
   
   void unexecute()
   {
      std::cout << "Undo...\t";
      T* g = m_object.get();
      (g->*m_undo)();
   }
   
   private:
   std::shared_ptr<T> m_object = nullptr;
   Action m_do = nullptr;
   Action m_undo = nullptr;
};

using GntCmd = Command<Giant>;

int main()
{
   auto x = std::make_shared< Giant >();
   
   std::vector<GntCmd> v =
   {
      GntCmd{ x, &Giant::fee, &Giant::unFee },
      GntCmd{ std::make_shared<Giant>(), &Giant::phi, &Giant::unPhi },
      GntCmd{ std::make_shared<Giant>(), &Giant::pho, &Giant::unPho },
      GntCmd{ std::make_shared<Giant>(), &Giant::fee, &Giant::unFee },
      GntCmd{ std::make_shared<Giant>(), &Giant::phi, &Giant::unPhi },
      GntCmd{ std::make_shared<Giant>(), &Giant::pho, &Giant::unPho }
   };

   for( int j = 0; j < 5; ++j )
   {
      for(int i = 0; i < v.size(); i++) v[i].execute();
      std::cout << "\n";
   }


   
   for(int i = 0; i < v.size(); i++) v[i].unexecute();
   std::cout << "\n";
   for(int i = 0; i < v.size(); i++) v[i].unexecute();
   std::cout << "\n";
   for(int i = 0; i < v.size(); i++) v[i].execute();

 
   std::cout << '\n';
}
