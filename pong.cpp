#include<iostream>
#include<sfml/Graphics.hpp>

class ball
{
	sf::CircleShape* circle;
	float rad;
public:
	sf::Vector2f velocity;
	int posx, posy;
	ball(sf::RenderWindow& win)
	{
		this->velocity.x = 2;
		this->velocity.y = 4;
		this->posx = win.getSize().x / 2;
		this->posy = win.getSize().y / 2;
		this->rad = 20;
	}
	void move()
	{
		this->posx += this->velocity.x;
		this->posy += this->velocity.y;
	}
	void draw(sf::RenderWindow& win)
	{
		this->circle = new sf::CircleShape(this->rad);
		this->circle->setPosition(this->posx, this->posy);
		this->circle->setFillColor(sf::Color::White);
		win.draw(*this->circle);
	}
	sf::CircleShape getshape()
	{
		return *this->circle;
	}
};
float clip(float n, float lower, float upper)
{
	n = (n > lower) * n + !(n > lower) * lower;
	return (n < upper) * n + !(n < upper) * upper;
}
bool collision(sf::RectangleShape r, sf::CircleShape c)
{
	float closex = clip(c.getPosition().x, r.getPosition().x, r.getPosition().x + r.getSize().x);
	float closey = clip(c.getPosition().y, r.getPosition().y, r.getPosition().y + r.getSize().y);
	float distx = c.getPosition().x - closex;
	float disty = c.getPosition().y - closey;
	int distsq = (distx) * (distx)+(disty) * (disty);
	if (distsq < c.getRadius() * c.getRadius() && closey == c.getPosition().y)
	{
		return 1;
	}
	else if (distsq < c.getRadius() * c.getRadius() && closex != c.getPosition().x && closey != c.getPosition().y)
	{
		return 1;
	}
	else if (distsq < c.getRadius() * c.getRadius() && closex == c.getPosition().x)
	{
		return 1;
	}
}

class racket
{
public:
	sf::RectangleShape* pad;
	int posx, posy;
	void setpos(sf::Vector2f pos)
	{
		this->pad->setPosition(pos);
	}
	racket()
	{
		this->pad = new sf::RectangleShape(sf::Vector2f(150, 20));
		this->pad->setFillColor(sf::Color::White);
		this->pad->setPosition(this->posx, this->posy);
	}
	void draw(sf::RenderWindow& win)
	{
		win.draw(*this->pad);
	}
};
void autobot(racket up, ball* b, sf::CircleShape c, sf::RectangleShape r, sf::RenderWindow& win)
{
	r.setPosition(c.getPosition().x, r.getPosition().y);
	win.draw(r);
	if (collision(r, c) == 1)b->velocity.y = -b->velocity.y;
}
int main()
{
	bool gamestart = false;
	bool playauto = false;
	bool openmenu = true;
	sf::RenderWindow win(sf::VideoMode(1920, 1080), "Pong", sf::Style::Close);
	sf::Event e;
	win.setFramerateLimit(120);
	// score
	int score1 = 0, score2 = 0;
	sf::Text s1, s2, s3; sf::Font f; f.loadFromFile("font1.ttf");
	s1.setPosition(win.getSize().x / 2 - 400.f, win.getSize().y / 2 - 100.f);
	s1.setCharacterSize(80);
	s1.setFillColor(sf::Color::Cyan);
	s1.setFont(f);
	s1.setString(std::to_string(score1));
	s2.setPosition(win.getSize().x / 2 + 400.f, win.getSize().y / 2 - 100.f);
	s2.setCharacterSize(80);
	s2.setFillColor(sf::Color::Cyan);
	s2.setFont(f);
	s2.setString(std::to_string(score2));
	// ball
	ball* circle = new ball(win);
	// down racket
	racket* down = new racket;
	racket* up = new racket;
	sf::Vector2f pos;
	pos.x = win.getSize().x / 2 - 100.f;
	pos.y = win.getSize().y - 100.f;
	down->setpos(pos);
	pos.x = win.getSize().x / 2 - 100.f;
	pos.y = 100.f;
	up->setpos(pos);
	// background
	sf::Sprite bg; sf::Texture background; background.loadFromFile("b.jpg");
	bg.setTexture(background);
	while (win.isOpen())
	{
		while (win.pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed:
				win.close();
			}
			if (e.type == sf::Event::MouseButtonPressed)
			{
				if (e.mouseButton.button == sf::Mouse::Left)
				{
					int posx = (int)e.mouseButton.x;
					int posy = (int)e.mouseButton.y;
					if (posx >= 594 && posx <= 1346 && posy >= 460 && posy <= 520)
					{
						openmenu = false;
						gamestart = true;
					}
					if (posx >= 564 && posx <= 1430 && posy >= 621 && posy <= 669)
					{
						openmenu = false;
						playauto = true;
						gamestart = true;
					}
				}
			}
		}
		if (gamestart)
		{
			if (circle->posy <= 0)
			{
				if (circle->velocity.y > 0)
					circle->velocity.y = 2;
				else circle->velocity.y = -2;
				score1++;
				s1.setString(std::to_string(score1));
				circle->velocity.y *= -1;
				srand(time(NULL));
			}
			if (circle->posy >= win.getSize().y)
			{
				if (circle->velocity.y > 0)
					circle->velocity.y = 2;
				else circle->velocity.y = -2;
				if (circle->velocity.x > 0)
					circle->velocity.x = 2;
				else circle->velocity.x = -2;
				score2++;
				s2.setString(std::to_string(score2));
				circle->velocity.y *= -1;
			}
			if (circle->posx >= win.getSize().x)
			{
				if (circle->velocity.x > 0)
					circle->velocity.x = 2;
				else circle->velocity.x = -2;
				circle->velocity.x *= -1;
			}
			if (circle->posx <= 0)
			{
				circle->velocity.x *= -1;
				circle->velocity.x += rand() % 3 + 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				down->pad->move(-10.f, 0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				down->pad->move(10.f, 0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				up->pad->move(-10.f, 0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				up->pad->move(10.f, 0);
			}
			if (score1 == 10 || score2 == 10)
			{
				win.clear();
				win.draw(bg);
				s2.setPosition(win.getSize().x / 2 - 50.f, win.getSize().y / 2 - 50.f);
				s2.setCharacterSize(80);
				s2.setFillColor(sf::Color::White);
				s2.setFont(f);
				s2.setString("Game Over ;] ");
				win.draw(s2);

			}// here
			win.draw(bg);
			circle->move();
			circle->draw(win);
			win.draw(s1);
			if (collision(*down->pad, circle->getshape()) == 1)
			{
				circle->velocity.y = -circle->velocity.y;
			}
			if (collision(*up->pad, circle->getshape()) == 1)
			{
				circle->velocity.y = -circle->velocity.y;
			}
			if (playauto)
				autobot(*up, circle, circle->getshape(), *up->pad, win);
			if (!playauto)win.draw(*up->pad);
			win.draw(*down->pad);
			win.draw(s1);
			win.draw(s2);
		}
		if (openmenu)
		{

			sf::Sprite menubg; sf::Texture texture; texture.loadFromFile("menu.png");
			menubg.setTexture(texture);
			sf::Text play, play2; sf::Font f; f.loadFromFile("font1.ttf");
			play.setFillColor(sf::Color::Black);
			play.setFont(f);
			play.setCharacterSize(80);
			play.setPosition(win.getSize().x / 2 - 370.f, win.getSize().y / 2 - 100.f);
			play.setString("Plaver Versus Player");
			win.draw(menubg);
			int posx = (int)e.mouseMove.x;
			int posy = (int)e.mouseMove.y;
			if (posx >= 594 && posx <= 1346 && posy >= 460 && posy <= 520)
				play.setFillColor(sf::Color::Red);
			else play.setFillColor(sf::Color::Black);
			win.draw(play);
			play2.setFillColor(sf::Color::Black);
			play2.setFont(f);
			play2.setCharacterSize(80);
			play2.setPosition(win.getSize().x / 2 - 430.f, win.getSize().y / 2 + 30.f);
			play2.setString("Plaver Versus Computer");
			if (posx >= 564 && posx <= 1430 && posy >= 621 && posy <= 669)
				play2.setFillColor(sf::Color::Red);
			else play2.setFillColor(sf::Color::Black);
			win.draw(play2);
		}
		win.display();
	}
}

