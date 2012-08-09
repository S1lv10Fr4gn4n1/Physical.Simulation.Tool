//
//  SimulatedObject.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//  
//

#ifndef SIMULATEDOBJECT_H
#define SIMULATEDOBJECT_H

#include <vector>
#include "CommonStruct.h"
#include "Transform.h"

class SimulatedObject {
private:
    float * pointers;
    std::vector<Pointer *> * pointersAux;
    PhysicalFeature * physicalFeature;
    unsigned char * color;
    unsigned char * colorPoints;
    Color * colorAux;
    unsigned int mode; // GLenum
    std::vector<SimulatedObject *> * joinsSimulatedObject;
    BBox * bbox;
    float * matrixTransformation;
    
    bool showBBox;
    bool selected;
    bool immovable;
    
    void makePointers();
    void makeColorObject();
    void makeColorPoints();
    void makeBBox();
protected:
    
public:
    SimulatedObject();
    ~SimulatedObject();
    
    void initialize();
    void initBBox(float * _matrix);
    
    void addPointer(Pointer * _pointer);
    void deletePointer(Pointer * _pointer);
    void addAllPointers(std::vector<Pointer *> * _pointers);
    float * getPointers();

    std::vector<Pointer *> * getPointersAux();

    unsigned char * getColor();
    unsigned char * getColorPoints();
    
    void setColorAux(Color * _color);
    Color * getColorAux();
    
    PhysicalFeature * getPhysicalFeature();
    void setPhysicalFeature(PhysicalFeature * _physicalFeature);
    
    void setMode(unsigned int _mode);
    unsigned int getMode();
    
    BBox * getBBox();
    void setBBox(BBox * _bbox);
    
    bool isShowBBox();
    void setShowBBox(bool _show);
    
    bool isSelected();
    void setSelected(bool _selected);
    
    float * getMatrixTransformation();
    void setMatrixTransformation(float * _matrix);
    
    bool isImmovable();
    void setImmovable(bool _immovable);
};

#endif
