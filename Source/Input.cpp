/*
 This file is part of spatServerGRIS.
 
 Developers: Nicolas Masson
 
 spatServerGRIS is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 spatServerGRIS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with spatServerGRIS.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "Input.h"
#include "MainComponent.h"
#include "LevelComponent.h"

Input::Input(MainContentComponent * parent, GrisLookAndFeel * feel,int id){
    this->mainParent = parent;
    this->grisFeel = feel;
    this->idChannel = id;
    this->center = glm::vec3(10,0,10);
    
    this->vuMeter = new LevelComponent(this, this->grisFeel);
}
Input::~Input(){
    delete this->vuMeter;
}

glm::vec3 Input::getCenter(){
    return this->center;
}

float Input::getLevel(){
    return this->mainParent->getLevelsIn(this->idChannel-1);
}
void Input::setMuted(bool mute){
    this->mainParent->muteInput(this->idChannel, mute);
}
void Input::setSolo(bool solo){
    this->mainParent->soloInput(this->idChannel, solo);
}
void Input::setColor(Colour color, bool updateLevel){
    this->color.x = color.getFloatRed();
    this->color.y = color.getFloatGreen();
    this->color.z = color.getFloatBlue();
    if(updateLevel){
        this->vuMeter->setColor(color);
    }
}
glm::vec3 Input::getColor(){
    return this->color;
}

void Input::draw(){
    // Draw 3D sphere.
    glPushMatrix();
    glTranslatef(this->center.x, this->center.y, this->center.z);
    //glRotatef(90, 1, 0, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //GL_LINE
    glLineWidth(2);
    glColor3f(this->color.x, this->color.y, this->color.z);
    glutSolidSphere(sizeT, 8, 8);
    
    drawSpan();
    
    glTranslatef(-this->center.x, -this->center.y, -this->center.z);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();
}

void Input::drawSpan()
{

    float radCir = sqrt((this->center.x*this->center.x)+(this->center.z*this->center.z));
    float ang = -atan2(( - this->center.z * 10.0f), (this->center.x * 10.0f ));
    
    glTranslatef(-(cos(ang) * radCir) , 0, -(sin(ang) * radCir));
    ang = ((ang* 180.0f)/M_PI) ;

    glRotatef((360.0f-ang) + (this->azimSpan* 90), 0, 1, 0);

    glBegin(GL_POINTS);
    for(int i = 0; i <= this->azimSpan * 90 ; ++i){
        double angle = (M2_PI * i / 180);
        glVertex3f(cos(angle)*radCir, 0.0f, sin(angle)*radCir);
    }
    glEnd();

}

void Input::updateValues(float az, float ze, float azS, float zeS, float heS, float g){
    
    this->azimuth = az; //fmod(((az/M_PI)-M_PI)*-10.0f,(M2_PI));
    this->zenith  = ze; //(M_PI2) - (M_PI * ze);     //((ze-0.5f)/M_PI)*-10.0f;
    
    this->azimSpan = azS;
    this->zeniSpan = zeS;
    
    //cout << this->azimSpan << " <> "<< zeS<< endl;
    this->gain = g;
    
    this->center.x = (10.0f * sinf(this->zenith)*cosf(this->azimuth));
    this->center.z = (10.0f * sinf(this->zenith)*sinf(this->azimuth));
    this->center.y = ((10.0f * cosf(this->zenith)) + (sizeT/2.0f )) * heS;
    
}

void Input::updateValuesOld(float az, float ze, float azS, float zeS, float g){

    this->azimuth = fmod(((az/M_PI)-M_PI)*-10.0f,(M2_PI));
    this->zenith  = (M_PI2) - (M_PI * ze);     //((ze-0.5f)/M_PI)*-10.0f;
    
    this->azimSpan = (azS * M_PI);
    this->zeniSpan = zeS;
    
    //cout << this->azimSpan << " <> "<< zeS<< endl;
    this->gain = g;
    
    this->center.x = (10.0f * sinf(this->zenith)*cosf(this->azimuth));
    this->center.z = (10.0f * sinf(this->zenith)*sinf(this->azimuth));
    this->center.y = (10.0f * cosf(this->zenith)) + (sizeT/2.0f );

}
