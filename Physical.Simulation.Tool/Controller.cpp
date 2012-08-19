//
//  Controller.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/07/12.
//  
//

#include "Controller.h"

Controller * Controller::controller;

// TODO revise: variables for this place is correct?
real scaleObject = 1.0f;
real scaleZoom = 1.0f;
//real scalePanX = 0.0f;
//real scalePanY = 0.0f;
real previousRadians = 0.0f;
real previousZoom = 0.0f;
real previousScale = 0.0f;
//Vector3 * previousVector3 = NULL;

Controller::Controller()
{
    this->mainEngine = NULL;
    this->mainGraphic = NULL;
    this->objectEdition = NULL;
    this->objectOffset = NULL;
}

Controller::~Controller()
{
    this->freeObjects();
}

Controller * Controller::getInstance()
{
    if (!controller) {
        controller = new Controller();
        controller->initializeEngine();
    }
    
    return controller;
}

void Controller::freeObjects()
{
    if (this->mainGraphic) {
        delete this->mainGraphic;
    }
    
    if (this->mainEngine) {
        delete this->mainEngine;
    }
    
    if (this->objectEdition) {
        delete this->objectEdition;
    }

    if (this->objectOffset) {
        delete this->objectOffset;
    }

    this->mainGraphic = NULL;
    this->mainEngine = NULL;
    this->objectEdition = NULL;
    this->objectOffset = NULL;
}

void Controller::initializeContextOpenGLES()
{
    if (this->mainGraphic) {
        return;
    }
    
    // TODO revise, no objects 'no C++ ansi'
    NSString * vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
    NSString * fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
    const char * vertShaderSource = [[NSString stringWithContentsOfFile:vertShaderPathname encoding:NSUTF8StringEncoding error:0] UTF8String];
    const char * fragShaderSource = [[NSString stringWithContentsOfFile:fragShaderPathname encoding:NSUTF8StringEncoding error:0] UTF8String];

    this->mainGraphic = new MainGraphic();
    this->mainGraphic->initializeShader(vertShaderSource, fragShaderSource);
}

void Controller::initializeEngine()
{
    if (this->mainEngine) {
        return;
    }
    
    this->mainEngine = new MainEngine();
}

void Controller::resizeScreen(real _width, real _height)
{
    this->mainEngine->rotatedScreen(_width, _height);
}

void Controller::updateInformation(real _duration)
{
    this->mainEngine->updateInformation(_duration);
}

void Controller::draw()
{
    this->mainGraphic->draw(this->mainEngine->getWorld());
}

void Controller::stopSimulation()
{
    this->mainEngine->stop();
    scaleZoom = 1.0f;
    this->mainEngine->zoom(scaleZoom);
}

void Controller::startSimulation()
{
    this->mainEngine->start();
}

void Controller::editSimulation()
{
    // TODO put your code here
}

bool Controller::isRunning()
{
    if (!this->mainEngine) {
        return false;
    }
    
    return this->mainEngine->isRunning();
}

bool Controller::isInitialized()
{
    if (this->mainEngine && this->mainGraphic && this->mainEngine->isRunning()) {
        return true;
    }
    
    return false;
}

void Controller::touchesBegan(real _x, real _y)
{
    // TODO put your code here
}

void Controller::touchesEnded(real _x, real _y)
{
    // TODO put your code here
}

void Controller::touchesCancelled(real _x, real _y)
{
    // TODO put your code here
}

void Controller::touchesMoved(real _x, real _y, int _countFingers)
{
    Vector3 * vector = MakeVector3(_x, _y);

    // move object
    if (_countFingers == 1) {
        this->objectOffset = this->mainEngine->selectedSimulatedObject(vector);
        
        if (this->objectOffset && !this->objectOffset->isImmovable()) {
            if (this->objectEdition) {
                this->objectEdition->setSelected(false);
                this->objectEdition = NULL;
            }
            
            this->mainEngine->translateSimulatedObject(this->objectOffset, vector);
            this->mainEngine->updatePositionSimulatedObject(this->objectOffset, vector);
        }
        
        this->objectOffset = NULL;
    }
    
//    TODO revise: pan
//    move scene
//    if (countFingers == 2) {
//        if (previousVector) {
//            if (previousVector->x > vector->x) {
//                scalePanX -= 0.009;
//            } else if (previousVector->x < vector->x) {
//                scalePanX += 0.009;
//            }
//
//            if (previousVector->y > vector->y) {
//                scalePanY += 0.009;
//            } else if (previousVector->y < vector->y) {
//                scalePanY -= 0.009;
//            }
//
//            this->mainEngine->pan(scalePanX, scalePanY);
//
//            delete previousVector;
//            previousVector = NULL;
//        }
//        
//        previousVector = MakeVector3(vector);
//    }
    
    delete vector;
    vector = NULL;
}

void Controller::pinchDetected(real _scale, real _velocity, bool _began)
{
    if (this->objectEdition && this->objectEdition->isSelected() && !this->objectEdition->isImmovable()) {
        if (_began) {
            scaleObject = 1.0f;
            previousScale = 0.0;
        }
         
        if ((_scale - previousScale) < 0.0f) {
            scaleObject-=0.0005f;
        } else {
            scaleObject+=0.0005f;
        }
        this->mainEngine->scaleSimulatedObject(this->objectEdition, scaleObject);
        previousScale = _scale;
    } else {
        if ((_scale - previousZoom) < 0.0f) {
            scaleZoom+=0.03f;
        } else {
            scaleZoom-=0.03f;
        }
        // limit zoom in
        if (scaleZoom >= 3.0f){
            scaleZoom = 3.0f;
            return;
        }
        // limit zoom out
        if (scaleZoom <= 0.1f) {
            scaleZoom = 0.1f;
            return;
        }
        this->mainEngine->zoom(scaleZoom);
        previousZoom = _scale;
    }
}

void Controller::rotationDetected(real _radians, real _velocity, bool _began)
{
    if (_began) {
        previousRadians = 0.0f;
    }
  
    if (this->objectEdition && this->objectEdition->isSelected()) {
        this->mainEngine->rotateSimulatedObject(this->objectEdition, previousRadians < _radians ? -0.03 : 0.03);
    }
    previousRadians = _radians;
}

void Controller::doubleTapOneFingerDetected(real _x, real _y)
{
    Vector3 * vector = MakeVector3(_x, _y);
    
    this->objectEdition = this->mainEngine->selectedSimulatedObject(vector);
    
    if (this->objectEdition) {
        if (this->objectEdition->isSelected()) {
            this->objectEdition->setSelected(false);
            this->objectEdition = NULL;
        } else {
            this->objectEdition->setSelected(true);
        }
    }
    
    delete vector;
    vector = NULL;    
}

void Controller::longPressDetected(real _x, real _y)
{
    Vector3 * vector = MakeVector3(_x, _y);
    
    this->objectEdition = this->mainEngine->selectedSimulatedObject(vector);

    if (this->objectEdition) {
        this->mainEngine->deleteSimulatedObject(this->objectEdition);
    }
    
    this->objectEdition = NULL;
}

void Controller::swipeRightDetected(real _x, real _y)
{
    // TODO put your code here
}
void Controller::swipeLeftDetected(real _x, real _y)
{
    // TODO put your code here
}

void Controller::oneTapThreeFingerDetected(real _x, real _y)
{
//    delete previousVector;
//    previousVector = NULL;
    
    scaleZoom = 1.0f;
    this->mainEngine->zoom(scaleZoom);
}

void Controller::createSimulatedObject(TypeObject _typeObject)
{    
    if (this->objectEdition) {
        this->objectEdition->setSelected(false);
    }
    
    SimulatedObject * object = new SimulatedObject();
    ForceGravity * gravity = ForceGravity::getInstance();
    
    ForceRegistry::getInstance()->add(object, gravity);

    PhysicalFeature * physicalFeature = new PhysicalFeature();
    physicalFeature->setAcceleration(MakeVector3(0.0f, 0.0f)); // MakeVector3(0.0f, -10.0f); // -10 m/s2
    physicalFeature->setPosition(MakeVector3(0.0f, 0.0f));
    physicalFeature->setVelocicy(MakeVector3(getRand(5.0f), 0.0f));
    physicalFeature->setMass(2.0f);    // 1 kg
    physicalFeature->setVolume(0.0f);
    physicalFeature->setDensity(0.0f);
    physicalFeature->setDamping(0.9f); // TODO revise: change for drag forces
    
    object->setPhysicalFeature(physicalFeature);
    object->setColorAux(MakeRandonColor());
    object->setMode(GL_TRIANGLE_FAN);
    
    this->mainEngine->makeSimulatedObject(object, _typeObject);
    
//    // TODO for Tests
//    float x = getRand(3.0f);
//    x = fmodf(x, 0.04) <= 0.03 ? x : -x;
//
//    float y = getRand(3.0f);
//    y = fmodf(y, 0.04) <= 0.03 ? y : -y;
//
//    Vector3 *v = MakeVector3(x, y);
//    this->mainEngine->translateSimulatedObject(object, v);
//    this->mainEngine->updatePositionSimulatedObject(object, v);
}

void Controller::clearSimularion()
{
    this->mainEngine->deleteAllSimulatedObjects();
    
    if (this->objectEdition) {
        delete this->objectEdition;
    }
    
    if (this->objectOffset) {
        delete this->objectOffset;
    }
    
    this->objectEdition = NULL;
    this->objectOffset = NULL;
}