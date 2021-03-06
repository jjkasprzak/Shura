#include "Player.h"

void Player::addObstacles(std::vector<Didax::Entity_t*> obs)
{
    obstacles = obs;
}

void Player::onUpdate(Didax::Engine * eng)
{       
    auto dT = eng->getDeltaT();
    
    if(moving)
    {
        float x = 280 * cos((direction-90)*PI/180)*dT;
        float y = 280 * sin((direction-90)*PI/180)*dT;

        auto obst = isCollision();
        if(obst == nullptr)
        {
            me->move({x,y});
        }
        else
        {
            auto vec = obst->getPosition()+obst->getSize()-me->getPosition()-me->getSize();
            if(x* vec.x < 0)
                me->move({x,0});
            if(y* vec.y < 0)
                me->move({0,y});
        }
    }
    if(HP == 0)
    {
        setGhost();
    }      
    if(flittering)
    {
        flitteringTimer -= dT;
        if(flitteringTimer < 0)
        {
            flitteringTimer += flitteringTime;
            flitteringLeft--;
            if(flitteringLeft % 2)
                me->setColor(sf::Color(255,255,255,125)*basicColor);
            else
                me->setColor(basicColor);
            if(flitteringLeft == 0)
                flittering = false;
        }
    }
    if(haveArtifact)
    {
        if(artifactSafe > 0)
            artifactSafe-= dT;
        artifactTimer-=dT;
        artifactSafe-=dT;
        if(artifactSafe <=0)
            circ->setVisible(true);
        else
            circ->setVisible(false);
    }
    else
    {
        if(artifactSafe <= 0)
            artifactSafe = 5.0;
        circ->setVisible(false);
    }
        

    if(hpleft != nullptr)
        hpleft->setPosition(me->getPosition().x, me->getPosition().y - 30);
    if(circ != nullptr)
        circ->setPosition(me->getPosition().x+me->getSize().x/2-200, me->getPosition().y+me->getSize().y/2-200);
}

void Player::onStart(Didax::Engine * eng)
{
    me = eng->getMyEntityAnimable();
    me->setAnimationAsset("mario");
    me->setSize({60,60});
    me->setPriority(20);
    me->play();
    reSpawn();
    if(_id == 0)
        basicColor = sf::Color(125,255,255);
    else if(_id == 1)
        basicColor = sf::Color(125,125,255);
    else if(_id == 2)
        basicColor = sf::Color(125,255,125);
    else
        basicColor = sf::Color(255,255,255);
    me->setColor(basicColor);
    
    hpleft = eng->addEntity<Didax::Text<HPLeftText>>(std::make_shared<HPLeftText>(HP), "Text");   
    circ = eng->addEntity<Didax::Sprite<Circle>>(std::make_shared<Circle>(), "Circle"); 
    circ->setVisible(false);     
}  

void Player::setMoveState(bool mov, float dir)
{
    if(mov == moving && dir == direction)
        return;
    
    moving = mov;
    direction = dir;

    if(moving)
    {
        if(direction > 315 || direction < 45)
            me->setAnimation(6);
        else if(direction >= 45 && direction <= 135)
            me->setAnimation(7);
        else if(direction > 135 && direction < 225 )
            me->setAnimation(4);
        else
            me->setAnimation(5);       
    }
    else
    {
        if(direction > 315 || direction < 45)
            me->setAnimation(2);
        else if(direction >= 45 && direction <= 135)
            me->setAnimation(3);
        else if(direction > 135 && direction < 225 )
            me->setAnimation(0);
        else
            me->setAnimation(1);       
    }
}

bool Player::getIfMooving()const
{
    return moving;
}
float Player::getDirection()const
{
    return direction;
}

void Player::spawnBullets(std::vector<Didax::Sprite<Bullet> *> & bullets)
{
    artifactTimer = 0.4f;

    auto posx = me->getPosition().x + me->getSize().x/2;
    auto posy = me->getPosition().y + me->getSize().y/2;

    float x = 0, y = 0;
    if(moving)
    {
        x = 280 * cos((direction-90)*PI/180);
        y = 280 * sin((direction-90)*PI/180);
    }  

    float v_x[4], v_y[4];

    v_x[0] =400 * cos((bulletAngle)*PI/180)+x;
    v_y[0] =400 * sin((bulletAngle)*PI/180)+y;
    v_x[1] =400 * cos((bulletAngle+90)*PI/180)+x;
    v_y[1] =400 * sin((bulletAngle+90)*PI/180)+y;
    v_x[2] =400 * cos((bulletAngle+180)*PI/180)+x;
    v_y[2] =400 * sin((bulletAngle+180)*PI/180)+y;
    v_x[3] =400 * cos((bulletAngle+270)*PI/180)+x;
    v_y[3] =400 * sin((bulletAngle+270)*PI/180)+y;

    int i = 0;
    int j = 0;
    while(i< 4)
    {
        if(bullets[j]->getGameObject()->speed.x == 0)
        {
            bullets[j]->getGameObject()->setPosition(sf::Vector2f(posx, posy));
            bullets[j]->getGameObject()->speed.x = v_x[i];
            bullets[j]->getGameObject()->speed.y = v_y[i];
            i++;
        }
        j++;
    }

}

float  Player::getArtifactTimmer()const
{
    return artifactTimer;
}

void Player::reSpawn()
{
    if(_id == 0)
        me->setPosition(2150,2000);
    else if(_id == 1)
        me->setPosition(1800,2000);
    else if(_id == 2)
        me->setPosition(2000,2100);
    else if(_id == 3)
        me->setPosition(2000,1800);
    else
        me->setPosition(-1000, -1000);
        
}

void Player::startFilttering()
{
    flittering = true;
    flitteringTimer = flitteringTime;
    flitteringLeft = 6;
}

bool Player::isFlittering()
{
    return flittering;
}

void Player::giveArtifact()
{
    haveArtifact = true;
    artifactTimer = 0.2;
    artifactSafe = 5.0;
    me->setColor(sf::Color(255,125,125)*basicColor);
}

void Player::stealArtifact()
{
    haveArtifact = false;
    artifactTimer = 0.2;
    artifactSafe = 5.0;
    me->setColor(basicColor);
}

float Player::getArtifactSafe()
{
    return artifactSafe;
}

bool Player::isHaveingArtifact()
{
    return haveArtifact;
}

void Player::minusHP()
{
    HP--;
    hpleft->getGameObject()->setHP(HP);
    reSpawn();
    startFilttering();
}

int Player::getHP()
{
    return HP;
}

void Player::setBulletAngle(float a)
{
    bulletAngle = a;
    if(bulletAngle < 0)
        bulletAngle +=360;
    else if(bulletAngle > 360)
        bulletAngle -=360;
}

float Player::getBulletAngle()
{
    return bulletAngle;
}

void Player::setName(const std::string & n)
{
    name = n;
    hpleft->getGameObject()->setName(n);
}
std::string Player::getName()
{
    return name;
}

void Player::setGhost()
{
    me->setColor(sf::Color(255,255,255,125)*basicColor);
    ghost = true;
}
void Player::setNormal()
{
    me->setColor(basicColor);
    ghost = false;
}
bool Player::immune()
{
    return ghost || haveArtifact || flittering;
}

void Player::setHP(int H)
{
    if(H != HP)
    {
        HP = H;
        hpleft->getGameObject()->setHP(HP);
    }
}

void Player::setHavingArtfact()
{
    haveArtifact = true;
    me->setColor(sf::Color(255,125,125)*basicColor);
}