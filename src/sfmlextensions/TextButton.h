#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include <string>

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

//HAVE TO INITIALIZE IN ORDER
// 
// for example ~
//always have font above text

class Label
{
private:
	sf::Font font;
	sf::Text label;
	int fontSize;
	int stringLength;
	char relPos = 0; //0 is left, 1 is above, 2 is to the right, 3 is below, 4 is no relation
	sf::Vector2f position;
	sf::Vector2f posDiff;
	bool isActive = true;

public:
	Label() {}
	Label(sf::Vector2f pos, int fontSz, std::string string, char relation, sf::Font& font, sf::Vector2f relationSize = { 100,0 });

	void setPosition(sf::Vector2f pos, char relation, sf::Vector2f posDif);

	void setText(std::string setter);

	void setActivity(bool activity) { isActive = activity; };

	sf::String getText() { return label.getString(); };

	void setTextColor(sf::Color color) { label.setFillColor(color); };

	void drawTo(sf::RenderWindow& window);

};



class Textbox
{
public:

	bool numberOnly = true;

private:
	sf::Text textbox;
	sf::RectangleShape textRect;
	std::ostringstream text;
	bool isSelected = false;
	bool hasLimit = true;
	int limit = 5;
	bool hasLabel = false;
	bool canEdit = true;
	bool isActive = true;

	void inputLogic(int charTyped);

	void deleteLastChar();


public:
	Textbox() {}
	Textbox(sf::Vector2f pos, int fontSize, sf::Font& font, sf::Vector2f boxSize, sf::Color color, int limitInit, std::string string);
	Textbox(sf::Vector2f pos, int fontSize, sf::Vector2f boxSize, sf::Color color, int limitInit, std::string string, bool isEditable);

	void setPosition(sf::Vector2f pos);

	void setSelected(bool sel);

	void setBoxTrans() { textRect.setOutlineColor(sf::Color::Transparent); };

	void setActivity(bool activity) { isActive = activity; }

	void setTextColor(sf::Color color) { textbox.setFillColor(color); };

	std::string getText() { return textbox.getString(); }

	void setText(std::string setter);

	void drawTo(sf::RenderWindow& window);
	void typedOn(sf::Event input);
	bool isMouseOver(sf::RenderWindow& window);

};

class TextboxWLabel {
private:

public:
	std::unique_ptr<Label> label;
	std::unique_ptr<Textbox> textbox;
	//Textbox(sf::Vector2f pos, int fontSize, sf::Vector2f boxSize, sf::Color color, int limitInit, std::string string = "") {
	//Label(sf::Vector2f pos, int fontSz, std::string string, char relation, sf::Font &font, sf::Vector2f relationSize = { 100,0 }) {

	TextboxWLabel(sf::Vector2f pos, int fontSize, sf::Font& font, std::string labstring, std::string boxstring, sf::Vector2f boxSize, sf::Color color, int limitInit, char relation, sf::Vector2f relSize) {

		label = std::make_unique<Label>(pos, fontSize, labstring, relation, font, relSize);
		textbox = std::make_unique<Textbox>(pos, fontSize, font, boxSize, color, limitInit, boxstring);
		textbox->setBoxTrans();

	}


	void draw(sf::RenderWindow& window) {
		label->drawTo(window);
		textbox->drawTo(window);
	}

};





class TextButton
{
private:
	sf::Font bst;

	sf::Text text;
	float buttonSize = 0;
	sf::RectangleShape button;


public:
	TextButton(std::string t, sf::Vector2f pos, int charSize, sf::Color bgColor, sf::Color tColor, sf::Font& font, float butSz);

	/*
	text.setString(t);
	text.setFillColor(tColor);
	text.setCharacterSize(charSize);
	text.setFont(font);

	buttonSize = butSz;
	text.setPosition(pos);
	//button position needs to called to fix the sizing, because text doesn't have a size off the jump

	button.setFillColor(bgColor);
	button.setOutlineColor(sf::Color::Black);
	button.setOutlineThickness(2);
	*/


	void setBGColor(sf::Color color) { button.setFillColor(color); }

	void setPosition(sf::Vector2f pos);
	void draw(sf::RenderWindow& window);

	bool isMouseOver(sf::RenderWindow& window);
};



class ImageButton
{
private:
	sf::RectangleShape button;
	bool isSelected = false;


public:
	ImageButton(sf::Vector2f size, sf::Color bgColor);

	void setBGColor(sf::Color color) { button.setFillColor(color); }

	void setPosition(sf::Vector2f pos, sf::Sprite& sprite);

	void drawTo(sf::RenderWindow& window) { window.draw(button); }

	bool isMouseOver(sf::RenderWindow& window);

	void setSelected(bool selection) { isSelected = selection; }

	bool getSelected() { return isSelected; }
};


class SelectableText {
private:
	sf::Text textContents;
	sf::RectangleShape boxSelect;
	int fontSize = 18;
	bool isSelected = false;
	bool isActive = true;

public:
	SelectableText(float initWidth, std::string string, int fontSz, sf::Font& font);

	bool isMouseOver(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);
	void setPosition(sf::Vector2f pos);
	void setSelection();
	void stopSelection();

	void setActivity(bool active) { isActive = active; }
	void changeWidth(float a) { boxSelect.setSize({ a, 20 }); }
	std::string getString() { return textContents.getString(); }

};


class ListBox {

private:
	char numberOfItems = 0;
	char selection = -1;
	std::vector<SelectableText> contents;
	float width;
	sf::RectangleShape contBox;
	sf::Vector2f lbPosition;
	sf::Font font;
	int highestLength = 0;
	int fontSize;
	bool sizeAdjustable = true;



	void updateSize();


public:
	ListBox(sf::Vector2f pos, int fontSz, sf::Font& fnt, float initWidth);

	void pushback(std::string string);
	void popback();
	void clear();

	char isMouseOver(sf::RenderWindow& window);

	void setSelection(int a);

	void draw(sf::RenderWindow& window);

	void setPosition(sf::Vector2f pos);

	char getSelection() { return selection; }
	sf::RectangleShape getRectangle() { return contBox; }

};

class comboBox {

private:
	sf::Texture dropTexture;
	sf::Sprite ravelledSprite;
	sf::Sprite droppedSprite;
	bool isDropped = false;
	sf::Text text;

	char numberOfItems = 0;
	char selection = -1;
	std::vector<SelectableText> contents;
	float width;
	sf::RectangleShape contBox;
	sf::Vector2f cbPosition;
	sf::Font font;
	int highestLength = 0;
	int fontSize;
	bool sizeAdjustable = true;



	void dropDown();

	void ravelUp();

	void updateSize();


public:
	comboBox(sf::Vector2f pos, int fontSz, sf::Font& fnt, float initWidth);

	void pushback(std::string string);
	void popback();
	void clear();

	char isMouseOver(sf::RenderWindow& window);

	void setSelection(int a);

	void draw(sf::RenderWindow& window);

	void setPosition(sf::Vector2f pos);

	char getSelection() { return selection; }
	sf::RectangleShape getRectangle() { return contBox; }

};

class checkBox {
private:
	sf::Texture checkTexture;
	sf::Sprite uncheckedSprite;
	sf::Sprite checkedSprite;
	sf::Text text;
	bool isChecked = false;
	sf::Vector2f position;
	char relPos;
	sf::Vector2f posDiff;

public:
	checkBox(std::string string, sf::Vector2f pos, sf::Font& font, char relation, sf::Vector2f posDif = {100,0});
	checkBox(std::string string, sf::Vector2f pos, sf::Font& font, char relation, std::string fileLocation, sf::Vector2i spriteSize, sf::Vector2f posDif = {100,0});
	void setPosition(sf::Vector2f pos, char relation, sf::Vector2f posDif);
	void setChecked(bool checked);

	void setSpriteColor(sf::Color color) {
		uncheckedSprite.setColor(color);
	}

	void isMouseOver(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);

	bool getChecked() { return isChecked; }

};