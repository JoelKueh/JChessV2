import os
import sys

fen = sys.argv[1]
depth = int(sys.argv[2])

def perft_search(fen, depth):
    if (depth == 0):
        return

    fish_cmd = '''
    {{
       echo position fen {fen_stuff}
       echo go perft {depth_stuff}
    }} | ./../stockfish > temp_stockfish_perft_output_0
    '''.format(fen_stuff=fen, depth_stuff=depth)
    jc_cmd = f'./../../build/bin/perft {depth} {fen} > temp_jchess_perft_output_0'

    os.system(fish_cmd)
    os.system(jc_cmd)


    print(open('temp_stockfish_perft_output_0', "r").read())
    print(open('temp_jchess_perft_output_0', "r").read())

    os.system("head -n -3 temp_stockfish_perft_output_0 | tail -n +2 > temp_stockfish_perft_output")
    os.system("head -n -3 temp_jchess_perft_output_0 > temp_jchess_perft_output")

#    print(open('temp_stockfish_perft_output', "r").read())
#    print(open('temp_jchess_perft_output', "r").read())
    print()

    fish = open("temp_stockfish_perft_output", "r")
    jc = open("temp_jchess_perft_output", "r")


    jc_hash = {}
    fish_hash = {}
    non_matching = []

    for line in jc:
        line = line[:-1]
        parsed = line.split(": ")
        jc_hash[parsed[0]] = parsed[1]
        
    print("MISSING:")
    for line in fish:
        line = line[:-1]
        parsed = line.split(": ")
        fish_hash[parsed[0]] = parsed[1]

        if parsed[0] not in jc_hash:
            print(line)
    print()

    jc.seek(0)

    print("EXTRA:")
    for line in jc:
        line = line[:-1]
        parsed = line.split(": ")

        if parsed[0] not in fish_hash:
            print(line)
    print()

    jc.seek(0)

    print("NON MATCHING:")
    for line in jc:
        line = line[:-1]
        parsed = line.split(": ")

        if parsed[0] not in fish_hash:
            continue

        if fish_hash[parsed[0]] != parsed[1]:
            print(line + " " + fish_hash[parsed[0]])
            non_matching.append(parsed[0])
    print()

    if not non_matching:
        return

    print("-------------------------------------------------------------------------\n")
    position_cmd = '''
    {{
        echo position fen {fen_stuff} moves {moves_stuff}
        echo d
    }} | ./../stockfish | head -n -2 | tail -n +3 > temp_stockfish_display_output 
    '''.format(fen_stuff=fen, depth_stuff=depth, moves_stuff=non_matching[0])

    os.system(position_cmd)
    disp = open("temp_stockfish_display_output")
    for line in disp:
        line = line[:-1]
        print(line)
    new_fen = line.split(": ")[1]
    print("\n")

    perft_search(new_fen, depth - 1)

perft_search(fen, depth)
#os.remove("temp_stockfish_perft_output")
#os.remove("temp_jchess_perft_output")
#os.remove("temp_stockfish_display_output")
