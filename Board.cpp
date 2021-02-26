#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include "Card_header.h"
#include "Board.h"

board::board() :
	discard_reveal_num{}, score{ 0 }, board_elements(3), active_row{ 2 }, active_sub_row{ 0 }, active_column{ 0 },
	selected_column{ -1 }, selected_row{ -1 }, selected_sub_row{ -1 },
	card_deck{ permute_deck() },
	face_down_discard{ 0,23,card_deck },
	face_up_discard{ 0 },
	spades_foundation{ 0 },
	clubs_foundation{ 0 },
	diamonds_foundation{ 0 },
	hearts_foundation{ 0 },
	column_0_face_down_stack{ 0 },
	column_1_face_down_stack{ 24,24 , card_deck },
	column_2_face_down_stack{ 25,26 , card_deck },
	column_3_face_down_stack{ 27,29 , card_deck },
	column_4_face_down_stack{ 30,33 , card_deck },
	column_5_face_down_stack{ 34,38 , card_deck },
	column_6_face_down_stack{ 39,44 , card_deck },
	column_0_face_up_stack{ 45,45 , card_deck },
	column_1_face_up_stack{ 46,46 , card_deck },
	column_2_face_up_stack{ 47,47 , card_deck },
	column_3_face_up_stack{ 48,48 , card_deck },
	column_4_face_up_stack{ 49,49 , card_deck },
	column_5_face_up_stack{ 50,50 , card_deck },
	column_6_face_up_stack{ 51,51 , card_deck }
	// Board elements' three parts are for the upper deck parts, the face down lower deck parts, and the face up lower deck parts
{
	bool invalid_entry{ true };
	std::string input_string{};
	while (invalid_entry) {
		std::cout << "Enter the number of cards you wish to draw to the discard pile each time, either 1 or 3." << std::endl;
		std::getline(std::cin, input_string);
		if (input_string == "1") {
			discard_reveal_num = 1;
			invalid_entry = false;
		} else if (input_string == "3") {
			discard_reveal_num = 3;
			invalid_entry = false;
		}
	}
	// Initialising the upper deck elements, i.e. face down discard, face up discard, four foundation decks
	board_elements[0].push_back(&face_down_discard);

	board_elements[0].push_back(&face_up_discard);
	board_elements[0].push_back(&spades_foundation);
	board_elements[0].push_back(&clubs_foundation);
	board_elements[0].push_back(&diamonds_foundation);
	board_elements[0].push_back(&hearts_foundation);

	board_elements[1].push_back(&column_0_face_down_stack);
	board_elements[1].push_back(&column_1_face_down_stack);
	board_elements[1].push_back(&column_2_face_down_stack);
	board_elements[1].push_back(&column_3_face_down_stack);
	board_elements[1].push_back(&column_4_face_down_stack);
	board_elements[1].push_back(&column_5_face_down_stack);
	board_elements[1].push_back(&column_6_face_down_stack);
	board_elements[2].push_back(&column_0_face_up_stack);
	board_elements[2].push_back(&column_1_face_up_stack);
	board_elements[2].push_back(&column_2_face_up_stack);
	board_elements[2].push_back(&column_3_face_up_stack);
	board_elements[2].push_back(&column_4_face_up_stack);
	board_elements[2].push_back(&column_5_face_up_stack);
	board_elements[2].push_back(&column_6_face_up_stack);
}

bool board::reveal_discard() {
	bool success{ false };
	std::vector<card_collection*> top_row{ board_elements[0] };
	int face_down_stack_height{ top_row[0]->get_stack_height() };
	if (face_down_stack_height > 0) {
		success = true;
		face_up_stack temp_stack{};
		if (face_down_stack_height > discard_reveal_num) {
			temp_stack = top_row[0]->split(face_down_stack_height - discard_reveal_num, temp_stack);
			top_row[1] = top_row[1]->merge(temp_stack);
		} else {
			face_up_stack new_stack{ top_row[0]->split(0, temp_stack) };
			top_row[1] = top_row[1]->merge(temp_stack);
		}
	}
	return success;
}

bool board::discard_to_working(int working_index) {
	bool success{ false };
	std::vector<card_collection*> top_row{ board_elements[0] };
	std::vector<card_collection*> bottom_row{ board_elements[2] };
	card discard_card{ top_row[1]->get_card_info(top_row[1]->get_stack_height() - 1).get_suit() ,top_row[1]->get_card_info(top_row[1]->get_stack_height() - 1).get_value() };
	face_up_stack temp_stack{};
	if (bottom_row[working_index]->get_stack_height() != 0) {
		card accepting_card{ bottom_row[working_index]->get_card_info(bottom_row[working_index]->get_stack_height() - 1).get_suit(), bottom_row[working_index]->get_card_info(bottom_row[working_index]->get_stack_height() - 1).get_value() };
		if ((discard_card.get_value() == accepting_card.get_value() - 1) && (discard_card.get_colour() != accepting_card.get_colour())) {
			temp_stack = top_row[1]->split(top_row[1]->get_stack_height() - 1, temp_stack);
			bottom_row[working_index] = bottom_row[working_index]->merge(temp_stack);
			success = true;
		}
	} else if (discard_card.get_value() == 13) {
		temp_stack = top_row[1]->split(top_row[1]->get_stack_height() - 1, temp_stack);
		bottom_row[working_index] = bottom_row[working_index]->merge(temp_stack);
		success = true;
	}
	return success;
}

bool board::discard_to_foundation(int suit_index) {
	//2,3,4,5 are the foundation piles
	//Suits: 2 - spades, 3 - clubs, 4 - diamonds, 5 - hearts
	bool success{ false };
	std::vector<card_collection*> top_row{ board_elements[0] };
	card discard_card{ top_row[1]->get_card_info(top_row[1]->get_stack_height() - 1).get_suit(), top_row[1]->get_card_info(top_row[1]->get_stack_height() - 1).get_value() };
	face_up_stack temp_stack{};
	if (discard_card.get_suit() == suit_index && (discard_card.get_value() == top_row[suit_index]->get_stack_height() + 1)) {
		temp_stack = top_row[1]->split(top_row[1]->get_stack_height() - 1, temp_stack);
		top_row[suit_index] = top_row[suit_index]->merge(temp_stack);
		success = true;
	}
	return success;
}

bool board::working_to_foundation(int working_index, int suit_index) {
	bool success{ false };
	std::vector<card_collection*> top_row{ board_elements[0] };
	std::vector<card_collection*> bottom_row{ board_elements[2] };
	card working_card{ bottom_row[working_index]->get_card_info(bottom_row[working_index]->get_stack_height() - 1).get_suit(), bottom_row[working_index]->get_card_info(bottom_row[working_index]->get_stack_height() - 1).get_value() };
	face_up_stack temp_stack{};
	if (working_card.get_suit() == suit_index && working_card.get_value() == top_row[suit_index]->get_stack_height() + 1) {
		temp_stack = bottom_row[working_index]->split(bottom_row[working_index]->get_stack_height() - 1, temp_stack);
		top_row[suit_index] = top_row[suit_index]->merge(temp_stack);
		success = true;
	}
	return success;
}

bool board::foundation_to_working(int working_index, int suit_index) {
	bool success{ false };
	std::vector<card_collection*> top_row{ board_elements[0] };
	std::vector<card_collection*> bottom_row{ board_elements[2] };
	card foundation_card{ top_row[suit_index]->get_card_info(top_row[suit_index]->get_stack_height() - 1).get_suit(), top_row[suit_index]->get_card_info(top_row[suit_index]->get_stack_height() - 1).get_value() };
	face_up_stack temp_stack{};
	if (bottom_row[suit_index]->get_stack_height() != 0) {
		card working_card{ bottom_row[working_index]->get_card_info(bottom_row[working_index]->get_stack_height() - 1).get_suit(), bottom_row[working_index]->get_card_info(bottom_row[working_index]->get_stack_height() - 1).get_value() };
		if (foundation_card.get_colour() != working_card.get_colour() && (foundation_card.get_value() == working_card.get_value() - 1)) {
			temp_stack = top_row[suit_index]->split(top_row[suit_index]->get_stack_height() - 1, temp_stack);
			bottom_row[working_index] = bottom_row[working_index]->merge(temp_stack);
			success = true;
		}
	}
	else if (foundation_card.get_value() == 13) {
		temp_stack = top_row[suit_index]->split(top_row[suit_index]->get_stack_height(), temp_stack);
		bottom_row[working_index] = bottom_row[working_index]->merge(temp_stack);
		success = true;
	}
	return success;
}

bool board::working_to_working(int first_card_new_stack, int working_initial_index, int working_final_index) {
	bool success{ false };
	std::vector<card_collection*> bottom_row{ board_elements[2] };
	card top_moving_card{ bottom_row[working_initial_index]->get_card_info(first_card_new_stack).get_suit(), bottom_row[working_initial_index]->get_card_info(first_card_new_stack).get_value() };
	face_up_stack temp_stack{};
	if (bottom_row[working_final_index]->get_stack_height() != 0) {
		card accepting_card{ bottom_row[working_final_index]->get_card_info(bottom_row[working_final_index]->get_stack_height() - 1).get_suit(), bottom_row[working_final_index]->get_card_info(bottom_row[working_final_index]->get_stack_height() - 1).get_value() };
		if ((top_moving_card.get_value() == accepting_card.get_value() - 1) && (top_moving_card.get_colour() != accepting_card.get_colour())) {
			temp_stack = bottom_row[working_initial_index]->split(first_card_new_stack, temp_stack);
			bottom_row[working_final_index] = bottom_row[working_final_index]->merge(temp_stack);
			success = true;
		}
	} else if (top_moving_card.get_value() == 13) {
		temp_stack = bottom_row[working_initial_index]->split(first_card_new_stack, temp_stack);
		bottom_row[working_final_index] = bottom_row[working_final_index]->merge(temp_stack);
		success = true;
	}
	return success;
}

void board::reveal_working(int working_index) {
	// This function is a void, because if it is called the board is being redrawn regardless of the outcome of this function
	bool success{ false };
	std::vector<card_collection*> middle_row{ board_elements[1] };
	std::vector<card_collection*> bottom_row{ board_elements[2] };
	face_up_stack temp_stack{};
	if (bottom_row[working_index]->get_stack_height() == 0 && middle_row[working_index]->get_stack_height() != 0) {
		temp_stack = middle_row[working_index]->split(middle_row[working_index]->get_stack_height() - 1, temp_stack);
		bottom_row[working_index] = bottom_row[working_index]->merge(temp_stack);
	}
	return;
}

bool board::reset_discard() {
	bool success{ false };
	std::vector<card_collection*> top_row{ board_elements[0] };
	face_up_stack temp_stack{ 0 };
	if (top_row[0]->get_stack_height() == 0) {
		top_row[1]->split(0, temp_stack);
		top_row[0] = top_row[0]->merge(temp_stack);
		success = true;
	}
	return success;
}

std::vector<card> board::permute_deck() {
	std::vector<card> card_deck(52);
	for (int suit{ 0 }; suit < 4; suit++) {
		for (int value{ 0 }; value < 13; value++) {
			card temp{ suit + 2, value + 1 };
			card_deck[suit * 13 + value] = temp;
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
			std::cout << "'reveal', reveals either one or three cards from the face down discard pile." << std::endl;
			std::cout << "'reset', resets the discard pile if the face down discard pile is empty." << std::endl;
			std::cout << "'select', selects the active pile if there is no current selected pile." << std::endl;
			std::cout << "'select', if there is a currently selected pile, and if the move is valid, moves the selected pile to the active pile." << std::endl;
			std::cout << "The active pile is shown by being offset from other cards by one line down and one line to the right." << std::endl;
			std::cout << "The selected pile is shown by being flanked by '#'." << std::endl;
			std::cout << "'up', moves the selection to the top row." << std::endl;
			std::cout << "'down', moves the selection to the bottom row." << std::endl;
			std::cout << "'w', moves the active selection to include one less card in its pile." << std::endl;
			std::cout << "'s', moves the active selection to include one more card in its pile." << std::endl;
			std::cout << "'a', moves the active selection one stack to the left." << std::endl;
			std::cout << "'d', moves the active selection one stack to the right." << std::endl;
		} else if (input_string == "exit") {
			return false;
		} else if (input_string == "reveal") {
			if (reveal_discard()) {
				return true;
			} else { std::cout << "Face down discard pile has no cards to reveal, game has not been redrawn." << std::endl; }
		} else if (input_string == "reset") {
			if (reset_discard()) {
				score -= 100;
				if (score < 0) { score = 0; }
				if (active_row == 0 && active_column == 1) {
					active_column = 2;
				}
				return true;
			} else { std::cout << "Face down discard pile is not empty, game has not been redrawn." << std::endl; }
		} else if (input_string == "up") {
			if (active_row == 2) {
				// If you're on the bottom row
				active_row = 0;
				active_column = 2;
				active_sub_row = board_elements[0][1]->get_stack_height() - 1;
				if (active_sub_row < 0) { active_sub_row = 0; }
				return true;
			} else { std::cout << "Invalid movement, game has not been redrawn." << std::endl; }
		} else if (input_string == "down") {
			if (active_row == 0) {
				// If you're on the top row
				std::vector<card_collection*> bottom_row{ board_elements[2] };
				active_row = 2;
				active_column = 0;
				active_sub_row = 0;
				return true;
			} else { std::cout << "Invalid movement, game has not been redrawn." << std::endl; }
		} else if (input_string == "d") {
			if (active_row == 0) {
				// You're on the top row, discard pile is 1, suit piles are 2,3,4,5.
				std::vector<card_collection*> top_row{ board_elements[0] };
				if (active_column != 5) {
					active_column++;
					return true;
				}
				else { std::cout << "Invalid movement, game has not been redrawn." << std::endl; }
			} else {
				// You're on the bottom row, piles are 0,1,2,3,4,5,6.
				std::vector<card_collection*> bottom_row{ board_elements[2] };
				if (active_column != 6) {
					active_column++;
					active_sub_row = 0;
					return true;
				} else { std::cout << "Invalid movement, game has not been redrawn." << std::endl; }
			}
		} else if (input_string == "a") {
			if (active_row == 0) {
				// You're on the top row, discard pile is 1, suit piles are 2,3,4,5.
				std::vector<card_collection*> top_row{ board_elements[0] };
				if (active_column != 1) {
					if (active_column != 2 || top_row[1]->get_stack_height() != 0) {
						active_column--;
						return true;
					}
				} else { std::cout << "Invalid movement, game has not been redrawn." << std::endl; }
			} else {
				// You're on the bottom row, piles are 0,1,2,3,4,5,6.
				std::vector<card_collection*> bottom_row{ board_elements[2] };
				if (active_column != 0) {
					active_column--;
					active_sub_row = 0;
					return true;
				} else { std::cout << "Invalid movement, game has not been redrawn." << std::endl; }
			}
		} else if (input_string == "w") {
			if (active_row == 2) {
				if (active_sub_row > 0) {
					active_sub_row--;
					return true;
				}
			} else { std::cout << "Invalid movement, game has not been redrawn." << std::endl; }
		} else if (input_string == "s") {
			if (active_row == 2) {
				std::vector<card_collection*> bottom_row{ board_elements[2] };
				if (active_sub_row < bottom_row[active_column]->get_stack_height()) {
					active_sub_row++;
					return true;
				}
			} else { std::cout << "Invalid movement, game has not been redrawn." << std::endl; }
		} else if (input_string == "deselect") {
			if (selected_row >= 0 && selected_column >= 0 && selected_sub_row >= 0) {
				selected_row = -1;
				selected_column = -1;
				selected_sub_row = -1;
				return true;
			} else { std::cout << "No pile was initially selected, game has not been redrawn." << std::endl; }
		} else if (input_string == "select") {
			if (selected_row < 0) {
				// Meaning a pile hasn't been selected yet
				selected_row = active_row;
				selected_column = active_column;
				selected_sub_row = active_sub_row;
				return true;
			} else if (selected_row != active_row || selected_column != active_column) {
				// Meaning if you're not trying to put a stack on top of itself, or a substack of it
				if (selected_row == 0) {
					// You're moving a card from either the discard or foundation
					if (selected_column == 1) {
						// From the discard
						if (active_row == 0) {
							// To the foundation
							if (discard_to_foundation(active_column)) {
								selected_column = -1;
								selected_row = -1;
								selected_sub_row = -1;
								score += 10;
								return true;
							} else { std::cout << "Either the suits don't, or the values do not ascend, game has not been redrawn." << std::endl; }
						} else {
							// To the working area
							if (discard_to_working(active_column)) {
								selected_column = -1;
								selected_row = -1;
								selected_sub_row = -1;
								score += 5;
								return true;
							} else { std::cout << "Either the colours match, or the values do not descend, game has not been redrawn." << std::endl; }
						}
					} else {
						// From the foundation to the working area
						if (foundation_to_working(active_column, selected_column)) {
							selected_column = -1;
							selected_row = -1;
							selected_sub_row = -1;
							score -= 10;
							if (score < 0) { score = 0; }
							return true;
						} else { std::cout << "Either the colours match, or the values do not descend, game has not been redrawn." << std::endl; }
					}
				} else {
					// You're moving a pile from the working area
					if (active_row == 0) {
						// To the foundation
						if (working_to_foundation(selected_column, active_column)) {
							score += 10;
							reveal_working(selected_column);
							selected_column = -1;
							selected_row = -1;
							selected_sub_row = -1;
							return true;
						} else { std::cout << "Either the suits don't, or the values do not ascend, game has not been redrawn." << std::endl; }
					} else {
						// To another working pile
						if (working_to_working(selected_sub_row, selected_column, active_column)) {
							reveal_working(selected_column);
							selected_column = -1;
							selected_row = -1;
							selected_sub_row = -1;
							return true;
						} else { std::cout << "Either the colours match, or the values do not descend, game has not been redrawn." << std::endl; }
					}
				}
			} else { std::cout << "Cannot move a pile onto itself, game has not been redrawn." << std::endl; }
		} else { std::cout << "Unrecognised command. Please refer to 'help' for a list of commands." << std::endl; }
	}
	return false;
}

void board::draw() {
	// Drawing default board
	// Row 0 blank, rows 1 - 6 default cards, rows 8 - 11 face up discard stacking, rows 12-14 blank
	// Rows 15 - 58 working height, rows 59 - 60 blank, possibly showing score below.
	std::vector<std::string> string_rows(60);
	std::string space_string(57, ' ');
	int highlight{ 0 };
	for (int i{ 0 }; i < string_rows.size(); i++) {
		string_rows[i] = space_string;
	}
	std::vector<card_collection*> top_row{ board_elements[0] };
	// Drawing either face down card, or blank card
	if (top_row[0]->get_stack_height() == 0) {
		string_rows = draw_card(highlight, 0, 0, 1, 1, string_rows);
		// Place a blank card there
	} else { string_rows = draw_card(0, 0, 1, 1, 1, string_rows); }
	// Place a card back there
// Drawing either 1, 2, or 3 face up discard cards, or blank card
	if (top_row[1]->get_stack_height() == 0) {
		// The face-up discard pile is empty
		string_rows = draw_card(highlight, 0, 0, 9, 1, string_rows);
	} else {
		// The face-up discard pile is not empty
		int discard_height{ top_row[1]->get_stack_height() };
		if (discard_height < discard_reveal_num) {
			for (int i{ 0 }; i < discard_height; i++) {
				if (selected_row == 0 && selected_column == 1 && i == discard_height - 1) {
					highlight = 2;
				} else if (active_row == 0 && active_column == 1 && i == discard_height - 1) {
					highlight = 1;
				} else { highlight = 0; }
				string_rows = draw_card(highlight, top_row[1]->get_card_info(i).get_suit(), top_row[1]->get_card_info(i).get_value(), 9, 1 + 3 * i, string_rows);
			}
		} else {
			for (int i{ 0 }; i < discard_reveal_num; i++) {
				if (selected_row == 0 && selected_column == 1 && i == discard_reveal_num - 1) {
					highlight = 2;
				} else if (active_row == 0 && active_column == 1 && i == discard_reveal_num - 1) {
					highlight = 1;
				} else { highlight = 0; }
				string_rows = draw_card(highlight, top_row[1]->get_card_info(discard_height - discard_reveal_num + i).get_suit(), top_row[1]->get_card_info(discard_height - discard_reveal_num + i).get_value(), 9, 1 + 3 * i, string_rows);
			}
		}
	}
	// Drawing either a blank card, or the highest value foundation card
	for (int i{ 2 }; i < 6; i++) {
		int foundation_height{ top_row[i]->get_stack_height() };
		if (foundation_height == 0) {
			if (active_row == 0 && active_column == i) {
				highlight = 1;
			}
			else { highlight = 0; }
			string_rows = draw_card(highlight, i, 0, 9 + 8 * i, 1, string_rows);
		} else {
			if (selected_row == 0 && selected_column == i) {
				highlight = 2;
			} else if (active_row == 0 && active_column == i) {
				highlight = 1;
			} else { highlight = 0; }
			string_rows = draw_card(highlight, top_row[i]->get_card_info(foundation_height - 1).get_suit(), top_row[i]->get_card_info(foundation_height - 1).get_value(), 9 + 8 * i, 1, string_rows);
		}
	}
	std::vector<card_collection*> middle_row{ board_elements[1] };
	std::vector<card_collection*> bottom_row{ board_elements[2] };
	for (int i{ 0 }; i < 7; i++) {
		int down_stack_height{ middle_row[i]->get_stack_height() };
		int vertical_index{ 0 };
		if (down_stack_height > 0) {
			while (vertical_index < down_stack_height) {
				string_rows = draw_card(0, 0, 1, 1 + 8 * i, 15 + 2 * vertical_index, string_rows);
				vertical_index++;
			}
		}
		int up_stack_height{ bottom_row[i]->get_stack_height() };
		if (up_stack_height > 0) {
			while (vertical_index < down_stack_height + up_stack_height) {
				highlight = 0;
				if (selected_row == 2 && selected_column == i && vertical_index - down_stack_height + 1 > selected_sub_row) {
					highlight = 2;
				} else if (active_row == 2 && active_column == i && vertical_index - down_stack_height + 1 > active_sub_row) {
					highlight = 1;
				} else { highlight = 0; }
				string_rows = draw_card(highlight, bottom_row[i]->get_card_info(vertical_index - down_stack_height).get_suit(), bottom_row[i]->get_card_info(vertical_index - down_stack_height).get_value(), 1 + 8 * i, 15 + 2 * vertical_index, string_rows);
				vertical_index++;
			}
		}
		else {
			if (selected_row == 2 && selected_column == i) {
				highlight = 2;
			}
			else if (active_row == 2 && active_column == i) {
				highlight = 1;
			}
			else { highlight = 0; }
			string_rows = draw_card(highlight, 0, 0, 1 + 8 * i, 15, string_rows);
		}
	}
	for (int i{ 0 }; i < string_rows.size(); i++) {
		std::cout << string_rows[i] << std::endl;
	}
	std::cout << "Your current score is " << score << "." << std::endl;
}

std::vector<std::string> board::draw_card(int highlight, int suit, int value, int x_pos, int y_pos, std::vector<std::string> &string_rows) {
	std::string filename{ ".txt" };
	// 2 - spades, 3 - clubs, 4 - hearts, 5 - diamonds
	if (suit == 0 && value == 0) {
		filename = "blank_card" + filename;
	} else if (suit == 0 && value == 1) {
		filename = "card_back" + filename;
	} else if (suit == 2) {
		filename = "_spades" + filename;
	} else if (suit == 3) {
		filename = "_clubs" + filename;
	} else if (suit == 4) {
		filename = "_hearts" + filename;
	} else if (suit == 5) {
		filename = "_diamonds" + filename;
	}
	if (suit != 0) {
		if (value == 0) {
			filename = "foundation" + filename;
		} else if (value == 1) {
			filename = "ace" + filename;
		} else if (value == 2) {
			filename = "two" + filename;
		} else if (value == 3) {
			filename = "three" + filename;
		} else if (value == 4) {
			filename = "four" + filename;
		} else if (value == 5) {
			filename = "five" + filename;
		} else if (value == 6) {
			filename = "six" + filename;
		} else if (value == 7) {
			filename = "seven" + filename;
		} else if (value == 8) {
			filename = "eight" + filename;
		} else if (value == 9) {
			filename = "nine" + filename;
		} else if (value == 10) {
			filename = "ten" + filename;
		} else if (value == 11) {
			filename = "jack" + filename;
		} else if (value == 12) {
			filename = "queen" + filename;
		} else if (value == 13) {
			filename = "king" + filename;
		}
	}
	//filename = "D:/Documents/University/C++/Final_project/card_art/" + filename;
	filename = "card_art/" + filename;
	std::ifstream card_filename{ filename };
	int starting_column{ x_pos };
	int starting_line{ y_pos };
	if (highlight == 1) {
		starting_column++;
		starting_line++;
	}
	while (!card_filename.eof()) {
		std::string board_row{ string_rows[starting_line] };
		std::string card_line;
		std::getline(card_filename, card_line);
		for (int i{ 0 }; i < card_line.length(); i++) {
			board_row[i + starting_column] = card_line[i];
		}
		if (highlight == 2) {
			board_row[starting_column - 1] = '#';
			board_row[starting_column + 7] = '#';
		}
		string_rows[starting_line] = board_row;
		starting_line++;
	}
	return string_rows;
}