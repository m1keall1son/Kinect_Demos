//
//  forces.h
//  pushBack_vidDraw
//
//  Created by Mike Allison on 3/3/13.
//
//

#ifndef __pushBack_vidDraw__forces__
#define __pushBack_vidDraw__forces__

#include <iostream>
#include "ofMain.h"
#include "Particle.h"

class Forces {
public:
    
    Forces();
    ofVec2f airResistance(Particle m, float _c);
    ofVec2f attract(Particle m, ofVec2f loc);
    void dampen(Particle m);
    
    float c,G;
};


#endif /* defined(__pushBack_vidDraw__forces__) */
