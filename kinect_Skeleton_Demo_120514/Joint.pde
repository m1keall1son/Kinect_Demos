class Joint {
 
   int jointType;//my type, head, neck, hand, etc.
   int parentJoint; //what joint am I connected to, -1 = for root
   PVector location;
   
   Joint( int _type, int _parent ){
     
     location = new PVector();
     jointType = _type;
     parentJoint = _parent;
     
   }
   
   
      
}
