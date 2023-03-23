//Uwaga! Co najmniej C++17!!!
//Project-> ... Properties->Configuration Properties->General->C++ Language Standard == ISO C++ 17 Standard (/std:c++17)

#include "SFML/Graphics.hpp"
#include "ShareTechMono-Regular.h"
#include <numeric>
#include <iostream>

//Tak – dobrze państwo widzą – TO jest zmienna globalna! Czytanie i przetwarzanie fontów w SFML jest bardzo kosztowne. Dlatego zrobimy to raz. 
//Co nie zmienia faktu, że można by to zrobić bez zmiennej globalnej i to całkiem ładnie. Jak? To już dla Państwa :-)
std::shared_ptr<sf::Font> font;
//sf::Text text;
float brightness = 1.0f;

class hexagon : public sf::Drawable
{
protected:
    sf::Vector2f left_top, right_bottom, center, p_1c, p_50;
    sf::Vector2f p[6]; // Kolejność punktów opisana w pliku PDF do laboratorium.
public:
    void Set_Borders(sf::Vector2f _left_top, sf::Vector2f _right_bottom);
    void Draw_Border(sf::RenderTarget& target, sf::RenderStates states, sf::String name) const;
    bool rhombus(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p, float& alpha, float& beta) const;
private:
    float d_coefficient(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p) const;
};

//Trochę matematyki jeszcze nikomu nie zaszkodziło. Więc dwie kolejne metody to czysta matematyka.
float hexagon::d_coefficient(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p) const
{
    float A = p0.y - p1.y;
    float B = p1.x - p0.x;
    float C = p0.x * (p1.y - p0.y) - p0.y * (p1.x - p0.x);

    return 2 * ((A * p.x + B * p.y + C) / sqrt(A * A + B * B)) / sqrt(3.0f);
}

bool hexagon::rhombus(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p, float& alpha, float& beta) const
{
    float L = sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
    sf::Vector2f p3(0.5f * (p0.x - p1.x) + 0.5f * sqrt(3.0f) * (p1.y - p0.y) + p0.x, 0.5f * sqrt(3.0f) * (p0.x - p1.x) - 0.5f * (p1.y - p0.y) + p0.y);

    beta = -d_coefficient(p0, p1, p) / L;
    alpha = d_coefficient(p0, p3, p) / L;

    if ((alpha > 1.0f) || (beta > 1.0f) || (alpha < 0.0f) || (beta < 0.0f)) return false; else return true;
}

void hexagon::Set_Borders(sf::Vector2f _left_top, sf::Vector2f _right_bottom)
{
    left_top = _left_top;
    right_bottom = _right_bottom;
    float a = ((right_bottom.y - left_top.y) / 2.0f + 0.5f) - 20.0f;
    center = left_top + sf::Vector2f((right_bottom.x - left_top.x) / 2.0f + 0.5f, (right_bottom.y - left_top.y) / 2.0f + 0.5f + 10);
    p_1c.x = (p[1].x + center.x) / 2;
    p_1c.y = (p[1].y + center.y) / 2;
    p_50.x = (p[0].x + p[5].x) / 2;
    p_50.y = (p[0].y + p[5].y) / 2;

    p[0] = center - sf::Vector2f(0.0f, a);
    p[1] = center - sf::Vector2f(0.5f * sqrt(3.0f) * a, 0.5f * a);
    p[2] = center - sf::Vector2f(0.5f * sqrt(3.0f) * a, -0.5f * a);
    p[3] = center + sf::Vector2f(0.0f, a);
    p[4] = center + sf::Vector2f(0.5f * sqrt(3.0f) * a, 0.5f * a);
    p[5] = center + sf::Vector2f(0.5f * sqrt(3.0f) * a, -0.5f * a);
}

void hexagon::Draw_Border(sf::RenderTarget& target, sf::RenderStates states, sf::String name) const
{
    // Tu trzeba narysować ramkę. I napisy.

    float a = ((right_bottom.y - left_top.y) / 2.0f + 0.5f) - 20.0f;
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(2 * a, 2 * a));
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(1.0f);
    rectangle.setPosition(sf::Vector2f(p[1].x - 15, p[0].y));

    target.draw(rectangle);


    //sf::Text text;
    //text.setFont(*font);
    //text.setCharacterSize(10);
    //text.setFillColor(sf::Color::Red);
    //text.setPosition(center);
    //text.setString(name);
    //target.draw(text);

}

class hexagon_RGB : public hexagon
{
public:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

void hexagon_RGB::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::VertexArray quad(sf::Quads, 12);


    quad[0].position = p[0]; quad[0].color = sf::Color(255*brightness, 0, 0);
    quad[1].position = p[1]; quad[1].color = sf::Color(255 * brightness, 255 * brightness, 0);
    quad[2].position = center; quad[2].color = sf::Color(255 * brightness, 255 * brightness, 255 * brightness);
    quad[3].position = p[5]; quad[3].color = sf::Color(255 * brightness, 0, 255 * brightness);

    quad[4].position = p[1]; quad[4].color = sf::Color(255 * brightness, 255 * brightness, 0);
    quad[5].position = p[2]; quad[5].color = sf::Color(0, 255 * brightness, 0);
    quad[6].position = p[3]; quad[6].color = sf::Color(0, 255 * brightness, 255 * brightness);
    quad[7].position = center; quad[7].color = sf::Color(255 * brightness, 255 * brightness, 255 * brightness);

    quad[8].position = p[3]; quad[8].color = sf::Color(0, 255 * brightness, 255 * brightness);
    quad[9].position = p[4]; quad[9].color = sf::Color(0, 0, 255 * brightness);
    quad[10].position = p[5]; quad[10].color = sf::Color(255 * brightness, 0, 255 * brightness);
    quad[11].position = center; quad[11].color = sf::Color(255 * brightness, 255 * brightness, 255 * brightness);

    target.draw(quad);
    Draw_Border(target, states, "RGB");

}

class hexagon_CMY : public hexagon
{
public:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

void hexagon_CMY::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //Tu trzeba narysować sześciokąt CMY.
    sf::VertexArray line(sf::LinesStrip, 2);
    for (int i = 0; i < 5; i++)
    {

        line[0].position = p[i];     line[0].color = sf::Color(0, 0, 0);
        line[1].position = p[i + 1]; line[1].color = sf::Color(0, 0, 0);
        target.draw(line);
    }
    line[0].position = p[5];     line[0].color = sf::Color(0, 0, 0);
    line[1].position = p[0]; line[1].color = sf::Color(0, 0, 0);
    target.draw(line);


    sf::VertexArray quad(sf::Quads, 12);


    quad[0].position = p[0]; quad[0].color = sf::Color(0, 255, 255,255 * brightness);
    quad[1].position = p[1]; quad[1].color = sf::Color(0, 0, 255,255 * brightness);
    quad[2].position = center; quad[2].color = sf::Color(0, 0, 0,255 * brightness);
    quad[3].position = p[5]; quad[3].color = sf::Color(0, 255, 0,255 * brightness);

    quad[4].position = p[1]; quad[4].color = sf::Color(0, 0, 255, 255 * brightness);
    quad[5].position = p[2]; quad[5].color = sf::Color(255, 0, 255, 255 * brightness);
    quad[6].position = p[3]; quad[6].color = sf::Color(255, 0, 0, 255 * brightness);
    quad[7].position = center; quad[7].color = sf::Color(0, 0, 0, 255 * brightness);

    quad[8].position = p[3]; quad[8].color = sf::Color(255, 0, 0, 255 * brightness);
    quad[9].position = p[4]; quad[9].color = sf::Color(255, 255, 0, 255 * brightness);
    quad[10].position = p[5]; quad[10].color = sf::Color(0, 255, 0, 255 * brightness);
    quad[11].position = center; quad[11].color = sf::Color(0, 0, 0, 255 * brightness);

    target.draw(quad);
    Draw_Border(target, states, "CMY");
}

class hexagon_HSL : public hexagon
{
public:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
float GetHue(float x1, float x2, float xH) {
    if (xH < 0) 
    {
        xH += 1;
    }
    if (xH > 1) 
    {
        xH -= 1;
    }
    if (6 * xH < 1) 
    {
        return x1 + (x2 - x1) * 6 * xH;
    }
    if (2 * xH < 1) 
    {
        return x2;
    }
    if (3 * xH < 2) 
    {
        return x1 + (x2 - x1) * (2.0 / 3.0 - xH) * 6;
    }
    return x1;
}
sf::Color HSLToRGB(float H, float S, float L) {
    if (S == 0) 
    {
        return sf::Color(L * 255, L * 255, L * 255);
    }
    else 
    {
        float Q, P;
        if (L < 0.5) 
        {
            P = L * (1 + S);
        }
        else 
        {
            P = (L + S) - (S * L);
        }
        Q = 2 * L - P;
        int R = 255 * GetHue(Q, P, H + (1.0 / 3.0));
        int G = 255 * GetHue(Q, P, H);
        int B = 255 * GetHue(Q, P, H - (1.0 / 3.0));
        
        return sf::Color(R, G, B);
    }
}

void hexagon_HSL::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //Tu trzeba narysować sześciokąt HSL.
    sf::VertexArray line(sf::LinesStrip, 2);
    for (int i = 0; i < 5; i++)
    {

        line[0].position = p[i];     line[0].color = sf::Color(0, 0, 0);
        line[1].position = p[i + 1]; line[1].color = sf::Color(0, 0, 0);
        target.draw(line);
    }
    line[0].position = p[5];     line[0].color = sf::Color(0, 0, 0);
    line[1].position = p[0]; line[1].color = sf::Color(0, 0, 0);
    target.draw(line);


    int frameSize = p[5].x - p[1].x+70;
    int lengthOfEdge = sqrt((p[1].x - p[0].x) * (p[1].x - p[0].x) + (p[1].y - p[0].y) * (p[1].y - p[0].y));
    float a = 0.0;
    float b = 0.0;
    float H;
    float S;
    float L;
    int pointCount = 0;
    sf::VertexArray point(sf::Points, frameSize * frameSize);
    for (int i = 0; i < frameSize; i++)
    {
        
        for (int j = 0; j < frameSize; j++)
        {
            
            
            float x = p[1].x + i;
            float y = p[0].y + j;
            if (rhombus(p[0], p[1], sf::Vector2f(x, y), a, b))
            {
                H = 0;
                S = a;
                L = b;
                point[pointCount].position = sf::Vector2f(x, y);
                point[pointCount].color = HSLToRGB(H, S, L * brightness);
           
            }
            else if (rhombus(p[2], p[3], sf::Vector2f(x, y), a, b))
            {
                H = b;
                S = 1;
                L = a;
                point[pointCount].position = sf::Vector2f(x, y);
                point[pointCount].color = HSLToRGB(H, S, L * brightness);
                
            }
            else if (rhombus(center, p[3], sf::Vector2f(x, y), a, b))
            {
                H = 1 - a;
                S = 1 - b;
                L = 1;
                point[pointCount].position = sf::Vector2f(x, y);
                point[pointCount].color = HSLToRGB(H, S, L * brightness);
               
            }
            pointCount++;
            
        }
    }
    target.draw(point);    
    Draw_Border(target, states, "HSL");
}

class hexagon_HSB : public hexagon
{
public:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
sf::Color HSBToRGB(float H, float S, float V) {
    if (S == 0) 
    {
        return sf::Color(V * 255, V * 255, V * 255);
    }
    else 
    {
        float tmpH = H * 6;
        if (tmpH == 6) 
        {
            tmpH = 0;
        }
        int H2 = tmpH;
        float x1 = V * (1 - S);
        float x2 = V * (1 - S * (tmpH - H2));
        float x3 = V * (1 - S * (1 - (tmpH - H2)));
        float R, G, B;
        if (H2 == 0) 
        {
            R = V;
            G = x3;
            B = x1;
        }
        else if (H2 == 1) 
        {
            R = x2;
            G = V;
            B = x1;
        }
        else if (H2 == 2) 
        {
            R = x1;
            G = V;
            B = x3;
        }
        else if (H2 == 3) 
        {
            R = x1;
            G = x2;
            B = V;
        }
        else if (H2 == 4) 
        {
            R = x3;
            G = x1;
            B = V;
        }
        else 
        {
            R = V;
            G = x1;
            B = x2;
        }
        return sf::Color(R * 255, G * 255, B * 255);
    }
}
void hexagon_HSB::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //Tu trzeba narysować sześciokąt HSB.
    sf::VertexArray line(sf::LinesStrip, 2);
    for (int i = 0; i < 5; i++)
    {

        line[0].position = p[i];     line[0].color = sf::Color(0, 0, 0);
        line[1].position = p[i + 1]; line[1].color = sf::Color(0, 0, 0);
        target.draw(line);
    }
    line[0].position = p[5];     line[0].color = sf::Color(0, 0, 0);
    line[1].position = p[0]; line[1].color = sf::Color(0, 0, 0);
    target.draw(line);


    int frameSize = p[5].x - p[1].x + 70;
    float lengthOfEdge;
    float a = 0.0;
    float b = 0.0;
    float H;
    float S;
    float V;
    int pointCount = 0;
    sf::VertexArray point(sf::Points, frameSize*frameSize);
    for (int i = 0; i < frameSize; i++)
    {
       
        for (int j = 0; j < frameSize; j++)
        {
            lengthOfEdge = sqrt((p[1].x - p[0].x) * (p[1].x - p[0].x) + (p[1].y - p[0].y) * (p[1].y - p[0].y));
            float x = p[1].x + i;
            float y = p[0].y + j;
            if (rhombus(p[0], p[1], sf::Vector2f(x, y), a, b))
            {
                H = 0;
                S = a;
                V = b;
                point[pointCount].position = sf::Vector2f(x, y);
                point[pointCount].color = HSBToRGB(H, S, V * brightness);
                
            }
            else if (rhombus(p[2], p[3], sf::Vector2f(x, y), a, b))
            {
                H = b;
                S = 1;
                V = a;
                point[pointCount].position = sf::Vector2f(x, y);
                point[pointCount].color = HSBToRGB(H, S, V * brightness);
            }
            else if (rhombus(center, p[3], sf::Vector2f(x, y), a, b))
            {
                H = 1 - a;
                S = 1 - b;
                V = 1;
                point[pointCount].position = sf::Vector2f(x, y);
                point[pointCount].color = HSBToRGB(H, S, V * brightness);
            }
            pointCount++;
        }
        
    }
    target.draw(point);
    
    Draw_Border(target, states, "HSB");
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "GFK Lab 02", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

    hexagon_RGB h_RGB;
    hexagon_CMY h_CMY;
    hexagon_HSL h_HSL;
    hexagon_HSB h_HSB;

    sf::Clock frame_clock, around_half_secound_clock;
    sf::Int64 mean_frames_time = 0;
    std::vector<sf::Int64> frame_times;
    sf::Text text;
    sf::Text text3;
    font = std::make_shared<sf::Font>();
    font->loadFromMemory(&(ShareTechMono_Regular_ttf[0]), ShareTechMono_Regular_ttf.size());
    text.setFont(*font);
    text.setCharacterSize(21);
    text.setFillColor(sf::Color::Black);

    //Nie mogę uniknąć tych "magic numbers" bo bym zdradził w ten sposób co ma być w sf::Event::Resize. A tego byśmy nie chcieli - prawda? :-D
    text.setPosition(window.getSize().x-140, window.getSize().y-60);
    h_RGB.Set_Borders(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(300.0f, 294.0f));
    h_CMY.Set_Borders(sf::Vector2f(300.0f, 0.0f), sf::Vector2f(600.0f, 294.0f));
    h_HSL.Set_Borders(sf::Vector2f(0.0f, 294.0f), sf::Vector2f(300.0f, 588.0f));
    h_HSB.Set_Borders(sf::Vector2f(300.0f, 294.0f), sf::Vector2f(600.0f, 588.0f));

    around_half_secound_clock.restart();


    sf::VertexArray slider(sf::Quads, 4);
    slider[0].position = sf::Vector2f(window.getSize().x - 120, 70);
    slider[1].position = sf::Vector2f(window.getSize().x - 70, 70);

    slider[2].position = sf::Vector2f(window.getSize().x - 70, window.getSize().y - 120);
    slider[3].position = sf::Vector2f(window.getSize().x - 120, window.getSize().y - 120);

    slider[0].color = sf::Color::Black;
    slider[1].color = sf::Color::Black;

    slider[2].color = sf::Color::White;
    slider[3].color = sf::Color::White;


    sf::VertexArray triangles(sf::Triangles, 3 * 2);
    triangles[0].position = sf::Vector2f(window.getSize().x - 120, window.getSize().y - 120);
    triangles[1].position = sf::Vector2f(window.getSize().x - 140, window.getSize().y - 130);
    triangles[2].position = sf::Vector2f(window.getSize().x - 140, window.getSize().y - 110);

    triangles[3].position = sf::Vector2f(window.getSize().x - 70, window.getSize().y - 120);
    triangles[4].position = sf::Vector2f(window.getSize().x - 50, window.getSize().y - 130);
    triangles[5].position = sf::Vector2f(window.getSize().x - 50, window.getSize().y - 110);

    for (int i = 0; i < 6; i++)
    {
        triangles[i].color = sf::Color::Black;
    }


    while (window.isOpen())
    {
        sf::Event event;
        window.clear(sf::Color::White);

        frame_clock.restart(); // Start pomiaru czasu.

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            // Tu trzeba obsłużyć zdarzenia: zmianę rozmiaru okna oraz klikanie w „suwaczek”. 
            else if (event.type == sf::Event::Resized)
            {
                float scale = triangles[0].position.y / (slider[3].position.y - slider[0].position.y);

                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));

                h_RGB.Set_Borders(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(event.size.width / 2 - 100, event.size.height / 2));
                h_CMY.Set_Borders(sf::Vector2f(event.size.width / 2 - 100, 0.0f), sf::Vector2f(event.size.width - 200, event.size.height / 2));
                h_HSL.Set_Borders(sf::Vector2f(0.0f, event.size.height / 2), sf::Vector2f(event.size.width / 2 - 100, event.size.height - 10));
                h_HSB.Set_Borders(sf::Vector2f(event.size.width / 2 - 100, event.size.height / 2), sf::Vector2f(event.size.width - 200, event.size.height - 10));

                text.setPosition(sf::Vector2f(event.size.width - 120, event.size.height - 50));

                slider[0].position = sf::Vector2f(event.size.width - 120, 70);
                slider[1].position = sf::Vector2f(event.size.width - 70, 70);
                slider[2].position = sf::Vector2f(event.size.width - 70, event.size.height - 120);
                slider[3].position = sf::Vector2f(event.size.width - 120, event.size.height - 120);


                sf::Vector2f tmp = triangles[0].position;
                scale = scale * (slider[3].position.y - slider[0].position.y);

                triangles[0].position = sf::Vector2f(event.size.width - 120, scale);
                triangles[1].position = sf::Vector2f(event.size.width - 140, scale + 10);
                triangles[2].position = sf::Vector2f(event.size.width - 140, scale - 10);

                triangles[3].position = sf::Vector2f(event.size.width - 70, scale);
                triangles[4].position = sf::Vector2f(event.size.width - 50, scale + 10);
                triangles[5].position = sf::Vector2f(event.size.width - 50, scale - 10);

            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if ((event.mouseButton.x > slider[0].position.x && event.mouseButton.x < slider[1].position.x) && (event.mouseButton.y > slider[0].position.y && event.mouseButton.y < slider[3].position.y))
                {
                    brightness = (event.mouseButton.y - slider[0].position.y) / (slider[3].position.y - slider[0].position.y);

                    triangles[0].position = sf::Vector2f(window.getSize().x - 120, event.mouseButton.y);
                    triangles[1].position = sf::Vector2f(window.getSize().x - 140, event.mouseButton.y + 10);
                    triangles[2].position = sf::Vector2f(window.getSize().x - 140, event.mouseButton.y - 10);

                    triangles[3].position = sf::Vector2f(window.getSize().x - 70, event.mouseButton.y);
                    triangles[4].position = sf::Vector2f(window.getSize().x - 50, event.mouseButton.y + 10);
                    triangles[5].position = sf::Vector2f(window.getSize().x - 50, event.mouseButton.y - 10);


                    window.clear(sf::Color::White);
                    window.draw(slider);
                    window.draw(h_RGB);
                    window.draw(h_CMY);
                    window.draw(h_HSL);
                    window.draw(h_HSB);
                    text.setString(std::to_string(mean_frames_time) + "us");
                    window.draw(text);

                    window.display();
                }
                
            }
        }


        window.clear(sf::Color::White);
        window.draw(h_RGB);
        window.draw(h_CMY);
        window.draw(h_HSL);
        window.draw(h_HSB);
        // Pewnie tu gdzieś wypadało by dorysować jeszcze suwaczek... 


        window.draw(slider);


        text.setString(std::to_string(mean_frames_time) + "us");
        window.draw(text);
        window.draw(triangles);
        // Pomiar czasu uśrednimy w okresie około 1/2 sekundy.
        frame_times.push_back(frame_clock.getElapsedTime().asMicroseconds());
        if (around_half_secound_clock.getElapsedTime().asSeconds() >= 0.5f && frame_times.size() >= 1)
        {
            mean_frames_time = (sf::Int64)((float)std::reduce(frame_times.begin(), frame_times.end()) / (float)frame_times.size());
            frame_times.clear();
            around_half_secound_clock.restart();
        }

        frame_clock.restart(); // Stop pomiaru czasu.
        window.display();
    }

    //Hmmm ... :-/
    font.reset();

    return 0;
}