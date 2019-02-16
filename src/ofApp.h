#pragma once

#include "ofMain.h"
#include <inttypes.h>
#include "shmdata/cwriter.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    //taille texture
    int32_t texture_width;
    int32_t texture_height;

    //taille fenêtre
    int32_t appwidth;
    int32_t appheight;

    //variables shmdata

    //objet gérant l'affichage des logs en provenance de shmdata
    ShmdataLogger logger;

    //objet écrivant sur le socket unix de shmdata les données de texture, permet la communication en boiteàlux et splash
    ShmdataWriter writer;

    //contient les pixels de notre texture à transmettre à splash
    //unsigned char texture_buffer[1024*1024*4];
    ofPixels texture_buffer;
    ofTexture texture;

};
