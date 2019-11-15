#include <iostream>
//#define LINUX //can use -D{NAME} to define a preprocessor definition from compiler's command line

//Code originally from:
//https://sourcemaking.com/design_patterns/abstract_factory/cpp/before-after

//about 90 lines w/o comments

using namespace std;

/**
 * Abstract base product.
 */
class Widget {
 public:
  virtual void draw() = 0;
};

/**
 * Concrete product family 1.
 */
class LinuxButton : public Widget {
 public:
  void draw() { cout << "LinuxButton\n"; }
};

class LinuxMenu : public Widget {
 public:
  void draw() { cout << "LinuxMenu\n"; }
};

/**
 * Concrete product family 2.
 */
class WindowsButton : public Widget {
 public:
  void draw() { cout << "WindowsButton\n"; }
};
class WindowsMenu : public Widget {
 public:
  void draw() { cout << "WindowsMenu\n"; }
};

/**
 * The client creates "product" objects directly, and must 
 * embed all possible platform permutations in nasty 
 * looking code.
 *
 * Here's a client, which uses concrete products directly.
 * It's code filled up with nasty switch statements
 * which check the product type before its use.
 */
class Client {
 public:
  void draw() {
#ifdef LINUX
    Widget *w = new LinuxButton;
#elif WINDOWS
    Widget *w = new WindowsButton;
#else
    #error ouch
#endif
    w->draw();
    display_window_one();
    display_window_two();
  }

  void display_window_one() {
#ifdef LINUX
    Widget *w[] = {
        new LinuxButton,
        new LinuxMenu
    };
#else // WINDOWS
    Widget *w[] = {
        new WindowsButton,
        new WindowsMenu
    };
#endif
    w[0]->draw();
    w[1]->draw();
  }

  void display_window_two() {
#ifdef LINUX
    Widget *w[] = {
        new LinuxMenu,
        new LinuxButton
    };
#else // WINDOWS
    Widget *w[] = {
        new WindowsMenu,
        new WindowsButton
    };
#endif
    w[0]->draw();
    w[1]->draw();
  }
};

int main() {
  Client *c = new Client();
  c->draw();
  return 0;
}