// QuadtreeSimple.cpp : Demonstrates the use of Quadtree using SFML
//

#include "Quadtree.h"
#include "Object.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <iostream>

int main()
{
  using namespace std;

  float X = 800.0f;
  float Y = 600.0f;
  
  int maxDepth = 0;

  sf::RenderWindow app( sf::VideoMode( X, Y, 32 ), "Quadtree" );
  app.SetFramerateLimit( 15 );


  Quadtree quadtree( 0.0f, 0.0f, X, Y, 0, 15 );

  vector<Object> objects;

//  for (int i = 50; i > 0 ; --i)
//  {
//    objects.push_back( Object( rand() % ( (int) X ), rand() % ( (int) Y ), 1, 1 ) );
//  }


  while ( app.IsOpened() )
  {
    sf::Event event;
    sf::Vector2f mousePosition = app.ConvertCoords(app.GetInput().GetMouseX(), app.GetInput().GetMouseY());
    while ( app.GetEvent( event ) )
    {
      if ( event.Type == sf::Event::KeyPressed )
      {
        if ( event.Key.Code == sf::Key::Escape )
        {
          app.Close();
        }
      }
      if ( app.GetInput().IsMouseButtonDown( sf::Mouse::Left ) )
      {
        objects.push_back( Object( mousePosition.x, mousePosition.y, 1, 1 ) );
      }
    }
    app.Clear();

    for ( int n = 0; n < objects.size(); ++n )
    {
      quadtree.AddObject( &objects[n] );
      objects[n].Draw( app );
    }
    quadtree.Draw( app );

    vector<Object*> returnObjects = quadtree.GetObjectsAt( mousePosition.x, mousePosition.y );
    cout << returnObjects.size() << endl;
    quadtree.Clear();

    app.Display();
  }

  return 0;
}