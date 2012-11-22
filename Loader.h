/* 
 * File:   Loader.h
 * Author: azer
 *
 * Created on November 22, 2012, 10:03 PM
 */

#ifndef LOADER_H
#define	LOADER_H

#include "Quadtree.h"
#include "Object.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

class Loader {
    Quadtree& quadtree;
    sf::RenderWindow& app;
public:

    Loader(Quadtree& _quadtree, sf::RenderWindow &_app) :
    quadtree(_quadtree), app( _app ) {

    }

    void operator()(Object& object) const {
        quadtree.AddObject( &object );
    }
private:

};

#endif	/* LOADER_H */

