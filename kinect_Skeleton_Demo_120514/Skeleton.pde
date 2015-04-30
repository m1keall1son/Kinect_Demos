class Skeleton {

  int userId; //what user is this? 
  Joint[] joints; //array of joints
  SimpleOpenNI context;

  Skeleton( SimpleOpenNI kinect, int _userId ) {
    userId = _userId;
    context = kinect;
    populateJoints();
  }

  void populateJoints() {
    //allocate the array
    joints = new Joint[15];
    //make new joints, the head has no parent, its the root joint
    joints[0] = new Joint( SimpleOpenNI.SKEL_HEAD, -1 );
    //fill the joint location using the simpleOpenNI context
    context.getJointPositionSkeleton( userId, SimpleOpenNI.SKEL_HEAD, joints[0].location );

    joints[1] = new Joint( SimpleOpenNI.SKEL_NECK, SimpleOpenNI.SKEL_HEAD );
    context.getJointPositionSkeleton( userId, SimpleOpenNI.SKEL_NECK, joints[1].location );

    joints[2] = new Joint( SimpleOpenNI.SKEL_LEFT_SHOULDER, SimpleOpenNI.SKEL_NECK );
    context.getJointPositionSkeleton( userId, SimpleOpenNI.SKEL_LEFT_SHOULDER, joints[2].location );

    joints[3] = new Joint( SimpleOpenNI.SKEL_LEFT_ELBOW, SimpleOpenNI.SKEL_LEFT_SHOULDER );
    context.getJointPositionSkeleton( userId, SimpleOpenNI.SKEL_LEFT_ELBOW, joints[3].location );

    joints[4] = new Joint( SimpleOpenNI.SKEL_LEFT_HAND, SimpleOpenNI.SKEL_LEFT_ELBOW );
    context.getJointPositionSkeleton( userId, SimpleOpenNI.SKEL_LEFT_HAND, joints[4].location );

    joints[5] = new Joint( SimpleOpenNI.SKEL_RIGHT_SHOULDER, SimpleOpenNI.SKEL_NECK );
    context.getJointPositionSkeleton( userId, SimpleOpenNI.SKEL_RIGHT_SHOULDER, joints[5].location );

    joints[6] = new Joint( SimpleOpenNI.SKEL_RIGHT_ELBOW, SimpleOpenNI.SKEL_RIGHT_SHOULDER );
    context.getJointPositionSkeleton( userId, SimpleOpenNI.SKEL_RIGHT_ELBOW, joints[6].location );

    joints[7] = new Joint( SimpleOpenNI.SKEL_RIGHT_HAND, SimpleOpenNI.SKEL_RIGHT_ELBOW );
    context.getJointPositionSkeleton( userId, SimpleOpenNI.SKEL_RIGHT_HAND, joints[7].location );

    joints[8] = new Joint( SimpleOpenNI.SKEL_TORSO, SimpleOpenNI.SKEL_NECK );
    context.getJointPositionSkeleton( userId, SimpleOpenNI.SKEL_TORSO, joints[8].location );

    joints[9] = new Joint( SimpleOpenNI.SKEL_LEFT_HIP, SimpleOpenNI.SKEL_TORSO );
    context.getJointPositionSkeleton( userId, SimpleOpenNI.SKEL_LEFT_HIP, joints[9].location );

    joints[10] = new Joint( SimpleOpenNI.SKEL_LEFT_KNEE, SimpleOpenNI.SKEL_LEFT_HIP );
    context.getJointPositionSkeleton( userId, SimpleOpenNI.SKEL_LEFT_KNEE, joints[10].location );

    joints[11] = new Joint( SimpleOpenNI.SKEL_LEFT_FOOT, SimpleOpenNI.SKEL_LEFT_KNEE );
    context.getJointPositionSkeleton( userId, SimpleOpenNI.SKEL_LEFT_FOOT, joints[11].location );

    joints[12] = new Joint( SimpleOpenNI.SKEL_RIGHT_HIP, SimpleOpenNI.SKEL_TORSO );
    context.getJointPositionSkeleton( userId, SimpleOpenNI.SKEL_RIGHT_HIP, joints[12].location );

    joints[13] = new Joint( SimpleOpenNI.SKEL_RIGHT_KNEE, SimpleOpenNI.SKEL_RIGHT_HIP );
    context.getJointPositionSkeleton( userId, SimpleOpenNI.SKEL_RIGHT_KNEE, joints[13].location );

    joints[14] = new Joint( SimpleOpenNI.SKEL_RIGHT_FOOT, SimpleOpenNI.SKEL_RIGHT_KNEE );
    context.getJointPositionSkeleton( userId, SimpleOpenNI.SKEL_RIGHT_FOOT, joints[14].location );
  }

  void update(boolean useWorldSpace) {

    for (Joint joint : joints) {
      PVector tmp = new PVector();
      context.getJointPositionSkeleton( userId, joint.jointType, tmp);
      if (!useWorldSpace) {
        context.convertRealWorldToProjective( tmp, joint.location );
      } else {
        joint.location = tmp;
      }
    }
  }


  PVector getLocationForJoint( int jointType ) {

    //PVector to fill
    PVector location = new PVector();

    for (Joint joint : joints) {
      if (joint.jointType == jointType) {
        //copy the joint location into the result pvector (this doesn't have to be a copy)
        location.x = joint.location.x;
        location.y = joint.location.y;
        break; //because we can stop searching
      }
    }

    return location; //return the result
  }

  void draw() {

    for (Joint joint : joints) {

      if (joint.jointType == -1) {
        //the head is the root so don't draw a connection to a parent joint
        ellipse( joint.location.x, joint.location.y, 5, 5 );
      } else {
        ellipse( joint.location.x, joint.location.y, 5, 5 );

        PVector parent = getLocationForJoint( joint.parentJoint );
        
        //make sure the parent actually exists
        if(parent.mag() != 0)
          line( joint.location.x, joint.location.y, parent.x, parent.y );
      }
    }
  }
}

