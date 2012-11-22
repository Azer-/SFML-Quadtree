// QuadtreeSimple.cpp : Demonstrates the use of Quadtree using SFML
//

#include "Quadtree.h"
#include "Object.h"
#include "Loader.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <iostream>

#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_do.h"
#include "tbb/tick_count.h"

int main( )
{
    using namespace std;

    tbb::task_scheduler_init init;

    float X = 1280.0f;
    float Y = 900.0f;

    int maxDepth[1];

    maxDepth[0] = 20;

    sf::RenderWindow app( sf::VideoMode( X, Y, 32 ), "Quadtreeeee" );
    app.SetFramerateLimit( 15 );

    Quadtree quadtree( 0.0f, 0.0f, X, Y, 0, maxDepth );

    vector<Object> objects;

    for ( int i = 10000; i > 0; --i )
    {
        objects.push_back( Object( rand( ) % ( ( int ) X ), rand( ) % ( ( int ) Y ), 1, 1 ) );
    }

    while ( app.IsOpened( ) )
    {
        sf::Event event;
        sf::Vector2f mousePosition = app.ConvertCoords( app.GetInput( ).GetMouseX( ), app.GetInput( ).GetMouseY( ) );
        while ( app.GetEvent( event ) )
        {
            if ( event.Type == sf::Event::KeyPressed )
            {
                if ( event.Key.Code == sf::Key::Escape )
                {
                    app.Close( );
                }
                else if ( event.Key.Code == sf::Key::Up )
                {
                    ++maxDepth[0];
                }
                else if ( event.Key.Code == sf::Key::Down )
                {
                    --maxDepth[0];
                }
            }
            if ( app.GetInput( ).IsMouseButtonDown( sf::Mouse::Left ) )
            {
                objects.push_back( Object( mousePosition.x, mousePosition.y, 1, 1 ) );
            }
        }
        app.Clear( );

        tbb::parallel_do( objects.begin( ), objects.end( ), Loader( quadtree, app ) );

        quadtree.Draw( app );

        vector<Object*> returnObjects = quadtree.GetObjectsAt( mousePosition.x, mousePosition.y );
        cout << returnObjects.size( ) << endl;
        quadtree.Clear( );

        app.Display( );
    }

    return 0;
}