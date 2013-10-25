import SimpleOpenNI.*;
import controlP5.*;

SimpleOpenNI kinect;
ControlP5 GUI;

int nearThreshold;
int farThreshold;
int kWidth, kHeight;
Zone[] zones;

void setup() {
  size(640, 480, P2D);
  kinect = new SimpleOpenNI(this);
  kinect.setMirror(true);
  kinect.enableDepth();
  nearThreshold = 1445;
  farThreshold = 2500;
  initGUI();
  kWidth = kinect.depthWidth();
  kHeight = kinect.depthHeight();
  zones = new Zone[4];
  int w = 200;
  int h = 100;
  zones[0] = new Zone(50, 50, w, h);
  zones[1] = new Zone(70+w, 50, w, h);
  zones[2] = new Zone(50, 70+h, w, h);
  zones[3] = new Zone(70+w, 70+h, w, h);
}

void draw() {

  background(255);

  kinect.update();

  int[] depth = kinect.depthMap();

 
  
  loadPixels();
  for (int x = 0; x < kWidth; x++) {
    for (int y = 0; y < kHeight; y++) {

      int rawDepth = depth[x+y*kWidth];
      
      if (rawDepth > nearThreshold &&
        rawDepth < farThreshold) {
        pixels[x+y*kWidth] = color(0, 200, 40);
        for(Zone z : zones){
        if ((x > z.x && x < z.x + z.w) && (y > z.y && y < z.y + z.h)) {
           z.add(1);
        } 
        }
  
      } else {
        pixels[x+y*kWidth] = kinect.depthImage().pixels[x+y*kWidth];
      }
    }
  }
  updatePixels();
  
  for(Zone z : zones) {
    z.update();
    z.draw();
    if(z.isActive()) println(z.getTimesInvaded());
  }
  
}

void keyPressed() {
}

void mouseDragged() {
}

