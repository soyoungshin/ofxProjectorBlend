#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Some dummy values. We actually load these values from xml.
    vector<float> pixelOverlaps;
    pixelOverlaps.push_back(0);
    pixelOverlaps.push_back(0);

    blender.setup(PROJECTOR_WIDTH, PROJECTOR_HEIGHT, PROJECTOR_COUNT, pixelOverlaps);
	blender.setWindowToDisplaySize();

	gui.addToggle("Show Blend", blender.showBlend);

    for (int i=0; i<PROJECTOR_COUNT-1; i++)
    {
        gui.addSlider("Blend Power "+ofToString(i+1), blender.blendPower[i], 0.0, 4.0);
        gui.addSlider("Gamma "+ofToString(i+1), blender.gamma[i], 0.1, 4.0);
        gui.addSlider("Luminance "+ofToString(i+1), blender.luminance[i], 0.0, 1.0);
        gui.addSlider("Blend Width"+ofToString(i+1), blender.pixelOverlap[i], 0.0, 200);
    }

    gui.loadFromXML();
    gui.show();
}

//--------------------------------------------------------------
void ofApp::update(){

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
	}
	blender.end(); //call when you are finished drawing


	//this draws to the main window
	blender.draw();


    // We might not want the GUI to be blended...
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key=='g' || key=='G')
        gui.toggleDraw();
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
