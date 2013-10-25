class Zone {
  int w;
  int h;
  int x;
  int y;
  int activeCount;
  int timesInvaded;
  boolean active;
  boolean addOne;
  
  Zone(int _x, int _y, int _w, int _h) {
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    activeCount = 0;
    timesInvaded = 0;
    active = false;
    addOne = false;
  }

  void add( int inc ) {
    activeCount += inc;
  }
  
  void update(){
   
   if (activeCount > 30) active = true;
   else active = false;
   
   if(active && !addOne){
     timesInvaded++;
     addOne = true;
   }
   
   if(!active) addOne = false;
    
  }

  void draw() {
    noStroke();
    if (active) fill((timesInvaded*10)%255, (timesInvaded*2)%255, (timesInvaded*25)%255, 60); else fill(30, 30, 30, 60);
    rect(x, y, w, h);
    activeCount = 0;
  }
  
  
  int getTimesInvaded(){
   return timesInvaded; 
  }
  
  boolean isActive(){
   return active; 
  }
  
}

