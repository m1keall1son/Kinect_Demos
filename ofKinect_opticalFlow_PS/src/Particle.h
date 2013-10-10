//
//  Particle.h
//  pushBack
//
//  Created by Mike Allison on 3/2/13.
//
//

#ifndef __pushBack__Particle__
#define __pushBack__Particle__
#include "ofMain.h"
#include <iostream>

class Particle{
public:
    
    Particle();
    Particle(ofVec2f position, ofVec2f loc, ofColor _color, int _life);
    void addForce(ofVec2f force);
    void update();
    void checkEdges();
    float getMass();
    void display();
    void displayRadiant();
    ofColor getColor();
    bool isHome();
    bool isDead();
    int getID();
    ofVec2f getLoc();
    
    ofVec2f location,velocity, acceleration,home;
    float w,h,d,mass,G;
    float life;
    bool colorChange;
    bool ash;
    ofColor color;
    int ParticleID;
    
    
};



#endif /* defined(__pushBack__Particle__) */
