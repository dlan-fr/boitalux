#pragma once

#include "ofMain.h"
#include "enum.h"
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


    void remplirFace(ofRectangle face,ofColor color);
    ofColor pickColor(float x,float y,uint32_t start_index,ofRectangle face,FillOrient orient);

    void majFaceDepuisGrille(ofRectangle face,FaceIndex index,FillOrient orient);

    void updateGrille(FaceIndex index, int poscol,int fillval,ofColor colfill);

    void initCube();

    //taille texture
    int32_t texture_width;
    int32_t texture_height;

    //valeurs grilles

    //nombre de carré pour un coté d'une face du cube
    uint32_t div_grille;
    uint32_t total_face_grille;
    uint32_t total_grille;

    //contient la couleur de chaque carré des face du cube
    vector<ofColor> grille;

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


    //variable positionnement face du cube
    ofRectangle front;
    ofRectangle back;
    ofRectangle top;
    ofRectangle left;
    ofRectangle right;


};
