
void initGUI(){
  GUI = new ControlP5(this);
  GUI.addSlider("nearThreshold").setPosition(10,10).setRange(0, 8000);
  GUI.addSlider("farThreshold").setPosition(10,30).setRange(0, 8000);

}



