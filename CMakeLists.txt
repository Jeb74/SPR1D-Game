cmake_minimum_required(VERSION 3.20)
project(final_project C)

set(CMAKE_C_STANDARD 99)

add_executable(final_project main.c headers/structures.h portable_utils/utils.c portable_utils/utils.h two_players_games/tris.c headers/tris.h structures.c two_players_games/paridispari.c headers/paridispari.h two_players_games/dadi.c headers/dadi.h two_players_games/morracinese.c headers/morracinese.h two_players_games/biglie.c headers/biglie.h two_players_games/murra.c headers/murra.h scrematura.c headers/scrematura.h filemanagement.c headers/filemanagement.h four_players_games/impiccato.c headers/impiccato.h four_players_games/ponte_vetro.c headers/ponte_vetro.h two_players_games/blackjack.c headers/blackjack.h portable_utils/colors.c portable_utils/colors.h bots.c headers/bots.h)

target_link_libraries(final_project PRIVATE m)