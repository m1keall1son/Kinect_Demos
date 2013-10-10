//
//  Particles.cpp
//  pushBack
//
//  Created by Mike Allison on 3/2/13.
//
//

#include "Particle.h"

Particle::Particle(){
    home = ofVec2f(0,0);
    location = ofVec2f(0,0);
    velocity = ofVec2f(0,0);
    acceleration = ofVec2f(0,0);
    w = 0;
    mass = 0;
    G = 0;
    life = 0;
    color = ofColor(255,0,0);
    
}


Particle::Particle(ofVec2f position, ofVec2f loc, ofColor _color, int _life){
    home = position;
    location = loc;
    velocity = ofVec2f(0,0);
    acceleration = ofVec2f(0,0);
    w = .5;
    mass = 10;
    ash = false;
    life = _life;
    color = _color;
    
}

void Particle::addForce(ofVec2f force){
    ofVec2f f = force / mass;
    acceleration+=f;
    
}
ofColor Particle::getColor(){
    return color;
    
}
ofVec2f Particle::getLoc(){
    
    return location;
    
}
bool Particle::isHome(){
    ofVec2f f =location - home;
    float distance = f.length();
    if(distance<10){
        return true;
    }else{
        return false;
    }
    
}


void Particle::update(){
    
    velocity+=acceleration;
    location+=velocity;
    
    acceleration*=0;
    life--;
    float rr = 0;
    float gg = 0;
    float bb = 0;
    float aa = ofMap(life,0,60,0,255);
    
    /* if(life<5){
     rr = ofMap(life,5,0,0,color.r);
     gg = ofMap(life,5,0,0,color.g);
     bb = ofMap(life,5,0,0,color.b);
     }
     */
    
    color = ofColor(color.r-rr,color.g-gg,color.b-bb,255);
}

void Particle::checkEdges(){
    
    if (location.x  > 640) {
        location.x = 640;
        ash = false;
    }
    
    if (location.x < 0 ) {
        location.x = 0;
    }
    
    if (location.y > 480) {
        location.y = 480;
    }
    
    if (location.y < 0 ) {
        location.y = 0;
    }
    
}

bool Particle::isDead(){
    if(life<0) {
        return true;
    }else{
        return false;
    }
}

int Particle::getID(){
    return ParticleID;
}

float Particle::getMass(){
    while (true) {
        float prabability = w;
        mass = ofRandom(10, 50);
        if (mass>w/2) {
            return mass;
        }
    }
}



void Particle::display(){
    
    //nothing here
    
}



