#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	camWidth = 640;  // try to grab at this size.
	camHeight = 480;
	gui.setup();
	
	gui.add(label.setup("each pixel block size", ""));
	gui.add(intSlider.setup("power of 2", 1, 0, 10));
	
	//get back a list of devices.
	vector<ofVideoDevice> devices = vidGrabber.listDevices();
	
	for(size_t i = 0; i < devices.size(); i++){
		if(devices[i].bAvailable){
			//log the device
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
		}else{
			//log the device and note it as unavailable
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
		}
	}
	
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(30);
	vidGrabber.setup(camWidth, camHeight);
	
	videoInverted.allocate(camWidth, camHeight, OF_PIXELS_RGB);
	videoTexture.allocate(videoInverted);
	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100, 100, 100);
	vidGrabber.update();
	
	if(vidGrabber.isFrameNew()){
		ofPixels & pixels = vidGrabber.getPixels();

		// intSlider refers to how many times we will power 2 to get the size of the pixel blocks in pixels.
		int groupingInPowers = intSlider;
		
		int groupingLength = std::pow(2, groupingInPowers);
		int groupingTotal = std::pow(groupingLength, 2);
		
		// populated keeps track of the blocks that we have already averaged out, so that we can skip
		// when we reach another pixel in that r/c combination.
		std::set<int> populated = {};
		
		// pixels have 3 channels for RGB, so jump by 3 to get to each pixel.
		for(size_t i = 0; i < pixels.size(); i += 3){
			
			// get pixel index and original row and column.
			int pI = std::floor(i / 3);
			int r = std::floor(pI / camWidth);
			int c = pI % camWidth;
			
			// get row and column in the new grouped pixels image.
			int nR = std::floor(r/groupingLength);
			int nC = std::floor(c/groupingLength); //
			
			// if we have already calculated this r/c combo, skip
			if (populated.find(nR * camWidth + nC) != populated.end()) {
				continue;
			}
			
			float sumR = 0, sumG = 0, sumB = 0;
			// iterate over all pixels in the current block
			for (int x = nR * groupingLength; x < (nR + 1) * groupingLength; x++) {
				for (int y = nC * groupingLength; y < (nC + 1) * groupingLength; y++) {
					if (x < camHeight && y < camWidth) {
						sumR += pixels[3 * (x * camWidth + y)];         // red
						sumG += pixels[3 * (x * camWidth + y) + 1];     // green
						sumB += pixels[3 * (x * camWidth + y) + 2];     // blue
					}
				}
			}
			// take the average of all pixels in the block
			float avgR = sumR / groupingTotal;
			float avgG = sumG / groupingTotal;
			float avgB = sumB / groupingTotal;
			
			// update all pixels in the block
			for (int x = nR * groupingLength; x < (nR + 1) * groupingLength; x++) {
				for (int y = nC * groupingLength; y < (nC + 1) * groupingLength; y++) {
					if (x < camHeight && y < camWidth) {
						videoInverted[3 * (x * camWidth + y)] = avgR;         // red
						videoInverted[3 * (x * camWidth + y) + 1] = avgG;     // green
						videoInverted[3 * (x * camWidth + y) + 2] = avgB;     // blue
					}
				}
			}
			
			// mark block as populated
			populated.insert(nR * camWidth + nC);
		}
		
		//load the inverted pixels
		videoTexture.loadData(videoInverted);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetHexColor(0xffffff);
	vidGrabber.draw(20, 20);
	videoTexture.draw(20 + camWidth, 20, camWidth, camHeight);
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
