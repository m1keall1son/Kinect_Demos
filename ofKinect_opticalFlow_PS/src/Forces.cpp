//
//  forces.cpp
//  pushBack_vidDraw
//
//  Created by Mike Allison on 3/3/13.
//
//

#include "Forces.h"

Forces::Forces(){
    c = .02;
    G = .1;
    
}

ofVec2f Forces::airResistance(Particle m, float _c) {
    float speed = m.velocity.length();
    float dragMag;
    dragMag = _c*speed*speed;
    ofVec2f drag = m.velocity;
    drag.scale(-1);
    drag.normalize();
    drag.scale(dragMag);
    return drag;
}

ofVec2f Forces::attract(Particle m, ofVec2f loc){
    ofVec2f f = loc - m.location;
    float distance = f.length();
    f.normalize();
    f.scale(distance);
    return f;
    
}

