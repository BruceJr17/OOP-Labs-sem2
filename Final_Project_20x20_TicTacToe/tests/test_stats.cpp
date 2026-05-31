#include "player/my_player.hpp"
#include "test_stats.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
    std::cout << "Testing MyPlayer 1 vs MyPlayer 2 \n";
    if (argc >= 2) {
        std::srand(atoi(argv[1]));
    }

    
    ttt::my_player::MyPlayer p1("MyPlayer 1"); ////поместите вашего игрока сюда
    ttt::my_player::MyPlayer p2("MyPlayer 2");
    auto result = ttt::test::run_game_tests(p1, p2, 500); //здесь вы можете изменить количество тестовых итераций ~~ 100
    
    
    ttt::test::print_test_results(result, "MyPlayer 1", "MyPlayer 2");
    
    return 0;
}
