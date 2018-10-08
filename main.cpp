#include <iostream>
#include <random>
#include <utility>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <vector>


class Checkmarks {
public:
    std::pair<float,float> position;
    sf::Color MapArea;
};

typedef std::vector< Checkmarks > randomPairs;


float random_generator(float, float);
randomPairs generateRandoms(int, sf::RenderWindow*);
sf::Color getColoroflocation(std::pair<float,float> location, randomPairs* data);
float getDistancetoPk(std::pair<float,float> pixel_location, std::pair<float,float> Pk_location );
void Updatewindow(sf::RenderWindow*, randomPairs *);
void DrawVoronoiImage(sf::RenderWindow*, randomPairs*);
void DrawCircles (float, sf::Color, randomPairs* , sf::RenderWindow*);

void DrawCircles (float alpha, sf::Color circleColor, randomPairs* Points , sf::RenderWindow* window ){
    for (auto i: *Points){
        sf::CircleShape circle;
        circle.setFillColor(circleColor);
        circle.setRadius(4.0);
        circle.setPosition(i.position.first, i.position.second);
        window->draw(circle);
    }
}

randomPairs generateRandoms(int CountRnd, sf::RenderWindow* window){
    sf::Vector2u size = window->getSize();
    std::vector <sf::Color> Colors;
    Colors.push_back(sf::Color(240,100,100));
    Colors.push_back(sf::Color(100,240,100));
    Colors.push_back(sf::Color(100,100,240));
    Colors.push_back(sf::Color(2,200,150));
    Colors.push_back(sf::Color(2,200,255));
    Colors.push_back(sf::Color(100,100,100));
    Colors.push_back(sf::Color(250,250,100));

    randomPairs randomNumbers;
    for (int i=0; i< CountRnd; i++)
    {
        float randomX = random_generator(0.0f, (float) size.x);
        float randomY = random_generator(0.0f, (float) size.y);
        Checkmarks newCheckmark ;
        newCheckmark.position = std::make_pair(randomX, randomY);
        newCheckmark.MapArea = Colors[(int)random_generator(0, (int)Colors.size())];
        randomNumbers.push_back(newCheckmark);
    }
    return randomNumbers;
}

float random_generator(float min, float max){
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(mt);

}

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 400), "Voronoi");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    randomPairs randomNumbers;
    bool shouldUpdate = true;
    int randomCount = 55;

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    shouldUpdate = true;

                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                    randomCount ++;
                    shouldUpdate = true;
                    std::cout << "Random Number Count: "<< randomCount<< std::endl;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    randomCount --;
                    if (randomCount < 10){
                        randomCount = 16;
                    }
                    std::cout << "Random Number Count: "<< randomCount<< std::endl;
                    shouldUpdate = true;
                }
        }

        if (shouldUpdate) {
            randomNumbers = generateRandoms(randomCount, &window);
            std::cout << "Random Numbers Updated" << std::endl;
            Updatewindow(&window, &randomNumbers);
            shouldUpdate = false;
        }




    }
    return 0;
}

void Updatewindow(sf::RenderWindow* window, randomPairs *points){
    window->clear(sf::Color::White);
    DrawVoronoiImage(window, points);
    DrawCircles (1, sf::Color::Red, points, window);
    window->display();
}

void DrawVoronoiImage(sf::RenderWindow* window, randomPairs* points){
    sf::Vector2u size = window->getSize();
    sf::Uint8 *pixels  = new sf::Uint8[size.x * size.y * 4];
    sf::Image image;
    sf::Texture texture;
    sf::Sprite SP;
    for (int x=0; x< size.x; x++)
    {
        for (int y=0; y< size.y; y++)
        {
            sf::Color c = getColoroflocation(std::make_pair(y,x), points);
            pixels[(y  + x* size.y)*4] = c.r;
            pixels[(y + x* size.y)*4 + 1] = c.g;
            pixels[(y  + x* size.y)*4 + 2] = c.b;
            pixels[(y  + x* size.y)*4 + 3] = c.a;
        }
    }
    image.create(size.x, size.y,pixels);
    texture.loadFromImage(image);
    SP.setTexture(texture);
    window->draw(SP);
    delete [] pixels;
}

sf::Color getColoroflocation(std::pair<float,float> location, randomPairs* data){
    std::vector <float> distances ;
    for (auto &i : *data){
        distances.push_back(getDistancetoPk(location, i.position));
    }
    int min_pos = std::distance(distances.begin(),min_element(distances.begin(),distances.end()));
    return (*data)[min_pos].MapArea;
}

float getDistancetoPk(std::pair<float,float> pixel_location, std::pair<float,float> Pk_location )
{
    float first_exp = pixel_location.first - Pk_location.first;
    float second_exp = pixel_location.second - Pk_location.second;
    float root_exp = std::sqrt(first_exp*first_exp + second_exp*second_exp);
    float final_value = root_exp;
    //float final_value = std::abs(first_exp) + std::abs(second_exp);
    return final_value;
}