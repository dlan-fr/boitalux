#include "ofApp.h"

const char my_user_data[6] = "hello";

void mylog(void* userdata,const char* str) {
    printf("%s\n",str);
}

//--------------------------------------------------------------
void ofApp::setup(){

    std::string path = ofFilePath::getCurrentExeDir();

    std::string socket_path = path;

    socket_path.append("data/socket");

    appwidth = 1024;
    appheight = 768;

   texture_width = 1024;
   texture_height = 1024;

   logger = shmdata_make_logger(&mylog,&mylog,&mylog,&mylog,&mylog,&mylog,(void*)my_user_data);

   char caps[1024] = {0};

   sprintf(caps,"video/x-raw,format=(string)RGB,width=(int)%d,height=(int)%d,framerate=(fraction)1/1",texture_width,texture_height);

   texture_buffer.allocate(texture_width,texture_height,OF_PIXELS_RGB);

   texture_buffer.swapRgb();

   writer = shmdata_make_writer(socket_path.c_str(),texture_buffer.size(),caps,NULL,NULL,NULL,logger);


   ofColor couleurbase = ofColor::blue;

   //assignation couleur de départ
   for (int y = 0; y < texture_height; y++){
       for (int x = 0; x < texture_width; x++){

              texture_buffer.setColor(x,y,couleurbase);

       }
   }

    texture.allocate(texture_buffer);

}

//--------------------------------------------------------------
void ofApp::update(){
    texture.loadData(texture_buffer);
    shmdata_copy_to_shm(writer,texture_buffer.getData(),texture_buffer.size());
}

//--------------------------------------------------------------
void ofApp::draw(){

    texture.draw(0,0,appwidth,appheight);

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

void ofApp::exit() {
    shmdata_delete_logger(logger);
    shmdata_delete_writer(writer);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
