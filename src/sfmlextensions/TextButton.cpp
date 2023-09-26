#include "TextButton.h"

// ~~~~~~Label ~~~~~~~~~~~
Label::Label(sf::Vector2f pos, int fontSz, std::string string, char relation, sf::Font& fnt, sf::Vector2f relationSize) {
	stringLength = string.length();
	fontSize = fontSz;
	font = fnt;

	label.setCharacterSize(fontSize);
	label.setFillColor(sf::Color::Black);
	label.setFont(font);

	label.setString(string);

	setPosition(pos, relation, relationSize);
}

void Label::setPosition(sf::Vector2f pos, char relation, sf::Vector2f posDif) {
	position = pos;
	relPos = relation;
	posDiff = posDif;

	float offset = label.getLocalBounds().left + label.getLocalBounds().width + 5;
	std::string coutString = label.getString();

	switch (relPos) {
	case 0:
		//left
		//label.setPosition({ pos.x - (stringLength * 2 / 3 * fontSize), pos.y });

		label.setPosition({ pos.x - offset, pos.y });
		break;

	case 1:
		//above
		label.setPosition({ pos.x,pos.y - posDif.y });
		break;

	case 2:
		//right
		label.setPosition({ pos.x + posDif.x + 2, pos.y });
		break;

	case 3:
		//below
		label.setPosition({ pos.x,pos.y + posDif.y });
		break;
	case 4:
		//no adjust
		label.setPosition(pos);
		break;
	}
}

void Label::setText(std::string setter) {
	label.setString(setter);
	setPosition(position, relPos, posDiff);
}

void Label::drawTo(sf::RenderWindow& window) {
	if (isActive) {
		window.draw(label);
	}
	else {
		std::cout << "inactive" << std::endl;
	}
}


//~~~~~~~~~~~~~~ TEXTBOX ~~~~~~~~~~~~~~~~~~~~~~~~
void Textbox::inputLogic(int charTyped) {
	if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
		if (isdigit(static_cast<char>(charTyped)) && numberOnly) {

			text << static_cast<char>(charTyped);
		}
		else if (!numberOnly) {
			text << static_cast<char>(charTyped);
		}
		else {

		}
	}
	else if (charTyped == DELETE_KEY) {
		if (text.str().length() > 0) {
			deleteLastChar();
		}
	}
	textbox.setString(text.str());

}

void Textbox::deleteLastChar() {
	std::string t = text.str();
	std::string newT = "";
	for (int i = 0; i < t.length() - 1; i++) {
		newT += t[i];
	}
	text.str("");
	text << newT;

	textbox.setString(text.str());
}

Textbox::Textbox(sf::Vector2f pos, int fontSize, sf::Font& font, sf::Vector2f boxSize, sf::Color color, int limitInit, std::string string = "") {
	setPosition(pos);

	textbox.setFont(font);

	textbox.setCharacterSize(fontSize);
	textbox.setFillColor(sf::Color::Black);

	textbox.setString(string);
	text.str(string);
	textRect.setSize(boxSize);
	textRect.setFillColor(color);

	limit = limitInit;

	textRect.setOutlineThickness(1);
	textRect.setOutlineColor(sf::Color::Black);
}

void Textbox::setPosition(sf::Vector2f pos) {
	textRect.setPosition(pos);
	textbox.setPosition(pos);
}

void Textbox::setSelected(bool sel) {
	isSelected = sel;
	if (!sel) {
		std::string t = text.str();
		std::string newT = "";
		for (int i = 0; i < t.length(); i++) {
			newT += t[i];
		}
		textbox.setString(newT);
	}
}

void Textbox::setText(std::string setter) {
	textbox.setString(setter);
	text.str(setter);
}

void Textbox::drawTo(sf::RenderWindow& window) {
	if (isActive) {
		window.draw(textRect);
		window.draw(textbox);
	}
}
void Textbox::typedOn(sf::Event input) {
	if (isSelected && canEdit) {
		int charTyped = input.text.unicode;
		if (charTyped == ENTER_KEY) {
			isSelected = false;
			return;
		}
		if (charTyped < 128) {
			if (hasLimit) {
				if (text.str().length() < limit) {
					inputLogic(charTyped);
				}
				else if (text.str().length() >= limit && charTyped == DELETE_KEY) {
					deleteLastChar();
				}
			}
			else {
				inputLogic(charTyped);
			}
		}
	}
}
bool Textbox::isMouseOver(sf::RenderWindow& window) {
	float mouseX = sf::Mouse::getPosition(window).x;
	float mouseY = sf::Mouse::getPosition(window).y;

	float btnPosX = textRect.getPosition().x;
	float btnPosY = textRect.getPosition().y;

	float btnxPosWidth = textRect.getPosition().x + textRect.getLocalBounds().width;
	float btnyPosHeight = textRect.getPosition().y + textRect.getLocalBounds().height;

	if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
		isSelected = true;
		return true;
	}
	isSelected = false;
	return false;
}

//~~~~~~~~~~~~~~~~ TEXTBUTTON ~~~~~~~~~~~~~~~~~~

TextButton::TextButton(std::string t, sf::Vector2f pos, int charSize, sf::Color bgColor, sf::Color tColor, sf::Font& font, float butSz) {
	text.setString(t);
	text.setFillColor(tColor);
	text.setCharacterSize(charSize);
	text.setFont(font);

	buttonSize = butSz;
	text.setPosition(pos);
	sf::Vector2f buttSz = { text.getLocalBounds().width, text.getLocalBounds().height };
	button.setSize({ buttSz.x + buttonSize * 2, buttSz.y + buttonSize * 2 });

	button.setPosition({ pos.x + text.getLocalBounds().left - buttonSize, pos.y + text.getLocalBounds().top - buttonSize });
	text.setPosition(pos);

	button.setFillColor(bgColor);
	button.setOutlineColor(sf::Color::Black);
	button.setOutlineThickness(2);
}

void TextButton::setPosition(sf::Vector2f pos) {
	sf::Vector2f buttSz = { text.getLocalBounds().width, text.getLocalBounds().height };
	button.setSize({ buttSz.x + buttonSize * 2, buttSz.y + buttonSize * 2 });

	button.setPosition({ pos.x + text.getLocalBounds().left - buttonSize, pos.y + text.getLocalBounds().top - buttonSize });
	text.setPosition(pos);
}
void TextButton::draw(sf::RenderWindow& window) {
	window.draw(button);
	window.draw(text);
}

bool TextButton::isMouseOver(sf::RenderWindow& window) {
	float mouseX = sf::Mouse::getPosition(window).x;
	float mouseY = sf::Mouse::getPosition(window).y;

	float btnPosX = button.getPosition().x;
	float btnPosY = button.getPosition().y;

	float btnxPosWidth = button.getPosition().x + button.getLocalBounds().width;
	float btnyPosHeight = button.getPosition().y + button.getLocalBounds().height;

	if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
		return true;
	}
	return false;
}


// ~~~~~~~~~~~~~ IMAGEBUTTON ~~~~~~~~~~~~~~~~~~~~~~

ImageButton::ImageButton(sf::Vector2f size, sf::Color bgColor) {
	button.setSize(size);
	button.setFillColor(bgColor);
}

bool ImageButton::isMouseOver(sf::RenderWindow& window) {
	float mouseX = sf::Mouse::getPosition(window).x;
	float mouseY = sf::Mouse::getPosition(window).y;

	float btnPosX = button.getPosition().x;
	float btnPosY = button.getPosition().y;

	float btnxPosWidth = button.getPosition().x + button.getLocalBounds().width;
	float btnyPosHeight = button.getPosition().y + button.getLocalBounds().height;

	if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
		return true;
	}
	return false;
}
void ImageButton::setPosition(sf::Vector2f pos, sf::Sprite& sprite) {
	button.setPosition(pos);
	sprite.setPosition({ pos.x + 4, pos.y + 4 });
}

//~~~~~~~~~~~~~~~~~~ SelectableText ~~~~~~~~~~~~~~~~~~~~~~

SelectableText::SelectableText(float initWidth, std::string string, int fontSz, sf::Font& font) {
	textContents.setCharacterSize(fontSize);
	textContents.setFillColor(sf::Color::Black);

	textContents.setString(string);
	fontSize = fontSz;

	boxSelect.setSize({ initWidth, 20 });

	textContents.setFont(font);
}

bool SelectableText::isMouseOver(sf::RenderWindow& window) {
	if (isActive) {
		float mouseX = sf::Mouse::getPosition(window).x;
		float mouseY = sf::Mouse::getPosition(window).y;

		float btnPosX = boxSelect.getPosition().x;
		float btnPosY = boxSelect.getPosition().y;

		float btnxPosWidth = boxSelect.getPosition().x + boxSelect.getLocalBounds().width;
		float btnyPosHeight = boxSelect.getPosition().y + boxSelect.getLocalBounds().height;

		if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
			boxSelect.setFillColor(sf::Color::Blue);
			textContents.setFillColor(sf::Color::White);
			return true;
		}
	}
	return false;
}
void SelectableText::draw(sf::RenderWindow& window) {
	if (isActive) {
		window.draw(boxSelect);
		window.draw(textContents);
	}
}
void SelectableText::setPosition(sf::Vector2f pos) {
	textContents.setPosition(pos);
	boxSelect.setPosition(pos);
}


void SelectableText::setSelection() {
	boxSelect.setFillColor(sf::Color::Blue);
	textContents.setFillColor(sf::Color::White);
	isSelected = true;
}
void SelectableText::stopSelection() {
	boxSelect.setFillColor({ 0,0,0,0 });
	textContents.setFillColor(sf::Color::Black);
	isSelected = false;
}

//~~~~~~~~~~~~~~~~~~~~~~~ ListBox ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ListBox::ListBox(sf::Vector2f pos, int fontSz, sf::Font& fnt, float initWidth = -1) {
	if (initWidth != -1) { sizeAdjustable = false; }
	lbPosition = pos;
	width = initWidth;
	fontSize = fontSz;

	font = fnt;

	contBox.setSize({ initWidth,0 });
	contBox.setPosition(pos);
	contBox.setOutlineThickness(1);
	contBox.setOutlineColor(sf::Color::Black);

}

void ListBox::updateSize() {
	contBox.setSize({ width, static_cast<float>(numberOfItems) * 20 });
	for (int i = 0; i < numberOfItems; i++) {
		contents[i].changeWidth(width);
	}
}

void ListBox::pushback(std::string string) {
	if (string.length() > highestLength && sizeAdjustable) {
		highestLength = string.length();
		width = static_cast<float>(fontSize) * (static_cast<float>(highestLength) / 2);
	}
	contents.push_back(SelectableText(width, string, fontSize, font));
	numberOfItems++;
	contents[numberOfItems - 1].setPosition({ lbPosition.x, lbPosition.y + 20 * (numberOfItems - 1) });

	this->updateSize();

}
void ListBox::popback() {
	contents.pop_back();
	numberOfItems--;
	this->updateSize();
}
void ListBox::clear() {
	contents.clear();
	numberOfItems = 0;
	highestLength = 0;
	selection = -1;
}

char ListBox::isMouseOver(sf::RenderWindow& window) {
	for (int i = 0; i < numberOfItems; i++) {
		if (contents[i].isMouseOver(window)) {
			for (int ii = 0; ii < numberOfItems; ii++) {
				if (i != ii) { contents[ii].stopSelection(); }
			}
			if (selection != i) {
				selection = i;
				return selection;
			}
			return -2;
		}
	}
	return -1;
}

void ListBox::setSelection(int a) {
	for (int ii = 0; ii < numberOfItems; ii++) {
		if (a != ii) { contents[ii].stopSelection(); }
	}
	if (numberOfItems > 0) {
		selection = a;
		contents[a].setSelection();
	}
}

void ListBox::draw(sf::RenderWindow& window) {
	if (numberOfItems > 0) {

		window.draw(contBox);
		for (int i = 0; i < numberOfItems; i++) {
			contents[i].draw(window);
		}
	}
}

void ListBox::setPosition(sf::Vector2f pos) {
	lbPosition = pos;
	contBox.setPosition(pos);
	for (int i = 0; i < numberOfItems; i++) {
		contents[i].setPosition({ pos.x, pos.y + 20 * i });
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~ comboBox ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void comboBox::dropDown() {
	contBox.setSize({ width, static_cast<float>(numberOfItems + 1) * 20 });
	for (int i = 0; i < numberOfItems; i++) {
		contents[i].setActivity(true);
	}
	isDropped = true;
}

void comboBox::ravelUp() {
	contBox.setSize({ width, 20 });
	for (int i = 0; i < numberOfItems; i++) {
		contents[i].setActivity(false);
	}
	isDropped = false;
}

void comboBox::updateSize() {
	contBox.setSize({ width, 20 });
	for (int i = 0; i < numberOfItems; i++) {
		contents[i].changeWidth(width);
	}
	droppedSprite.setPosition({ contBox.getPosition().x + contBox.getLocalBounds().width, contBox.getPosition().y });
	ravelledSprite.setPosition({ contBox.getPosition().x + contBox.getLocalBounds().width, contBox.getPosition().y });
}

comboBox::comboBox(sf::Vector2f pos, int fontSz, sf::Font& fnt, float initWidth = -1) {
	if (initWidth != -1) { sizeAdjustable = false; }
	cbPosition = pos;
	width = initWidth;
	fontSize = fontSz;

	dropTexture.loadFromFile("resources\\dropbox.png");

	ravelledSprite.setTexture(dropTexture);
	ravelledSprite.setTextureRect({ 16,0,16,16 });
	droppedSprite.setTexture(dropTexture);
	droppedSprite.setTextureRect({ 0,0,16,16 });
	text.setString("");
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(18);

	//ravelledSprite.setPosition({ pos.x + width, pos.y });
	//droppedSprite.setPosition({ pos.x + width, pos.y });
	text.setPosition(pos);

	contBox.setSize({ initWidth,0 });
	contBox.setPosition(pos);
	contBox.setOutlineThickness(1);
	contBox.setOutlineColor(sf::Color::Black);
	text.setFont(fnt);
	font = fnt;
}


void comboBox::pushback(std::string string) {
	if (string.length() > highestLength && sizeAdjustable) {
		highestLength = string.length();
		width = static_cast<float>(fontSize) * (static_cast<float>(highestLength) / 2);
	}
	contents.push_back(SelectableText(width, string, fontSize, font));
	numberOfItems++;
	contents[numberOfItems - 1].setPosition({ cbPosition.x, cbPosition.y + 20 * (numberOfItems) });
	contents[numberOfItems - 1].setActivity(isDropped);
	this->updateSize();

}
void comboBox::popback() {
	contents.pop_back();
	numberOfItems--;
	this->updateSize();
}
void comboBox::clear() {
	contents.clear();
	numberOfItems = 0;
	highestLength = 0;
	selection = -1;
	text.setString("");
}

char comboBox::isMouseOver(sf::RenderWindow& window) {

	float mouseX = sf::Mouse::getPosition(window).x;
	float mouseY = sf::Mouse::getPosition(window).y;

	float btnPosX = droppedSprite.getPosition().x;
	float btnPosY = droppedSprite.getPosition().y;

	float btnxPosWidth = droppedSprite.getPosition().x + droppedSprite.getLocalBounds().width;
	float btnyPosHeight = droppedSprite.getPosition().y + droppedSprite.getLocalBounds().height;

	if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {

		if (isDropped) {
			isDropped = false;
			ravelUp();
		}
		else {
			isDropped = true;
			dropDown();
		}
	}



	for (int i = 0; i < numberOfItems; i++) {
		if (contents[i].isMouseOver(window)) {
			for (int ii = 0; ii < numberOfItems; ii++) {
				if (i != ii) { contents[ii].stopSelection(); }
			}
			if (selection != i) {
				selection = i;
				text.setString(contents[i].getString());
				return selection;
			}
			return -2;
		}
	}
	return -1;
}

void comboBox::setSelection(int a) {
	for (int ii = 0; ii < numberOfItems; ii++) {
		if (a != ii) { contents[ii].stopSelection(); }
	}
	if (numberOfItems > 0) {
		selection = a;
		contents[a].setSelection();
	}
	text.setString(contents[a].getString());
}

void comboBox::draw(sf::RenderWindow& window) {
	if (numberOfItems > 0) {
		window.draw(contBox);
		for (int i = 0; i < numberOfItems; i++) {
			contents[i].draw(window);
		}
	}
	if (isDropped) { window.draw(droppedSprite); }
	else { window.draw(ravelledSprite); }
	window.draw(text);
}

void comboBox::setPosition(sf::Vector2f pos) {
	cbPosition = pos;
	contBox.setPosition(pos);
	for (int i = 0; i < numberOfItems; i++) {
		contents[i].setPosition({ pos.x, pos.y + 20 * i });
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~ checkBox ~~~~~~~~~~~~~~~~~~~~~~~~
checkBox::checkBox(std::string string, sf::Vector2f pos, sf::Font& font, char relation, sf::Vector2f posDif) {

	if (!checkTexture.loadFromFile("checkbox.png")) {
		std::cout << "image load failed " << std::endl;
	}


	uncheckedSprite.setTexture(checkTexture);
	uncheckedSprite.setTextureRect({ 0,0,16,16 });
	checkedSprite.setTexture(checkTexture);
	checkedSprite.setTextureRect({ 16,0,16,16 });
	text.setString(string);
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(18);

	uncheckedSprite.setPosition(pos);
	checkedSprite.setPosition(pos);
	text.setPosition({ pos.x + 20, pos.y - 4 });

	text.setFont(font);
	setPosition(pos, relation, posDif);
}

checkBox::checkBox(std::string string, sf::Vector2f pos, sf::Font& font, char relation, std::string fileLocation, sf::Vector2i spriteSize, sf::Vector2f posDif) {

	if (!checkTexture.loadFromFile(fileLocation)) {
		std::cout << "image load failed " << std::endl;
	}

	uncheckedSprite.setTexture(checkTexture);
	uncheckedSprite.setTextureRect({ 0,0,spriteSize.x, spriteSize.y });
	checkedSprite.setTexture(checkTexture);
	checkedSprite.setTextureRect({ spriteSize.x,0,spriteSize.x,spriteSize.y });
	text.setString(string);
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(18);

	text.setFont(font);
	setPosition(pos, relation, posDif);
}



void checkBox::setPosition(sf::Vector2f pos, char relation, sf::Vector2f posDif = { 100,0 }) {
	position = pos;
	relPos = relation;
	posDiff = posDif;

	float offset = (text.getLocalBounds().left + text.getLocalBounds().width + 5);

	switch (relPos) {
	case 0:
		//left
		//label.setPosition({ pos.x - (stringLength * 2 / 3 * fontSize), pos.y });

		text.setPosition({ pos.x - offset + posDif.x, pos.y - 5 + posDif.y });
		checkedSprite.setPosition({ pos.x, pos.y });
		uncheckedSprite.setPosition({ pos.x, pos.y });
		break;

	case 1:
		//above
		text.setPosition({ pos.x,pos.y - 20 });
		checkedSprite.setPosition({ pos.x, pos.y });
		uncheckedSprite.setPosition({ pos.x, pos.y});
		break;

	case 2:
		//right
		text.setPosition({ pos.x + posDif.x + 2, pos.y - 4 });
		checkedSprite.setPosition({ pos.x, pos.y });
		uncheckedSprite.setPosition({ pos.x, pos.y });
		break;

	case 3:
		//below
		text.setPosition({ pos.x,pos.y + 20 });
		checkedSprite.setPosition({ pos.x,pos.y});
		uncheckedSprite.setPosition({ pos.x,pos.y});
		break;
	case 4:
		//no adjust CENTERED??
		text.setPosition(pos);
		checkedSprite.setPosition(pos);
		uncheckedSprite.setPosition(pos);
		break;
	}
}

void checkBox::isMouseOver(sf::RenderWindow& window) {
	float mouseX = sf::Mouse::getPosition(window).x;
	float mouseY = sf::Mouse::getPosition(window).y;

	float btnPosX = checkedSprite.getPosition().x;
	float btnPosY = checkedSprite.getPosition().y;

	float btnxPosWidth = checkedSprite.getPosition().x + checkedSprite.getLocalBounds().width;
	float btnyPosHeight = checkedSprite.getPosition().y + checkedSprite.getLocalBounds().height;

	if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {

		if (isChecked) {
			isChecked = false;
		}
		else {
			isChecked = true;
		}
	}
}
void checkBox::draw(sf::RenderWindow& window) {
	if (isChecked) { window.draw(checkedSprite); }
	else { window.draw(uncheckedSprite); }
	window.draw(text);
}
void checkBox::setChecked(bool checked) {
	isChecked = checked;
}
