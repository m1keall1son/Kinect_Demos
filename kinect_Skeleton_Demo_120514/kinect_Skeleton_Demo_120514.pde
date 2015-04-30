import SimpleOpenNI.*;

SimpleOpenNI kinect;
boolean autoCalib=true; //use auto calibration

int kWidth, kHeight;

ArrayList<Skeleton> users;

void setup() {
  size(640, 480, P2D);

  //create the openNI instance
  kinect = new SimpleOpenNI(this);
  //mirror the image
 // kinect.setMirror(true);
  
  //get kinect width and height
  kWidth = kinect.depthWidth();
  kHeight = kinect.depthHeight();

  //depth must be enabled to track users.
  kinect.enableDepth();

  //Enable the skeleton tracking, passing the constant for all joints
  kinect.enableUser( SimpleOpenNI.SKEL_PROFILE_ALL );

  //initialize our arraylist of users
  users = new ArrayList<Skeleton>();
}

void draw() {

  background(255);

  //update the kinect
  kinect.update();
  //draw debug depth image
  image( kinect.depthImage(), 0,0, width, height );
  //loop over skeletons and update and draw them
  for (Skeleton s : users) {
    if (kinect.isTrackingSkeleton(s.userId)) {
      fill(0,255,0);
      stroke(0,255,0);
      s.update(false); //false = joint positions are in screen coordinates, true = worldSpace coordinates
    } else {
      //draw the skeleton red if we arn't tracking them
      fill(255,0,0);
      stroke(255,0,0);
    }
    s.draw();
  }
  
}


// -----------------------------------------------------------------
// SimpleOpenNI events

void onNewUser(int userId)
{
  println("onNewUser - userId: " + userId);
  println("  start pose detection");

  if (autoCalib)
    kinect.requestCalibrationSkeleton(userId, true);
  else    
    kinect.startPoseDetection("Psi", userId);
}

void onLostUser(int userId)
{
  println("onLostUser - userId: " + userId);
}

void onExitUser(int userId)
{
  println("onExitUser - userId: " + userId);

  //user has truely exited, lets remove their skeleton
  for(Skeleton s : users){
    if(s.userId == userId){
      //found them, trash the skeleton and break out of the loop
      users.remove(s);
      break;
    }
  }
  
  
}

void onReEnterUser(int userId)
{
  println("onReEnterUser - userId: " + userId);
}

void onStartCalibration(int userId)
{
  println("onStartCalibration - userId: " + userId);
}

void onEndCalibration(int userId, boolean successfull)
{
  println("onEndCalibration - userId: " + userId + ", successfull: " + successfull);

  if (successfull) 
  { 
    println("  User calibrated !!!");
    kinect.startTrackingSkeleton(userId); 

    //we sucessfully tracked a user skeleton! lets create a skeleton for them
    users.add( new Skeleton( kinect, userId ) );
  } else 
  { 
    println("  Failed to calibrate user !!!");
    println("  Start pose detection");
    kinect.startPoseDetection("Psi", userId);
  }
}

void onStartPose(String pose, int userId)
{
  println("onStartPose - userId: " + userId + ", pose: " + pose);
  println(" stop pose detection");

  kinect.stopPoseDetection(userId); 
  kinect.requestCalibrationSkeleton(userId, true);
}

void onEndPose(String pose, int userId)
{
  println("onEndPose - userId: " + userId + ", pose: " + pose);
}

