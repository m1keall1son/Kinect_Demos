#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D sprite;

varying vec4 vertColor;
varying vec2 texCoord;
varying vec4 pos;


void main() {  
  gl_FragColor = texture2D(sprite, texCoord) * vertColor;
}