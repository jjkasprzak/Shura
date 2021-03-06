#pragma once

#include <memory>
#include <mutex>
#include <SFML/Graphics.hpp>

#include "HasMember.h"

namespace Didax
{

class Engine;

class Entity_t: public sf::Drawable
{
public:
    Entity_t(Engine * parent, const std::string & name)
    {
        _parent = parent;
        _name = name;
    }

    virtual void update(double dT) = 0;
    virtual void input(const sf::Event & evt) = 0;
    virtual void kill() = 0;
    virtual void start() = 0;

    bool getToKill()const
    {
        return _toKill;
    }

    void setToKill()
    {
        _toKill = true;
    }

    void setPriority(int p)
    {
        _priority = p;
    }

    int getPriority()const
    {
        return _priority;
    }

    std::string getName()const
    {
        return _name;
    }

    void setPosition(const sf::Vector2f & pos)
    {
        _position = pos;
        updatePosition();
    }
	void setPosition(float x, float y)
    {
        setPosition({x,y});
    }
	void move(sf::Vector2f deltaS)
    {
        setPosition(_position + deltaS);
    }
	sf::Vector2f getPosition()const
    {
        return _position;
    }
    void setSize(const sf::Vector2f & s)
    {
        _size = s;
        updateSize();
    }
    sf::Vector2f getSize()const
    {
        return _size;
    }

    void setVisible(bool v)
    {
        _visible = v;
    }

    bool isVisible()const
    {
        return _visible;
    }

    bool dectectColision(Entity_t * sec)
    {
        auto sizeU = sec->getSize();
        auto posU = sec->getPosition();
        if(_position.x +_size.x >= posU.x && _position.x < posU.x + sizeU.x  && 
           _position.y + _size.y>= posU.y && _position.y < posU.y + sizeU.y )
           return true;
        return false;
    }

protected:
    virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const = 0;

    std::string _name{};
    Engine * _parent{nullptr};

    bool _toKill{false};
    bool _added{true};
    bool _visible{true};
    int _priority{0};

    virtual void updatePosition() = 0;
	virtual void updateSize() = 0;

    sf::Vector2f _position{0,0};
    sf::Vector2f _size{0,0};
};


template <typename T>
class Entity : public Entity_t
{
public:

    Entity(Engine * parent, const std::string & name, std::shared_ptr<T> gameObj)
        :Entity_t(parent, name)
    {
        _gameObj = gameObj;      
    }

    virtual void update(double dt)
    {
        if(_added)
        {
            _added = false;            
        }
        if constexpr (detail::has_onUpdate<T, void(Engine *)>::value)
        {
            _gameObj->onUpdate(_parent);
        }           
        _update(dt);   
    }

    virtual void start()
    {
        if constexpr (detail::has_onStart<T, void(Engine *)>::value)
             _gameObj->onStart(_parent);
    }

    virtual void input(const sf::Event & evt)
    {
        if constexpr (detail::has_onInput<T, void(Engine *, const sf::Event &)>::value)
        {
            _gameObj->onInput(_parent, evt);
        }
        _input(evt);
    }

    virtual void kill()
    {
        if constexpr (detail::has_onKill<T, void(Engine *)>::value)
            _gameObj->onKill(_parent);
    }

    T * getGameObject()
    {
        return _gameObj.get();
    }

protected:

    virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const
    {
        _draw(target, states);
    }

    virtual void _update(double dt) = 0;
    virtual void _input(const sf::Event & evt) = 0;
    virtual void _draw(sf::RenderTarget & target, sf::RenderStates states)const = 0;

    virtual void updatePosition() = 0;
	virtual void updateSize() = 0;

protected:

    std::shared_ptr<T> _gameObj;
};

}

