Этап 1. Реализация комбинирующего генератора для анализа


Комбинирующий генератор (G) - криптографический объект, который генерирует некоторую псевдослучайную последовательность.
Именено его мы и будем пытаться атаковать. Для начала нам следует реализовать сам генератор.


Функция: createG() // создание G 

Вход:  пустой
Выход: G // комбирирующий генератор

[ 
G представляет собой структуру / класс и состоит из:
	uint8_t N;     // количество РСЛОС в G	
	uint16_t r[N]; // реверснутые полиномы обратной связи для РСЛОС
	uint16_t a[N]; // начальные состония РСЛОС	
]
	
{
Замечу, что я сделал несколько конкретизаций: 
	1) Длина РСЛОС может быть любая, я же принял длину всех РСЛОС в 16 ячейки памяти;
	2) Функция f может иметь достаточно сложную структуру, я же предлагаю рассматривать функцию, определенную для генератора Геффе.
}
	
1. N <- 3;
2. Для i = 0 .. N - 1:
	2.1. Если i == 0, то r[i] <- 0x8003; 
		 Если i == 1, то r[i] <- 0x8011;
         Если i == 2, то r[i] <- 0x8081;  		 
	2.2. a[i] <- R[0, 65535]; // R[a, b] - выбрать рандомно из промежутка от a до b включительно
3. Вернуть G;
	
	
Функция: workG() // работа G 

Вход:  G, uint32_t l // G - комбирирующий генератор, l - длина выходной последовательности, которую мы хотим получить
Выход: uint8_t z[l]  // z[l] - выходная битовая последовательность

[
uint8_t tmp[N];     // временный массив
uint8_t z[l] = {0}; // выходная битовая последовательность
]

1. Для i = 0 .. l - 1:
	1.1. Для j = 0 .. N - 1:
		1.1.1. Если (a[j] & 0x0001) == 1:
					То a[j] <- (a[j] ^ r[j] >> 1) | 0x8000, tmp[j] <- 1;
					Иначе a[j] <- a[j] >> 1, tmp[j] <- 0;
	1.2. z[i] <- (tmp[0] * tmp[1]) ^ (tmp[0] ^ 1) * tmp[2];
2. Вернуть z;
		
		
Этап 2. Реализация базовой корреляционной атаки Зигинталера


На этом этапе будет реализована базовая атака Зигинталера


#Функция: createAndWorkTestR() // работа генератора для алгоритма Зигинталера

Вход:  uint16_t d, uint32_t l, uint8_t i
Выход: x[d + l] // x[d + l] - выходная битовая последовательность

[
uint16_t r;       // реверснутые полином обратной связи для РСЛОС
uint16_t a;       // начальное состоние РСЛОС
uint8_t x[d + l]; // выходная битовая последовательность
]

1. Если i == 0, то r <- 0x8003; 
   Если i == 1, то r <- 0x8011;
   Если i == 2, то r <- 0x8081;  		 
2. a <- d;
3. Для i = 0 .. d + l - 1:	
	3.1. Если (a & 0x0001) == 1:
		 То a <- (a ^ r >> 1) | 0x8000, x[i] <- 1;
		 Иначе a <- a >> 1, x[i] <- 0;	
4. Вернуть x;


#Фукция: zigentalerAlg() // алгоритм Зигинталера

Вход: G, uint32_t l, uint8_t z[l] // G - комбирирующий генератор, l - длина выходной последовательности, z[l] - сгенерированная битовая последовательность
Выход: uint16_t a[N]              // полученные с помощью атаки начальные состония РСЛОС

[
double p[N];          // априорные вероятности
double c;             // кросс-корреляционная функция
uint8_t x[l + 65535]; // выходная битовая последовательность
uint32_t tmp = 0;     // временнная переменная
double cor;           // корреляция (нет)
uint16_t a[N];       // полученные с помощью атаки начальные состония РСЛОС
]

1. uint32_t n = l / 2;
2. Для j = 0 .. N - 1:
		2.1. Если j == 0, то p[j] <- 0.5; 
		     Если j == 1, то p[j] <- 0.75;
			 Если j == 2, то p[j] <- 0.75;
		2.2. Для d = 1 .. 65535:
				2.2.1. x <- createAndWorkTestR(d, l, j);
				2.2.2. Для i = 0 .. n - 1:
							2.2.2.1.  с <- c  + ((-1) ^ z[i] * (-1) ^ x[i + d])
				2.2.3. c <- c / n;
				2.2.4. Если c < 0.4, то перейти на 2.2 // 0.4 - это прямо чуть ли не рандомно, книга где вроде как должно быть написано как считать это число для конкретных данных стоит 19 баксов, найти в откртыом доступе на торрентах я не смог, поэтому будем подгонять
				2.2.5. Для i = n .. l:
							Если z[i] == x[i], то tmp <- tmp + 1;
				2.2.6. cor <- tmp / n;
				2.2.7. Если cor > p[j] - 0.05 && cor < p[j] + 0.05:
						То a[j] <- d; переод на 2;
						Иначе переход на 2.2;
3. Вернуть a;


Этап 3. Основная функция (main)


Вход: пустой
Выход: 0	

[
uint32_t l = 100000; // выбранная на шару длина битовой последовательности
uint32_t z[l]; // выходная битовая последовательность
uint16_t a[N]; // полученные с помощью атаки начальные состония РСЛОС
]

1. G <- createG();
2. z <- workG(G, l);
3. a <- zigentalerAlg(G, l, z[l]);
4. Для j = 0 .. N - 1:
	Если a[j] != G.a[j], то вывести на экран "Мы НЕ сдали лабу(" и переход на 6;
5. Вывести на экран "Мы сдали лабу)";
6. Вернуть 0;

(c) Written by [Maxim Kazlovsky](https://github.com/MaksimKazlovski/). 
