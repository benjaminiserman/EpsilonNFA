#include "EpsilonNfa.h"
#include <iostream>

using namespace final_project;

void yayWooTest()
{
    // set up
    auto nfa = new EpsilonNfa<char, std::string>();

    // add nodes
    nfa->AddInitialNode("s₀", false);
    nfa->AddNode("s₁", false);
    nfa->AddNode("s₂", false);
    nfa->AddNode("s₃", true);
    nfa->AddNode("s₄", false);
    nfa->AddNode("s₅", false);
    nfa->AddNode("s₆", true);

    // add edges
    nfa->AddEdge("s₀", [](auto _) { return true; }, "s₀");

    nfa->AddEdge("s₀", 'y', "s₁");
    nfa->AddEdge("s₁", 'a', "s₂");
    nfa->AddEdge("s₂", 'y', "s₃");

    nfa->AddEdge("s₀", 'w', "s₄");
    nfa->AddEdge("s₄", 'o', "s₅");
    nfa->AddEdge("s₅", 'o', "s₆");

    // tests
    auto test = [nfa](const char* testString)
    {
        std::cout << "\"" << testString << "\": " << (nfa->TryAcceptString(std::string(testString)) ? "True\n" : "False\n");
    };

    test("yay");
    test("test");
    test("powoo");
    test("yay!");
    test("allay");
    test("lay down");
    test("ay");
}

void everyThirdTest()
{
    // set up
    auto nfa = new EpsilonNfa<char, std::string>();

    // add nodes
    nfa->AddInitialNode("s₀", true);
    nfa->AddNode("s₁", true);
    nfa->AddNode("s₂", false);
    nfa->AddNode("s₃", true);
    nfa->AddNode("s₄", true);

    // add edges

    nfa->AddEdge("s₀", '0', "s₂");
    nfa->AddEdge("s₀", '1', "s₁");
    nfa->AddEdge("s₁", '0', "s₄");
    nfa->AddEdge("s₁", '1', "s₄");
    nfa->AddEdge("s₂", '0', "s₂");
    nfa->AddEdge("s₂", '1', "s₂");
    nfa->AddEdge("s₃", '0', "s₂");
    nfa->AddEdge("s₃", '1', "s₁");
    nfa->AddEdge("s₄", '0', "s₃");
    nfa->AddEdge("s₄", '1', "s₃");
    // tests
    auto test = [nfa](const char* testString)
    {
        std::cout << "\"" << testString << "\": " << (nfa->TryAcceptString(std::string(testString)) ? "True\n" : "False\n");
    };

    test("10011110");
    test("1101001");
    test("111111111");
    test("1");
    test("");

    test("00011110");
    test("1100001");
    test("111111011");

    test("0");
    test("0001");
}

void stupidLoopTest()
{
    // set up
    auto nfa = new EpsilonNfa<char, std::string>();

    // add nodes
    nfa->AddInitialNode("s₀", true);
    nfa->AddNode("s₁", false);

    // add edges

    nfa->AddEdge( "s₀", [](auto) { return true; }, "s₀");

    nfa->AddEpsilonEdge("s₀", "s₁");
    nfa->AddEpsilonEdge("s₁", "s₀");

    // tests
    auto test = [nfa](const char* testString)
    {
        std::cout << "\"" << testString << "\": " << (nfa->TryAcceptString(std::string(testString)) ? "True\n" : "False\n");
    };

    test("10011110");
    test("1101001");
    test("111111111");
    test("1");
    test("");

    test("00011110");
    test("1100001");
    test("111111011");

    test("0");
    test("0001");
}

void evenZeros()
{
    // set up
    auto nfa = new EpsilonNfa<char, std::string>();

    // add nodes
    nfa->AddInitialNode("S₁", true);
    nfa->AddNode("S₂", false);

    // add edges

    nfa->AddEdge("S₁", '0', "S₂");
    nfa->AddEdge("S₁", '1', "S₁");
    nfa->AddEdge("S₂", '0', "S₁");
    nfa->AddEdge("S₂", '1', "S₂");

    // tests
    auto test = [nfa](const char* testString)
    {
        std::cout << "\"" << testString << "\": ";
        if (nfa->TryAcceptString(std::string(testString)))
        {
            std::cout << "True\n";
        }
        else
        {
            std::cout << "False\n";
        }
    };

    test("00");
    test("11");
    test("1001");
    test("1110101011110");
    test("");

    test("0");
    test("101");
    test("01001");
    test("10");
    test("000");
}

void complicatedExample()
{
    // set up
    auto nfa = new EpsilonNfa<char, std::string>();

    // add nodes
    nfa->AddInitialNode("q₀", false);
    nfa->AddNode("q₁", true);
    nfa->AddNode("q₂", false);

    // add edges

    nfa->AddEdge("q₀", '0', "q₀");
    nfa->AddEdge("q₀", '1', "q₂");
    nfa->AddEpsilonEdge("q₀", "q₁");

    nfa->AddEdge("q₂", '1', "q₂");
    nfa->AddEdge("q₂", '1', "q₁");

    // tests
    auto test = [nfa](const char* testString)
    {
        std::cout << "\"" << testString << "\": ";
        if (nfa->TryAcceptString(std::string(testString)))
        {
            std::cout << "True\n";
        }
        else
        {
            std::cout << "False\n";
        }
    };

    test("000");
    test("11");
    test("00111");
    test("0000000000011");
    test("");

    test("10");
    test("101");
    test("01001");
    test("1");
    test("1111");
}

int main()
{
    // std::cout << "\nyayWooTest:\n";
    // yayWooTest();

    // std::cout << "\neveryThirdTest:\n";
    // everyThirdTest();

    // std::cout << "\nstupidLoopTest:\n";
    // stupidLoopTest();

    std::cout << "\nevenZeros:\n";
    evenZeros();

    // std::cout << "\ncomplicatedExample:\n";
    // complicatedExample();
}