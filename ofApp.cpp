#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(255);
	ofEnableDepthTest();

	this->cap_size = cv::Size(1920, 1080);

	this->view_image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->view_mat = cv::Mat(this->view_image.getHeight(), this->view_image.getWidth(), CV_MAKETYPE(CV_8UC3, this->view_image.getPixels().getNumChannels()), this->view_image.getPixels().getData(), 0);

	this->number_of_frames = 0;

	auto file_path = "D:\\MP4\\Pexels Videos 2795744.mp4";
	this->cap.open(file_path);
	int frame_count = this->cap.get(cv::CAP_PROP_FRAME_COUNT);
	int read_count = 0;
	for (int i = 0; i < frame_count; i++) {

		cv::Mat src, tmp;
		this->cap >> src;
		if (src.empty()) {

			continue;
		}

		cv::resize(src, tmp, this->cap_size);
		cv::cvtColor(tmp, tmp, cv::COLOR_BGR2RGB);

		this->frame_list.push_back(tmp);
		read_count++;
	}
	this->number_of_frames += read_count;

	cout << number_of_frames << endl;
}

//--------------------------------------------------------------
void ofApp::update() {

	int blend_count = 4;
	int blend_span = 5;
	cv::Mat blend;
	for (int i = blend_count; i > 0; i--) {

		int frame_index = (ofGetFrameNum() + i * blend_span) % this->number_of_frames;
		if (i == blend_count) {

			this->frame_list[frame_index].copyTo(blend);
		}
		else {

			int prev_frame_index = (ofGetFrameNum() + (i - 1) *blend_span) % this->number_of_frames;
			cv::Mat tmp = this->frame_list[frame_index] - this->frame_list[prev_frame_index];
			tmp -= 255;
			blend += tmp;
		}
	}

	blend.copyTo(this->view_mat);
	this->view_image.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	this->view_image.draw(this->view_image.getWidth() * -0.5, this->view_image.getHeight() * -0.5);
	
	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}