#include "ofxProjectorBlend.h"
#include "ofxProjectorBlendShader.h"


// --------------------------------------------------
ofxProjectorBlend::ofxProjectorBlend()
{
	showBlend = true;
	//gamma = gamma2 = 0.5;
	//blendPower = blendPower2 = 1;
	//luminance = luminance2 = 0;
    gamma.resize(2, 0.5);
    blendPower.resize(2, 1);
    luminance.resize(2, 0);
	numProjectors = 0;
	threshold = 0;
}


// --------------------------------------------------
void ofxProjectorBlend::setup(int resolutionWidth, 
							  int resolutionHeight, 
							  int _numProjectors, 
							  vector<float> _pixelOverlap,
							  ofxProjectorBlendLayout _layout, 
							  ofxProjectorBlendRotation _rotation)
{

    if(_numProjectors <= 0){
		ofLog(OF_LOG_ERROR, "Cannot initialize with " + ofToString(_numProjectors) + " projectors.");
		return;
	}

    if (_pixelOverlap.size() < _numProjectors - 1) {
        ofLog(OF_LOG_NOTICE, "ofxProjectorBlend: Expected %d values for pixelOverlap, found %d. Using 0 for additional overlap values.", _numProjectors - 1, _pixelOverlap.size());
    } else if (_pixelOverlap.size() >= _numProjectors) {
        ofLog(OF_LOG_NOTICE, "ofxProjectorBlend: Expected %d values for pixelOverlap, found %d. Ignoring additional overlap values.", _numProjectors - 1, _pixelOverlap.size());
    }

    pixelOverlap = _pixelOverlap;
    pixelOverlap.resize(_numProjectors - 1, 0);

	string l = "horizontal";
	if(layout==ofxProjectorBlend_Vertical) l = "vertical";
	
	string r = "normal";
	if(rotation==ofxProjectorBlend_RotatedLeft) r = "rotated left";
	else if(rotation==ofxProjectorBlend_RotatedRight) r = "rotated right";



    stringstream s;
    for (vector<float>::iterator it = pixelOverlap.begin(); it != pixelOverlap.end(); ++it) {
        s << " " << *it;
    }

	ofLog(OF_LOG_NOTICE, "ofxProjectorBlend: res: %d x %d * %d, pixelOverlap values: %s, layout: %s, rotation: %s\n", resolutionWidth, resolutionHeight, _numProjectors, s.str().c_str(), l.c_str(), r.c_str());
	numProjectors = _numProjectors;
	layout = _layout;
	rotation = _rotation;
	
    projectorHeightOffset.clear();
	//allow editing projector heights
	for(int i = 0; i < numProjectors; i++){
		projectorHeightOffset.push_back( 0 );
	}
    
    float totalPixelOverlap = accumulate(pixelOverlap.begin(), pixelOverlap.end(), 0);


	if(rotation == ofxProjectorBlend_NoRotation) {
		singleChannelWidth = resolutionWidth;
		singleChannelHeight = resolutionHeight;
	}
	else {
		singleChannelWidth = resolutionHeight;
		singleChannelHeight = resolutionWidth;
	}
	
	if(layout == ofxProjectorBlend_Vertical) {
		fullTextureWidth = singleChannelWidth;
		fullTextureHeight = singleChannelHeight*numProjectors - totalPixelOverlap;
	}
	else if(layout == ofxProjectorBlend_Horizontal) {
		fullTextureWidth = singleChannelWidth*numProjectors - totalPixelOverlap;
		fullTextureHeight = singleChannelHeight;
	} else {
		ofLog(OF_LOG_ERROR, "ofxProjectorBlend: You have used an invalid ofxProjectorBlendLayout in ofxProjectorBlend::setup()");
		return;
	}
	
	
	displayWidth = resolutionWidth*numProjectors;
	displayHeight = resolutionHeight;
	
	fullTexture.allocate(fullTextureWidth, fullTextureHeight, GL_RGB, 4);
    
    blendShader.unload();
    blendShader.setupShaderFromSource(GL_FRAGMENT_SHADER, ofxProjectorBlendFragShader(numProjectors-1));
    blendShader.setupShaderFromSource(GL_VERTEX_SHADER, ofxProjectorBlendVertShader);
    blendShader.linkProgram();
    
    gamma.resize(numProjectors-1, 0.5);
    blendPower.resize(numProjectors-1, 1);
    luminance.resize(numProjectors-1, 0);
}


// --------------------------------------------------
void ofxProjectorBlend::begin() {
	
	fullTexture.begin();
	
	ofPushStyle();
	ofClear(0,0,0,0);
}


// --------------------------------------------------
float ofxProjectorBlend::getDisplayWidth() {
	return displayWidth;
}


// --------------------------------------------------
float ofxProjectorBlend::getDisplayHeight() {
	return displayHeight;
}


// --------------------------------------------------
void ofxProjectorBlend::moveDisplayVertical(unsigned int targetDisplay, int yOffset)
{
	if(targetDisplay >= numProjectors){
		ofLog(OF_LOG_ERROR, "targetDisplay (" + ofToString(targetDisplay) + ") is invalid.");
		return;
	}
	
	projectorHeightOffset[targetDisplay] += yOffset;
}


// --------------------------------------------------
// This changes your app's window size to the correct output size
void ofxProjectorBlend::setWindowToDisplaySize()
{
	ofSetWindowShape(getDisplayWidth(), getDisplayHeight());
}


// --------------------------------------------------
float ofxProjectorBlend::getCanvasWidth()
{
	return fullTextureWidth;
}


// --------------------------------------------------
float ofxProjectorBlend::getCanvasHeight()
{
	return fullTextureHeight;
}



// --------------------------------------------------
void ofxProjectorBlend::end()
{
	fullTexture.end();
	ofPopStyle();
}


// --------------------------------------------------
void ofxProjectorBlend::updateShaderUniforms()
{
	
	blendShader.setUniform1f("OverlapTop", 0.0f);
	blendShader.setUniform1f("OverlapLeft", 0.0f);
	blendShader.setUniform1f("OverlapBottom", 0.0f);
	blendShader.setUniform1f("OverlapRight", 0.0f);

    blendShader.setUniform1fv("BlendPower", &blendPower[0], blendPower.size());
    blendShader.setUniform1fv("SomeLuminanceControl", &luminance[0], luminance.size());
    blendShader.setUniform1fv("GammaCorrection", &gamma[0], gamma.size());
    
	blendShader.setUniform1f("projectors", this->numProjectors);
	blendShader.setUniform1f("threshold", threshold);
}


// --------------------------------------------------
void ofxProjectorBlend::draw(float x, float y) {
	ofSetHexColor(0xFFFFFF);
	glPushMatrix();
	glTranslatef(x, y, 0);
	if(showBlend) {
		blendShader.begin();
		blendShader.setUniform1f("width", singleChannelWidth);
		blendShader.setUniform1f("height", singleChannelHeight);
		
		updateShaderUniforms();

		blendShader.setUniformTexture("Tex0", fullTexture.getTextureReference(), 0);
		
		
		ofVec2f offset(0,0);
		glPushMatrix();
		
		// loop through each projector and glTranslatef() to its position and draw.
		for(int i = 0; i < numProjectors; i++) {
			blendShader.setUniform2f("texCoordOffset", offset.x, offset.y);

            float overlapLeft = 0, overlapRight = 0, overlapTop = 0, overlapBottom = 0;
            if (layout == ofxProjectorBlend_Horizontal) {
                // do not set for first projector
                if (i != 0) {
                    overlapLeft = pixelOverlap[i-1];
                }

                // do not set for last projector
                if (i + 1 != numProjectors) {
                    overlapRight = pixelOverlap[i];
                }

                blendShader.setUniform1f("OverlapLeft", overlapLeft);
                blendShader.setUniform1f("OverlapRight", overlapRight);
            } else {
                // do not set for first projector
                if (i != 0) {
                    overlapBottom = pixelOverlap[i-1];
                }

                // do not set for last projector
                if (i + 1 != numProjectors) {
                    overlapTop = pixelOverlap[i];
                }


                blendShader.setUniform1f("OverlapTop", overlapTop);
                blendShader.setUniform1f("OverlapBottom", overlapBottom);
            }

			glPushMatrix(); {
				if(rotation == ofxProjectorBlend_RotatedRight) {
					glRotatef(90, 0, 0, 1);
					glTranslatef(0, -singleChannelHeight, 0);
				}
				else if(rotation == ofxProjectorBlend_RotatedLeft) {
					glRotatef(-90, 0, 0, 1);
					glTranslatef(-singleChannelWidth, 0, 0);
				}
				
				glTranslatef(0, (float)projectorHeightOffset[i], 0);
				
				glBegin(GL_QUADS);
				
				glTexCoord2f(0, 0);
				glVertex2f(0, 0);
				
				glTexCoord2f(singleChannelWidth, 0);
				glVertex2f(singleChannelWidth, 0);
				
				glTexCoord2f(singleChannelWidth, singleChannelHeight);
				glVertex2f(singleChannelWidth, singleChannelHeight);
				
				glTexCoord2f(0, singleChannelHeight);
				glVertex2f(0, singleChannelHeight);
				
				glEnd();
			}
			glPopMatrix();
			
			// move the texture offset and where we're drawing to.
			if(layout == ofxProjectorBlend_Horizontal) {
				offset.x += singleChannelWidth - overlapRight;
			}
			else {
				offset.y += singleChannelHeight - overlapTop;
			}
			
			if(rotation == ofxProjectorBlend_RotatedLeft || rotation == ofxProjectorBlend_RotatedRight) {
				glTranslatef(singleChannelHeight, 0, 0);
			}
			else {
				glTranslatef(singleChannelWidth, 0, 0);
			}
			
		}
		glPopMatrix();
	
		blendShader.end();
	} else {
		fullTexture.draw(x, y);
	}
	glPopMatrix();
}


