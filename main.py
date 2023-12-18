import pygame
import sys 
from variables import *
from Game import Game
pygame.init()
gameState = 'start'
AI_turn = 1
AI_test = 0

playState = Game()
pygame.display.set_caption('oanquantet')
#ccw 1, cw 0
# program_path_1 = "main.exe"
# program_path_2 = "main.exe"
strategy = ['random', 'minimax', 'expectimax']
strategy_1 = 'minimax'
strategy_2 = 'expectimax'
run = True
while run:
    timer.tick(fps)
    pygame.time.delay(100)

    #event handling - capturing everything going on your keyboard
    
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
            sys.exit()
        if event.type == pygame.KEYDOWN and (event.key == pygame.K_RETURN or event.key == pygame.K_KP_ENTER):
            if gameState == 'end':
                playState = Game()
                gameState = 'start'
        if playState.game_over(): 
            gameState = 'end'
            playState.turn_step = 4
        playState.redraw()
        if gameState == 'end':
            continue
        playState.fix_empty_rows()
        if AI_test:
            if playState.turn_step == 0:
                playState.make_AI_move(strategy_1, depth=7)
            elif playState.turn_step == 2:
                playState.make_AI_move(strategy_2, depth=3)
        elif AI_turn and playState.turn_step == 2:
            playState.make_AI_move(strategy_2, depth=5)
        else:
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1: #check left mouse click
                playState.check_valid_move(event)
            
            if event.type == pygame.KEYDOWN and playState.state.selection != -1 and (event.key == pygame.K_LEFT or event.key == pygame.K_RIGHT):  
                playState.play(playState.state.selection, event.key)
        
            
pygame.quit()