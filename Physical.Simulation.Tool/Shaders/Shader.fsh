//
//  Shader.fsh
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 06/05/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
