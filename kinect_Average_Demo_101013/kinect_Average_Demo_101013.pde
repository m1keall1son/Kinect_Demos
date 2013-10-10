import SimpleOpenNI.*;
import controlP5.*;

SimpleOpenNI kinect;
ControlP5 GUI;

int nearThreshold;
int farThreshold;
PVector averageLocation;
float averageTotalDepth;
int kWidth, kHeight;

void setup() {
  size(640, 480, P2D);
  kinect = new SimpleOpenNI(this);
  kinect.setMirror(true);
  kinect.enableDepth();
  nearThreshold = 1445;
  farThreshold = 2500;
  initGUI();
  averageLocation = new PVector(0,0);
  kWidth = kinect.depthWidth();
  kHeight = kinect.depthHeight();
}

void draw() {

  background(255);

  kinect.update();

  int[] depth = kinect.depthMap();

  float sumX = 0;
  float sumY = 0;
  float totalDepth = 0;
  int count = 0;


  loadPixels();
  for (int x = 0; x < kWidth; x++) {
    for (int y = 0; y < kHeight; y++) {

      int rawDepth = depth[x+y*kWidth];
      
      if (rawDepth > nearThreshold && rawDepth < farThreshold) {
        sumX += x;
        sumY += y;
        totalDepth += rawDepth;
        count++;
        pixels[x+y*kWidth] = color(0, 200, 40);
      }
      else {
        pixels[x+y*kWidth] = kinect.depthImage().pixels[x+y*kWidth];
      }
    }
  }
  
  averageLocation.x = sumX/count;
  averageLocation.y = sumY/count;
  averageTotalDepth = totalDepth/count;
  
  for (int x = 0; x < kWidth; x++) {
    for (int y = 0; y < kHeight; y++) {

      int rawDepth = depth[x+y*kWidth];
      
      if (rawDepth > averageTotalDepth && rawDepth < averageTotalDepth+10) {
        pixels[x+y*kWidth] = color(255, 10, 10);
      }
    }
  }
  updatePixels();
  
  ellipseMode(CENTER);
  fill(0,10,240,150);
  ellipse(averageLocation.x, averageLocation.y, 50,50);
  ellipse(averageLocation.x, averageLocation.y, 10,10);
  
}

void keyPressed() {
}

void mouseDragged() {
}

