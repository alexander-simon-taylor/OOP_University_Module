// ConsoleApplication6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class card
{
private:
	int suit{};
	std::string colour{};
	int value{};
public:
	card(int suit_input, int value_input) :
		value{ value_input }, suit{ suit_input }, colour{} {
		if (suit == 2 || suit == 3) {
			colour = "black";
			//2 - spades, 3 - clubs
		} else {
			colour = "red";
			//4 - diamonds, 5 - hearts
			// The counting starts at 2 to match the position of the foundation piles of each
		}
	};
	card(card* input_card) :
		suit{ input_card->suit }, colour{ input_card->colour }, value{ input_card->value } {}
	int get_suit() { return suit; }
	int get_value() { return value; }
	std::string get_colour() { return colour; }
	~card() {};
};

class face_up_stack;

class card_collection
{
public:
	virtual ~card_collection() {};
	virtual card_collection* split(int first_card_new_stack) = 0;
	virtual card_collection* merge(face_up_stack* incoming_stack) = 0;
	virtual card* get_card_info(int i) = 0;
	virtual int get_stack_height() = 0;
	virtual std::vector<card*> get_card_list() = 0;
};

class face_up_stack : public card_collection
{
private:
	int stack_height{};
	std::vector<card*> card_list{};
public:
	face_up_stack() = default;
	face_up_stack(card* starting_card) :
		card_list{ 1 }, stack_height{ 1 } {
			card_list[0] = starting_card;
		};
	face_up_stack(int first_card, int last_card, std::vector<card*> larger_stack):
		stack_height{ last_card - first_card + 1 }, card_list{ stack_height } {
			for (int i{ first_card }; i <= last_card; i++) {
				card_list[i - first_card] = larger_stack[i];
			}
		};
	face_up_stack(card_collection* existing_stack):
		stack_height{ existing_stack->get_stack_height() }, card_list{existing_stack->get_card_list()} {}
	~face_up_stack() {};
	face_up_stack* merge(face_up_stack* incoming_stack) {
		card_list.resize(stack_height + incoming_stack->stack_height);
		for (int i{}; i < incoming_stack->stack_height; i++) {
			card_list[i + stack_height] = incoming_stack->card_list[i];
		}
		stack_height += incoming_stack->stack_height;
		return this;
	};
	face_up_stack* split(int first_card_new_stack) {
		face_up_stack new_stack{ first_card_new_stack, stack_height - 1, card_list };
		card_list.resize(stack_height - first_card_new_stack - 1);
		return &new_stack;
	};
	card* get_card_info(int i) { return card_list[i]; }
	int get_stack_height() { return stack_height; }
	std::vector<card*> get_card_list() { return card_list; }
};

class foundation_pile : public card_collection
{
private:
	int stack_height{0};
	std::vector<card*> card_list{0};
public:
	foundation_pile() = default;
	~foundation_pile() {};
	foundation_pile* merge(face_up_stack* incoming_stack) {
		card_list.resize(stack_height + 1);
		card_list[stack_height + 1] = incoming_stack->get_card_info(0);
		stack_height++;
		return this;
	};
	face_up_stack* split(int first_card_new_stack) {
		face_up_stack new_stack{ stack_height - 2, stack_height - 1, card_list };
		card_list.resize(stack_height - 1);
		return &new_stack;
	};
	card* get_card_info(int i) { return card_list[i]; }
	int get_stack_height() { return stack_height; }
	std::vector<card*> get_card_list() { return card_list; }
};

class face_down_collection : public card_collection
{
private:
	int stack_height{0};
	std::vector<card*> card_list{ 0 };
public:
	face_down_collection() = default;
	// The default constructor is used once - the face down collection pile for the lower deck part with only the face up card.
	// Creating this with zero length keeps the arrays nicely organised later
	face_down_collection(int first_card, int last_card, std::vector<card*> larger_stack) :
		stack_height{ last_card - first_card + 1 }, card_list(stack_height) {
		for (int i{ first_card }; i <= last_card; i++) {
			card_list[i - first_card] = larger_stack[i];
		}
	};
	~face_down_collection() {};
	face_up_stack* split(int first_card_new_stack) {
		face_up_stack new_stack{ card_list.back() };
		card_list.resize(stack_height - 1);
		return &new_stack;
	};
	face_up_stack* merge(face_up_stack* incoming_stack) {
		return incoming_stack;
	};
	// Merge is never used for face-down cards, so this function definition is purely to get the code to compile
	card* get_card_info(int i) { return card_list[i]; }
	int get_stack_height() { return stack_height; }
	std::vector<card*> get_card_list() { return card_list; }
};

class board
{
private:
	std::vector<std::vector <card_collection*>> board_elements;
	int score{};
	int discard_reveal_num{};
	int active_row{2};
	int active_column{ 0 };
	int active_sub_row{0};
	int selected_row{-1};
	int selected_column{-1};
	int selected_sub_row{-1};
	std::vector<face_down_collection> face_down_collection_vector{ 8 };
	std::vector<foundation_pile> foundation_pile_vector{ 5 };
	std::vector<face_up_stack> face_up_stack_vector{ 7 };
public:
	board();
	~board() {};
	void draw() {};
	// function that draws the current card setup
	bool discard_to_foundation(int suit_index);
	// function that moves one card from the discard pile to the foundation, the four ordered piles
	bool discard_to_working(int working_index);
	// function that moves one card from the discard pile to the working area
	bool working_to_foundation(int working_index, int suit_index);
	bool foundation_to_working(int working_index, int suit_index);
	bool working_to_working(int first_card_new_stack, int working_initial_index, int working_final_index);
	void reveal_working(int working_index);
	bool reset_discard();
	bool reveal_discard();
	bool keyboard_input();
	std::vector<card*> permute_deck();
};

board::board() :
	discard_reveal_num{}, score{ 0 }, board_elements(3), active_row{ 2 }, active_sub_row{ 0 }, active_column{ 0 },
	selected_column{ -1 }, selected_row{ -1 }, selected_sub_row{ -1 }, face_down_collection_vector{ 8 },
	foundation_pile_vector{ 5 }, face_up_stack_vector{ 7 }
	// Board elements' three parts are for the upper deck parts, the face down lower deck parts, and the face up lower deck parts
{
	bool invalid_entry{ true };
	std::string input_string{};
	while (invalid_entry) {
		std::cout << "Enter the number of cards you wish to draw to the discard pile each time, either 1 or 3." << std::endl;
		std::getline(std::cin,input_string);
		if (input_string == "1") {
			discard_reveal_num = 1;
			invalid_entry = false;
		} else if (input_string == "3") {
			discard_reveal_num = 3;
			invalid_entry = false;
		}
	}
	std::vector<card*> card_deck{ permute_deck() };
	// Initialising the upper deck elements, i.e. face down discard, face up discard, four foundation decks
	face_down_collection face_down_discard{ 0,23,card_deck };
	board_elements[0].push_back(&face_down_discard);
	face_down_collection_vector[0] = face_down_discard;
	int assigned_cards{ 24 };

	foundation_pile face_up_discard{};
	foundation_pile_vector[0] = face_up_discard;
	foundation_pile spades_foundation{};
	foundation_pile_vector[1] = spades_foundation;
	foundation_pile clubs_foundation{};
	foundation_pile_vector[2] = clubs_foundation;
	foundation_pile diamonds_foundation{};
	foundation_pile_vector[3] = diamonds_foundation;
	foundation_pile hearts_foundation{};
	foundation_pile_vector[4] = hearts_foundation;
	for (int i{ 0 }; i < 5; i++) {
		board_elements[0].push_back(&foundation_pile_vector[i]);
	}

	face_down_collection column_0_face_down_stack{};
	board_elements[1].push_back(&column_0_face_down_stack);
	face_up_stack column_0_face_up_stack{ card_deck[assigned_cards] };
	board_elements[2].push_back(&column_0_face_up_stack);
	assigned_cards++;

	face_down_collection column_1_face_down_stack{assigned_cards,assigned_cards + 1, card_deck};
	board_elements[1].push_back(&column_1_face_down_stack);
	assigned_cards += 1;
	face_up_stack column_1_face_up_stack{ card_deck[assigned_cards] };
	board_elements[2].push_back(&column_1_face_up_stack);
	assigned_cards++;

	face_down_collection column_2_face_down_stack{ assigned_cards,assigned_cards + 2, card_deck };
	board_elements[1].push_back(&column_2_face_down_stack);
	assigned_cards += 2;
	face_up_stack column_2_face_up_stack{ card_deck[assigned_cards] };
	board_elements[2].push_back(&column_2_face_up_stack);
	assigned_cards++;

	face_down_collection column_3_face_down_stack{ assigned_cards,assigned_cards + 3, card_deck };
	board_elements[1].push_back(&column_3_face_down_stack);
	assigned_cards += 3;
	face_up_stack column_3_face_up_stack{ card_deck[assigned_cards] };
	board_elements[2].push_back(&column_3_face_up_stack);
	assigned_cards++;

	face_down_collection column_4_face_down_stack{ assigned_cards,assigned_cards + 4, card_deck };
	board_elements[1].push_back(&column_4_face_down_stack);
	assigned_cards += 4;
	face_up_stack column_4_face_up_stack{ card_deck[assigned_cards] };
	board_elements[2].push_back(&column_4_face_up_stack);
	assigned_cards++;

	face_down_collection column_5_face_down_stack{ assigned_cards,assigned_cards + 5, card_deck };
	board_elements[1].push_back(&column_5_face_down_stack);
	assigned_cards += 5;
	face_up_stack column_5_face_up_stack{ card_deck[assigned_cards] };
	board_elements[2].push_back(&column_5_face_up_stack);
	assigned_cards++;

	face_down_collection column_6_face_down_stack{ assigned_cards,assigned_cards + 6, card_deck };
	board_elements[1].push_back(&column_6_face_down_stack);
	assigned_cards += 6;
	face_up_stack column_6_face_up_stack{ card_deck[assigned_cards] };
	board_elements[2].push_back(&column_6_face_up_stack);
	assigned_cards++;
}

bool board::reveal_discard() {
	bool success{ false };
	std::vector<card_collection*> top_row{ board_elements[0] };
	int face_down_stack_height{ top_row[0]->get_stack_height() };
	if (face_down_stack_height > 0) {
		success = true;
		if (face_down_stack_height > discard_reveal_num) {
			face_up_stack new_stack{ top_row[0]->split(face_down_stack_height - discard_reveal_num) };
			top_row[1] = top_row[1]->merge(&new_stack);
		} else {
			face_up_stack new_stack{ top_row[0]->split(0) };
			top_row[1] = top_row[1]->merge(&new_stack);
		}
	}
	return success;
}

bool board::discard_to_working(int working_index) {
	bool success{ false };
	std::vector<card_collection*> top_row{ board_elements[0] };
	std::vector<card_collection*> bottom_row{ board_elements[2] };
	card* discard_card{ top_row[1]->get_card_info(top_row[1]->get_stack_height() - 1) };
	if (bottom_row[working_index]->get_stack_height() != 0) {
		card* accepting_card{ bottom_row[working_index]->get_card_info(bottom_row[working_index]->get_stack_height() - 1) };
		if ((discard_card->get_value() == accepting_card->get_value() - 1) && (discard_card->get_colour() != accepting_card->get_colour())) {
			face_up_stack new_stack{ top_row[1]->split(top_row[1]->get_stack_height() - 1) };
			bottom_row[working_index] = bottom_row[working_index]->merge(&new_stack);
			success = true;
		}
	} else if (discard_card->get_value() == 13) {
		face_up_stack new_stack{ top_row[1]->split(top_row[1]->get_stack_height() - 1) };
		bottom_row[working_index] = bottom_row[working_index]->merge(&new_stack);
		success = true;
	}
	return success;
}

bool board::discard_to_foundation(int suit_index) {
	//2,3,4,5 are the foundation piles
	//Suits: 2 - spades, 3 - clubs, 4 - diamonds, 5 - hearts
	bool success{ false };
	std::vector<card_collection*> top_row{ board_elements[0] };
	card* discard_card{ top_row[1]->get_card_info(top_row[1]->get_stack_height() - 1) };
	if (discard_card->get_suit() == suit_index && (discard_card->get_value() == top_row[suit_index]->get_stack_height() - 1)) {
		face_up_stack new_stack{ top_row[1]->split(top_row[1]->get_stack_height() - 1) };
		top_row[suit_index] = top_row[suit_index]->merge(&new_stack);
		success = true;
	}
	return success;
}

bool board::working_to_foundation(int working_index, int suit_index) {
	bool success{ false };
	std::vector<card_collection*> top_row{ board_elements[0] };
	std::vector<card_collection*> bottom_row{ board_elements[2] };
	card* working_card{ bottom_row[working_index]->get_card_info(bottom_row[working_index]->get_stack_height() - 1) };
	if (working_card->get_suit() == suit_index && (working_card->get_value() == top_row[suit_index]->get_stack_height() - 1)) {
		face_up_stack new_stack{ bottom_row[working_index]->split(bottom_row[working_index]->get_stack_height() - 1) };
		top_row[suit_index] = top_row[suit_index]->merge(&new_stack);
		success = true;
	}
	return success;
}

bool board::foundation_to_working(int working_index, int suit_index) {
	bool success{ false };
	std::vector<card_collection*> top_row{ board_elements[0] };
	std::vector<card_collection*> bottom_row{ board_elements[2] };
	card* foundation_card{ top_row[suit_index]->get_card_info(top_row[suit_index]->get_stack_height() - 1) };
	if (bottom_row[suit_index]->get_stack_height() != 0) {
		card* working_card{ bottom_row[working_index]->get_card_info(bottom_row[working_index]->get_stack_height() - 1) };
		if (foundation_card->get_colour() != working_card->get_colour() && (foundation_card->get_value() == working_card->get_value() - 1)) {
			face_up_stack new_stack{ top_row[suit_index]->split(0) };
			bottom_row[working_index] = bottom_row[working_index]->merge(&new_stack);
			success = true;
		}
	} else if (foundation_card->get_value() == 13) {
		face_up_stack new_stack{ top_row[suit_index]->split(0) };
		bottom_row[working_index] = bottom_row[working_index]->merge(&new_stack);
		success = true;
	}
	return success;
}

bool board::working_to_working(int first_card_new_stack, int working_initial_index, int working_final_index) {
	bool success{ false };
	std::vector<card_collection*> bottom_row{ board_elements[2] };
	card* top_moving_card{ bottom_row[working_initial_index]->get_card_info(first_card_new_stack) };
	if (bottom_row[working_final_index]->get_stack_height() != 0) {
		card* accepting_card{ bottom_row[working_final_index]->get_card_info(bottom_row[working_final_index]->get_stack_height() - 1) };
		if ((top_moving_card->get_value() == accepting_card->get_value() - 1) && (top_moving_card->get_colour() != accepting_card->get_colour())) {
			face_up_stack new_stack{ bottom_row[working_initial_index]->split(bottom_row[working_initial_index]->get_stack_height() - 1) };
			bottom_row[working_final_index] = bottom_row[working_final_index]->merge(&new_stack);
			success = true;
		}
	} else if (top_moving_card->get_value() == 13) {
		face_up_stack new_stack{ bottom_row[working_initial_index]->split(bottom_row[working_initial_index]->get_stack_height() - 1) };
		bottom_row[working_final_index] = bottom_row[working_final_index]->merge(&new_stack);
		success = true;
	}
	return success;
}

void board::reveal_working(int working_index) {
	// This function is a void, because if it is called the board is being redrawn regardless of the outcome of this function
	bool success{ false };
	std::vector<card_collection*> middle_row{ board_elements[1] };
	std::vector<card_collection*> bottom_row{ board_elements[2] };
	if (bottom_row[working_index]->get_stack_height() == 0 && middle_row[working_index]->get_stack_height() != 0) {
		face_up_stack new_stack{ middle_row[working_index]->split(middle_row[working_index]->get_stack_height() - 1) };
		bottom_row[working_index] = bottom_row[working_index]->merge(&new_stack);
	}
	return;
}

bool board::reset_discard() {
	bool success{ false };
	std::vector<card_collection*> top_row{ board_elements[0] };
	int stack_height{ top_row[1]->get_stack_height() };
	std::cout << top_row[1]->get_stack_height() << std::endl;
	if (top_row[1]->get_stack_height() != 0) {
		face_up_stack discard_pile{0,top_row[1]->get_stack_height() - 1,top_row[1]->get_card_list()};
		top_row[0] = top_row[0]->merge(&discard_pile);
		success = true;
	}
	return success;
}

std::vector<card*> board::permute_deck() {
	std::vector<card*> card_deck(52);
	for (int suit{ 0 }; suit < 4; suit++) {
		for (int value{ 0 }; value < 13; value++) {
			card_deck[suit * 13 + value] = &card(suit, value);
		}
	}
	std::random_shuffle(card_deck.begin(), card_deck.end());
	return card_deck;
}

bool board::keyboard_input() {
	// A function that asks for keyboard input, and changes the board correspondingly.
	// A 'true' return of this function makes the game board re-draw, a 'false' return doesn't
	// The 'return false;' at the end should never be used, but better safe than sorry
	bool invalid_input{ true };
	while (invalid_input) {
		std::string input_string;
		std::cout << "Enter a command to begin. Enter 'help' for a list of commands." << std::endl;
		std::cin >> input_string;
		if (input_string == "help") {
			std::cout << "List of commands:" << std::endl;
			std::cout << "'exit', exits the game." << std::endl;
			std::cout << "'reveal', reveals either one or three cards from the discard pile." << std::endl;
			std::cout << "'reset', resets the discard pile." << std::endl;
			std::cout << "'select', selects the current pile." << std::endl;
			std::cout << "'up', moves the selection to the top row." << std::endl;
			std::cout << "'down', moves the selection to the bottom row." << std::endl;
			std::cout << "'w', makes the selection include one less card in its pile." << std::endl;
			std::cout << "'s', makes the selection include one more card in its pile." << std::endl;
			std::cout << "'a', moves the selection one stack to the left." << std::endl;
			std::cout << "'d', moves the selection one stack to the right." << std::endl;
		} else if (input_string == "exit") {
			return false;
		} else if (input_string == "reveal") {
			if (reveal_discard()) {
				return true;
			}
		} else if (input_string == "reset") {
			if (reset_discard()) {
				score -= 100;
				return true;
			}
		} else if (input_string == "up") {
			if (active_row == 2) {
				// If you're on the bottom row
				std::vector<card_collection*> top_row{ board_elements[0] };
				for (int i{ 1 }; i < 6; i++) {
					if (top_row[i]->get_stack_height() != 0) {
						active_row = 0;
						active_sub_row = top_row[i]->get_stack_height() - 1;
						return true;
					}
				}
			} else { std::cout << "Redundant command, game has not been redrawn." << std::endl; }
		} else if (input_string == "down") {
			if (active_row == 0) {
				// If you're on the top row
				std::vector<card_collection*> bottom_row{ board_elements[2] };
				for (int i{ 1 }; i < 6; i++) {
					if (bottom_row[i]->get_stack_height() != 0) {
						active_row = 2;
						active_sub_row = 0;
						return true;
					}
				}
			} else { std::cout << "Redundant command, game has not been redrawn." << std::endl; }
		} else if (input_string == "d") {
			if (active_row == 0) {
				// You're on the top row, discard pile is 1, suit piles are 2,3,4,5.
				std::vector<card_collection*> top_row{ board_elements[0] };
				if (active_column != 5) {
					int new_active_column{ active_column + 1 };
					while (new_active_column <= 5) {
						if (top_row[new_active_column]->get_stack_height() != 0) {
							active_column = new_active_column;
							return true;
						} else { new_active_column++; }
					}
				} else { std::cout << "Redundant command, game has not been redrawn." << std::endl; }
			} else {
				// You're on the bottom row, piles are 0,1,2,3,4,5,6.
				std::vector<card_collection*> bottom_row{ board_elements[2] };
				if (active_column != 6) {
					int new_active_column{ active_column + 1 };
					while (new_active_column <= 6) {
						if (bottom_row[new_active_column]->get_stack_height() != 0) {
							active_column = new_active_column;
							active_sub_row = 0;
							return true;
						} else { new_active_column++; }
					}
				} else { std::cout << "Redundant command, game has not been redrawn." << std::endl; }
			}
		} else if (input_string == "a") {
			if (active_row == 0) {
				// You're on the top row, discard pile is 1, suit piles are 2,3,4,5.
				std::vector<card_collection*> top_row{ board_elements[0] };
				if (active_column != 1) {
					int new_active_column{ active_column - 1 };
					while (new_active_column >= 1) {
						if (top_row[new_active_column]->get_stack_height() != 0) {
							active_column = new_active_column;
							return true;
						} else { new_active_column--; }
					}
				} else { std::cout << "Redundant command, game has not been redrawn." << std::endl; }
			} else {
				// You're on the bottom row, piles are 0,1,2,3,4,5,6.
				std::vector<card_collection*> bottom_row{ board_elements[2] };
				if (active_column != 0) {
					int new_active_column{ active_column - 1 };
					while (new_active_column >= 0) {
						if (bottom_row[new_active_column]->get_stack_height() != 0) {
							active_column = new_active_column;
							active_sub_row = 0;
							return true;
						} else { new_active_column--; }
					}
				} else { std::cout << "Redundant command, game has not been redrawn." << std::endl; }
			}
		} else if (input_string == "w") {
			if (active_row == 2) {
				if (active_sub_row > 0) {
					active_sub_row--;
					return true;
				}
			} else { std::cout << "Redundant command, game has not been redrawn." << std::endl; }
		}
		else if (input_string == "s") {
			if (active_row == 2) {
				std::vector<card_collection*> bottom_row{ board_elements[2] };
				if (active_sub_row < bottom_row[active_column]->get_stack_height()) {
					active_sub_row++;
					return true;
				}
			} else { std::cout << "Redundant command, game has not been redrawn." << std::endl; }
		} else if (input_string == "deselect") {
			selected_row = -1;
			selected_column = -1;
			selected_sub_row = -1;
			return true;
		} else if (input_string == "select") {
			if (selected_row < 0) {
				// Meaning a pile hasn't been selected yet
				selected_row = active_row;
				selected_column = active_column;
				selected_sub_row = active_sub_row;
				return true;
			} else if (selected_row != active_row && selected_column != active_column) {
				// Meaning if you're not trying to put a stack on top of itself, or a substack of it
				if (selected_row == 0) {
					// You're moving a card from either the discard or foundation
					if (selected_column == 1) {
						// From the discard
						if (active_row == 0) {
							// To the foundation
							if (discard_to_foundation(active_column)) {
								score += 10;
								return true;
							}
						} else {
							// To the working area
							if (discard_to_working(active_column)) {
								score += 5;
								return true;
							}
						}
					} else {
						// From the foundation to the working area
						if (foundation_to_working(active_column, selected_column)) {
							score -= 10;
							return true;
						}
					}
				} else {
					// You're moving a pile from the working area
					if (active_row == 0) {
						// To the foundation
						if (working_to_foundation(selected_column, active_column)) {
							score += 10;
							reveal_working(selected_column);
							return true;
						}
					} else {
						// To another working pile
						if (working_to_working(active_sub_row, selected_column, active_column)) {
							reveal_working(selected_column);
							return true;
						}
					}
				}
			}
		} else { std::cout << "Unrecognised command. Please refer to 'help' for a list of commands." << std::endl; }
	}
	return false;
}

int main()
{
    std::cout << "Hello World!\n";
	bool game_check{ true };
	board game_board{};
	while (game_check) {
		game_check = game_board.keyboard_input();
	}
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
