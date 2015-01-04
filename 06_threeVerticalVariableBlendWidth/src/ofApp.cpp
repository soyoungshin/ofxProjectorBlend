#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    vector<float> pixelOverlaps;
    pixelOverlaps.push_back(20);
    pixelOverlaps.push_back(40);

    blender.setup(640, 480, 3, pixelOverlaps, ofxProjectorBlend_Vertical);
    blender.setWindowToDisplaySize();

    radius = 40;
    pos.x = ofRandom(radius, blender.getCanvasWidth()-radius);
    pos.y = ofRandom(radius, blender.getCanvasHeight()-radius);
    vel.set(10, 10);
}

//--------------------------------------------------------------
void ofApp::update(){
    pos += vel;
    if(pos.x > blender.getCanvasWidth()-radius) {
        pos.x = blender.getCanvasWidth()-radius;
        vel.x *= -1;
    }
    if(pos.x < radius) {
        pos.x = radius;
        vel.x *= -1;
    }
    if(pos.y > blender.getCanvasHeight()-radius) {
        pos.y = blender.getCanvasHeight()-radius;
        vel.y *= -1;
    }
    if(pos.y < radius) {
        pos.y = radius;
        vel.y *= -1;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	blender.begin(); //call blender.begin() to draw onto the blendable canvas
    {
        //light gray backaground
        ofSetColor(100, 100, 100);
        ofRect(0, 0, blender.getCanvasWidth(), blender.getCanvasHeight());

        //thick grid lines for blending
        ofSetColor(255, 255, 255);
        ofSetLineWidth(3);

        //vertical line
        for(int i = 0; i <= blender.getCanvasWidth(); i+=40){
            ofLine(i, 0, i, blender.getCanvasHeight());
        }

        //horizontal lines
        for(int j = 0; j <= blender.getCanvasHeight(); j+=40){
            ofLine(0, j, blender.getCanvasWidth(), j);
        }

        ofSetColor(255, 0, 0);
        ofCircle(pos, radius);

        //instructions
        ofSetColor(255, 255, 255);
        ofRect(10, 10, 350, 115);
        ofSetColor(0, 0, 0);
        ofDrawBitmapString("SPACE - toggle show blend\n[f/F][g/G] - adjust gamma\n[o/O][p/P] - adjust blend power\n[k/K][l/L] adjust luminance\n[z/Z][x/X] - adjust blend width\nfor screen pairs 1/2 and 2/3 respectively", 15, 35);
	}
	blender.end(); //call when you are finished drawing


	//this draws to the main window
	blender.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	//hit spacebar to toggle the blending strip
	if(key == ' '){
		//toggling this variable effects whether the blend strip is shown
		blender.showBlend = !blender.showBlend;
	}

	// more info here on what these variables do
	// http://local.wasp.uwa.edu.au/~pbourke/texture_colour/edgeblend/

    else if(key == 'f'){
		blender.gamma[0] -= .05;
	}
	else if(key == 'F'){
		blender.gamma[0] += .05;
	}
	else if(key == 'g'){
		blender.gamma[1]  -= .05;
	}
	else if(key == 'G'){
		blender.gamma[1]  += .05;
	}
    else if(key == 'k'){
		blender.luminance[0]  -= .05;
	}
	else if(key == 'K'){
		blender.luminance[0]  += .05;
	}
	else if(key == 'l'){
		blender.luminance[1]  -= .05;
	}
	else if(key == 'L'){
		blender.luminance[1]  += .05;
	}
    else if(key == 'o'){
		blender.blendPower[0] -= .05;
	}
	else if(key == 'O'){
		blender.blendPower[0] += .05;
	}
	else if(key == 'p'){
		blender.blendPower[1]  -= .05;
	}
	else if(key == 'P'){
		blender.blendPower[1]  += .05;
	}
    else if(key == 'z'){
		blender.pixelOverlap[0] = max(0.0, (blender.pixelOverlap[0] - 1.0));
	}
	else if(key == 'Z'){
		blender.pixelOverlap[0]++;
	}
    else if(key == 'x'){
		blender.pixelOverlap[1] = std::max(0.0, (blender.pixelOverlap[1] - 1.0));
	}
	else if(key == 'X'){
		blender.pixelOverlap[1]++;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
