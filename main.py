import pygame
import sys 
from variables import *
from Table import *
import math
pygame.init()

###MORE PIECES ? TAKE CARE LATER.
board = Table()
pygame.display.set_caption('oanquantet')
#draw main game board
def text_render():
    screen.blit(font.render('Player 1 point:' + str(player_point[0]), True, 'white'), (0, HEIGHT - 30))
    screen.blit(font.render('Player 2 point:' + str(player_point[1]), True, 'white'), (0, 0))
    status_text = ['Player 1: Select a square to play!', 'Use keyboard < or > to select a direction!',
                    'Player 2: Select a square to play!', 'Use keyboard < or > to Select a direction!', '']
    if turn_step < 2:
        screen.blit(big_font.render(status_text[turn_step], True, 'white'), ((WIDTH - big_font.size(status_text[turn_step])[0]) / 2, HEIGHT - 100))
    else:
        screen.blit(big_font.render(status_text[turn_step], True, 'white'), ((WIDTH - big_font.size(status_text[turn_step])[0]) / 2, 50))
    if gameState == 'end':
        winner = 1 if player_point[0] > player_point [1] else 2
        text_1 = 'Congrats, player ' + str(winner) + ' has won the game!'
        text_2 = 'Press enter or return to play again'
        screen.blit(big_font.render(text_1,  True, 'white'), ((WIDTH - big_font.size(text_1)[0]) / 2, 50))
        screen.blit(big_font.render(text_2,  True, 'white'), ((WIDTH - big_font.size(text_2)[0]) / 2, 125))
    
#main game loop
def get_coord(x, y):
    sol = (y - (HEIGHT/2 - 100)) // 100 * 5
    if sol == 0: #player 2's side
        return (x-200) // ((WIDTH - 400) / 5) + 1
    else: #player 1's side
        return 11 - (x-200) // ((WIDTH - 400) / 5)
#ccw 0, cw 1
def get_nxt(i, direction):
    return (i + (1 if direction else -1)) % 12

def update(direction, player): #ccw 0, cw 1
    temp, board.table[board.selection][0] = board.table[board.selection][0], 0
    # print(board.table[board.selection][0], board.selection, temp)
    pygame.time.wait(500)
    update_board()
    while(temp != 0):
        for i in range(temp):
            board.picking = get_nxt(board.picking, direction)
            board.table[board.picking][0] += 1

            pygame.time.wait(500)
            update_board()
        board.picking = get_nxt(board.picking, direction)
        if board.table[board.picking][0] == 0:
            temp = 0
            while(board.table[board.picking][0] == 0):
                board.picking = get_nxt(board.picking, direction)
                if(board.table[board.picking][0] != 0):
                    player_point[player] += board.table[board.picking][0] + board.table[board.picking][1] * 5
                    board.table[board.picking] = [0,0]
                    
                    pygame.time.wait(500)
                    update_board()
                else:
                    break
                board.picking = get_nxt(board.picking, direction)
        elif board.picking == 0 or board.picking == 6: #stepped into 'quan' square, switch turn
            break
        elif board.table[board.picking][0] != 0:
            temp, board.table[board.picking][0] = board.table[board.picking][0], 0
            pygame.time.wait(500)
            update_board()
    board.picking = 100    
    return           
    
def update_board():                
    timer.tick(fps)
    screen.fill(screen_color)
    text_render()
    board.render()
    pygame.display.flip()
run = True
while run:
    update_board()
    #event handling - capturing everything going on your keyboard
    
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
            sys.exit()
        if event.type == pygame.KEYDOWN and (event.key == pygame.K_RETURN or event.key == pygame.K_KP_ENTER):
            if gameState == 'end':
                board = Table()
                turn_step = 0
                player_point = [0, 0]
                gameState = 'start'
        if gameState == 'end': 
            continue
        if (turn_step == 0 and board.empty_rows(0)) or (turn_step == 2 and board.empty_rows(1)):
            player = turn_step//2
            player_point[player] -= 5
            for i in range(1 + (1-player) * 6, 6 + (1-player) * 6):
                board.table[i][0] = 1
                board.picking = i
                pygame.time.wait(500)
                update_board()
            board.picking = 100            
        if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1: #check left mouse click
            x_coord = event.pos[0]
            y_coord = event.pos[1]
            
            cur = int(get_coord(x_coord, y_coord))
            if 0 < cur < 12 and board.table[cur] != [0,0]:
                if (turn_step <= 1 and 6 < cur < 12) or (turn_step > 1 and 0 < cur < 6):
                    board.selection, board.picking = cur, cur
                    turn_step = 1 if turn_step <= 1 else 3
        
        if event.type == pygame.KEYDOWN and board.selection != 100 and (event.key == pygame.K_LEFT or event.key == pygame.K_RIGHT):  

            if turn_step == 1:
                if pygame.key.get_pressed()[pygame.K_LEFT]:
                    update(1, 0)
                elif pygame.key.get_pressed()[pygame.K_RIGHT]:
                    update(0, 0)
            elif turn_step == 3:
                if pygame.key.get_pressed()[pygame.K_LEFT]:
                    update(0, 1)
                elif pygame.key.get_pressed()[pygame.K_RIGHT]:
                    update(1, 1)
            
            turn_step = (turn_step + 1) % 4
            board.selection = 100
        if board.table[0] == [0,0] and board.table[6] == [0, 0]:
            player_point[0] += sum(board.table[i][0] for i in range(1, 6))
            player_point[1] += sum(board.table[i][0] for i in range(7, 12))
            gameState = 'end'
            turn_step = 4
pygame.quit()