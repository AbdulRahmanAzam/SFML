#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

int main()
{
    RenderWindow window(VideoMode(800, 800), "simple shoot game!", Style::Default);
    window.setFramerateLimit(60);

    bool dir = true;
    CircleShape hoop;
    hoop.setRadius(50.f);
    hoop.setFillColor(Color::Black);
    hoop.setOutlineThickness(2.f);
    hoop.setOutlineColor(Color::Red);
    hoop.setPosition(0.f, 10.f);

    CircleShape ball;
    bool isShot = false;
    ball.setRadius(20.f);
    ball.setPosition(Vector2f(0, window.getSize().y - ball.getRadius()*3));
    ball.setFillColor(Color::Magenta);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

        }

        //update hoop
        if(hoop.getPosition().x == 0)
            dir = true; 
        else if(hoop.getPosition().x + hoop.getRadius()*2 >= window.getSize().x)
            dir = false;

        if(dir)
            hoop.move(5.f,0.f);
        else
            hoop.move(-5.f,0.f);

        //update ball
        if(Mouse::isButtonPressed(Mouse::Left))
            isShot = true;

        if(isShot){
            ball.move(0, -5.f);
        }else
            ball.setPosition(Mouse::getPosition(window).x, ball.getPosition().y);

        //intersection of the ball
        if(ball.getPosition().y < 0 || ball.getGlobalBounds().intersects(hoop.getGlobalBounds())){
            isShot = false;
            ball.setPosition(ball.getPosition().x, window.getSize().y - ball.getRadius()*3);
        }

        window.clear(Color::White);

        // DRAW
        window.draw(hoop);
        window.draw(ball);

        window.display();
    }

    return 0;
}
