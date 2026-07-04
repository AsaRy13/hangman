// hangman.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <array>

class PhraseLetter {
private:
    bool hidden = true;
    char letter;

public:
    PhraseLetter(char letter) {
        this->letter = letter;
    }

    char getLetter() const {
        return this->letter;
    }

    void setHidden(bool state) {
        this->hidden = state;
    }

    bool getHidden() const {
        return this->hidden;
    }
    
    char printLetter() const {
        if (this->hidden) {
            return '_';
        }
        else {
            return this->letter;
        }
    }
};

class PhraseWord {
private:
    std::string word;
    std::vector<PhraseLetter> letterClasses;
    std::set<char> letters;

public:
    PhraseWord(const std::string& word) {
        this->word = word;
        for (char letter : word) {
            if (letter != ' ') {
                PhraseLetter letterClass(letter);
                letterClasses.push_back(letterClass);
                if (!letters.contains(letter)) {
                    letters.insert(letter);
                }
            }
        }
    }

    std::string display() {
        std::string partWord;
        for (const PhraseLetter& letter : letterClasses) {
            partWord += letter.printLetter();
        }
        partWord += " ";
        return partWord;
    }

    int guess(char letter) {
        if (letters.contains(letter)) {
            for (PhraseLetter& l : letterClasses) {
                if (l.getLetter() == letter) {
                    l.setHidden(false);
                }
            }
            return 0;
        }
        return 1;
    }
    
    bool allFound() {
        for (const PhraseLetter& l : letterClasses) {
            if (l.getHidden()) {
                return false;
            }
        }
        return true;
    }
};

class PhraseClass {
private:
    std::string phrase;
    std::vector<PhraseWord> words;

public:
    PhraseClass(const std::string& phrase) {
        this->phrase = phrase;
        std::string partPhrase = phrase;
        while (partPhrase != "") {
            auto partLocation = partPhrase.find(' ');
            if (partLocation != std::string::npos) {
                PhraseWord word(partPhrase.substr(0, partLocation));
                words.push_back(word);
                partPhrase.erase(0, partLocation + 1);
            }
            else {
                PhraseWord word(partPhrase.substr(0, partPhrase.size()));
                words.push_back(word);
                partPhrase.erase();
            }
        }
    }

    std::string display() {
        std::string partPhrase;
        for (PhraseWord& word : words) {
            partPhrase += word.display();
        }
        return partPhrase;
    }

    int guess(char letter) {
        std::vector<int> output;
        for (PhraseWord& word : words) {
            output.push_back(word.guess(letter));
        }
        auto result = std::find(output.begin(), output.end(), 0);
        if (result != output.end()) {
            return 0;
        }
        return 1;
    }

    bool allFound() {
        for (PhraseWord& word : words) {
            if (!word.allFound()) {
                return false;
            }
        }
        return true;
    }
};

std::string hangmanStages(int stageNum) {
    if (stageNum == 0) {
        return R"<|>(
        ------
       |      |
       |       
       |       
       |       
       |       
       |        
-----------------
)<|>";
    } else if (stageNum == 1) {
        return R"<|>(
        ------
       |      |
       |      O
       |       
       |       
       |       
       |       
-----------------
)<|>";
    } else if (stageNum == 2) {
        return R"<|>(
        ------
       |      |
       |      O
       |      |
       |      |
       |       
       |       
-----------------
)<|>";
    } else if (stageNum == 3) {
        return R"<|>(
        ------
       |      |
       |      O
       |     \|
       |      |
       |       
       |       
-----------------
)<|>";
    } else if (stageNum == 4) {
        return R"<|>(
        ------
       |      |
       |      O
       |     \|/
       |      |
       |       
       |       
-----------------
)<|>";
    } else if (stageNum == 5) {
        return R"<|>(
        ------
       |      |
       |      O
       |     \|/
       |      |
       |     / 
       |       
-----------------
)<|>";
    } else if (stageNum == 6) {
        return R"<|>(
        ------
       |      |
       |      O
       |     \|/
       |      |
       |     / \
       |       
-----------------
)<|>";
    }
}

// Only return by reference when the value outlives the function
const std::string& randPhrase() {
    static const std::array<std::string, 1> phrases = {"test phrase"};
    return phrases[rand() % phrases.size()];
}

int main()
{
    std::string rphrase = randPhrase();
    PhraseClass phrase(rphrase);
    int stage = 0;
    bool quit = false;

    do {
        std::cout << "\033[2J\033[1;1H";
        std::string input;
        std::cout << hangmanStages(stage) << "\n";
        std::cout << phrase.display() << "\n\n";

        if (phrase.allFound()) {
            std::cout << "You won!\n";
            quit = true;
            continue;
        }

        if (stage == 6) {
            std::cout << "You lost.\n";
            quit = true;
        }
        else {
            std::cout << "To quit input: |\n";
            std::cout << "Guess: ";
            std::cin >> input;
            std::cout << "\n";

            if (input.length() > 1) {
                std::string x;
                std::cout << "Please only input one character.\n";
                std::cout << "Type something and press enter to continue...";
                std::cin >> x;
                std::cout << "\n";
            }
            else {
                char inputChar = input[0];
                if ((inputChar >= 'A' && inputChar <= 'Z') || (inputChar >= 'a' && inputChar <= 'z')) {
                    char inputLower = std::tolower(inputChar);
                    stage += phrase.guess(inputLower);
                }
                else if (inputChar == '|') {
                    quit = true;
                }
                else {
                    std::string x;
                    std::cout << "Please only input letters or | to quit.\n";
                    std::cout << "Type something and press enter to continue...";
                    std::cin >> x;
                    std::cout << "\n";
                }
            }
        }
    } 
    while (!quit);
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
