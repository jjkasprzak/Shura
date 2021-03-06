#pragma once

#include "Entity.h"
#include "AssetManager.h"

namespace Didax
{

class Engine;

template <typename T>
class Sprite : public Entity<T>
{
public:

    Sprite(Engine * parent, const std::string & name, std::shared_ptr<T> gameObj) :Entity<T>(parent, name, gameObj)
    {}

    void setTexture(const std::string & name)
    {
        _sprite.setTexture(AssetManager::getAsset<TextureAsset>(name)->_texture);
        this->setSize({(float)(_sprite.getTexture()->getSize().x), (float)(_sprite.getTexture()->getSize().y)});
    }

    void setColor(const sf::Color & c)
    {
        _sprite.setColor(c);
    }

    sf::Color getColor()const
    {
        return _sprite.getColor();
    }

protected:

    virtual void _update(double dt)
    {}
    virtual void _input(const sf::Event & evt)
    {}
    virtual void _draw(sf::RenderTarget & target, sf::RenderStates states)const
    {
        if(this->_visible)
            target.draw(_sprite, states);
    }
    virtual void updatePosition()
    {
        _sprite.setPosition(this->_position);
    }
	virtual void updateSize()
    {
        if (_sprite.getTexture() == nullptr)
		    return;
	    auto textureSize = _sprite.getTexture()->getSize();
	    _sprite.setScale({ this->_size.x / textureSize.x, this->_size.y / textureSize.y });
    }

    sf::Sprite _sprite;

};

}