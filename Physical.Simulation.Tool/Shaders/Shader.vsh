//
//  Shader.vsh
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 06/05/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

attribute vec4 position;
attribute vec4 color;
varying vec4 colorVarying;

//attribute vec3 normal;
//
//varying lowp vec4 colorVarying;
//
//uniform mat4 modelViewProjectionMatrix;
//uniform mat3 normalMatrix;

void main()
{
//    vec3 eyeNormal = normalize(normalMatrix * normal);
//    vec3 lightPosition = vec3(0.0, 0.0, 1.0);
//    vec4 diffuseColor = vec4(0.4, 0.4, 1.0, 1.0);
//    
//    float nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)));
//                 
//    colorVarying = diffuseColor * nDotVP;
//    
//    gl_Position = modelViewProjectionMatrix * position;
    gl_Position = position;
    gl_PointSize = 3.0; // for tests
    colorVarying = color;
}