//
//  MapSprite.cpp
//  Shader
//
//  Created by Akihiro Matsuura on 5/15/14.
//
//

#include "MapSprite.h"
#include "HelloWorldScene.h"

MapSprite::~MapSprite()
{
    
}

MapSprite* MapSprite::create(const char *pszFilename, int _tex,  CCLayer* _parent)
{
    MapSprite* sprite = new MapSprite();
    std::string file = pszFilename;
    file += ".png";
    if (sprite && sprite->initWithFile(file.c_str()))
    {
        sprite->parent = _parent;
        sprite->tex = _tex;
        sprite->baseName = pszFilename;
        sprite->autorelease();
        
        sprite->linkShader();
        
        return sprite;
    }
    
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void MapSprite::linkShader()
{
    // フラグメントシェーダーを読込んでリンクする
    int len = 0;
    const GLchar * fragmentSource = (GLchar*)CCFileUtils::sharedFileUtils()->getFileData("MapShader.fsh", "rb", (unsigned long*)(&len));
    this->shader = new CCGLProgram();
    this->setShaderProgram(shader);
    shader->initWithVertexShaderByteArray(ccPositionTextureA8Color_vert, fragmentSource);
    shader->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    shader->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    shader->link();
    shader->updateUniforms();
    
    // 初期値を入れておく
    normalMapUniformLocation = glGetUniformLocation(this->shader->getProgram(), "u_normalMapTexture");
    glUniform1i(normalMapUniformLocation, 1+tex);
    lightNormalUniformLocation = glGetUniformLocation(this->shader->getProgram(), "u_lightNormal");
    glUniform3f(lightNormalUniformLocation, 1, 0, 0);
    
    // ファイル名に「_n」を付けたリソースを法線マップとして読込む
    std::string map = baseName + "_n.png";
    colorRampTexture = CCTextureCache::sharedTextureCache()->addImage(map.c_str());
    colorRampTexture->setAliasTexParameters();
    
    // OpenGLのテクスチャーにセット。引数のtex番目にセットする
    this->shader->use();
    glActiveTexture(GL_TEXTURE1+tex);
    glBindTexture(GL_TEXTURE_2D, colorRampTexture->getName());
    glActiveTexture(GL_TEXTURE0);
    
    //スケジュールアップデート
    this->scheduleUpdate();
}

void MapSprite::update(float dt)
{
    float xx, yy, zz, length;
    //シェーダー使う宣言
    this->shader->use();
    
    HelloWorld* layer = (HelloWorld*)parent;
    
    //スプライト座標から、光源座標までの長さを計算
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    xx = (layer->getLight().x - this->getPositionX()) / size.width;
    yy = (layer->getLight().y - this->getPositionY()) / size.height;

    length = sqrtf( xx * xx + yy * yy );
    
    //奥行きは、長さから勝手に割出して三次元で再計算
    zz = 1-length;
    length = sqrtf( zz * zz + xx * xx + yy * yy );
    zz = zz / length;
    xx = xx / length;
    yy = yy / length;
    
    //シェーダープログラムに光源の位置を送る
    glUniform3f(lightNormalUniformLocation, zz, yy, xx);
}