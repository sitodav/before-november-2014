int fromjs_r=255,fromjs_g=0,fromjs_b=0;

interface Javascript{
	void fillXY(int x,int y);
}

Javascript js;



void bindJS(Javascript toBind){
	js=toBind;
}

void set(int r,int g,int b){
fromjs_r=r,fromjs_g=g,fromjs_b=b;
}

void setup(){
	size(1000,500,P3D);
	background(0);
	stroke(fromjs_r,fromjs_g,fromjs_b);
	noLoop();
}

void draw(){
	fill(0,0,0,100);
	rect(-1,-1,width+2,height+2);

}

void mouseMoved(){
	
	redraw();
	stroke(fromjs_r,fromjs_g,fromjs_b);
	line(mouseX,0,mouseX,height);
	line(0,mouseY,width,mouseY);
	if(js!=null)
		js.fillXY(mouseX,mouseY);
	
}

