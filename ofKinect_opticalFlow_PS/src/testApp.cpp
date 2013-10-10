#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetVerticalSync(true);
    
    ofSetFrameRate(60);
    
    //initialize the kinect
    kinect.init();
    
    //you have to initialize all bools, ints and floats or they may start with weird values that will make your computer cry
    debug = false;
    
    //our max number of particles is one for each pixel of the kinect image (640x480)
    particleCount = resizeX*resizeY;
    
    //initialize the vectors and lists to be empty
    forces.clear();
    lifeCounters.clear();
    particles.clear();
    
    //resize them to the right size at first
    forces.resize(particleCount);
    lifeCounters.resize(particleCount);
    particles.resize(particleCount);
    
    //meshes come in many modes, but for point sprites we need POINTS
    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    //set the sprite size directly to GL
    glPointSize(2.f);
    
    
    int width = resizeX;
    int height = resizeY;
    
    
    for (int y = 0; y < height; y++){
		for (int x = 0; x<width; x++){
            
            //since this double for loop gives us the pixels in a cartesian coordiantes (x,y) we need to convert that to the corrosponding position in the array, because an array is a single sequence 1,2,3,4 etc. not 2 valued grid coordinate, so the math is position (x,y) in the grid corrosponds to position x+y*width in the array
            int index = x+y*width;
            
			mesh.addVertex(ofPoint(x,y,0)); //set initial vertex positions for the mesh
			mesh.addColor(ofFloatColor(0,0,0));  // placeholder for color data, we'll get this from the camera later
            mesh.addTexCoord(ofVec2f(x,y));
            //set all counters and forces to 0 using our converted index value
            lifeCounters[index] = 0;
            forces[index] = ofVec2f(0,0);
		}
	}
	
    
    //set the indicies for each vertex so they get positioned in the right order to create a triagle strip
	for (int y = 0; y<height-1; y++){
		for (int x=0; x<width-1; x++){
			mesh.addIndex(x+y*width);				// 0
			mesh.addIndex((x+1)+y*width);			// 1
			mesh.addIndex(x+(y+1)*width);			// 10
			
			mesh.addIndex((x+1)+y*width);			// 1
			mesh.addIndex((x+1)+(y+1)*width);		// 11
			mesh.addIndex(x+(y+1)*width);
           	// 10
		}
	}
    
    //for point sprites in OF, you have to disable the ARB texture before loading the sprite image
    ofDisableArbTex();
    //load the sprite image
	pointTexture.loadImage("shaders/cir.png");
    //alpha blending
	ofEnableAlphaBlending();
    //load the shaders
    pointShader.load("shaders/particle");
    
    
    //We are making a Frame Buffer Object (fbo) to draw the particle system into so we can show it in debug mode as well, its like a separate screen that only draws when and how we tell it.  First we have to set it up and clear it
    
    //first we have to allocate the memory for the fbo, so we pass it the max size x and y it will draw
    debugFbo.allocate(drawScaleX, drawScaleY);
    //bind the fbo
    debugFbo.begin();
    //clear it out, otherwise we may see random glitch crap form the GPU at first
    ofClear(0);
    //unbind the fbo
    debugFbo.end();
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    //Grab the frame rate, convert it to a string and set it to the window title to keep track of how fast things are running and try not to be mad when it drops below 30
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    //update the kinect image
    kinect.update();
    
    //if we have a new frame and the kinect is connedted, lets do our calculations
    if(kinect.isFrameNew() && kinect.isConnected()){
        
        
        //since the optical flow algorithm we are using is so CPU heavy and slows our frame rate way down we need to scale dow the image, to do that we use an ofImage and setFromPixels() to copy the kinect pixels into a fresh ofImage.  the kinect library doesn't allow us to resize the kinect from inside the class so we have to do it manually
        
        //First the depth image, which is in grayscale
        kinectDepth.setFromPixels(kinect.getDepthPixels(), kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
        //then just resize the image
        kinectDepth.resize(resizeX, resizeY);
        
        //next we do the color image, so we have both to sample at smaller sizes
        kinectColor.setFromPixels(kinect.getPixels(), kinect.getWidth(), kinect.getHeight(), OF_IMAGE_COLOR);
        //resize the image
        kinectColor.resize(resizeX, resizeY);
        
        //set up the optical flow with some values...not sure what these are or do...
        farneback.setWindowSize(8);
        farneback.setNumLevels(5);
        farneback.setNumIterations(2);
        farneback.setPyramidScale(.5);
        
        
        //calculate the optical flow by passing in the color image because the CV works better than on the depth
        farneback.calcOpticalFlow(kinectColor);
        
        //loop through the result of the optical flow and for each position in the flow field, lets look to see if we can make a particle there.
        for(int x=0;x<farneback.getWidth();x++){
            for(int y=0;y<farneback.getHeight();y++){
                
                //same cartesian coordinate to array position conversion
                int index = x+y*farneback.getWidth();
                
                //the raw force vector we get back for each postition from CV is a vector pointing from (0,0) to current (x,y)
                ofVec2f flowForce = farneback.getFlowPosition(x, y);
                
                //We have to do some vector math to get the instantaneous velocity vector for the current position (x,y) from that raw force. Subtracting the flow vector from the current position gives us a vector with magnatude and direction representing the instantaneous velocity (forces chapter in nature of code ;)
                ofVec2f flowVelocity = ofVec2f(x,y)-flowForce;
                
                //we store the final force for each position in the forces array for later use
                //forces[index] = flowVelocity;
                
                //grab the color from the scaled down RGB camera on the kinect...just for fun
                ofColor camColor = kinectColor.getColor(x, y);
                
                //instead of using the raw depth valuse form the kinect we will use the gray scale valuse from the image instead, which is essentially the same thing, but with less resolution of depth
                ofColor depthThresholdGrayscaleSample = kinectDepth.getColor(x,y);
                
                //finally do the check to see if we've met the conditions to create a new particle.  If the optical flow force vector magnitude is greater than 1, meaning somthing is moving there, and at that current position there is not already a particle flying around and if the raw depth is between the thresholds we set then lets make a new particle.
                if(flowVelocity.length() > 5 && lifeCounters[index]<=0 && depthThresholdGrayscaleSample.r > nearThreshold && depthThresholdGrayscaleSample.r < farThreshold){
                    //push a new particle onto th back of the particles list and pass the new particle a position, color and life span
                    particles.push_back(Particle(ofVec2f(x,y),ofVec2f(ofMap(x,0,resizeX,0,drawScaleX),ofMap(y,0,resizeY,0,drawScaleY)), camColor, life));
                    //set that particles life span counter so we don't get a new particle popping out of this position until this one is dead
                    lifeCounters[index] = life;
                    forces[index] = flowVelocity;
                }else{
                    //if we arn't making a particle then don't reset the velocity
                    forces[index] = ofVec2f(0,0);
                }
                
                
            }
        }
        
    } //this closes the if kinect frame is new condition, everthing else can happen regardless of whether or not the kinect successfully grabbed a new image, which could slow down the following calculations
    
    
    //loop through all the life counters, if they are not 0 then count them down
    for (int i = 0;i<particleCount;i++){
        if(lifeCounters[i]<0){
            lifeCounters[i] = 0;
        }else{
            lifeCounters[i]--;
        }
    }
    
    //this bit of code gives us the single average optical flow vector for each frame
    float sumX, sumY;
    ofVec2f opticalFlowForceAverage;
    for (int i = 0; i<forces.size();i++){
        if(forces[i].x > 5 && forces[i].y > 5){
            sumX += forces[i].x;
            sumY += forces[i].y;
        }
    }
    opticalFlowForceAverage = ofVec2f((sumX / forces.size()), (sumY / forces.size()) );
    
    
    //time to loop through the list of particles and since a list datatype can't be accessed like an array with [index] we need to use an interator to increment through. so instead of 'int i=0' we create an iterator and set it equal to the beginning of the list and insead of the condition being 'i<array.length' we just make sure the iterator is not on the last list position.  then we increment the iterator with ++ like usual.
    for(list<Particle>::iterator iter = particles.begin();iter != particles.end();++iter){
        
        //the particles have a "home" position which was its starting position, we can use that to do the cartesian to array converstion again.  We have to do it this way because we don't know the value of iter, its a pointer not a number like 'i' would be in a ragular for loop set up
        int index = iter->home.x+iter->home.y*kinectColor.getWidth();
        
        //iter is a pointer, so we have to use an -> istead of a .
        //iter now is a pointer to a list item in the particles list, so it is acutally a particle
        //add the optical flow velocity to the particle, but scale it up and make it the same direction of the movement
        iter->addForce(forces[index]*-18);
        //iter->addForce( opticalFlowForceAverage * -600 );
        
        ///i took the air resistance out...
        //add some air resistance we can grab out of the forces class
        //since iter is a pointer or reference (just an address not the acutal data), we have to dereference it with an * to pass that actual data into our force funciton.  the .02 is the drag force cooeffient
        // iter->addForce(f.airResistance(*iter,.002));
        
        //check edges is commented out, but the particles could bounce off the edges...
        // iter->checkEdges();
        //once all the forces are applied, then calcualte the new positions with update()
        iter->update();
        
        //While we are already looping, we need to update the VBO mesh with the particles current color and position
        
        //the getLoc() function returns an ofVec2f which is our current position
        ofVec2f newLoc = iter->getLoc();
        
        //get color returns the color of our particle and setColor sets the vertex at index to that color
        ofColor tempColor = iter->getColor();
        tempColor.a = ofMap(lifeCounters[index], 0,life,0,255);
        mesh.setColor(index,tempColor);
        
        //some c++ vodo to quickly create a reference to the same index position in the vertex array and then that reference can directly change the value of the vertex's data, so we set the old location to the new one
        ofVec3f& oldLoc = mesh.getVertices()[index];
        oldLoc.x = newLoc.x;
        oldLoc.y = newLoc.y;
        
        
    }
    
    
    //Now we have to iterate seperately through the list backwards to delete particles or you run into problems with a list datatype, you just have to go backwards.  so we do the same thing, but just have it iterate top-down instead
    for(list<Particle>::iterator iter = particles.end();iter != particles.begin();--iter){
        
        //if isDead returns true
        if(iter->isDead()){
            //do the conversion again to know which array position it is
            int index = iter->home.x+iter->home.y*kinectColor.getWidth();
            //set the vertex position to be off screen since we are keeping the vertex
            mesh.setVertex(index, ofVec3f(-1,-1,0));
            
            //this is how you delete the list item using an iterator
            iter = particles.erase(iter);
            
        }
    }
    
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    //finally we can DRAW!!
    
    ofBackground(0,0,0);
    
    //bind fbo
    debugFbo.begin();
    //clear the fbo or it draws over itself
    ofClear(0);
    //we only want the particles to have an additive blend mode, otherwise the background dissapears, so pushStyle()
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    //bind the shader
    pointShader.begin();
    //then enable point sprites with OF's special call
    ofEnablePointSprites();
    //draw the VBO mesh
    mesh.draw();
    //disable, unbind and pop that stlye like its hot
    ofDisablePointSprites();
    pointShader.end();
    ofDisableBlendMode();
    ofPopStyle();
    //unbind fbo
    debugFbo.end();
    
    //set up debug
    if(debug){
        //these are the same size so draw one next to the other
        //kinect.drawDepth(0,0);
        farneback.draw(0, 240);
        kinectColor.draw(0, 0);
        kinectDepth.draw(resizeX,0);
        //draw fbo half size and in the bottom corner
        debugFbo.draw(320,240,320,240);
    }
    else {
        //draw fbo at full size from the origin
        debugFbo.draw(0,0);
    }
    
    //then put on your rollerblades and cruise
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key=='d')debug = !debug;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}