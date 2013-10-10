#version 120
//varying vec4 cur;


void main(){
    
	gl_TexCoord[0] = gl_MultiTexCoord0;
   // cur = gl_Vertex;
	//get our current vertex position so we can modify it
	vec4 pos = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

   	gl_Position = pos;
    gl_FrontColor =  gl_Color;
}
