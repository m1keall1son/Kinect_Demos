#version 120
uniform sampler2D tex;


void main(void){
	//this is the fragment shader
	//this is where the pixel level drawing happens
	//gl_FragCoord gives us the x and y of the current pixel its drawing
	
	//we grab the x and y and store them in an int
	//float xVal = gl_FragCoord.x;
	//float yVal = gl_FragCoord.y;
	
    
    
    
    
    gl_FragColor = texture2D(tex,gl_TexCoord[0].st) * gl_Color;

    

	
}