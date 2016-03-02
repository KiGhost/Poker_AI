
#pragma once


class POKEIT_API Card
{
private:
	/*
	0 = clubs
	1 = spades
	2 = diamonds
	3 = hearts
	*/
	int color;

	/*
	0 = 2
	1 = 3
	2 = 4
	3 = 5
	4 = 6
	5 = 7
	6 = 8
	7 = 9
	8 = 10
	9 = jack
	10 = queen
	11 = king
	12 = ace
	*/
	int value;

public:
	Card();
	Card(const int, const int);
	~Card();

	int getColor();
	int getValue();

	// overloaded comparison operators
	inline bool operator==(const Card& card) const;
	inline bool operator!=(const Card& card) const;
	inline bool operator< (const Card& card) const;
	inline bool operator> (const Card& card) const;
	inline bool operator<=(const Card& card) const;
	inline bool operator>=(const Card& card) const;

};