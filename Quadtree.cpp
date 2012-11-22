#include "Quadtree.h"
#include "Object.h"
#include <iostream>
#include <sstream>

using namespace std;

Quadtree::Quadtree( float _x, float _y, float _width, float _height, int _level, int* _maxLevel ) :
x( _x ),
y( _y ),
width( _width ),
height( _height ),
level( _level ),
maxLevel( _maxLevel )
{
    std::atomic<int> caca;

    shape = sf::Shape::Rectangle( x, y, x + width, y + height, sf::Color( 0, 0, 0, 0 ), 2.0f, sf::Color( 32, 128, 255 ) );

    if ( level == *maxLevel )
    {
        return;
    }
    else if ( level == 0 )
    {
        NW = new Quadtree( x, y, width / 2.0f, height / 2.0f, level + 1, maxLevel );
        NE = new Quadtree( x + width / 2.0f, y, width / 2.0f, height / 2.0f, level + 1, maxLevel );
        SW = new Quadtree( x, y + height / 2.0f, width / 2.0f, height / 2.0f, level + 1, maxLevel );
        SE = new Quadtree( x + width / 2.0f, y + height / 2.0f, width / 2.0f, height / 2.0f, level + 1, maxLevel );
    }
    else
    {
        NW = NULL;
        NE = NULL;
        SW = NULL;
        SE = NULL;
    }
}

bool
Quadtree::Contains( Quadtree *child, Object *object )
{
    return !( object->x < child->x ||
            object->y < child->y ||
            object->x > child->x + child->width ||
            object->y > child->y + child->height ||
            object->x + object->width < child->x ||
            object->y + object->height < child->y ||
            object->x + object->width > child->x + child->width ||
            object->y + object->height > child->y + child->height );
}

void Quadtree::AddObject( Object *object )
{
    if ( level == *maxLevel )
    {
        objects.push_back( object );
        return;
    }

    if ( NW == NULL && NE == NULL && SE == NULL && SW == NULL && objects.size( ) == 0 )
    { // no object in the zone
        objects.push_back( object );
        return;
    }



    if ( NW == NULL )
    {
        Quadtree* NW_t = new Quadtree( x, y, width / 2.0f, height / 2.0f, level + 1, maxLevel );
        if ( Contains( NW_t, object ) )
        {
            NW_t->AddObject( object );
            NW = NW_t;
            return;
        }
    }
    else if ( Contains( NW, object ) )
    {
        NW->AddObject( object );
        return;
    }
    if ( NE == NULL )
    {
        Quadtree* NE_t = new Quadtree( x + width / 2.0f, y, width / 2.0f, height / 2.0f, level + 1, maxLevel );
        if ( Contains( NE_t, object ) )
        {
            NE_t->AddObject( object );
            NE = NE_t;
            return;
        }
    }
    else if ( Contains( NE, object ) )
    {
        NE->AddObject( object );
        return;
    }
    if ( SW == NULL )
    {
        Quadtree* SW_t = new Quadtree( x, y + height / 2.0f, width / 2.0f, height / 2.0f, level + 1, maxLevel );
        if ( Contains( SW_t, object ) )
        {
            SW_t->AddObject( object );
            SW = SW_t;
            return;
        }
    }
    else if ( Contains( SW, object ) )
    {
        SW->AddObject( object );
        return;
    }

    if ( SE == NULL )
    {
        Quadtree* SE_t = new Quadtree( x + width / 2.0f, y + height / 2.0f, width / 2.0f, height / 2.0f, level + 1, maxLevel );
        if ( Contains( SE_t, object ) )
        {
            SE_t->AddObject( object );
            SE = SE_t;
            return;
        }
    }
    else if ( Contains( SE, object ) )
    {
        SE->AddObject( object );
        return;
    }

    if ( Contains( this, object ) )
    {
        objects.push_back( object );
    }
}

vector<Object*> Quadtree::GetObjectsAt( float _x, float _y )
{
    if ( level == *maxLevel )
    {
        return objects;
    }

    vector<Object*> returnObjects, childReturnObjects;
    if ( !objects.empty( ) )
    {
        returnObjects = objects;
    }
    if ( _x > x + width / 2.0f && _x < x + width )
    {
        if ( _y > y + height / 2.0f && _y < y + height && SE )
        {
            childReturnObjects = SE->GetObjectsAt( _x, _y );
            returnObjects.insert( returnObjects.end( ), childReturnObjects.begin( ), childReturnObjects.end( ) );
            return returnObjects;
        }
        else if ( _y > y && _y <= y + height / 2.0f && NE )
        {
            childReturnObjects = NE->GetObjectsAt( _x, _y );
            returnObjects.insert( returnObjects.end( ), childReturnObjects.begin( ), childReturnObjects.end( ) );
            return returnObjects;
        }
    }
    else if ( _x > x && _x <= x + width / 2.0f )
    {
        if ( _y > y + height / 2.0f && _y < y + height && SW )
        {
            childReturnObjects = SW->GetObjectsAt( _x, _y );
            returnObjects.insert( returnObjects.end( ), childReturnObjects.begin( ), childReturnObjects.end( ) );
            return returnObjects;
        }
        else if ( _y > y && _y <= y + height / 2.0f && NW )
        {
            childReturnObjects = NW->GetObjectsAt( _x, _y );
            returnObjects.insert( returnObjects.end( ), childReturnObjects.begin( ), childReturnObjects.end( ) );
            return returnObjects;
        }
    }
    return returnObjects;
}

void Quadtree::Clear( )
{
    if ( level == *maxLevel )
    {
        objects.clear( );
        return;
    }
    else
    {
        if ( NW )
            NW->Clear( );
        if ( NE )
            NE->Clear( );
        if ( SW )
            SW->Clear( );
        if ( SE )
            SE->Clear( );
    }
    if ( !objects.empty( ) )
    {
        objects.clear( );
    }
}

void
Quadtree::Draw( sf::RenderTarget &canvas )
{
    stringstream ss;
    ss << objects.size( );
    string numObjectsStr = ss.str( );

    for ( vector<Object*>::iterator object = objects.begin( ); object < objects.end( ); ++object )
    {
        ( *object )->Draw( canvas );
    }


    canvas.Draw( shape );

    if ( level != *maxLevel )
    {
        if ( NW )
            NW->Draw( canvas );
        if ( NE )
            NE->Draw( canvas );
        if ( SW )
            SW->Draw( canvas );
        if ( SE )
            SE->Draw( canvas );
    }
}
