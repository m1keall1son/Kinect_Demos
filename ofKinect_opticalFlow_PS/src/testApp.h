#pragma once

#include "ofMain.h"

//include the addons, particle, and forces header files

#include "ofxKinect.h"
#include "ofxCv.h"
#include "Forces.h"
#include "Particle.h"

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    
    /* This type of CPU based particle system is really slow in this casel, but here's how it works:
     Each frame we get a new image from the kinect and we pass it to the CV optical flow algorithm to get the movement forces and we look at the raw depth array to threshold the kinect image.  For every pixel that is over the threshold and has an instantaneous velocity (taken from the optical flow result) over 1 and whos position has a 0 life count (meaning there is not currently a particle flying around that blongs to that position) then we create a new Partilcle and add it to the particles list and set its life counter to full.  Then we iterate through the particles list with an iterator (beacuse a linked list datatype does not allow normal array access such as arrayName[index]) and apply forces to the particle.  meanwhile, we count down its life every frame as well, if it reaches 0, then we have to delete it from the list.
     
     To Draw this system we use a shader to put an image texture at the position of every particle.  So the particle class doesn't actually handle drawing, it just allows us to compute and hold the data for the positions and colors as calculated from the optical flow and applied physics forces. To draw this system we create a Vertex Buffer Object Mesh (VBO Mesh) that holds a vertex for every pixel of the kinect image, the finite number of particles we have to work with.  After computing the forces, checking the life count and deleting dead particles we then update the corrosponding vertex from the VBO mesh with the correct position and color which we get from the particles list, which holds the data for all the living particles. Once the position and color of each vertex is set, in draw we just have to bind the shader program, enable point sprites and draw the mesh. The shader looks at each vertex and creates a little tiny 6 point mesh using the vertex position as the center onto which the frag shader can draw the texture of the point sprite image and change its color to the current vertex color. There is a geometry shader that helps in the background of OF to make the tiny meshes, so it not in the code here, only the vertex and fragment shaders.
     
     This one runs too slow becase the Farneback optical flow algorithm is really slow.  :( i don't like the slow frame rate but its too late now to change the way it works...there's nothing worse than a choking particle system...
     
     */
    
    
    //create global variables
    
    ofxKinect kinect;
    
    //Optical flow: Farneback algorithm gives you a flowfield stlye analysis, but is very slow....
    ofxCv::FlowFarneback farneback;
    
    //to turn on and off debug mode
    bool debug;
    
    int particleCount;
    
    //const means that in the program, this value will never be changed.  so we set these hard values here:
    //thresholds represent the depth range in grayscale value
    const int nearThreshold = 230; //threshold for raw distance from kinect, between 0-255
    const int farThreshold = 255;
    const int life = 50; //life span in frames of each new particle
    const int resizeX = 320; //size to resize the kinect image to to make for a faster analysis
    const int resizeY = 240;
    const int drawScaleX = ofGetWindowWidth(); //size to draw to, we've scaled up
    const int drawScaleY = ofGetWindowHeight();
    //list datatype is like an array but can deal with deleting random list items faster than a ragular array
    list<Particle> particles; //holds our particles
    
    //Vectors are bascially arrays
    vector<ofVec2f> forces; //holds the current optical flow force for each position calculated by the optical flow
    vector<int> lifeCounters; //holds the current life count for each particle
    
    Forces f; //create an instance of the force class that gives us environment physics like drag force, i just wrapped up the forces into a class for convenience
    
    
    //the shader program, texture and VBO mesh for the point strites and drawing the particles
    ofShader pointShader;
    ofImage pointTexture;
    ofVboMesh mesh;
    
    //images for resizing
    ofImage kinectDepth, kinectColor;
    //fbo to give us drawing flexibility
    ofFbo debugFbo;
    
};
