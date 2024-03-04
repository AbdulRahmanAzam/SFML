#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include  <thread>
#include <chrono>
#include <cstdlib>

using namespace sf;
using namespace std;


int main(){
    srand(time(NULL));
    RenderWindow window(VideoMode(800, 600), "Ball Shooter");
    window.setFramerateLimit(60);

    //BALLS 

    //PLAYER
    CircleShape player;
    player.setFillColor(Color::White);
    player.setRadius(50.f);
    player.setPosition(window.getSize().x /2 - player.getRadius(), window.getSize().y - player.getRadius()*2 - 10.f);

    Vector2f playerCentre;

    int shootTimer = 0;

    //PROJECTILE
    CircleShape projectile;
    projectile.setRadius(5.f);
    projectile.setFillColor(Color::Red);

    vector<CircleShape> projectiles;
    projectiles.push_back(projectile);

    // ENEMY
    RectangleShape enemy;
    enemy.setSize(Vector2f(50.f,50.f));
    enemy.setFillColor(Color::Blue);

    int enemySpawnTimer = 0;
    vector<RectangleShape> enemies;
    enemies.push_back(enemy);

    while(window.isOpen()){
        Event event;
        while(window.pollEvent(event)){
            if(event.type == Event::Closed)
                window.close();
        }

        
    window.clear();
    //UPDATE
    // making small balls to get throw player
    playerCentre = Vector2f(player.getPosition().x + player.getRadius(), player.getPosition().y + player.getRadius());
    //delayinig time
    if(shootTimer < 5)
        shootTimer++;

    // setting position according to mouse
    player.setPosition(Mouse::getPosition(window).x, player.getPosition().y);

    // throw balls on click
    if(Mouse::isButtonPressed(Mouse::Left) && shootTimer >= 5){
        projectile.setPosition(playerCentre);
        projectiles.push_back(projectile);
        shootTimer = 0;
    }

    for(int i=0; i< projectiles.size() ;i++){
        projectiles[i].move(0.f, -10.f);

        if(projectiles[0].getPosition().y <= 0 )
            projectiles.erase(projectiles.begin() + i);
    }

    // enemy 
    if(enemySpawnTimer < 20)
        enemySpawnTimer++;

    if(enemySpawnTimer >= 20){
        enemy.setPosition((rand() % int(window.getPosition().x - enemy.getPosition().x)), 0.f);
        enemies.push_back(enemy);

        enemySpawnTimer = 0;
    }

    for(int i=0; i< enemies.size(); i++){
        enemies[i].move(0.f, 5.f);

        if(enemies[i].getPosition().y > window.getSize().y){
            enemies.erase(enemies.begin() + i);
        }
    }

    // COLLISION
    for(int i=0; i< projectiles.size(); i++){
        for(int j =0; j < enemies.size(); j++){
            if(projectiles[i].getGlobalBounds().intersects(enemies[j].getGlobalBounds())){
                projectiles.erase(projectiles.begin() + i);
                enemies.erase(enemies.begin() + j);
                break;
            }
        }
    }

    bool isLoose = false;
    for(int i=0; i< enemies.size(); i++){
        if(player.getGlobalBounds().intersects(enemies[i].getGlobalBounds())){
            isLoose = true;
            break;
        }
    }
    //DRAW
    window.draw(player);

    for(int i=0; i< enemies.size(); i++){
        window.draw(enemies[i]);
    }

    for(int i=0; i< projectiles.size(); i++){
        window.draw(projectiles[i]);
    }

    window.display();

    if(isLoose){
        std::this_thread::sleep_for(2s);
        break;
    }
        
        
    }
}
