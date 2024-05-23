#include <iostream>
#include <cmath>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <string>
#include <iomanip>
#include <SFML/Graphics.hpp>

using namespace std;
//g — ускорение свободного падения (приблизительно 9.81м/с^2).
const double g = 9.81;
//число пи
const double PI = 3.1415;

struct Telo
{
	double H;
	double V;
	double alpha;
};
	
double tPolet(double H, double V, double alpha) { //длительность полета
	// Перевод радиан
	double Rad = alpha * PI / 180;
	//начал. скорости
	double Vo = V * sin(Rad);

	// квадрат уравнение для
	double D = pow(Vo, 2) + 2 * g * H;
	double t = (Vo + sqrt(D)) / g;

	return t;
}

double xDalnost(double V, double alpha, double t) {//дальность полета
	double Rad = alpha * PI / 180;
	//дальность
	double Dal = V * cos(Rad) * t;

	return Dal;
}

double yMax(double H, double V, double alpha) {//макс точка
    double Rad = alpha * PI / 180;

    // Макс высота
    double maxY = H + pow(V * sin(Rad), 2) / (2 * g);

    return maxY;
}

void mudlP(const string& slova, const int& slep) {//медленный вывод
	for (const char c : slova) {
		cout << c << flush; //flush - очистка буфера:/
		this_thread::sleep_for(chrono::milliseconds(slep));
	}
}
void mudlPendl(const string& slova, const int& slep) {//медленный вывод
	for (const char c : slova) {
		cout << c << flush; //flush - очистка буфера:/
		this_thread::sleep_for(chrono::milliseconds(slep));
	}
	cout << endl;
}

vector<pair<double, double>> Troect(double H, double V, double alpha) {
	//pair - все что содержит pair то имеет два объекта(первый и второй) 
	//emplace_back - cоздает элемент вектор и используя аргумент из функции
	int koords = 8;
	double Rad = alpha * PI / 180;

	double Vx = V * cos(Rad);
	double Vy = V * sin(Rad);

	double allTime = tPolet(H, V, alpha);

	vector<pair <double, double>> arr;//вектор для хранение x и y

	for (double j = 0; j <= koords; j++) {
		double t = j * allTime / koords; //рассчет времени
		double x = Vx * t; //координата x в момент времени t
		double y = H + Vy * t - 0.5 * g * pow(t, 2);//координата y в момент времени t
		
		if (y < 0) y = 0; //приземлился упал
		arr.emplace_back(x, y);
	}
	return arr;
}



//to_string - для того чтоб переделать функцию и можно было передать в string
int main()
{
	Telo maks;
	setlocale(LC_ALL, "");
	system("Title Программа для решения задач по кинематике");

	mudlP("Введите начальную высоту(метры):", 40);
	cin >> maks.H;
	mudlP("Введите начальную скорость(м/с):", 40);
	cin >> maks.V;
	mudlP("Введите угол, под которым будет сброшенно тело:", 40);
	cin >> maks.alpha;
	cout << "--------------------------------------------------------" << endl;
	int Prod;
	mudlPendl("1.Время полета.", 20);
	mudlPendl("2.Дальность полета.", 20);
	mudlPendl("3.Максимальная высота.", 20);
	mudlPendl("4.Координаты полета(от старта до приземления).", 20);
	mudlPendl("5.Отрисовать график.", 20);
	mudlPendl("0 - выход из программы", 20);
	cin >> Prod;

	if (Prod == 1) {
		cout << "-------------------" << endl; 
		cout << fixed << setprecision(2) << "Время полета:" << tPolet(maks.H, maks.V, maks.alpha) << ' ' << "(в минутах)" << endl;
		this_thread::sleep_for(chrono::milliseconds(20));
	}
	if (Prod == 2) {
		cout << "-------------------" << endl;
		cout << fixed << setprecision(2) << "Дальность полета:" << xDalnost(maks.V, maks.alpha, tPolet(maks.H, maks.V, maks.alpha)) << ' ' << "(в метрах)" << endl;
		this_thread::sleep_for(chrono::milliseconds(20));
	}
	if (Prod == 3) {
		cout << "-------------------" << endl;
		cout << fixed << setprecision(2) << "Максимальная высота:" << yMax(maks.H, maks.V, maks.alpha) << ' ' << "(в метрах)" << endl;
		this_thread::sleep_for(chrono::milliseconds(20));
	}
	if (Prod == 4) {
		cout << "-------------------" << endl;
		mudlPendl("Координаты:", 20);
		vector<pair<double, double>> arr = Troect(maks.H, maks.V, maks.alpha);
		for (auto p : arr) {
			cout << fixed << setprecision(2) << "x:" << p.first << ' ' << "y:" << p.second << endl;
			this_thread::sleep_for(chrono::milliseconds(60));
			}
		}
	if (Prod == 5) {
		sf::RenderWindow window(sf::VideoMode(800, 600), "Trajectory");
		window.setFramerateLimit(60);

		vector<pair<double, double>> arr = Troect(maks.H, maks.V, maks.alpha);
		int scale = 3;

		sf::VertexArray lines(sf::LineStrip, arr.size());

		for (size_t i = 0; i < arr.size(); ++i) {
			lines[i].position = sf::Vector2f(arr[i].first * scale, window.getSize().y - arr[i].second * scale);
			lines[i].color = sf::Color::White;
		}
		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}
			window.clear(sf::Color::Black);
			window.draw(lines);

			//траектории
			for (const auto& point : arr) {
				sf::CircleShape shape(2);
				shape.setFillColor(sf::Color::Red);
				shape.setPosition(point.first * scale, window.getSize().y - point.second * scale);
				window.draw(shape);
			}

			window.display();
		}
	}


	while (Prod != 0) {
		cout << "-------------------" << endl;
		mudlPendl("1.Время полета.", 20);
		mudlPendl("2.Дальность полета.", 20);
		mudlPendl("3.Максимальная высота.", 20);
		mudlPendl("4.Координаты полета(от старта до приземления).", 20);
		mudlPendl("5.Отрисовать график.", 20);
		mudlPendl("0 - выход из программы", 20);
		cin >> Prod;

		if (Prod == 1) {
			cout << "-------------------" << endl;
			cout << fixed << setprecision(2) << tPolet(maks.H, maks.V, maks.alpha) << endl;
			this_thread::sleep_for(chrono::milliseconds(20));
		}
		if (Prod == 2) {
			cout << "-------------------" << endl;
			cout << fixed << setprecision(2) << "Дальность полета:" << xDalnost(maks.V, maks.alpha, tPolet(maks.H, maks.V, maks.alpha)) << ' ' << "(в метрах)" << endl;
			this_thread::sleep_for(chrono::milliseconds(20));
		}
		if (Prod == 3) {
			cout << "-------------------" << endl;
			cout << fixed << setprecision(2) << "Максимальная высота:" << yMax(maks.H, maks.V, maks.alpha) << ' ' << "(в метрах)" << endl;
			this_thread::sleep_for(chrono::milliseconds(20));

		}
		if (Prod == 4) {
			cout << "-------------------" << endl;
			mudlPendl("Координаты:", 20);
			vector<pair<double, double>> arr = Troect(maks.H, maks.V, maks.alpha);
			for(auto p : arr){
				cout << fixed << setprecision(2) << "x:" << p.first << ' ' << "y:" << p.second << endl;
				this_thread::sleep_for(chrono::milliseconds(60));
			}
		}
		if (Prod == 5) {
			sf::RenderWindow window(sf::VideoMode(800, 600), "Trajectory");
			window.setFramerateLimit(60);

			vector<pair<double, double>> arr = Troect(maks.H, maks.V, maks.alpha);
			int scale = 3;

			sf::VertexArray lines(sf::LineStrip, arr.size());

			for (size_t i = 0; i < arr.size(); ++i) {
				lines[i].position = sf::Vector2f(arr[i].first * scale, window.getSize().y - arr[i].second * scale);
				lines[i].color = sf::Color::White;
			}
			while (window.isOpen()) {
				sf::Event event;
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed)
						window.close();
				}
				window.clear(sf::Color::Black);
				window.draw(lines);

				//траектории
				for (const auto& point : arr) {
					sf::CircleShape shape(2);
					shape.setFillColor(sf::Color::Red);
					shape.setPosition(point.first * scale, window.getSize().y - point.second * scale);
					window.draw(shape);
				}
				window.display();
			}
		}
	}
}