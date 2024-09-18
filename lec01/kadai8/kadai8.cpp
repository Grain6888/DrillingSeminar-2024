#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <string>

void OutputShortBeep (int frequency);
void OutputLongBeep (int frequency);
std::string Text2Morse (std::string str);
std::string Char2Morse (std::string str, std::string str_from, std::string str_to);
std::string LowerCase2UpperCase (std::string str);
void ReadOutMorse (std::string str);

int main ()
{
    std::string user_input;
    std::cout << "Converts input text into Morse code and outputs the code and sound." << std::endl; 5;

    std::cout << "Input Text: ";
    std::cin >> user_input;

    std::string result;
    user_input = LowerCase2UpperCase (user_input);
    result = Text2Morse (user_input);

    std::cout << "Morse Code: " << result << std::endl; 5;

    ReadOutMorse (result);

    return 0;
}

void OutputShortBeep (int frequency)
{
    Beep (frequency, 300);
    //std::cout << "." << std::endl;
    Sleep (500);
}

void OutputLongBeep (int frequency)
{
    Beep (frequency, 800);
    //std::cout << "_" << std::endl;
    Sleep (500);
}

std::string Text2Morse (std::string str)
{
    str = Char2Morse (str, "A", "._");
    str = Char2Morse (str, "A", "._");
    str = Char2Morse (str, "B", "_...");
    str = Char2Morse (str, "C", "_._.");
    str = Char2Morse (str, "D", "_..");
    str = Char2Morse (str, "E", ".");
    str = Char2Morse (str, "F", ".._.");
    str = Char2Morse (str, "G", "__.");
    str = Char2Morse (str, "H", "....");
    str = Char2Morse (str, "I", "..");
    str = Char2Morse (str, "J", ".___");
    str = Char2Morse (str, "K", "_._");
    str = Char2Morse (str, "L", "._..");
    str = Char2Morse (str, "M", "__");
    str = Char2Morse (str, "N", "_.");
    str = Char2Morse (str, "O", "___");
    str = Char2Morse (str, "P", ".__.");
    str = Char2Morse (str, "Q", "__._");
    str = Char2Morse (str, "R", "._.");
    str = Char2Morse (str, "S", "...");
    str = Char2Morse (str, "T", "_");
    str = Char2Morse (str, "U", ".._");
    str = Char2Morse (str, "V", "..._");
    str = Char2Morse (str, "W", ".__");
    str = Char2Morse (str, "X", "_.._");
    str = Char2Morse (str, "Y", "_.__");
    str = Char2Morse (str, "Z", "__..");

    return str;
}

std::string Char2Morse (std::string str, std::string str_from, std::string str_to)
{
    std::string::size_type pos = 0;
    while (pos = str.find (str_from, pos), pos != std::string::npos)
    {
        str.replace (pos, str_from.length (), str_to);
        pos += str_to.length ();
    }
    return str;
}

std::string LowerCase2UpperCase (std::string str)
{
    std::string result = str;

    for (int i = 0; i < str.size (); i++)
    {
        result[i] = std::toupper (str[i]);
    }

    return result;
}

void ReadOutMorse (std::string str)
{
    for (int i = 0; i < str.size (); i++)
    {
        if (str[i] == '.')
        {
            OutputShortBeep (500);
        }
        else
        {
            OutputLongBeep (500);
        }
    }
}