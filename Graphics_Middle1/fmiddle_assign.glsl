in vec4 vs_color;
uniform int i;

void main(void)
{
	if(i == 0){
	gl_FragColor = vec4(0.5, 0.25, 0.0, 1.0);
	}
	else if(i == 1){
	gl_FragColor = vec4(1.0, 1.0, 0.5, 1.0);
	}
	else if(i == 2){
	gl_FragColor = vec4(0.82, 0.6, 0.43, 0.0);
	}
	else if(i == 3){
	gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);
	}
	else if(i == 4){
	gl_FragColor = vec4(1.0, 0.5, 1.0, 1.0);
	}
	else if(i == 5){
	gl_FragColor = vec4(0.5, 1.0, 0.5, 1.0);
	}
	else{
	    gl_FragColor = vs_color;
	}
}