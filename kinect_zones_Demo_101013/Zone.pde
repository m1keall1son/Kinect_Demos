class Zone {
  int w;
  int h;
  int x;
  int y;
  int activeCount;
  Zone(int _x, int _y, int _w, int _h) {
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    activeCount = 0;
  }

  void add( int inc ) {
    activeCount += inc;
  }

  void draw() {
    noStroke();
    if (activeCount > 0) fill(200, 70, 70, 60); else fill(30, 30, 30, 60);
    rect(x, y, w, h);
    activeCount = 0;
  }
}

