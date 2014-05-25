//
//  MapSprite.h
//  Shader
//
//  Created by Akihiro Matsuura on 5/15/14.
//
//

#ifndef __Shader__MapSprite__
#define __Shader__MapSprite__

#include "cocos2d.h"

using namespace cocos2d;

class MapSprite : public CCSprite
{
private:
    void linkShader();
    
    CCGLProgram *shader;
    int normalMapUniformLocation;
    CCTexture2D* colorRampTexture;
    int lightNormalUniformLocation;
    int tex;
    std::string baseName;
public:
    virtual ~MapSprite();
    static MapSprite* create(const char *pszFilename, int tex, CCLayer* _parent);
    virtual void update(float dt);
    CCLayer* parent;
};

#endif /* defined(__Shader__MapSprite__) */
