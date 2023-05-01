#include "Level.h"

// Level c-tor
Level::Level(Pacman & pac, vector<std::unique_ptr<Demon>>& m_demons,int levelNumber)
{
	read_level(pac, m_demons,levelNumber);
}


// this function returns the board size
sf::Vector2f Level::getBoardSize()
{
	sf::Vector2f boardSize;
	boardSize.x = m_board.size() * P_SIZE;
	boardSize.y = m_board[0].size() * P_SIZE;
	return boardSize;
}

// this function draw the board
void Level::drawStatic(sf::RenderWindow & window)
{
	 
	for (size_t i = 0; i < m_board.size(); i++)
	{
		for (size_t j = 0; j < m_board[i].size(); j++)
		{
				if (m_board[i][j] != nullptr)
					m_board[i][j]->draw(window, sf::Vector2f(j * P_SIZE, i * P_SIZE));
			
		}
	}
}

void Level::clear_vector()
{
	for (size_t i = 0; i < m_board.size(); i++)
	{
		for (size_t j = 0; j < m_board[i].size(); j++)
		{
			if (m_board[i][j] != nullptr)
				m_board[i][j].release();

		}
	}
}

// this function opens the .txt file to read
void Level::open_file(ifstream& input, int levelNumber)
{
	string file = "level" + std::to_string(levelNumber) + ".txt";
	input.open(file);
	if (!input.is_open())
	{
		std::cerr << "could not open file\n";
		exit(EXIT_FAILURE);
	}
}

// this function reads the .txt file and sets the board accordingly
void Level::read_level(Pacman & pac, vector<std::unique_ptr<Demon>>& m_demons, int levelNumber)
{
	
	open_file(m_file,levelNumber);

	char c;

	m_file >> m_rows >> m_cols;
	setBoardSize(m_rows, m_cols);

	for (size_t i = 0; i < m_rows; i++)
	{
		m_file.get();
		for (size_t j = 0; j < m_cols; j++)
		{
			c = m_file.get();
			add_to_board(c, i, j,pac,m_demons);
		}
	}
	m_file.seekg(0, std::ios::beg);
	m_file.close();
}

// this function sets the pacman's position and color and saves its 
// starting position
void Level::setPacman(Pacman & pac, Color_t color, sf::Vector2f pos)
{
	pac.SetPosition(pos);
	pac.setStartingPosition(pos);
	pac.SetColor(color);
}

// this function sets the demon's type (random or smart) and position  
void Level::setDemon(vector<std::unique_ptr<Demon>>& m_demons, Color_t color, sf::Vector2f pos)
{
	if (!m_demons.size() % 2 == 0)
		m_demons.push_back(std::make_unique < SmartDemon >(color));
	else
		m_demons.push_back(std::make_unique < RandomDemon >(color));

	int size = (int)m_demons.size() - 1;
	m_demons[size]->SetPosition(pos);
	m_demons[size]->setStartingPosition(pos);
}

// this function returns the static icon in the board according the a
// certain position
StaticIcon * Level::getIcon(sf::Vector2f pos)
{	
	int i = pos.y / P_SIZE;
	int j = pos.x / P_SIZE;
	return m_board[i][j].get();
}

// this function sets the board
vector<vector<std::unique_ptr<StaticIcon>>>& Level::getBoard()
{
	return m_board;
}

// this fuction adds the matching object to the 
// game according to a the character from the .txt file
void Level::add_to_board(const char c,
	const size_t i,
	const size_t j,
	Pacman & pac,
	vector<std::unique_ptr<Demon>>& m_demons)
{
	switch (c)
	{
	case '#':
		m_board[i][j] = std::make_unique < Wall >(sf::Color::White);
		break;
	/*case 'E':
		m_board[i][j] = std::make_unique < Wall >(sf::Color::Green);
		break;*/
	case 'D':
		m_board[i][j] = std::make_unique < Door >(sf::Color::White);
		
		break;
	case '*':
		m_board[i][j] = std::make_unique < Cookie >(sf::Color::Red, sf::Vector2i(j, i));
		pac.setCookieCounter(1);
		break;
	case '%':
		m_board[i][j] = std::make_unique < Key >(sf::Color::White, sf::Vector2i(j, i));
		break;
	case '$':
		m_board[i][j] = std::make_unique < Gift >(sf::Color::White, sf::Vector2i(j, i));
		break;
	case '&':
		setDemon(m_demons, RED, sf::Vector2f(j*P_SIZE, i*P_SIZE));
		break;
	/*case 'T':
		setDemon(m_demons, GREEN, sf::Vector2f(j*P_SIZE, i*P_SIZE));
		break;
	case 'G':
		setDemon(m_demons, BLUE, sf::Vector2f(j*P_SIZE, i*P_SIZE));
		break;*/
	case '@':
		setPacman(pac, RED, sf::Vector2f(j*P_SIZE, i*P_SIZE));
		break;
	case 'a':
		setPacman(pac, GREEN, sf::Vector2f(j*P_SIZE, i*P_SIZE));
		break;
	/*case 'S':
		setPacman(pac, BLUE, sf::Vector2f(j*P_SIZE, i*P_SIZE));
		break;*/
	default:
		break;
	}
}

// this function sets the board to a new size
void Level::setBoardSize(int rows, int cols)
{
	m_board.resize(rows);
	for (size_t i = 0; i < rows; i++)
		m_board[i].resize(cols);
}



