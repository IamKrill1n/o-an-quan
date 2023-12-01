import pygame
import sys 
from variables import *
from Game import Game
pygame.init()
gameState = 'start'
AI_turn = 1

playState = Game()
pygame.display.set_caption('oanquantet')
#ccw 1, cw 0
   
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
        if gameState == 'end': 
            continue
        playState.fix_empty_rows()
        if AI_turn and playState.turn_step == 2:
            playState.make_AI_move()

        else:
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1: #check left mouse click
                playState.check_valid_move(event)
            
            if event.type == pygame.KEYDOWN and playState.table.selection != -1 and (event.key == pygame.K_LEFT or event.key == pygame.K_RIGHT):  
                playState.play(playState.table.selection, event.key)
        playState.render()
    
    if playState.game_over():
        gameState = 'end'
        playState.EndGameText()
        playState.turn_step = 4
        
pygame.quit()