#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include <string>

using namespace sf;
// VECTOR MATH
// LENGTH OF VECTOR
// |V| = sqrt(pow(2, Vx)  + pow(2, Vy));
//NORMALIZE
// U = V / |V| = (Vx, Vy) / sqrt(pow(2, Vx)  + pow(2, Vy));


class Bullet{
public:
    Sprite shape;

    Bullet(Texture *texture, float posx, float posy){
        this->shape.setTexture(*texture);
        this->shape.setScale(0.07f,0.07f); 
        this->shape.setRotation(45);
        this->shape.setPosition(posx, posy + 37.f);
    }
};

class Player{
public:
    Sprite shape;
    Texture *texture;
    int HP;
    int HPMAX;

    std::vector <Bullet> bullets;

    Player(Texture *texture){
        this->HPMAX = 10;
        this->HP = this->HPMAX;

        this->texture = texture;
        this->shape.setTexture(*texture);

        this->shape.setScale(0.07f, 0.07f);
        this->shape.setRotation(90);
        this->shape.setPosition(400, 300);
    }

    ~Player(){

    }
};

class Enemy{
public:
    Sprite shape;
    int HP;
    int HPMAX;

    Enemy(Texture *texture, Vector2u windowSize){
        this->HPMAX = rand() % 3 + 1;
        this->HP = this-> HPMAX;
        this->shape.setTexture(*texture);

        this->shape.setScale(0.2f, 0.2f);
        this->shape.setPosition(windowSize.x - this->shape.getGlobalBounds().width, rand() % int(windowSize.y - this->shape.getGlobalBounds().height));
    }
    ~Enemy(){

    }
};

// class HPtext{
// public:
//     Text hp;
    
//     HPtext(int size, Color c){
//         Font font;
//         if(!font.loadFromFile("C:..\\Font and Texture\\Balleny.ttf"))
//         throw "font could not open";

//         this->hp.setFont(font);
//         hp.setCharacterSize(size);
//         hp.setFillColor(c);
//     }
// };

int main()
{
    srand(time(NULL));
    RenderWindow window(VideoMode(1366, 768), "Shooter 360!", Style::Default);
    window.setFramerateLimit(60);
    

    //init Textures
    Font font;
    if(!font.loadFromFile("C:..\\Font and Texture\\Balleny.ttf"))
    throw "font could not open";

    Texture playerTex;
    playerTex.loadFromFile("..\\Font and Texture\\ship.png");

    Texture enemyTex;
    enemyTex.loadFromFile("..\\Font and Texture\\enemy.png");
    
    Texture bulletTex;
    bulletTex.loadFromFile("..\\Font and Texture\\missile.png");

    Text scoreT;
    scoreT.setFont(font);
    scoreT.setCharacterSize(40);
    scoreT.setFillColor(Color::White);
    scoreT.setPosition(window.getSize().x - 110.f, 10.f);

    
    Text end;
    end.setFont(font);
    end.setCharacterSize(100);
    end.setFillColor(Color::Red);
    end.setPosition(window.getSize().x/2 - 200 , window.getSize().y/2 - 100);
    end.setString("GAME OVER!");

    // init player
    int score = 0;
    Player player(&playerTex);
    int shootTimer = 20;
    Text hp1;
    hp1.setFont(font);
    hp1.setCharacterSize(20);
    hp1.setFillColor(Color::White);

    // init ENEMY
    std::vector<Enemy> enemies;
    int enemyTimer = 20;
    Text ehp1;
    ehp1.setFont(font);
    ehp1.setCharacterSize(20);
    ehp1.setFillColor(Color::White);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                window.close();

        }
        if(player.HP > 0){

        // PLayer
        if(Keyboard::isKeyPressed(Keyboard::W))
            player.shape.move(0.f, -10.f);
        if(Keyboard::isKeyPressed(Keyboard::S))
            player.shape.move(0.f, 10.f);
        if(Keyboard::isKeyPressed(Keyboard::D))
            player.shape.move(10.f, 0.f);
        if(Keyboard::isKeyPressed(Keyboard::A))
            player.shape.move(-10.f, 0.f);

        // Collision with window
        if(player.shape.getPosition().x <= player.shape.getGlobalBounds().width) // LEFT
            player.shape.setPosition(player.shape.getGlobalBounds().width, player.shape.getPosition().y);

        if(player.shape.getPosition().x >= window.getSize().x) // RIGHT
            player.shape.setPosition(window.getSize().x, player.shape.getPosition().y);

        if(player.shape.getPosition().y <= 0 ) // TOP
            player.shape.setPosition(player.shape.getPosition().x, 0);

        if(player.shape.getPosition().y >= window.getSize().y - player.shape.getGlobalBounds().height) // BOTTOM
            player.shape.setPosition(player.shape.getPosition().x, window.getSize().y - player.shape.getGlobalBounds().height);


        // UPDATE Controls
        hp1.setPosition(player.shape.getPosition().x - hp1.getGlobalBounds().width, player.shape.getPosition().y );
        hp1.setString(std::to_string(player.HP) + "/" + std::to_string(player.HPMAX));

        scoreT.setString("Score:" + std::to_string(score));

        // BULLETS
        // Shooting
        if(shootTimer < 7)
            shootTimer++;
        else if(Mouse::isButtonPressed(Mouse::Left) && shootTimer >= 7){
            shootTimer = 0;
            player.bullets.push_back(Bullet(&bulletTex, player.shape.getPosition().x, player.shape.getPosition().y));
        }
        for(int i =0; i<player.bullets.size(); i++){
            // Move
            player.bullets[i].shape.move(10.f, 0.f);

            // Out of window Bound
            if(player.bullets[i].shape.getPosition().x > window.getSize().x)
                player.bullets.erase(player.bullets.begin() + i);

            // Collision
            for(int k=0; k < enemies.size();k++){
                if(player.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds())){

                    if(enemies[k].HP <= 1){
                        score += enemies[k].HPMAX;
                        enemies.erase(enemies.begin() + k); 
                    }
                    else
                        enemies[k].HP--;

                    player.bullets.erase(player.bullets.begin() + i);
                    break;
                }
            }
        }

        // ENEMIES
        if(enemyTimer >= 20){
            enemyTimer = 0;
            enemies.push_back(Enemy(&enemyTex, window.getSize()));
        }else{
            enemyTimer++;
        }


        for(int i =0; i< enemies.size(); i++){
            // MOVE
            enemies[i].shape.move(-7.f, 0.f);
            // Out of Window Bound
            if(enemies[i].shape.getPosition().x <= 0){
                enemies.erase(enemies.begin() + i);
                break;
            }
            // INTERSECTS
            if(enemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds())){
                enemies.erase(enemies.begin() + i);
                player.HP--;
                break;
            }
        }
        }
        // DRAW
        window.clear();

        window.draw(player.shape);

        for(int i=0; i< player.bullets.size(); i++){
            window.draw(player.bullets[i].shape);
        }

        for(int i=0; i< enemies.size(); i++){
            ehp1.setString(std::to_string(enemies[i].HP) + "/" + std::to_string(enemies[i].HPMAX));
            ehp1.setPosition(enemies[i].shape.getPosition().x, enemies[i].shape.getPosition().y - ehp1.getGlobalBounds().height);
            window.draw(ehp1);
            window.draw(enemies[i].shape);
        }

        window.draw(scoreT);
        window.draw(hp1);

        if(player.HP <=0){
            window.draw(end);
        }
        window.display();
    }

    return 0;
}
