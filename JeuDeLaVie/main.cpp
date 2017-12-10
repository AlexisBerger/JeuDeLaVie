#pragma warning(disable:4996)
#include <SFML/Graphics.hpp>

#define WINDOW_TITLE "Jeu de la vie"
#define COLOR_BACKGROUND sf::Color::White
#define WINDOW_HEIGHT 900.0f
#define WINDOW_WIDTH 900.0f
#define COLOR_GRID sf::Color(127, 133, 142)
#define GRID_SIZE 300
#define GRID_THICKNESS 100


void renderingThread(sf::RenderWindow* window);
void computingThread(sf::RenderWindow* window);
int getVoisin(int i, int j);
void populateArray(bool(&arrayA)[GRID_SIZE][GRID_SIZE], const bool(&arrayB)[GRID_SIZE][GRID_SIZE]);

//init window
sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
//init rendering thread
sf::Thread thread(&renderingThread, &window);
sf::Thread thread2(&computingThread, &window);


static bool cellule[GRID_SIZE][GRID_SIZE];
static bool buffer[GRID_SIZE][GRID_SIZE];
/*
bool** cellule;
bool** buffer;
*/
int main()
{
	/*
	cellule = new bool*[GRID_SIZE];
	for (int i = 0; i < GRID_SIZE; ++i)
		cellule[i] = new bool[GRID_SIZE];
	buffer = new bool*[GRID_SIZE];
	for (int i = 0; i < GRID_SIZE; ++i)
		buffer[i] = new bool[GRID_SIZE];
	*/
	for (int i = GRID_SIZE - 1; i >= 0; i--)
	{
		for (int j = GRID_SIZE - 1; j >= 0; j--)
		{
			//*
			if (rand() % 2 == 0)
				cellule[i][j] = false;
			else
				cellule[i][j] = true;//*/
			//cellule[i][j] = false;
		}
	}
	/*
	cellule[0][0] = true;
	cellule[0][1] = true;
	cellule[1][0] = true;
	cellule[1][1] = true;
*/
	window.clear(COLOR_BACKGROUND);
	window.setVerticalSyncEnabled(true);
	window.setActive(false);

	thread.launch();
	thread2.launch();


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::sleep(sf::milliseconds(50));
	}

	return 0;
}
void bufCell() {
	/*for (int i = GRID_SIZE - 1; i >= 0; i--)
	{
		for (int j = GRID_SIZE - 1; j >= 0; j--)
		{
			buffer[i][j] = cellule[i][j];
		}
	}*/
	//	std::copy(cellule, cellule+GRID_SIZE*GRID_SIZE, buffer);
	populateArray(buffer, cellule);
}
void deBufCell() {
	/*for (int i = GRID_SIZE - 1; i >= 0; i--)
	{
		for (int j = GRID_SIZE - 1; j >= 0; j--)
		{
			cellule[i][j]=buffer[i][j];
		}
	}*/
	//std::copy(buffer, buffer + GRID_SIZE*GRID_SIZE, cellule);
	populateArray(cellule, buffer);
}
void populateArray(bool(&arrayA)[GRID_SIZE][GRID_SIZE], const bool(&arrayB)[GRID_SIZE][GRID_SIZE]) {

	memcpy(arrayA, arrayB, sizeof(arrayB));
}
void computingThread(sf::RenderWindow* window) {
	sf::sleep(sf::milliseconds(5000));
	long int loop = 0;
	while (window->isOpen())
	{

		bufCell();
		for (int i = GRID_SIZE - 1; i >= 0; i--)
		{
			for (int j = GRID_SIZE - 1; j >= 0; j--)
			{
				int voisin = getVoisin(i,j);
				if (voisin != 2) {
					if (cellule[i][j] == true && (voisin < 2 || voisin > 3)) {
						buffer[i][j] = false;
						
					}
					else if (cellule[i][j] == false && voisin == 3) {
						buffer[i][j] = true;
						
					}
				}
			}
		}
	
		deBufCell();
		//sf::sleep(sf::milliseconds(10));
	}

}

bool valideAdress(int i, int j) {
		return (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE);
}
int getVoisin(int i, int j) {
	int nb = 0;
	if (valideAdress(i - 1, j - 1) && cellule[i - 1][j - 1] == true)
		nb++;
	if (valideAdress(i, j - 1) && cellule[i][j - 1] == true)
		nb++;
	if (valideAdress(i - 1, j) && cellule[i - 1][j] == true)
		nb++;
	if (valideAdress(i - 1, j + 1) && cellule[i - 1][j + 1] == true)
		nb++;
	if (valideAdress(i + 1, j - 1) && cellule[i + 1][j - 1] == true)
		nb++;
	if (valideAdress(i + 1, j) && cellule[i + 1][j] == true)
		nb++;
	if (valideAdress(i, j + 1) && cellule[i][j + 1] == true)
		nb++;
	if (valideAdress(i + 1, j + 1) && cellule[i + 1][j + 1] == true)
		nb++;
	return nb;
}
void renderingThread(sf::RenderWindow* window)
{


	sf::VertexArray gridH[GRID_SIZE];
	sf::VertexArray gridV[GRID_SIZE];
	for (int i = GRID_SIZE - 1; i >= 0; i--)
	{
		sf::Vector2f pointH1(0, (WINDOW_HEIGHT / GRID_SIZE)*i);
		sf::Vector2f pointH2(WINDOW_WIDTH, (WINDOW_HEIGHT / GRID_SIZE)*i);
		sf::VertexArray lineH(sf::LineStrip, 2);
		lineH[0].position = pointH1;
		lineH[0].color = COLOR_GRID;
		lineH[1].position = pointH2;
		lineH[1].color = COLOR_GRID;
		gridH[i] = lineH;

		sf::Vector2f pointV1((WINDOW_WIDTH / GRID_SIZE)*i, 0);
		sf::Vector2f pointV2((WINDOW_WIDTH / GRID_SIZE)*i, WINDOW_HEIGHT);
		sf::VertexArray lineV(sf::LineStrip, 2);
		lineV[0].position = pointV1;
		lineV[0].color = COLOR_GRID;
		lineV[1].position = pointV2;
		lineV[1].color = COLOR_GRID;
		gridV[i] = lineV;

	}
	

	while (window->isOpen())
	{
		window->clear(COLOR_BACKGROUND);

		int nb = 0;
		for (int i = GRID_SIZE - 1; i >= 0; i--)
		{
			for (int j = GRID_SIZE - 1; j >= 0; j--)
			{
				if (cellule[i][j] == true) {
					sf::RectangleShape rectangle(sf::Vector2f((WINDOW_WIDTH / GRID_SIZE) - 1, (WINDOW_HEIGHT / GRID_SIZE) - 1));
					rectangle.setPosition(sf::Vector2f(((WINDOW_WIDTH / GRID_SIZE)*i), ((WINDOW_HEIGHT / GRID_SIZE)*j) + 1));
					rectangle.setFillColor(sf::Color::Black);
					window->draw(rectangle);
				}
			}
			window->draw(gridH[i]);
			window->draw(gridV[i]);
		}

		window->display();
		sf::sleep(sf::milliseconds(15));
	}
}

