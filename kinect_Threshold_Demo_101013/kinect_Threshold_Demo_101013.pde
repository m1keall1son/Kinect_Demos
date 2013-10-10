import SimpleOpenNI.*;
import controlP5.*;

SimpleOpenNI kinect;
ControlP5 GUI;

float nearThreshold;
float farThreshold;
int kWidth, kHeight;

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
}

void draw() {

  background(255);

  kinect.update();
 
  int[] depth = kinect.depthMap();
  
  loadPixels();
  for (int x = 0; x < kWidth; x++) {
    for (int y = 0; y < kHeight; y++) {
      
      int rawDepth = depth[x+y*kWidth];
      
      if (rawDepth > nearThreshold && rawDepth < farThreshold) {
        pixels[x+y*kWidth] = color(0,200,150);
      }else{
        pixels[x+y*kWidth] = kinect.depthImage().pixels[x+y*kWidth];
      }
      
    }
  }
  updatePixels();

}

void keyPressed() {
}

void mouseDragged() {

}

